.. Lord of the Uring documentation master file, created by
   sphinx-quickstart on Tue Apr  7 23:00:08 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Lord of the io_uring
===============================

``io_uring`` is a powerful new way to do asynchronous I/O programming under Linux. Without the limitations of previous generations of I/O subsystems, ``io_uring`` treats regular files and socket equally. For more detals on ``io_uring``, please see the chapter :ref:`what_is_io_uring`.


About
-----
This ``io_uring`` guide was created by `Shuveb Hussain <https://twitter.com/shuveb>`_, who is also the author of the Linux-focused blog, `unixism.net <https://unixism.net>`_.

Contributing
------------
The source code repository for this guide is `here on Github <https://github.com/shuveb/loti>`_. Please send me a pull request. If you find any bugs in either the documentation or the included source code examples, please raise an issue on GitHub. The repository for the example programs is maintained separately. Please see below.

Source code for examples
------------------------
Source code for all the example programs in this guide are available `on Github <https://github.com/shuveb/loti-examples>`_. If you find bugs in the examples, please raise issues on Github. I want to keep these examples simple and to the point. I many not merge pull requests that add features for that reason. Pull requests that fix bugs are welcome.

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

