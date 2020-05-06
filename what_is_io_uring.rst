.. _what_is_io_uring:

What is io_uring?
=================
io_uring is a new asynchronous I/O API for Linux created by Jens Axboe from Facebook. It aims to provide an API without the limitations of the current :c:func:`select`, :c:func:`poll`, ``epoll`` or ``aio`` family of system calls, which we discussed in the previous section. Given that users of asynchronous programming models choose it in the first place for performance reasons, it makes sense to have an API that has very low performance overheads. We shall see how io_uring achieves this in subsequent sections.


The io_uring interface
----------------------
The very name io_uring comes from the fact that the interfaces uses ring buffers as the main interface for kernel-user space communication. While there are system calls, they are kept to a minimum and there is a polling mode [TODO: add reference here] you can use to reduce the need to make system calls as much as possible.

The mental model
^^^^^^^^^^^^^^^^
The mental model you need to construct in order to use io_uring to build programs that process I/O asynchronously is fairly simple. 

* There are 2 ring buffers, one for submission of requests (submission queue or SQ) and the other that informs you about completion of those requests (completion queue or CQ).
* These ring buffers are shared between kernel and user space. You set these up with io_uring_setup and then mapping them into user space with a 3 :c:func:`mmap` calls.
* You can tell io_uring what you need to get done (read or write a file, accept client connections, etc), which you describe as part of a submission queue entry (SQE) and add it to the tail of the submission ring buffer.
* You then tell the kernel via the io_uring_enter [TODO: add reference] system call that you've added an SQE to the submission queue ring buffer. You can add multiple SQEs before making the system call as well.
* Optionally, io_uring_enter can also wait for a number of requests to be processed by the kernel before it returns so you know you're ready to read off the completion queue for results.
* The kernel processes requests submitted and adds completion queue events (CQEs) to the tail of the completion queue ring buffer.
* You read CQEs off the head of the completion queue ring buffer. There is one CQE corresponding to each SQE and it contains the status of that particular request.
* You continue adding SQEs and reaping CQEs as you need.
* There is a polling mode available, where the kernel polls for new entries in the submission queue. This avoids the overhead of calling io_uring_enter() every time you submit entries for processing.

io_uring performance
--------------------
[Add number from the io_uring paper]
Because of the shared ring buffers between the kernel and user space, io_uring can be a zero-copy system. Copying bytes around becomes necessary when there are system calls involved. But since the bulk of the communication in io_using is via these shared buffers, this huge performance overhead is completely avoided. While system calls (and we're used to making them a lot) many not seem like a significant overhead, in high performance applications, making a lot of them will begin to matter. Also, system calls are not as cheap as they used to be. Throw in the workarounds the operating system has in place to deal with `Specter and Meltdown <https://meltdownattack.com/>`_, we are talking about non-trivial overheads. So, avoiding system calls as much as possible is a fantastic idea in high-performance applications indeed.

While in synchronous programming or even when using asynchronous programming interfaces under Linux, there is at least one system call involved in the submission of each request. In ``io_uring``, you can add several requests, simply by adding multiple SQEs each describing the I/O operation you want and make a single call to io_uring_enter. For starers, that's a win right there. But it gets better.

You can have the kernel poll and pick up your SQEs for processing as you add them to the submission queue. This avoids the io_uring_enter call you need to make to tell the kernel to pick up SQEs. For high-performance applications, this means even lesser system call overheads.

With the clever use of shared ring buffers, ``io_uring`` performance is really memory-bound, since in polling mode, we can do away with system calls altogether. It is important to remember that performance benchmarking is a relative process with some kind of a common point of reference. According to the `io_uring paper <https://kernel.dk/io_uring.pdf>`_, on a reference machine, in polling mode, ``io_uring`` managed to clock 1.7M 4k IOPS, while ``aio`` manages 608k. Although much more than double, this isn't a fair comparison since ``aio`` doesn't feature a polled mode. But even when polled mode is disabled, ``io_uring`` hits 1.2M IOPS, close to double that of ``aio``.

To check the raw throughput of the ``io_uring`` interface, there is a no-op request type. With this, on the reference machine, ``io_uring`` achieves 20M messages per second.

An example using the low-level API
----------------------------------
Writing a small program that reads files and prints them on to the console, like how the Unix ``cat`` utility does might be a good starting point to get your hands wet with the ``io_uring`` API. Please see the next chapter for one such example.

Just use liburing
-----------------
While being acquainted with the low-level ``io_uring`` API is most certainly a good thing, in real, serious programs you probably want to use the higher-level interface provided by liburing. If liburing never existed, you'd have built some abstraction layer over the low-lever ``io_uring`` interface. liburing does that for you and it is a well thought-out interface as well. In short, you should probably put in some effort to understand how the low-level ``io_uring`` interface works, but by default you should really be using ``liburing`` in your programs. 

While there is a reference section here for it, there are some examples we'll see that use liburing in the next chapter.

