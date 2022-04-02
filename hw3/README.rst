======================
Homework assignment #3
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Question
========

Develop your own matrix-matrix multiplication code and measure the runtime.
Please implement 2 versions: (i) the naive algorithm and (ii) the tiling, and
compare with that of BLAS ``DGEMM`` subroutine.

The matrix size should be larger than or equal to ``1000x1000``.  The tiling
version must be 20% faster than the naive version (runtime must be less than
80%).

Submission
==========

Your PR needs to be created against the ``hw3`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_22sp/hw3/$username/``).  The hierarchy should be::

  - nsdhw_22sp (<- repository root)
    - hw3
      - username (<- your working directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - test_xxx.py (for unit testing)
        - performance.txt
        - (any other files you need)

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

The CI should pass with your submission.  The grader may reference the result
as well as additional information for grading.  To get it running, your branch
name should start with ``<username>-hw3**``, e.g, ``yungyuc-hw3-attempt1``.
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

Additional notes on grading
===========================

There are totally 5 points in this homework assignment.

1. Develop a two-dimensional matrix class (you may reuse the code in the course
   note) in C++.  (2 points)

   1. Implement a free function performing naive matrix-matrix multiplication.
      Name it ``multiply_naive()``.
   2. Implement a free function performing matrix-matrix multiplication with
      tiling.  Name it ``multiple_tile()``.
   3. Implement a free frunction that uses ``DGEMM`` for the multiplication.
      Name it ``multiply_mkl()``.
2. Make Python unit tests for the C++ matrix class and multiplication
   functions.  (1 point)

   1. Use the same names in Python.
   2. The unit tests should support py.test.
   3. ``make test`` should run the same unit tests.
3. Make sure the answers from the two multiplication functions match.  (1
   point)
4. Time the two multiplication functions and compare the performance. Then output
   ``performance.txt``. The format of ``performance.txt`` can be any. Just make
   it reasonable.  (1 point)

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
