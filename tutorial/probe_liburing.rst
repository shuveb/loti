.. _probing_liburing:

Probing supported capabilities
==============================
 The following program demonstrates how to use :c:func:`io_uring_get_probe`. It prints if an operation is supported or not.

.. code-block:: c
    :caption: A program to print details about supported and unsupported operations

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

Source code
-----------
Source code for this and other examples is `available on Github <https://github.com/shuveb/loti-examples>`_.