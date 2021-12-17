======================
Homework assignment #0
======================

This is a demonstration of NSD 22SP homework.

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Your PR needs to be created against the ``hw0`` branch.  (Not ``master``!)  The
name of your branch needs to start with ``<github-user-name>-hw#``, e.g.,
``username-hw2-attempt1``.  You need to create a sub-directory using exactly
your GitHub username as your working directory
(``nsdhw_22sp/hw0/$github-user-name/``).  The hierarchy should be::

  - nsdhw_22sp (<- repository root)
    - hw0
      - username (<- your working directory)
        - Your files

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow the rule can
cost you points.

Please make sure your PR passes the Github Action CI, and is compatible with
Ubuntu 20.04.  If the grader cannot build and run your code, **you can lose all
points**.

Everyone should write his/her own code.  It is OK to discuss, but there should
not be duplicated code.  If duplication is found, **all points** for the
duplicated part of the latter submitter may be deducted.

Question
========

1. Run ``countline.py``.

Grading guideline
=================

This homework assignment has 5 points.  The grader shall run the following
commands:

.. code-block:: bash

  cd nsdhw_22sp/hw0/username
  ../validate.sh | grep "GET POINT"

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
