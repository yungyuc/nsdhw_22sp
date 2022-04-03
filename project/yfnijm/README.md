#  RTSE: R-Tree Search Engine 

## Basic Information
R-tree is a girdless data structure base on B-tree to speed up searches in space, *R* is stand for *Rectangle*. The average search time would be O(lg(n)) and n is the object number.
Github repository: https://github.com/yfnijm/RTSE.git 

## Problem to solve
To solve the most of EDA problem which only have a few layer, this project would focus on providing the 2D R-tree related API. And it would be usful to the placement and routing problem especially. The object in EDA problem can be consider in different geometrical object, wire can be the combination of some rectangle or segement, vertex and via can be the an rectangle or point. So the search engine should accept to search the three main geometrical entities which are retangle(box), segement and point.

And consider the EDA problem, especially routing, are required large amount of computation, the project would use C++ code to implement the core funciton.

## Perspective users
The student of EDA related Lab.
And anyone need to speed up the area search in their works.

## System architecture
The system base on the main data structure class **R-tree** and users can add or delete new object and dynamically modify the data inside. And users also can query the imformation they what in the stucture at all times.

The data structure of R-tree is similar to B-tree, a non-leaf node would store some range's imformation which contain different node, and a leaf node would store the basic object in the parent's range. But the B-tree can't not slove the 2 or more D's problem, the R-tree according one of dimention to generate the tree. Althought the search time are not guaranteed in O(lg(n)) but it still be O(lg(n)) in average.

## API description
Currenet API phototype are refer to:
https://www.boost.org/doc/libs/1_77_0/libs/geometry/doc/html/index.html

the basic API:
:::info
model(): initial the R-tree data

insert(obj): add object into the R-tree

remove(obj): delete object from the R-tree

intersects(box): search the objects intersect with the box

covered_by(box): search the objects covered by the box
:::

## Engineering infrastructure
- Language: C++, Python3 (Pytest for testing)
- Bulid system: GNU make
- Version control: git

## Schedule

| Date | Target                                                                                                             |
| ---- | ------------------------------------------------------------------------------------------------------------------ |
| 4/4  | Built the API and implement the data structure and function in naive way.                                          |
| 4/11 | Implement the basic R-tree.                                                                                        |
| 4/18 | Add advanced funciton (segement, point search or related operation) if privious week target finished successfully. |
| 4/25 | Provide the API of Python3 and run the unit test.                                                                  |
| 5/2  | Generate more testcase and compare to previous related work (k-d tree, and naive structure).                       |
| 5/9  | Visualize the testcase and tried different heuristic function to generate R-tree.                                  |
| 5/16 | Compare to other's R/R+/R*-tree. And implement my privious EDA project with the project.                           |
| 5/23 | Prepare the presentation .                                                                                         |

