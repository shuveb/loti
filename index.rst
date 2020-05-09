.. Lord of the Uring documentation master file, created by
   sphinx-quickstart on Tue Apr  7 23:00:08 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Lord of the ``io_uring``s documentation!
===================================================

Overview
--------

``io_uring`` is a powerful new way to do asynchronous I/O programming under Linux. 


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
    tutorial/register_eventfd

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

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

