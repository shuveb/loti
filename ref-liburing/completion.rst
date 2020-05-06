.. _completion:

##########
Completion
##########

Introduction
============
These functions deal with completion of I/O requests in ``io_uring``. The snippet below is from the example :ref:`cat with liburing<cat_liburing>`. While there are other possible ways, it shows a general way in which you can use completion related functions. Once a request or multiple requests have been submitted, you can wait for a completion and process it as shown below.

.. code-block:: c

  int get_completion_and_print(struct io_uring *ring) {
      struct io_uring_cqe *cqe;
      int ret = io_uring_wait_cqe(ring, &cqe);

      if (ret < 0) {
          perror("io_uring_wait_cqe");
          return 1;
      }

      if (cqe->res < 0) {
          /* The system call invoked asynchonously failed */
          return 1;
      }

      /* Retrieve user data from CQE */
      struct file_info *fi = io_uring_cqe_get_data(cqe);
      /* process this request here */ 

      /* Mark this completion as seen */
      io_uring_cqe_seen(ring, cqe);
      return 0;
  }
  
* You can wait for a completion by calling :c:func:`io_uring_wait_cqe`. This function blocks until at least one completion is available. 
* It fills up details of the completion in the :c:struct:`io_uring_cqe` structure. 
* This structure's ``res`` member contains the return value of the system call that was executed as part of the request. For example, if :c:func:`io_uring_prep_read` was called to process a :man:`read(2)` like request, ``res`` will contain the number of characters read or if it is less than zero, denotes that :man:`read(2)` faced an error.
* In ``io_uring``, you can set a user data field in the submission side, which with kernel will then make available unchanged on the completion side. You can use this to uniquely identify requests. You can get the user data pointer from the CQE by calling :c:func:`io_uring_cqe_get_data`.
* Once you process the information you got from the completion, you need to mark the CQE as "seen". You can do this with the :c:func:`io_uring_cqe_seen` function.

Reference
---------

.. c:function:: int io_uring_wait_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr)

  Returns an I/O completion, waiting for it if necessary. 

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.

  **Return value**

  Returns 0 with ``cqe_ptr`` filled in on success, -errno on failure.

----

.. c:function:: int io_uring_wait_cqe_nr(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, unsigned wait_nr)

 Return an IO completion, waiting for ``wait_nr`` completions if one isn't readily available. 

 **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.
  * `wait_nr`: number of completions to wait for.

 **Return value**

 Returns 0 with ``cqe_ptr`` filled in on success, ``-errno`` on failure.

----

.. c:function:: int io_uring_wait_cqes(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, unsigned wait_nr, struct __kernel_timespec *ts, sigset_t *sigmask)


    Like :c:func:`io_uring_wait_cqe`, except it accepts a timeout value as well. Note that an SQE is used internally to handle the timeout. Applications using this function must never set ``sqe->user_data`` to ``LIBURING_UDATA_TIMEOUT``.
 
    If ``ts`` is specified, the application need not call :c:func:`io_uring_submit` before calling this function, as it will be done internally. From this it also follows that this function isn't safe to use for applications that split SQ and CQ handling between two threads and expect that to work without synchronization, as this function manipulates both the SQ and CQ side.

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.
  * `wait_nr`: number of completions to wait for.
  * `ts`: pointer to a ``__kernel_timespec`` structure.
  * `sigmask`: a pointer to ``sigset_t``, the set of signals to block. The prevailing signal mask is restored before returning.

  **Return value**

  Returns 0 with ``cqe_ptr`` filled in on success, ``-errno`` on failure.

----

.. c:function:: int io_uring_wait_cqe_timeout(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, struct __kernel_timespec *ts)

   Same as the :c:func:`io_uring_wait_cqes` except it does not take a ``sigmask`` argument and always sets ``wait_nr`` to 1.

----

.. c:function:: int io_uring_peek_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr)
 
  Returns an I/O completion, if one is readily available. Doesn't wait. 

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.

  **Return value**

  Returns 0 with cqe_ptr filled in on success, -errno on failure.

----

.. c:function:: unsigned io_uring_peek_batch_cqe(struct io_uring *ring, struct io_uring_cqe **cqes, unsigned count)

    Fills in an array of I/O completions up to ``count``, if they are available, returning the count of completions filled. Does not wait for completions. They have to be already available for them to be returned by this function.

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.
  * `count`: count of completions to fetch.

  **Return value**

  Returns the count of I/O completions filled.

----

.. c:function:: void *io_uring_cqe_get_data(const struct io_uring_cqe *cqe)

    Returns pointer to user data set with :c:func`io_uring_sqe_set_data`.

    **Parameters**

    * `cqe`: pointer to the CQE from which to fetch the user data pointer.

    **Return value**

    Returns pointer to user data.

----

.. c:function:: void io_uring_cqe_seen(struct io_uring *ring, struct io_uring_cqe *cqe)

    Must be called after :c:func:`io_uring_peek_cqe` or :c:func:`io_uring_wait_cqe` and after the cqe has been processed by the application.

  **Parameters**

  * `ring`: :c:struct:`io_uring` structure as set up by :c:func:`io_uring_queue_init`.
  * `cqe_ptr`: pointer to a pointer to a ``io_uring_cqe`` structure.

----

.. c:function:: unsigned io_uring_cq_ready(struct io_uring *ring)

    TODO: add documentation
	


