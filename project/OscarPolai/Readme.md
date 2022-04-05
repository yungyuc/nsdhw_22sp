Rocker : Pandas like Library  
================================================

Project name : [Rocker](https://github.com/OscarPolai/Rock)

I still remembered the first time when I use pandas Dataframe to analyze big dataset , I was surprised  the functionality the library provided.
It behaves high-performance, easy-to-use data structure internally for data analysis. But there was a time when I try to load a bunch of data 
(~10G) and convert it to dataframe entirely. It encountered run out of memory error. 

From this lesson learn, I realized we can not totally rely on the api library they provided , we still need to know how to tune it properly  to release its maximum speed.


Problem to Solve
================
Pandas is a Python package providing fast, flexible data structures designed to make working with “relational” or “labeled” data both easy and intuitive.
Without this kind of data structure ,we find it sometimes take a lot of time to do data cleaning and preparation.

Pandas in built on the top of Numpy array. In this project, I’ll try to implement my dataframe it in C++ ,and compare it with the performance.	

To make use to dataframe,  we need to understand different data type stored in the dataframe will impact the performance when it performs different operations.
Some of data occupy the whole memory space result in causing much of memory waste. 
That it, knowing how dataframe do its memory allocation is important.  Also knowing what kind of operation underground will avoid memory waste.
In this project, I will try to write the code and test the memory behavior parallelly to get more knowledge.

Usually,  there are two ways to implement arrays of composite types. SoA or the columnar arrays perform
better performance (cache locality) than AoS for heterogeneous data, especially when there are many field. 

For this purpose, designing a data structure handing large datasets with high performance is the core of this project I am focusing. It means not only to implement the pure 
Dataframe, but also design better data structures or operation for dataframe to reduce memory overhead.
  
Prospective Users
=================   

1. Those who do data analysis dealing with large datasets.
2. Anyone who wants customize their library to enhance the flexibility and functionality pandas can’t do.


System Architecture
===================

A main DataFrame class will be written in C++ and connect to python script with pybind11. 

API Description
===============

Main class will implement some of the most common and useful.

Here only list some of the most important method, if time allows, I will try to implement more.

* Constructor to Load data 
    * Dataframe(std::string csv_name);   // load data from csv file 
* Load data from class method

```cpp
#include <DataFrame/DataFrame.h>

DataFrame df;
std::vector<unsigned long>  idx_col1 = { 1, 2, 3, 4, 5};
std::vector<std::string>    col1 = { "A", "B", "C", "D", "E"};
std::vector<std::string>    col2 = { "QQ", "Hello World!", "man", "M", "N"};

df.load_data(std::move(idx_col1),
                     std::make_pair("string col", col1),
                     std::make_pair("Happy", col2));

```
* Popular methods
    * Dataframe.get_column(std::col_name); // get column from specific column name
    * Dataframe.drop_col(std::string col_name); //drop column by specifying name.
    * Dataframe.cols(); // return the number of columns in dataframe
    * Dataframe.rows(); //  return the number of rows in dataframe
* Aggregation methods 
    * Dataframe.describe(); // return insights in dataframe like mean, count,  min, max
* Viewing data 
    * Dataframe.print(); // print the scheme of dataframe
* Group by one or two columns 

Engineering Infrastructure
==========================

* Automatic build system: make
* Version control: git
* Testing framework: pytest

Schedule
========

* Planning phase (6 weeks including 2/21, 2/28, 3/7, 3/14, 3/21, 3/28):
* Week 1 (4/4): 
    * Implement the main class, and some of the method.
   	* Build automatic python unit test to test the correctness of output of C++ module.
* Week 2 (4/11):
    * Build the rest of method. 
	* Analyze the memory behavior in runtime, refined the previous design.
* Week 3 (4/18):
    * Prepare the presentation skeleton 
	* Adjust code implementation.
* Week 4 (4/25):
    * Compare and test performance with pandas library. 
	* Optimize and adjust the code. 
* Week 5 (5/2):
    * Analyze memory issue.
	* Discuss difficulty with teacher
    * Result performance visualization
	Optimize and adjust the code. 
* Week 6 (5/9):
    * Refactoring week (and accommodation of overflown work)
* Week 7 (5/16):
    * Refactoring week (and accommodation of overflown work)
* Week 8 (5/23):
    * Documentation and presentation preparing
