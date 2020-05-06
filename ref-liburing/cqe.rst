.. _cqe:

CQE: Completion Queue Event
===========================
There is a completion queue event (CQE) added by the kernel for each submission queue entry that is submitted. The CQE contains the status of the operation submitted as part of the SQE. There is exactly one CQE added to the completion queue as a result of the kernel processing an SQE. There is a one-to-one correlation between the two.

.. note::
    This structure is part of the raw ``io_uring`` interface and is defined in ``io_uring.h``.

``struct io_uring_cqe``

.. code-block:: c

    struct io_uring_cqe {
        __u64	user_data;	/* sqe->data submission passed back */
        __s32	res;		/* result code for this event */
        __u32	flags;
    };

