======================
Homework assignment #1
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Your PR needs to be created against the ``hw1`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_22sp/hw1/$username/``).  The hierarchy should be::

  - nsdhw_22sp (<- repository root)
    - hw1
      - username (<- your working directory)
        - Your files

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

The CI should pass with your submission.  The grader may reference the result
as well as additional information for grading.  To get it running, your branch
name should start with ``<username>-hw1**``, e.g, ``yungyuc-hw1-attempt1``.
See the CI configuration file and the Github Action documentation for more
information.

Your submission needs to be compatible with the runtime environment required by
the grader.  If the grader cannot get the expected results, **you can lose all
points**.

Everyone should write his/her own code.  It is OK to discuss, but there should
not be duplicated code.  If duplication is found, **all points** for the
duplicated part of the latter submitter may be deducted.

Question
========

1. Use bash to reimplement ``countline.py``.  You may not call Python.
2. Modify the script ``countline.py`` so that it reads the environment variable
   named "``PYTHON_BIN``" to switch the Python executable for the script.  The
   script must be runnable by both ``python2`` and ``python3``.  It should
   exit with an error code when the specified binary is not found.

   Hint: play a trick using bash.

Grading guideline
=================

This homework assignment has 5 points.  The grader will run the following
commands:

.. code-block:: bash

  cd nsdhw_22sp/hw1/username
  ../validate.sh | grep "GET POINT"

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
