.. _fixed_buffers:


Fixed buffers
=============
The idea with using fixed buffers is this: you provide a set of buffers you describe with an array of ``iovec`` structures and register them with the kernel using :c:func:`io_uring_register_buffers`. This causes the kernel to map these buffers in, avoiding future copies to and from user space. You can then use the "fixed buffer" functions like :c:func:`io_uring_prep_write_fixed` and :c:func:`io_uring_prep_read_fixed` specifying the index of the buffer you want to use.

.. code-block:: c

    #include <stdio.h>
    #include <string.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include "liburing.h"

    #define BUF_SIZE    512
    #define FILE_NAME   "/tmp/io_uring_test.txt"
    #define STR1        "What is this life if, full of care,\n"
    #define STR2        "We have no time to stand and stare."

    int fixed_buffers(struct io_uring *ring) {
        struct iovec iov[4];
        struct io_uring_sqe *sqe;
        struct io_uring_cqe *cqe;

        int fd = open(FILE_NAME, O_RDWR|O_TRUNC|O_CREAT, 0644);
        if (fd < 0 ) {
            perror("open");
            return 1;
        }

        for (int i = 0; i < 4; i++) {
            iov[i].iov_base = malloc(BUF_SIZE);
            iov[i].iov_len = BUF_SIZE;
            memset(iov[i].iov_base, 0, BUF_SIZE);
        }

        int ret = io_uring_register_buffers(ring, iov, 4);
        if(ret) {
            fprintf(stderr, "Error registering buffers: %s", strerror(-ret));
            return 1;
        }

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        int str1_sz = strlen(STR1);
        int str2_sz = strlen(STR2);
        strncpy(iov[0].iov_base, STR1, str1_sz);
        strncpy(iov[1].iov_base, STR2, str2_sz);
        io_uring_prep_write_fixed(sqe, fd, iov[0].iov_base, str1_sz, 0, 0);

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }
        io_uring_prep_write_fixed(sqe, fd, iov[1].iov_base, str2_sz, str1_sz, 1);

        io_uring_submit(ring);

        for(int i = 0; i < 2; i ++) {
            int ret = io_uring_wait_cqe(ring, &cqe);
            if (ret < 0) {
                fprintf(stderr, "Error waiting for completion: %s\n",
                        strerror(-ret));
                return 1;
            }
            /* Now that we have the CQE, let's process the data */
            if (cqe->res < 0) {
                fprintf(stderr, "Error in async operation: %s\n", strerror(-cqe->res));
            }
            printf("Result of the operation: %d\n", cqe->res);
            io_uring_cqe_seen(ring, cqe);
        }

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        io_uring_prep_read_fixed(sqe, fd, iov[2].iov_base, str1_sz, 0, 2);

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        io_uring_prep_read_fixed(sqe, fd, iov[3].iov_base, str2_sz, str1_sz, 3);

        io_uring_submit(ring);
        for(int i = 0; i < 2; i ++) {
            int ret = io_uring_wait_cqe(ring, &cqe);
            if (ret < 0) {
                fprintf(stderr, "Error waiting for completion: %s\n",
                        strerror(-ret));
                return 1;
            }
            /* Now that we have the CQE, let's process the data */
            if (cqe->res < 0) {
                fprintf(stderr, "Error in async operation: %s\n", strerror(-cqe->res));
            }
            printf("Result of the operation: %d\n", cqe->res);
            io_uring_cqe_seen(ring, cqe);
        }
        printf("Contents read from file:\n");
        printf("%s%s", iov[2].iov_base, iov[3].iov_base);
    }

    int main() {
        struct io_uring ring;

        int ret = io_uring_queue_init(8, &ring, 0);
        if (ret) {
            fprintf(stderr, "Unable to setup io_uring: %s\n", strerror(-ret));
            return 1;
        }
        fixed_buffers(&ring);
        io_uring_queue_exit(&ring);
        return 0;
    }

How it works
------------
We allocate 4 buffers via :man:`malloc(3)` and then register them with the kernel with the :c:func:`io_uring_register_buffers` function. The ``iovec`` structure describes each array by holding a base address and the size of the allocated buffer. We use an array of ``iovec`` structures 4 elements long to hold details about the 4 arrays we need.

This program is just a simple demonstration of how to use fixed buffers and as such does not to anything more useful beyond that. But it does serve to remind you about `the poem Leisure <https://en.wikipedia.org/wiki/Leisure_(poem)>`_ by W.H Davies, though. Two strings are written using two fixed write operations (:c:func:`io_uring_prep_write_fixed`) to a file using buffers in indices 0 and 1. Later, we read the file using two fixed read operations (:c:func:`io_uring_prep_read_fixed`) this time using buffer indices 2 and 3. We then print the results of these reads.

You can see how the output of this program looks like below:

.. highlight:: none

::

    Result of the operation: 36
    Result of the operation: 35
    Result of the operation: 36
    Result of the operation: 35
    Contents read from file:
    What is this life if, full of care,
    We have no time to stand and stare.

Source code
-----------
Source code for this and other examples is `available on Github <https://github.com/shuveb/loti-examples>`_.