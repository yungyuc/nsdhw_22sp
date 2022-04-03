## Project Name
------------------------------
Implementing Optimal Binary Search Tree(OBST) with C++ Program
------------------------------

## Basic Information
------------------------------
OBST is that given a sorted array key[0.. n-1] of search keys and an array freq[0.. n-1] of frequency counts, where freq[i] is the number of searches for keys[i]. And construct a binary search tree of all keys such that the total cost of all the searches is as small as possible.
------------------------------

## Problem to Solve
------------------------------
As known, there are many complicated and similar calculations in OBST. As a result, we are going to build a parallel C++ program to do these. Making it be an API called obst() to let user more convenience and get the answer faster.
------------------------------

## Prospective Users
------------------------------
OBST is applied in many fields as Huffman's code, graphic, NLP ,etc.
------------------------------

## System Architecture
------------------------------
The system architecture will be an API consists of input and output. Input is a class named OBST_INPUT, to collect the data, keys and frequencies. And will output the corresponding OBST for users.
------------------------------

## API Description
------------------------------
Class: OBST_INPUT{
    private :
        array key[] for keys
        arrat freq[] for frequencies
        sorting_keys() : sorting the keys for the users.
}

obst(OBST_INPUT) :
    computing the corresponding OBST for the input data.
------------------------------

## Engineering Infrastructure
------------------------------
Build system by using "make"
Control versin by using "git"
Test by using "pytest" to do the unit test
------------------------------

## Schedule
------------------------------
* Week 1 (4/4): Plan the architecture for overall program.
* Week 2 (4/11): Complete the class part, OBST_INPUT.
* Week 3 (4/18): Complete the algorithm part.
* Week 4 (4/25): Make the calculations part parallel.
* Week 5 (5/2): Using "pybind11" to build unit test to test the correctness of output of C++ module.
* Week 6 (5/9): Optimize the entire system.
* Week 7 (5/16): Test performance.
* Week 8 (5/23): Preparing the presentaion and final documents. 
------------------------------