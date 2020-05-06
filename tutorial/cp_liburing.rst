.. _cp_liburing:

``cp`` with liburing
====================

In the previous section, we saw how to build the equivalent of the Unix ``cat`` utility using the high-level interface to ``io_uring`` provided by liburing. We did not however queue more than one request at the same time in either of these examples. One of the life goals of ``io_uring`` is to be able to reduce the number of system calls required by letting users queue several operations at a time so that the kernel can pick those up in one swoop and process them without the program having to go through one or more system calls for each I/O request.

To that end, in this part, we build a copy program that copies files. It tries to be as efficient as possible by queuing as many requests as the queue depth will allow. Let’s see some code. To give credit where it is due, this is heavily based on `a program from the fio package <https://github.com/axboe/fio/blob/master/t/io_uring.c>`_.

.. code-block:: c

  #include <stdio.h>
  #include <fcntl.h>
  #include <string.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <assert.h>
  #include <errno.h>
  #include <sys/stat.h>
  #include <sys/ioctl.h>
  #include <liburing.h>

  #define QD  2
  #define BS (16 * 1024)

  static int infd, outfd;

  struct io_data {
      int read;
      off_t first_offset, offset;
      size_t first_len;
      struct iovec iov;
  };

  static int setup_context(unsigned entries, struct io_uring *ring) {
      int ret;

      ret = io_uring_queue_init(entries, ring, 0);
      if( ret < 0) {
          fprintf(stderr, "queue_init: %s\n", strerror(-ret));
          return -1;
      }

      return 0;
  }

  static int get_file_size(int fd, off_t *size) {
      struct stat st;

      if (fstat(fd, &st) < 0 )
          return -1;
      if(S_ISREG(st.st_mode)) {
          *size = st.st_size;
          return 0;
      } else if (S_ISBLK(st.st_mode)) {
          unsigned long long bytes;

          if (ioctl(fd, BLKGETSIZE64, &bytes) != 0)
              return -1;

          *size = bytes;
          return 0;
      }
      return -1;
  }

  static void queue_prepped(struct io_uring *ring, struct io_data *data) {
      struct io_uring_sqe *sqe;

      sqe = io_uring_get_sqe(ring);
      assert(sqe);

      if (data->read)
          io_uring_prep_readv(sqe, infd, &data->iov, 1, data->offset);
      else
          io_uring_prep_writev(sqe, outfd, &data->iov, 1, data->offset);

      io_uring_sqe_set_data(sqe, data);
  }

  static int queue_read(struct io_uring *ring, off_t size, off_t offset) {
      struct io_uring_sqe *sqe;
      struct io_data *data;

      data = malloc(size + sizeof(*data));
      if (!data)
          return 1;

      sqe = io_uring_get_sqe(ring);
      if (!sqe) {
          free(data);
          return 1;
      }

      data->read = 1;
      data->offset = data->first_offset = offset;

      data->iov.iov_base = data + 1;
      data->iov.iov_len = size;
      data->first_len = size;

      io_uring_prep_readv(sqe, infd, &data->iov, 1, offset);
      io_uring_sqe_set_data(sqe, data);
      return 0;
  }

  static void queue_write(struct io_uring *ring, struct io_data *data) {
      data->read = 0;
      data->offset = data->first_offset;

      data->iov.iov_base = data + 1;
      data->iov.iov_len = data->first_len;

      queue_prepped(ring, data);
      io_uring_submit(ring);
  }

  int copy_file(struct io_uring *ring, off_t insize) {
      unsigned long reads, writes;
      struct io_uring_cqe *cqe;
      off_t write_left, offset;
      int ret;

      write_left = insize;
      writes = reads = offset = 0;

      while (insize || write_left) {
          int had_reads, got_comp;

          /* Queue up as many reads as we can */
          had_reads = reads;
          while (insize) {
              off_t this_size = insize;

              if (reads + writes >= QD)
                  break;
              if (this_size > BS)
                  this_size = BS;
              else if (!this_size)
                  break;

              if (queue_read(ring, this_size, offset))
                  break;

              insize -= this_size;
              offset += this_size;
              reads++;
          }

          if (had_reads != reads) {
              ret = io_uring_submit(ring);
              if (ret < 0) {
                  fprintf(stderr, "io_uring_submit: %s\n", strerror(-ret));
                  break;
              }
          }

          /* Queue is full at this point. Let's find at least one completion */
          got_comp = 0;
          while (write_left) {
              struct io_data *data;

              if (!got_comp) {
                  ret = io_uring_wait_cqe(ring, &cqe);
                  got_comp = 1;
              } else {
                  ret = io_uring_peek_cqe(ring, &cqe);
                  if (ret == -EAGAIN) {
                      cqe = NULL;
                      ret = 0;
                  }
              }
              if (ret < 0) {
                  fprintf(stderr, "io_uring_peek_cqe: %s\n",
                          strerror(-ret));
                  return 1;
              }
              if (!cqe)
                  break;

              data = io_uring_cqe_get_data(cqe);
              if (cqe->res < 0) {
                  if (cqe->res == -EAGAIN) {
                      queue_prepped(ring, data);
                      io_uring_cqe_seen(ring, cqe);
                      continue;
                  }
                  fprintf(stderr, "cqe failed: %s\n",
                          strerror(-cqe->res));
                  return 1;
              } else if (cqe->res != data->iov.iov_len) {
                  /* short read/write; adjust and requeue */
                  data->iov.iov_base += cqe->res;
                  data->iov.iov_len -= cqe->res;
                  queue_prepped(ring, data);
                  io_uring_cqe_seen(ring, cqe);
                  continue;
              }

              /*
               * All done. If write, nothing else to do. If read,
               * queue up corresponding write.
               * */

              if (data->read) {
                  queue_write(ring, data);
                  write_left -= data->first_len;
                  reads--;
                  writes++;
              } else {
                  free(data);
                  writes--;
              }
              io_uring_cqe_seen(ring, cqe);
          }
      }

      return 0;
  }

  int main(int argc, char *argv[]) {
      struct io_uring ring;
      off_t insize;
      int ret;

      if (argc < 3) {
          printf("Usage: %s <infile> <outfile>\n", argv[0]);
          return 1;
      }

      infd = open(argv[1], O_RDONLY);
      if (infd < 0) {
          perror("open infile");
          return 1;
      }

      outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (outfd < 0) {
          perror("open outfile");
          return 1;
      }

      if (setup_context(QD, &ring))
          return 1;

      if (get_file_size(infd, &insize))
          return 1;

      ret = copy_file(&ring, insize);

      close(infd);
      close(outfd);
      io_uring_queue_exit(&ring);
      return ret;
  }


Program structure
-----------------
This copy program, like most others, copies the file pointed to by the first argument into the file pointed to in the second argument. The core of the program is the :c:func:`copy_file` function. Here, we set up an outer ``while`` loop, which turn contains 2 other ``while`` loops at the same level nested within it. While the outer ``while`` loop is there to ensure that all bytes from the source file are copied, the first nested ``while`` loop is tasked with creating as many :c:func:`readv` requests as possible. In fact, it enqueues as many as the queue depth will allow.

Once the queue is full, we come to the second nested ``while`` loop. This loop reaps up completion queue entries and submits requests to write the destination file, now that the data is read. There are several variables that track state and it can get a little confusing. But how difficult can an asynchronous file copying program be? :)

**Based on** `unixism.net/2020/04/io-uring-by-example-part-2-queuing-multiple-requests/ <https://unixism.net/2020/04/io-uring-by-example-part-2-queuing-multiple-requests/>`_
