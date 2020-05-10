liburing Examples
=================
As discussed in previous chapters, while being aware of how the low-level ``io_uring`` interface works is most certainly helpful, you might want to use ``liburing`` in more serious programs. ``liburing`` provides a more high-level interface to ``io_uring`` making it far more productive for you, while also removing a lot of boilerplate code, making your programs a lot shorter and to-the-point.


.. toctree::
    :maxdepth: 1
    :name: toc-tutorial

    cat_liburing
    cp_liburing
    webserver_liburing
    probe_liburing
    link_liburing
    fixed_buffers
    sq_poll
    register_eventfd

Source code
-----------
Source code for this and other examples is `available on Github <https://github.com/shuveb/loti-examples>`_.