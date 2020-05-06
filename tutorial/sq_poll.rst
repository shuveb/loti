.. _sq_poll:

Submission Queue Polling
========================

Reducing the number of system calls is a major aim for ``io_uring``. To this end, ``io_uring`` lets you submit I/O requests without you having to make a single system call. This is done via a special submission queue polling feature that ``io_uring`` supports. In this mode, ``io_uring`` starts a special kernel thread that polls the submission queues for entries. That way, you just have to submit entries into the shared queue and the kernel thread should see it and pick up the submission queue entry. This is a benefit of having a queue that is shared between user space and the kernel.

How to use this mode? The idea is simple. You tell ``io_uring`` that you want use this mode by setting the ``IORING_SETUP_SQPOLL`` flag in the ``io_uring_params`` structure's ``flags`` member. If the kernel thread that starts along with your process does not see any submission for a period of time, it will quit and you'll need to use the :c:func:`io_uring_enter` system call once more to wake it up. This period of time is configurable via :c:struct`io_uring_params` structure's ``sq_thread_idle`` member. If you keep the submissions coming however, the kernel poller thread should never sleep.

.. note::

    The kernel's poller thread can take up a lot of CPU. You need to be careful about using this feature. Setting a very large ``sq_thread_idle`` value will cause the kernel thread to continue to consume CPU while there are no submissions happening from your program. It is a good idea to use this feature if you truly expect to handle large amounts of I/O. And even when you do so, it might be a good idea to set the poller thread's idle value to a few seconds at most.

If you need to use this feature however, you will also need to use it with :c:func:`io_uring_register_files`. Using this, you tell the kernel about an array of file descriotprs beforehand. This is just a regular array of file descriotprs you open before initiating I/O. During submission time, rather than passing a file descriptor as you normally would to calls like :c:func:`io_uring_prep_read` or :c:func:`io_uring_prep_write`, you need to set the ``IOSQE_FIXED_FILE`` flag in the ``flags`` field of the SQE and pass the index of the file descriptor in the array of file descriptors you setup before. 


::

    ➜  sudo ./sq_poll
    [sudo] password for shuveb: 
    Kernel thread io_uring-sq is not running.
    Result of the operation: 36
    Result of the operation: 35
       1750 ?        00:00:00 io_uring-sq
    Kernel thread io_uring-sq found running...
    Result of the operation: 36
    Result of the operation: 35
    Contents read from file:
    What is this life if, full of care,
    We have no time to stand and stare.%                                                                         ➜   


::

    ➜  sudo bpftrace -e 'tracepoint:io_uring:io_uring_submit_sqe {printf("%s(%d)\n", comm, pid);}'
    Attaching 1 probe...
    fixed_buffers(30336)
    fixed_buffers(30336)
    fixed_buffers(30336)
    fixed_buffers(30336)

::

    ➜  sudo bpftrace -e 'tracepoint:io_uring:io_uring_submit_sqe {printf("%s(%d)\n", comm, pid);}'
    io_uring-sq(30429)
    io_uring-sq(30429)
    io_uring-sq(30429)
    io_uring-sq(30429)

