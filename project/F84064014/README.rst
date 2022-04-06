======================================================================
A Simple End-to-End NN Framework for Deep Learning
======================================================================

Basic Information
=================

Deep Learning is now widely used to solve a lot of problem from many different field, 
such as computer vision, data classification, natural language processing, etc., this
project we will implement a End-to-End Deep Learning Framework and accelerate it with C++

Problem to Solve
================

In the project we try to make deep learning faster and easier

Deep Learning is a powerful tool even for people who never write code before, however, 
build a deep learning model is not a walk in the park, we hope to build a framework
that user have to care only about which dataset they are going to use, how many layers they
want, which loss function they want, then they could train a powerful model on their task.

Neaural Network is a important part of deep learning model. Training a neural network
may cost a lot of time especially when there is no gpu available, thus some component
would be written in C++ to make it faster

Prospective Users
=================

People who wants to use deep learning method but have no idea of programming or people
who just start to learn about deep learning.

System Architecture
===================

A main class FNN will be written in C++ and connect to python script with pybind11.
In python script FNN should be a callable class with some different function. Some
fucntion like dense layer, activation are subclass defined inside FNN object.

Each FNN maintain a list(doubly linked) of layer object. All layers are made of a
2d double array with size=(input_size, output_size), and followed by an activation
fucntion.

API Description
===============

FNN class includes complete implementation of a Feed Forward Neaural Network, user only
have to initialize model and train in python script. model configuration is
determined by a string, each line of string start with layer name and followed with
their parameter

API function

- FNN.__init__(): initialize parameter(layer) and flag
- FNN.__call__(): two mode, train and eval, train mode will calcaulate gradient and update parameter, eval only forward and output result
- FNN.trian(): set train mode
- FNN.eval(): set eval mode
- FNN.forward(): forward input data to all layer and get gradient if necessary
- FNN.backward(): update parameter

Also layer and some activation function are class that defined in FNN, all these class
have identical function forward(), df_dx()

API class

- FNN.sigmoid
- FNN.linear
- FNN.identical
- FNN.ReLu
- FNN.softmax
- FNN.CrossEntropy
- FNN.SSE

Engineering Infrastructure
==========================

Unit tests will be provided in Python. Public API will be wrapped to Python for testing.
Correctness will be verified by using numpy: computation carried out in C++ should produce exactly identical results to that in numpy.

Schedule
========

* Planning phase (6 weeks including 2/21, 2/28, 3/7, 3/14, 3/21, 3/28):
* Week 1 (4/4):
    Complete pure python version of FNN class. Start the presentation skeleton.
* Week 2 (4/11):
    Test pure python model with FNN with some dataset (regression or classification)
    to check FNN functionality and performance
* Week 3 (4/18):
    Build C++ module for every single function inside FNN and python interface (pybind11)
* Week 4 (4/25):
    Build automatic python unit test to test the correctness of output of C++ module
* Week 5 (5/2):
    Test the C++ accelerated version's correctness and performance on different dataset 
* Week 6 (5/9):
    result performance visualiation
* Week 7 (5/16):
    optimizing and refactoring
* Week 8 (5/23):
    Documentation and presentation preparing

References
==========
#. `F84064014 <https://github.com/F84064014>`__ for
   `SimpleNN <README.rst>`__: https://github.com/F84064014/SimpleNN