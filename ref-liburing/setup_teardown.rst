.. _setup_and_tear_down:

###################
Setup and tear down
###################

This section describes functions that help you setup and tear down ``io_uring`` usage in your programs.

.. c:struct:: io_uring
.. code-block:: c

    struct io_uring {
        struct io_uring_sq sq;
        struct io_uring_cq cq;
        unsigned flags;
        int ring_fd;
    };


----

TODO: Are the below two structs really required? Remove them if they are only used internally.

.. c:struct:: io_uring_sq
.. code-block:: c

    struct io_uring_sq {
        unsigned *khead;
        unsigned *ktail;
        unsigned *kring_mask;
        unsigned *kring_entries;
        unsigned *kflags;
        unsigned *kdropped;
        unsigned *array;
        struct io_uring_sqe *sqes;

        unsigned sqe_head;
        unsigned sqe_tail;

        size_t ring_sz;
        void *ring_ptr;
    };

----

.. c:struct:: io_uring_cq
.. code-block:: c

    struct io_uring_cq {
        unsigned *khead;
        unsigned *ktail;
        unsigned *kring_mask;
        unsigned *kring_entries;
        unsigned *koverflow;
        struct io_uring_cqe *cqes;

        size_t ring_sz;
        void *ring_ptr;
    };

----

.. c:function:: int io_uring_queue_init(unsigned entries, struct io_uring *ring, unsigned flags)

    Initializes ``io_uring`` for use in your programs. You'd want to call this function before you get to do anything else with ``io_uring``.

    **Parameters**

    * `entries`: the number of entries you want to request for the submission queue. Each request holds details about one I/O operation.
    * `ring`: pointer to :c:struct:`io_uring` structure which will be filled up by the kernel.
    * `flags`: flags you want to pass. See :ref:`io_uring_setup` for details.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

    
    .. seealso:: :ref:`cat utility using liburing example <eg-cat-uring>`

----

.. c:function:: int io_uring_queue_init_params(unsigned entries, struct io_uring *ring, struct io_uring_params *p)

    Functionally equivalent to :c:func:`io_uring_queue_init`, but additionally takes a pointer to :c:struct:`io_uring_params` structure, allowing you to specify your own :c:struct:`io_uring_params` structure.

    In the :c:struct:`io_uring_params` structure, you can only specify ``flags`` which can be used to set :ref:`various flags <io_uring_setup>` and ``sq_thread_cpu`` and ``sq_thread_idle`` fields, which are used to set the CPU affinity and submit queue idle time. Other fields of the structure are filled up by the kernel on return. When you use :c:func:`io_uring_queue_init`, you don't get to specify these values. This function's existence solves this problem for you.

    TODO: example program would help here.

----

.. c:function:: int io_uring_queue_mmap(int fd, struct io_uring_params *p, struct io_uring *ring)

    This is a low-level function you'll only want to use when you want to control a lot of aspects of the ``io_uring`` initialization. Before calling this function, you should have already called the low-level :c:func:`io_uring_setup`. You can then use this function to :man:`mmap(2)` the rings for you.

    **Parameters**

    * `fd`: the file descriptor returned by :c:func:`io_uring_setup`
    * `p`: a pointer to :c:struct:`io_uring_params`
    * `ring`: a pointer to :c:struct:`io_uring`

    .. seealso:: :ref:`io_uring low-level interface programming <low_level>`

----

.. c:function:: int io_uring_ring_dontfork(struct io_uring *ring)

    Use this call if you do not want child processes of your process inheriting the ring mappings.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as setup by :c:func:`io_uring_queue_init`.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

    .. seealso:: :man:`madvise(2)`, especially ``MADV_DONTFORK``.

----

.. c:function:: void io_uring_queue_exit(struct io_uring *ring)

    Tear down function for ``io_uring``. Unmaps all setup shared ring buffers and closes the low-level ``io_uring`` file descriptor returned by the kernel.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as setup by :c:func:`io_uring_queue_init`.

