======================
Homework assignment #6
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Question
========

Based on what you wrote in the previous homework assignment or the course
notes, expose the internal buffer of the matrix object as `numpy.ndarray` in
Python.

Submission
==========

Your PR needs to be created against the ``hw6`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_22sp/hw6/$username/``).  The hierarchy should be::

  - nsdhw_22sp (<- repository root)
    - hw6
      - username (<- your working directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - (any other files you need)

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

The CI should pass with your submission.  The grader may reference the result
as well as additional information for grading.  To get it running, your branch
name should start with ``<username>-hw6**``, e.g, ``yungyuc-hw6-attempt1``.
See the CI configuration file and the Github Action documentation for more
information.

Your submission needs to be compatible with the runtime environment required by
the grader.  If the grader cannot get the expected results, **you can lose all
points**.

There are totally 5 points in this homework assignment.  The grader will check
the result from the ``validate.py`` script.  The grader may attempt to run the
script with the environment variable ``PRELOAD_MKL`` set, if ``validate.py``
doesn't pass without it.  Your work is considered pass in either case.

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
