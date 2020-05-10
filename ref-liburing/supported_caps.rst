.. _supported_caps:

Supported capabilities
======================
Functions that allow you to check for supported operations and capabilities.

.. c:struct:: io_uring_probe
.. code-block:: c

    struct io_uring_probe {
        __u8 last_op;	/* last opcode supported */
        __u8 ops_len;	/* length of ops[] array below */
        __u16 resv;
        __u32 resv2[3];
        struct io_uring_probe_op ops[0];
    };

----

.. c:struct:: io_uring_probe_op
.. code-block:: c

    struct io_uring_probe_op {
    __u8 op;
    __u8 resv;
    __u16 flags;	/* IO_URING_OP_* flags */
    __u32 resv2;
    };

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: struct io_uring_probe *io_uring_get_probe_ring(struct io_uring *ring)

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

    **Return value**: on success, a pointer to a :c:struct:`io_uring_probe` which is used to probe the capabilities of the ``io_uring`` subsystem of the running kernel. The :c:struct:`io_uring_probe` contains the list of supported operations. On failure, NULL is returned.

    .. note::
        This function allocates memory to hold the structure :c:struct:`io_uring_probe`. It is your responsibility to free it once you're done.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: struct io_uring_probe *io_uring_get_probe(void)

    Returns a pointer to a :c:struct:`io_uring_probe` which is used to probe the capabilities of the ``io_uring`` subsystem of the running kernel. The :c:struct:`io_uring_probe` contains the list of supported operations.

    This function is pretty much the same as :c:func:`io_uring_get_probe_ring`, except that there is no need for you to setup a ring or have a reference to one. It sets up a temporary ring so that it can fetch the details of supported operations for you. It then destroys the ring before it returns.

    .. note::
        This function allocates memory to hold the structure :c:struct:`io_uring_probe`. It is your responsibility to free it once you're done.

.. seealso::

    :ref:`Example program <probing_liburing>` to print supported ``io_uring`` operation in the running kernel.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_opcode_supported(struct io_uring_probe *p, int op)

    Function to determine of an ``io_uring`` operation is supported by your kernel. Returns 0 if the operation is not supported and a non-zero value if support is present. Take a look at :ref:`supported operations example program <probing_liburing>` to see this function in action.

    **Parameters**

    * `p`: pointer to a ``io_uring_probe`` structure.
    * `op`: operation you want to check support for. One of the IO_URING_OP_* macros.

    **Return value**: 0 if operation not supported, 1 otherwise.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_register_probe(struct io_uring *ring, struct io_uring_probe *p, unsigned nr)

    Low-level function that lets you get ``io_uring`` capabilities.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `p`: pointer to ``io_uring_probe`` structure.
    * `nr`: number of structures in the array that ``p`` points to.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.
