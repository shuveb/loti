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


 The following program demonstrates how to use :c:func:`io_uring_get_probe`. It prints if an operation is supported or not.

.. code-block:: c
    :caption: A program to print details about supported and unsupported operations
    :name: eg-supported-ops

    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/utsname.h>
    #include <liburing.h>
    #include <liburing/io_uring.h>

    static const char *op_strs[] = {
            "IORING_OP_NOP",
            "IORING_OP_READV",
            "IORING_OP_WRITEV",
            "IORING_OP_FSYNC",
            "IORING_OP_READ_FIXED",
            "IORING_OP_WRITE_FIXED",
            "IORING_OP_POLL_ADD",
            "IORING_OP_POLL_REMOVE",
            "IORING_OP_SYNC_FILE_RANGE",
            "IORING_OP_SENDMSG",
            "IORING_OP_RECVMSG",
            "IORING_OP_TIMEOUT",
            "IORING_OP_TIMEOUT_REMOVE",
            "IORING_OP_ACCEPT",
            "IORING_OP_ASYNC_CANCEL",
            "IORING_OP_LINK_TIMEOUT",
            "IORING_OP_CONNECT",
            "IORING_OP_FALLOCATE",
            "IORING_OP_OPENAT",
            "IORING_OP_CLOSE",
            "IORING_OP_FILES_UPDATE",
            "IORING_OP_STATX",
            "IORING_OP_READ",
            "IORING_OP_WRITE",
            "IORING_OP_FADVISE",
            "IORING_OP_MADVISE",
            "IORING_OP_SEND",
            "IORING_OP_RECV",
            "IORING_OP_OPENAT2",
            "IORING_OP_EPOLL_CTL",
            "IORING_OP_SPLICE",
            "IORING_OP_PROVIDE_BUFFERS",
            "IORING_OP_REMOVE_BUFFERS",
    };

    int main() {
        struct utsname u;
        uname(&u);
        printf("You are running kernel version: %s\n", u.release);
        struct io_uring_probe *probe = io_uring_get_probe();
        printf("Report of your kernel's list of supported io_uring operations:\n");
        for (char i = 0; i < IORING_OP_LAST; i++ ) {
            printf("%s: ", op_strs[i]);
            if(io_uring_opcode_supported(probe, i))
                printf("yes.\n");
            else
                printf("no.\n");

        }
        free(probe);
        return 0;
    }

Here is the output from the above program as run on Linux kernel 5.6.4

.. highlight:: none

::

    You are running kernel version: 5.6.4-arch1-1
    Report of your kernel's list of supported io_uring operations:
    IORING_OP_NOP: yes.
    IORING_OP_READV: yes.
    IORING_OP_WRITEV: yes.
    IORING_OP_FSYNC: yes.
    IORING_OP_READ_FIXED: yes.
    IORING_OP_WRITE_FIXED: yes.
    IORING_OP_POLL_ADD: yes.
    IORING_OP_POLL_REMOVE: yes.
    IORING_OP_SYNC_FILE_RANGE: yes.
    IORING_OP_SENDMSG: yes.
    IORING_OP_RECVMSG: yes.
    IORING_OP_TIMEOUT: yes.
    IORING_OP_TIMEOUT_REMOVE: yes.
    IORING_OP_ACCEPT: yes.
    IORING_OP_ASYNC_CANCEL: yes.
    IORING_OP_LINK_TIMEOUT: yes.
    IORING_OP_CONNECT: yes.
    IORING_OP_FALLOCATE: yes.
    IORING_OP_OPENAT: yes.
    IORING_OP_CLOSE: yes.
    IORING_OP_FILES_UPDATE: yes.
    IORING_OP_STATX: yes.
    IORING_OP_READ: yes.
    IORING_OP_WRITE: yes.
    IORING_OP_FADVISE: yes.
    IORING_OP_MADVISE: yes.
    IORING_OP_SEND: yes.
    IORING_OP_RECV: yes.
    IORING_OP_OPENAT2: yes.
    IORING_OP_EPOLL_CTL: yes.
    IORING_OP_SPLICE: no.
    IORING_OP_PROVIDE_BUFFERS: no.
    IORING_OP_REMOVE_BUFFERS: no.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_opcode_supported(struct io_uring_probe *p, int op)

    Function to determine of an ``io_uring`` operation is supported by your kernel. Returns 0 if the operation is not supported and a non-zero value if support is present. Take a look at :ref:`supported operations example program <eg-supported-ops>` to see this function in action.

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
