.. _low_level_interface:

###################
Low-level interface
###################


.. c:struct:: io_uring_params
.. code-block:: c

    struct io_uring_params {
        __u32 sq_entries;
        __u32 cq_entries;
        __u32 flags;
        __u32 sq_thread_cpu;
        __u32 sq_thread_idle;
        __u32 features;
        __u32 wq_fd;
        __u32 resv[3];
        struct io_sqring_offsets sq_off;
        struct io_cqring_offsets cq_off;
    };

----

.. _setup_time_flags:

Setup time flags 
----------------

These are passed to :c:func:`io_uring_setup` and :c:func:`io_uring_queue_init`.

.. c:macro:: IORING_SETUP_IOPOLL

    io_context is polled 

.. c:macro:: IORING_SETUP_SQPOLL

    SQ poll thread

.. c:macro:: IORING_SETUP_SQ_AFF

    sq_thread_cpu is valid

.. c:macro:: IORING_SETUP_CQSIZE

    app defines CQ size

.. c:macro:: IORING_SETUP_CLAMP

    clamp SQ/CQ ring sizes

.. c:macro:: IORING_SETUP_ATTACH_WQ

    attach to existing wq

----

.. _sqe_flags:

SQE Flags
---------

       IOSQE_FIXED_FILE
              When  this flag is specified, fd is an index into the files array registered with the io_uring in‐
              stance (see the IORING_REGISTER_FILES section of the  io_uring_register(2)  man  page).  Available
              since 5.1.

       IOSQE_IO_DRAIN
              When  this  flag  is  specified, the SQE will not be started before previously submitted SQEs have
              completed, and new SQEs will not be started before this one completes. Available since 5.2.

       IOSQE_IO_LINK
              When this flag is specified, it forms a link with the next SQE in the submission ring.  That  next
              SQE  will not be started before this one completes.  This, in effect, forms a chain of SQEs, which
              can be arbitrarily long. The tail of the chain is denoted by the first SQE that does not have this
              flag  set.   This flag has no effect on previous SQE submissions, nor does it impact SQEs that are
              outside of the chain tail. This means that multiple chains can be executing in parallel, or chains
              and individual SQEs. Only members inside the chain are serialized. A chain of SQEs will be broken,
              if any request in that chain ends in error. io_uring considers any  unexpected  result  an  error.
              This  means  that, eg, a short read will also terminate the remainder of the chain.  If a chain of
              SQE links is broken, the remaining unstarted part of the chain will be  terminated  and  completed
              with -ECANCELED as the error code. Available since 5.3.

       IOSQE_IO_HARDLINK
              Like  IOSQE_IO_LINK, but it doesn't sever regardless of the completion result.  Note that the link
              will still sever if we fail submitting the parent request, hard links are only  resilient  in  the
              presence  of  completion results for requests that did submit correctly. IOSQE_IO_HARDLINK implies
              IOSQE_IO_LINK.  Available since 5.5.

       IOSQE_ASYNC
              Normal operation for io_uring is to try and issue an sqe as non-blocking first, and if that fails,
              execute it in an async manner. To support more efficient overlapped operation of requests that the
              application knows/assumes will always (or most of the time) block, the application can ask for  an
              sqe to be issued async from the start. Available since 5.6.
