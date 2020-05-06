.. _advanced_usage:

Advanced usage
==============

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: int io_uring_register_buffers(struct io_uring *ring, const struct iovec *iovecs, unsigned nr_iovecs)

    Register buffers to be used with fixed buffer operations such as :c:func:`io_uring_prep_write_fixed` and :c:func:`io_uring_prep_read_fixed`.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `iovecs`: pointer to ``iovec`` structure(s).
    * `nr_iovecs`: number of structures ``iovec`` structures

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * Example program :ref:`Fixed Buffers <fixed_buffers>` that uses this function.
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: int io_uring_unregister_buffers(struct io_uring *ring)

    Unregister buffers registered by :c:func:`io_uring_register_buffers`.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * Example program :ref:`Fixed Buffers <fixed_buffers>` that uses this function.
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: int io_uring_register_files(struct io_uring *ring, const int *files, unsigned nr_files)

   Register file descriptors to be used in subsequent operations, like those that involve submission queue pollig (``IORING_SETUP_SQPOLL``).

   **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `files`: pointer to an array of file descriptors.
    * `nr_files`: number of file descriptors in the array.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * Example program :ref:`Submission queue polling <sq_poll>` that uses this function.
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.1-green
    :align: right

.. c:function:: int io_uring_unregister_files(struct io_uring *ring)

    Unregister file descriptors registered by :c:func:`io_uring_register_files`.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * Example program :ref:`Submission queue polling <sq_poll>` that uses this function.
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.5-green
    :align: right

.. c:function:: int io_uring_register_files_update(struct io_uring *ring, unsigned off, int *files, unsigned nr_files)

    Update the set of file descriptors registered with :c:func:`io_uring_register_files`.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `off`: offset in the file descriptors array to update from.
    * `files`: array of file descriptors.
    * `nr_files`: number of file descriptors in the array supplied.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.2-green
    :align: right

.. c:function:: int io_uring_register_eventfd(struct io_uring *ring, int fd)

    By registering an :man:`eventfd(2)` file descriptor with ``io_uring``, it is possible to get notified of completion events on an ``io_uring`` instance and this function lets you do just that.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `fd`: the :man:`eventfd(2)` file descriptor

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_register_eventfd_async(struct io_uring *ring, int fd)

    Same as :c:func:`io_uring_register_eventfd`, except notifications are only posted for events that complete in an async manner. This means that events that complete inline while being submitted do not trigger a notification event.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `fd`: the :man:`eventfd(2)` file descriptor

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.2-green
    :align: right

.. c:function:: int io_uring_unregister_eventfd(struct io_uring *ring)

    Unregister the registered :man:`eventfd(2)` file descriptor.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

.. seealso::
    
   * :ref:`io_uring_register <io_uring_register>`

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_register_personality(struct io_uring *ring)

    This operation registers credentials of the running application with ``io_uring``, and returns an ID associated with these credentials. Applications wishing to share a ring between separate users/processes can pass in this credential ID in the SQE ``personality`` field. If set, that particular sqe will be issued with these credentials.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.

    **Return value**: returns the personality ID on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

----

.. image:: https://img.shields.io/badge/linux%20kernel-5.6-green
    :align: right

.. c:function:: int io_uring_unregister_personality(struct io_uring *ring, int id)

    Unregisters a personality previously registered with :c:func:`io_uring_register_personality`.

    **Parameters**

    * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
    * `id`: the personality ID to unregister.

    **Return value**: returns 0 on success and ``-errono`` on failure. You can use :man:`strerror(3)` to get a human readable version of the reason for failure.

