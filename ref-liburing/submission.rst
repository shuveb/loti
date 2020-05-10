.. _submission:

##########
Submission
##########

Introduction
============

Submitting an I/O request is a sequence that generally looks like this:

.. code-block:: c
      :name: submission-intro-snippet

      /* Get an SQE */
      struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
      /* Setup a readv operation */
      io_uring_prep_readv(sqe, file_fd, fi->iovecs, blocks, 0);
      /* Set user data */
      io_uring_sqe_set_data(sqe, fi);
      /* Finally, submit the request */
      io_uring_submit(ring);

This snippet is from the example :ref:`cat with liburing<cat_liburing>`.

You call :c:func:`io_ring_get_sqe` to get an submission queue entry or SQE, use one of the submission helpers for the type of I/O you're trying to get done like :c:func:`io_uring_prep_readv` or :c:func:`io_uring_prep_accept`, call :c:func:`io_uring_set_sqe_data` to get a pointer to a a data structure that uniquely identifies this request (you get this same user data back on the completion side), and finally call :c:func:`io_uring_submit` to submit the request.

You can also set up polling as to avoid calling the :c:func:`io_uring_submit` system call. TODO: link polling example here.

----

.. c:function:: struct io_uring_sqe *io_uring_get_sqe(struct io_uring *ring)

  This function returns a submission queue entry that can be used to submit an I/O operation. You can call this function multiple times to queue up I/O requests before calling :c:func:`io_uring_submit` to tell the kernel to process your queued requests.

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

  **Return value**: a pointer to :c:struct:`io_uring_sqe` that represents a vacant SQE. NULL is returned if the submission queue is full.

  Please see the :ref:`submission introduction code snippet <submission-intro-snippet>` for example usage.


  .. seealso:: 
    * :ref:`io_uring low-level interface programming <low_level>`
    * :ref:`cp utility with liburing<cp_liburing>`

----

.. c:function:: void io_uring_sqe_set_data(struct io_uring_sqe *sqe, void *data)

  This is an inline convenience function that sets the user data field of the SQE instance passed in.

  **Parameters**

  * `sqe`: the SQE instance for which you want to set the user data.
  * `data`: a pointer to the user data.

----

.. c:function:: void io_uring_sqe_set_flags(struct io_uring_sqe *sqe, unsigned flags)

  This is an inline convenience function that sets the flags field of the SQE instance passed in.

  **Parameters**

  * `sqe`: the SQE instance for which you want to set the user data.
  * `flags`: the flags you want to set. This is a bitmap field. Please see the :ref:`io_uring_enter` reference page for various SQE flags and what they mean.

----

.. c:function:: int io_uring_submit(struct io_uring *ring)

  Submits the SQEs acquired via :c:func:`io_uring_get_sqe` to the kernel. You can call this once after you have called :c:func:`io_uring_get_sqe` multiple times to set up multiple I/O requests.

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

  **Return value**: returns the number of SQEs submitted.

  .. seealso:: 
    * :ref:`io_uring low-level interface programming <low_level>`
    * :ref:`cp utility with liburing<cp_liburing>`

----

.. c:function:: int io_uring_submit_and_wait(struct io_uring *ring, unsigned wait_nr)

  Same as :c:func:`io_uring_submit`, but takes an additional parameter ``wait_nr`` that lets you specify how many completions to wait for. This call will block until ``wait_nr`` submission requests are processed by the kernel and their details placed in the completion queue.

  **Parameters**

  * `wait_nr`: The number of completions to wait for.

  **Return value**: returns the number of SQEs submitted.

----

Submission helpers
------------------

Submission helpers are convenience functions that make it easy to specify the I/O operation you want to request via an SQE. There is one function per supported I/O type.


Please see the :ref:`submission introduction code snippet <submission-intro-snippet>` for example usage of the :c:func:`io_uring_prep_readv` function.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_nop(struct io_uring_sqe *sqe)
  
  This function sets up the submission queue entry pointed to by ``sqe`` with an ``IORING_OP_NOP`` operation, which is a no-op. This kind of operation exists for testing purposes and serves to test the speed and efficiency of the ``io_uring`` interface.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_read(struct io_uring_sqe *sqe, int fd, void *buf, unsigned nbytes, off_t offset)

  This function sets up the submission queue entry pointed to by ``sqe`` with a read operation.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.
  * `buf`: the buffer to copy the read data into.
  * `nbytes`: number of bytes to read.
  * `offset`: absolute offset of the file to read from.

  .. seealso:: 
    * :man:`read(2)`
    * :man:`lseek(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_write(struct io_uring_sqe *sqe, int fd, const void *buf, unsigned nbytes, off_t offset)

  This function sets up the submission queue entry pointed to by ``sqe`` with a write operation.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to write to.
  * `buf`: the buffer to write data from.
  * `nbytes`: number of bytes to write.
  * `offset`: absolute offset of the file to write to.

  .. seealso:: 
    * :man:`write(2)`
    * :man:`lseek(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_readv(struct io_uring_sqe *sqe, int fd, const struct iovec *iovecs, unsigned nr_vecs, off_t offset)

  This function sets up the submission queue entry pointed to by ``sqe`` with a "scatter" read operation, much like :man:`readv(2)` or :man:`preadv(2)`, which are part of Linux's scatter/gather I/O family of system calls.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.
  * `iovecs`: pointer to an array of ``iovec`` structures
  * `nr_vecs`: number of ``iovec`` instances in the array pointed to by the ``iovecs`` argument.
  * `offset`: absolute offset of the file to read from.

  .. seealso:: 
    * :man:`readv(2)`
    * :ref:`cat utility example with liburing <cat_liburing>` which uses this function

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_read_fixed(struct io_uring_sqe *sqe, int fd, void *buf, unsigned nbytes, off_t offset, int buf_index)

  Much like :c:func:`io_uring_prep_read`, this function sets up the submission queue entry pointed to by ``sqe`` with a read operation. The main difference is that this function is designed to work with fixed set of pre-allocated buffers registered via :c:func:`io_uring_register`.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.
  * `buf`: the buffer to copy the read data into.
  * `nbytes`: number of bytes to read.
  * `offset`: absolute offset of the file to read from.
  * `buf_index`: index of the set of pre-allocated buffers to use.

  .. seealso:: 
    * :c:func:`io_uring_register`
    * :man:`read(2)`
    * :man:`lseek(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_writev(struct io_uring_sqe *sqe, int fd, const struct iovec *iovecs, unsigned nr_vecs, off_t offset)

  This function sets up the submission queue entry pointed to by ``sqe`` with a "gather" write operation, much like :man:`writev(2)` or :man:`pwritev(2)`, which are part of Linux's scatter/gather I/O family of system calls.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to write to.
  * `iovecs`: pointer to an array of ``iovec`` structures
  * `nr_vecs`: number of ``iovec`` instances in the array pointed to by the ``iovecs`` argument.
  * `offset`: absolute offset of the file to write to.

  .. seealso:: 
    * :man:`writev(2)`
    * :ref:`cp utility example with liburing <cp_liburing>` which uses this function

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_write_fixed(struct io_uring_sqe *sqe, int fd, const void *buf, unsigned nbytes, off_t offset, int buf_index)

  TODO: fixed buffers example to be added.

  Much like :c:func:`io_uring_prep_read`, this function sets up the submission queue entry pointed to by ``sqe`` with a read operation. The main difference is that this function is designed to work with fixed set of pre-allocated buffers registered via :c:func:`io_uring_register`.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.
  * `buf`: the buffer to copy the read data into.
  * `nbytes`: number of bytes to read.
  * `offset`: absolute offset of the file to read from.
  * `buf_index`: index of the set of pre-allocated buffers to use.

  .. seealso:: 
    * :c:func:`io_uring_register`
    * :man:`read(2)`
    * :man:`lseek(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_fsync(struct io_uring_sqe *sqe, int fd, unsigned fsync_flags)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`fsync(2)` like operation. This causes any "dirty" buffers of the file's data and metadata in the disk cache to be synced to disk.

  .. note::

    It is important to note that queuing up this operation does not guarantee that any write operations that are queued up before this operation will have the data they write to the file synced to disk. This is because operations from the submission queue could be picked up and executed by the kernel in parallel. This sync operation could finish well before other write operations that were queued in front of it. What effect it does have is any of the file's already existing "dirty" buffers--at the time at which this operation is executed--are synced to disk.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.
  * `fsync_flags`: This can either be 0 or ``IORING_FSYNC_DATASYNC``, which makes it act like :man:`fdatasync(2)`.

  .. seealso:: 
    * :c:func:`io_uring_register`
    * :man:`read(2)`
    * :man:`lseek(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_close(struct io_uring_sqe *sqe, int fd)
    
  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`close(2)` like operation. This causes the file descriptor pointed to by ``fd`` to be closed.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor to read from.

  .. seealso::
    * :man:`close(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_openat(struct io_uring_sqe *sqe, int dfd, const char *path, int flags, mode_t mode)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`openat(2)` like operation. This causes the file pointed to by ``path`` to be opened in a path relative to the directory represented by ``dfd`` directory file descriptor.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `dfd`: the directory file descriptor representing a directory relative to which the file is to be opened.
  * `path`: path name of the file to be opened.
  * `flags`: These are access mode flags. The same as in :man:`open(2)`.
  * `mode`: File permission bits applied when creating a new file. The same as in :man:`open(2)`. 

  .. seealso::
    * :man:`openat(2)`
    * :man:`open(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_openat2(struct io_uring_sqe *sqe, int dfd, const char *path, struct open_how *how)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`openat2(2)` like operation. This causes the file pointed to by ``path`` to be opened in a path relative to the directory represented by ``dfd`` directory file descriptor.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `dfd`: the directory file descriptor representing a directory relative to which the file is to be opened.
  * `path`: path name of the file to be opened.
  * `how`: a pointer to a ``open_how`` structure which let you control how exactly you want to open the file. See :man:`openat2(2)` for more details.

  .. seealso::
    * :man:`openat2(2)`
    * :man:`open(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_fallocate(struct io_uring_sqe *sqe, int fd, int mode, off_t offset, off_t len)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`fallocate(2)` like operation. The :man:`fallocate(2)` system call is used to allocate, deallocate, collapse, zero or increase file space for the file represented by the file descriptor ``fd``. See :man:`fallocate(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor of the file to conduct the operation on.
  * `mode`: described the operation to conduct on the file. See :man:`fallocate(2)` for details.
  * `offset`: The offset of the file at which to begin operation.
  * `len`: operation length.

  .. seealso::
    * :man:`fallocate(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_statx(struct io_uring_sqe *sqe, int dfd, const char *path, int flags, unsigned mask, struct statx *statxbuf)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`statx(2)` like operation. The :man:`statx(2)` system call gets meta information on the file pointed to by ``path`` which is filled up into a ``statx`` structure pointed to by ``statxbuf``. See :man:`statx(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `dfd`: depending on the value of this and ``path``, the file pointed to by ``path`` can be interpreted as an absolute, relative to process or relative to the directory referred to by a directory descriptor in ``dfd`` along with other types of interpretations are possible. See :man:`statx(2)` for details.
  * `path`: file path. Interpreted in combination with value in ``dfd``. See :man:`statx(2)` for details.
  * `flags`: this is used to influence how the path name is looked up. It can also influence what sort of synchronization the kernel will do when querying a file on a remote filesystem. See :man:`statx(2)` for details.

  .. seealso::
    * :man:`statx(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_fadvise(struct io_uring_sqe *sqe, int fd, off_t offset, off_t len, int advice)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`posix_fadvise(2)` like operation. The :man:`posix_fadvise(2)` system call lets the application advise the operating system how it plans to access data in the file represented by the file descriptor ``fd``--sequentially, randomly or otherwise. This is with the intention to better the performance of the application. See :man:`posix_fdavise(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the file descriptor of the file to give advice about.
  * `offset`: the offset of the file starting with which the advice applies.
  * `len`: the length until which the advice applies.

  .. seealso::
    * :man:`posix_fadvise(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_madvise(struct io_uring_sqe *sqe, void *addr, off_t length, int advice)

  This function sets up the submission queue entry pointed to by ``sqe`` with an :man:`madvise(2)` like operation. The :man:`madvise(2)` system call lets the application advise the operating system on memory pointed to by ``addr`` up to ``length`` bytes. The advice could be on how the application plans to access that said range of memory (sequentially, randomly or otherwise) or if the operating system should not share it when the process forks children, among other things. This is with the intention to better the performance of the application. See :man:`mdavise(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `addr`: starting address of the memory range to which the advice applies.
  * `len`: the length until which the advice applies.

  .. seealso::
    * :man:`madvise(2)`


----

.. image:: https://img.shields.io/badge/linux%20kernel-5.7-green
    :align: right

.. c:function:: void io_uring_prep_splice(struct io_uring_sqe *sqe, int fd_in, loff_t off_in, int fd_out, loff_t off_out, unsigned int nbytes, unsigned int splice_flags)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`splice(2)` like operation. The :man:`splice(2)` system call copies data between two file descriptors (``fd_in`` and ``fd_out``) without copying data between kernel address space and user address space. However, one of the file descriptors must represent a pipe. See :man:`splice(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd_in`: the file descriptor to read from.
  * `off_in`: Has to be NULL if ``fd_in`` refers to a pipe. If ``fd_in`` is not a pipe and:

    * ``off_in`` is NULL, then data from ``fd_in`` is read from its file offset and the file offset is adjusted accordingly.
    * ``off_in`` is non-NULL, then then ``off_in`` must point  to  a  buffer  which specifies  the starting offset from which bytes will be read from ``fd_in``. In this case, the file offset of ``fd_in`` is left unchanged.
  * `fd_out` and `off_out`: Analogous statement such as those for ``fd_in`` and ``off_in`` apply for these arguments.
  * `nbytes`: number of bytes to copy
  * `flags`: a bit mask that influences the copy. See :man:`splice(2)` for details.

  .. seealso::
    * :man:`splice(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.3-green
    :align: right

.. c:function:: void io_uring_prep_recvmsg(struct io_uring_sqe *sqe, int fd, struct msghdr *msg, unsigned flags)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`recvmsg(2)` like operation. The :man:`recvmsg(2)` system call is used to read data from a socket. It uses a ``msghdr`` structure to reduce the number of arguments it takes. This call works with both connection-oriented (like TCP) and connectionless (like UDP) sockets. See :man:`recvmsg(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the socket to read from.
  * `msg`: pointer to an ``msghdr`` structure.
  * `flags`: a bit mask that influences the read. See :man:`recvmsg(2)` for details.

  .. seealso::
    * :man:`recvmsg(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.3-green
    :align: right

.. c:function:: void io_uring_prep_sendmsg(struct io_uring_sqe *sqe, int fd, const struct msghdr *msg, unsigned flags)

    The same as :c:func:`io_uring_prep_recvmsg`, but for writing to a socket.

  .. seealso::
    * :man:`sendmsg(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_recv(struct io_uring_sqe *sqe, int sockfd, void *buf, size_t len, int flags)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`recv(2)` like operation. The :man:`recv(2)` system call is used to read data from a socket. It uses a ``msghdr`` structure to reduce the number of arguments it takes. This call works with both connection-oriented (like TCP) and connectionless (like UDP) sockets. Without the ``flags`` argument, it is the exact equivalent of :man:`read(2)` except one small difference while dealing with zero-length datagrams. See :man:`recv(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the socket to read from.
  * `buf`: pointer to a buffer to read data into.
  * `len`: count of bytes to read.
  * `flags`: a bit mask that influences the read. See :man:`recv(2)` for details.

  .. seealso::
    * :man:`recv(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_send(struct io_uring_sqe *sqe, int sockfd, const void *buf, size_t len, int flags)

    The same as :c:func:`io_uring_prep_recv`, but for writing to a socket.

  .. seealso::
    * :man:`send(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.5-green
    :align: right

.. c:function:: void io_uring_prep_accept(struct io_uring_sqe *sqe, int fd, struct sockaddr *addr, socklen_t *addrlen, int flags)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`accept4(2)` like operation. The :man:`accept4(2)` system call is used with connection-oriented socket types (SOCK_STREAM, SOCK_SEQPACKET). It extracts the first connection request on the queue of pending connections for the listening socket ``fd``. With the ``flags`` argument set to 0, :man:`accept4(2)` is the exact equivalent of :man:`accept(2)`. See :man:`accept4(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the listening socket.
  * `addr`: pointer to a ``sockaddr`` structure. This will be filled with the address of the peer.
  * `addrlen`: pointer to ``socklen_t``. A value-result argument that must be filled in with the size of the ``sockaddr`` structure for the call and which will be set to the size of the peer address.
  * `flags`: a bit mask that influences the system call. See :man:`accept4(2)` for details.

  .. seealso::
    * :man:`accept4(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.5-green
    :align: right

.. c:function:: void io_uring_prep_connect(struct io_uring_sqe *sqe, int fd, struct sockaddr *addr, socklen_t addrlen)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`connect(2)` like operation. The :man:`connect(2)` system call is used to connect the socket referred to in ``fd`` to the address specified in ``addr``. See :man:`connect(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: the listening socket.
  * `addr`: pointer to a ``sockaddr`` structure containing the address of the peer.
  * `addrlen`: pointer to ``socklen_t``. A value-result argument that must be filled in with the size of the ``sockaddr`` structure for the call and which will be set to the size of the peer address.

  .. seealso::
    * :man:`connect(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: void io_uring_prep_epoll_ctl(struct io_uring_sqe *sqe, int epfd, int fd, int op, struct epoll_event *ev)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`epoll_ctl(2)` like operation. The :man:`epoll_ctl(2)` system call is used to add or remove modify entries in the interest list of the :man:`epoll(7)` instance referred by ``epfd``. The add, remove or modify operation specified by ``op`` is applied on the file descriptor ``fd``. See :man:`epoll_ctl(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `epfd`: file descriptor representing and ``epoll`` instance.
  * `fd`: the file descriptor to add, delete or modify.
  * `op`: the ``epoll`` operation to perform (``EPOLL_CTL_ADD``, ``EPOLL_CTL_DEL`` or ``EPOLL_CTL_MOD``).
  * `ev`: pointer to an ``epoll_event`` structure.

  .. seealso::
    * :man:`epoll_ctl(2)`
    * :man:`epoll(7)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_poll_add(struct io_uring_sqe *sqe, int fd, short poll_mask)

  This function sets up the submission queue entry pointed to by ``sqe`` with a :man:`poll(2)` like operation to add a file descriptor to ``poll``'s interest list and to listen to events specified in ``poll_mask``. Unlike ``poll`` or ``epoll`` without ``EPOLLONESHOT``, this interface always works in one-shot mode. That is, once the poll operation is completed, it will have to be resubmitted. See :man:`poll(2)` for more details.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `fd`: file descriptor to poll for events.
  * `poll_mask`: bit mask containg events to listen for.

  .. seealso::
    * :man:`poll(2)`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: void io_uring_prep_poll_remove(struct io_uring_sqe *sqe, void *user_data)

   Remove from a request from monitoring by :man:`poll(2)`.

  **Parameters**

  * `sqe`: pointer to an SQE as generally returned by :c:func:`io_uring_get_sqe`.
  * `user_data`: pointer to user data. The request associated with this user data is removed from further monitoring.

  .. seealso::
    * :man:`poll(2)`

