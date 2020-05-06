.. Lord of the Uring documentation master file, created by
   sphinx-quickstart on Tue Apr  7 23:00:08 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Lord of the ``io_uring`` documentation!
==================================================

Overview
--------

libuv is a multi-platform support library with a focus on asynchronous I/O. It
was primarily developed for use by `Node.js`_, but it's also used by `Luvit`_,
`Julia`_, `pyuv`_, and `others`_.

.. note::
    In case you find errors in this documentation you can help by sending
    `pull requests <https://github.com/libuv/libuv>`_!

.. _Node.js: https://nodejs.org
.. _Luvit: https://luvit.io
.. _Julia: https://julialang.org
.. _pyuv: https://github.com/saghul/pyuv
.. _others: https://github.com/libuv/libuv/wiki/Projects-that-use-libuv


Features
--------

* Full-featured event loop backed by epoll, kqueue, IOCP, event ports.
* Asynchronous TCP and UDP sockets
* Asynchronous DNS resolution
* Asynchronous file and file system operations
* File system events
* ANSI escape code controlled TTY
* IPC with socket sharing, using Unix domain sockets or named pipes (Windows)
* Child processes
* Thread pool
* Signal handling
* High resolution clock
* Threading and synchronization primitives


Documentation
-------------

.. toctree::
    :maxdepth: 1
    :caption: Introduction
    
    async_intro
    what_is_io_uring
    low_level
    liburing_examples
    reference

.. toctree::
    :maxdepth: 1
    :caption: Tutorial
    :name: sec-tutorial

    tutorial/index
    tutorial/cat_liburing
    tutorial/cp_liburing
    tutorial/webserver_liburing
    tutorial/link_liburing
    tutorial/fixed_buffers
    tutorial/sq_poll

.. toctree::
    :maxdepth: 1
    :caption: liburing Reference
    :name: sec-liburing-reference

    ref-liburing/index
    ref-liburing/sqe
    ref-liburing/cqe
    ref-liburing/supported_caps
    ref-liburing/setup_teardown
    ref-liburing/submission
    ref-liburing/completion
    ref-liburing/advanced_usage
    ref-liburing/low_level

.. toctree::
    :maxdepth: 1
    :caption: io_uring Reference
    :name: sec-iouring-reference

    ref-iouring/index
    ref-iouring/io_uring_setup
    ref-iouring/io_uring_enter
    ref-iouring/io_uring_register

Downloads
---------

libuv can be downloaded from `here <https://dist.libuv.org/dist/>`_.


Installation
------------

Installation instructions can be found in `the README <https://github.com/libuv/libuv/blob/master/README.md>`_.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

