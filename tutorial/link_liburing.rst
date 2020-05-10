.. _link_liburing:

Linking requests
================
In ``io_uring``, completions do not arrive in the order that submissions were issues in. This was discussed in the chapter :ref:`The Low-level io_uring Interface <low_level>`. What if you want to force certain operations to happen in order? This is possible by linking requests together. The example here shows you how it's done.


.. code-block:: c

    #include <stdio.h>
    #include <string.h>
    #include <fcntl.h>
    #include "liburing.h"

    #define FILE_NAME   "/tmp/io_uring_test.txt"
    #define STR         "Hello, io_uring!"
    char buff[32];

    int link_operations(struct io_uring *ring) {
        struct io_uring_sqe *sqe;
        struct io_uring_cqe *cqe;

        int fd = open(FILE_NAME, O_RDWR|O_TRUNC|O_CREAT, 0644);
        if (fd < 0 ) {
            perror("open");
            return 1;
        }

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        io_uring_prep_write(sqe, fd, STR, strlen(STR), 0 );
        sqe->flags |= IOSQE_IO_LINK;

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        io_uring_prep_read(sqe, fd, buff, strlen(STR),0);
        sqe->flags |= IOSQE_IO_LINK;

        sqe = io_uring_get_sqe(ring);
        if (!sqe) {
            fprintf(stderr, "Could not get SQE.\n");
            return 1;
        }

        io_uring_prep_close(sqe, fd);

        io_uring_submit(ring);

        for (int i = 0; i < 3; i++) {
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
        printf("Buffer contents: %s\n", buff);
    }

    int main() {
        struct io_uring ring;

        int ret = io_uring_queue_init(8, &ring, 0);
        if (ret) {
            fprintf(stderr, "Unable to setup io_uring: %s\n", strerror(-ret));
            return 1;
        }
        link_operations(&ring);
        io_uring_queue_exit(&ring);
        return 0;
    }


This is a fairly simple program. We open an empty file, write a string to it, read the string from the file and then close it. Since ``io_uring`` does not guarantee that operations submitted will be executed in order, it might be a problem for our program. Since it is an empty file that is truncated at every run of the program, there will be nothing to read if the write operation does not complete before the read. Also, if the close operation completed before either the read or write operations or both operations, those might fail as well. For this reason, this program links operations with the ``IOSQE_IO_LINK`` flag. This ensures that operations are executed one after the other.

This program is fairly simple to understand. In the ``link_operations()`` function, we call :c:func:`io_uring_prep_write`, but then set the ``IOSQE_IO_LINK`` flag on it so that the next operation is linked to this operation. Next, we call :c:func:`io_uring_prep_read`, which is now linked to the previous write operation. We set the ``IOSQE_IO_LINK`` flag on this operation as well so that the subsequent close operation, which we set up with :c:func:`io_uring_prep_close` is linked with this. This causes ``io_uring`` to execute the write, read and close operations one after the other.

Failure in the chain
--------------------
When linked operations are involved, the failure of a one operation will cause all subsequent linked operations to fail with the error "Operation cancelled.". Normally, you should this output if you run this program on kernel version 5.6 or above:

::

    ➜  cmake-build-debug ./link
    Result of the operation: 16
    Result of the operation: 16
    Result of the operation: 0
    Buffer contents: Hello, io_uring!

If we switch them ``open()`` statement from 

.. code-block:: c

   int fd = open(FILE_NAME, O_RDWR|O_TRUNC|O_CREAT, 0644);

to this, in which we open the file in a write-only mode:

.. code-block:: c

    int fd = open(FILE_NAME, O_WRONLY|O_TRUNC|O_CREAT, 0644);

our write operation should go through, but our read operation will fail since the file is now open in write-only mode. Since the subsequent close operation is linked to the read operation, it should fail, too. The output of this flawed program will now be:

::

    ➜  cmake-build-debug ./link
    Error in async operation: Bad file descriptor
    Result of the operation: -9
    Error in async operation: Operation canceled
    Result of the operation: -125

The first error ("Bad file descriptor") you're seeing is from the failing read operation. The next error you're seeing ("Operation cancelled") is ``io_uring`` cancelling the linked close operation.

.. note::
   
   Please note that you need kernel 5.6 or better for this example to work since the read, write and close operations aren't supported in earlier versions.

Source code
-----------
Source code for this and other examples is `available on Github <https://github.com/shuveb/loti-examples>`_.