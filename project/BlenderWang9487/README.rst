========================
DummyML
========================



Basic Information
=================

A dummy ML library for NSD course term project.

Here is my Github repository:
`DummyML <https://github.com/BlenderWang9487/DummyML.git>`_

Problem to Solve
================

I want to build a machine learning library based on some mathematical theories.

There are many easy-to-use hybrid frameworks that built for deep learning, 
but fewer for other traditional ML algorithms. So I'm going to build one.

Some regression, classification and clustering models (like Gaussian linear 
regression, naive Bayes classifier, Kmeans, etc.) will be provided by this
library. (There will be atleast three approaches for each mission. One of them
is a "dummy" approach which can be treated as the baseline.)

Prospective Users
=================

People who are interested in machine learning and want to try some different 
ML algorithms may use my library to solve some basic classification problems.
For example, handwritten numbers in MNIST dataset can be classified using
naive Bayes classifier with an acceptable accurancy.

Data scientists can use this library's models as baselines against theirs, too.

System Architecture
===================

There may or may not be a class to handle input/output data (I might just use
Numpy array).

There will be a class, "Model", which is the parent class of all the other
models. It will provide an uniform interface for fitting, predicting, saving and
loading.

If there are some functions/data structures which are used by many algorithms
(like matrix operations), they will be declared in utils.hpp, making them easy
to maintain.

API Description
===============

Users can write something like this:

.. code:: python
    :number-lines:

    import dummyml
    import numpy as np
    
    # Initialize the model.
    classifier = dummyml.some_classifier(""" some properties """)
    
    # If there is a pretrained weights file, user will be able to load it from
    # the file.
    classifier.load('some_weights_1.dat')
    dummyinput = np.random.rand(10)

    # Model object will be callable.
    result = classifier(dummyinput)
    print(result)

    # User can save the weights to a file.
    classifier.save('some_weights_2.dat')

Engineering Infrastructure
==========================

1. Using Cmake to configure & build the project
2. Using Git to control different versions
3. Using Pytest for testing
4. Documentation: I plan to just write markdown files as documentation for now.

Schedule
========

* Planning phase (6 weeks including 2/21, 2/28, 3/7, 3/14, 3/21, 3/28): Planning
* Week 1 (4/4): Finish the Model class and simple API to check if the hybrid
  system can work.
* Week 2 (4/11): Add some unoptimized approaches of classification/regression 
  algorithms in C++ to get the baselines.
* Week 3 (4/18): Add some unoptimized approaches of clustering algorithms in C++.
* Week 4 (4/25): Optimize them and bind them to python using pybind.
* Week 5 (5/2): Write unit test/Github CI test and keep optimizing them.
* Week 6 (5/9): Buffer week.
* Week 7 (5/16): Write documentation and maybe test their performance with some
  famous datasets.
* Week 8 (5/23): Prepare for presentation.

References
==========

Some algorithms that I might implement:

* `Naive Bayes classifier <https://en.wikipedia.org/wiki/Naive_Bayes_classifier>`_
* `K-means clustering <https://en.wikipedia.org/wiki/K-means_clustering>`_