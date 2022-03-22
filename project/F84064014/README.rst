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
that user can simply use deep learning model by only put data in and get data out.

Neaural Network is a important part of deep learning model. Training a neural network
may cost a lot of time especially when there is no gpu available, thus some component
would be written in C++ to make it faster

Prospective Users
=================

People who wants to use deep learning method but have no idea of programming or people
who just start to learn about deep learning.

System Architecture
===================

All function and class will be defined in python class FNN, function with complicate 
computation process like forward, backward propagation, etc., would be written in C++
function with less computation would be written in python

API Description
===============

FNN class includes complete implementation of a Feed Forward Neaural Network, user only
have to initialize model and train in python script. model configuration is
determined by a string, each line of string start with layer name and followed with
their parameter

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
    Build C++ module for every single function inside FNN
* Week 4 (4/25):
    Bind C++ module to python interface
* Week 5 (5/2):
    Build automatic python unit test to test the correctness of output of C++ module
* Week 6 (5/9): 
    Test the C++ accelerated version's correctness and performance on different dataset 
* Week 7 (5/16):
    optimizing and refactoring
* Week 8 (5/23):
    result performance visualiation

References
==========

List the external references for the information provided in the proposal.
