.. _changelog:

Changelog
=========
So you know when things are out of date.

2020-06-15
----------
* Adding explanation around ``IORING_FEAT_SINGLE_MMAP`` and how it reduces the need for :man:`mmap(2)` calls from 3 to 2. Affected pages:
    * :ref:`what_is_io_uring`
    * :ref:`low_level`