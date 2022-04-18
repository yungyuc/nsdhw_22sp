======================
Homework assignment #4
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Question
========

Change the matrix-matrix multiplication code you developed to use a
``std::vector<double>`` for the internal memory buffer, and enhance it by
making the vector use a custom STL allocator, e.g., ``std::vector<double,
CustomAllocator<double>>``, to count the bytes used, allocated, and deallocated
for the data buffer of all the ``Matrix`` instances in the process.  Your
Python module must provide 3 functions ``bytes()``, ``allocated()``, and
``deallocated()`` to return the tracked information.

Submission
==========

Your PR needs to be created against the ``hw4`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_22sp/hw4/$username/``).  The hierarchy should be::

  - nsdhw_22sp (<- repository root)
    - hw4
      - username (<- your working directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - (any other files you need)

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

The CI should pass with your submission.  The grader may reference the result
as well as additional information for grading.  To get it running, your branch
name should start with ``<username>-hw4**``, e.g, ``yungyuc-hw4-attempt1``.
See the CI configuration file and the Github Action documentation for more
information.

Your submission needs to be compatible with the runtime environment required by
the grader.  If the grader cannot get the expected results, **you can lose all
points**.

There are totally 5 points in this homework assignment.  The grader may run on
a reference machine of his or her choice and check the result from the
``validate.py`` script.  The benchmark result may go off on different hardware.
Please test your code in the same or a similar environment.  The grader may
attempt to run the script with the environment variable ``PRELOAD_MKL`` set, if
``validate.py`` doesn't pass without it.  Your work is considered pass in
either case.

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
