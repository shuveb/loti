.. _sqe:

SQE: Submission Queue Entry
===========================
The submission queue entry (SQE) is what you use to tell ``io_uring`` what you want to do, like read a file, write a file, accept a connection on a socket, etc.

.. note::
    This structure is part of the raw ``io_uring`` interface and is defined in ``io_uring.h``.


``struct io_uring_sqe``
-----------------------
.. code-block:: c

    struct io_uring_sqe {
        __u8	opcode;		/* type of operation for this sqe */
        __u8	flags;		/* IOSQE_ flags */
        __u16	ioprio;		/* ioprio for the request */
        __s32	fd;		/* file descriptor to do IO on */
        union {
            __u64	off;	/* offset into file */
            __u64	addr2;
        };
        union {
            __u64	addr;	/* pointer to buffer or iovecs */
            __u64	splice_off_in;
        };
        __u32	len;		/* buffer size or number of iovecs */
        union {
            __kernel_rwf_t	rw_flags;
            __u32		fsync_flags;
            __u16		poll_events;
            __u32		sync_range_flags;
            __u32		msg_flags;
            __u32		timeout_flags;
            __u32		accept_flags;
            __u32		cancel_flags;
            __u32		open_flags;
            __u32		statx_flags;
            __u32		fadvise_advice;
            __u32		splice_flags;
        };
        /* data to be passed back at completion time */
        __u64	user_data;	
        union {
            struct {
                /* pack this to avoid bogus arm OABI complaints */
                union {
                    /* index into fixed buffers, if used */
                    __u16	buf_index;
                    /* for grouped buffer selection */
                    __u16	buf_group;
                } __attribute__((packed));
                /* personality to use, if used */
                __u16	personality;
                __s32	splice_fd_in;
            };
            __u64	__pad2[3];
        };
    };

