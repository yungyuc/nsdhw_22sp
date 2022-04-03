# Bounding Volume Hierarchy Trees Generator

## Basic Information

Bounding Volume Hierarchies (BVH) trees are an acceleration sturcture for ray tracing algorithm. The problem of ray tracing is to find the nearest intersection with a given ray and scene. In practice, it has to evaluate billions of such operations as the scene consists of millions of primitives, thus scene primitives are commonly arranged in spatial data structures to accelerate the search. BVH are such data structure that used mostly nowadays.

## Problem to Solve
There are many kinds of BVH trees, mostly divided into binary BVH and n-ary BVH, and each BVH trees has its own advantage. In this project, I want to make the process of building different BVH trees faster and eaiser by providing APIs, and evaluate their performance against each other.

## Perspective Users
Everyone that interested in computer graphics.

## System Architecture
There will be only one API and user only need to input which kinds of BVH trees he or she wants, then the output will be the performance of this BVH tree.

Performance will include the construction and traversal time it takes of this BVH tree.


## API Description

`build_bvh(bvh_types, other_features)` : 
* bvh types : types of bvh trees such as QBVH, SBVH, LBVH, etc
* other features : additional improve mechanism such as spatial split, sorting ray, etc

## Engineering Infrasturcture
* programming language : c++, python
* build system : GNU make
* version control : git
* testing : pytest

## Schedule
* week 1 : implement basic ray tracing algorithm with original binary BVH tree
* week 2 : build the API and implement a different type of BVH tree
* week 3 : build more types of BVH tree
* week 4 : evaluate the performance of BVH trees with different scenes
* week 5 : run the unit test
* week 6 : visualize the result and prepare for the presentation

## Reference
* [A Survey on Bounding Volume Hierarchies for Ray Tracing](https://meistdan.github.io/publications/bvh_star/paper.pdf)
* [GPU raytracer](https://github.com/jan-van-bergen/GPU-Raytracer)
* [bvh](https://github.com/madmann91/bvh)