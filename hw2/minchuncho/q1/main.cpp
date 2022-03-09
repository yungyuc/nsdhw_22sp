//
//  main.cpp
//  hw2-q1
//
//  Created by MinChunCho on 2022/3/7.
//

#include "line.hpp"

#include <iostream>
#include <vector>

int main(int, char **)
{
    Line line(3);
    line[0] = Point(0, 1);
    line[1] = Point(1, 3);
    line[2] = Point(2, 5);

    Line line2(line);
    line2[0].x(9);

    std::cout << "line: number of points = " << line.size() << std::endl;
    for(size_t it=0; it<line.size(); ++it){
     Point p = line[it];
     std::cout << "point " << it << ":"
               << " x = " << p.x()
               << " y = " << p.y() << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for(size_t it=0; it<line.size(); ++it){
     Point p = line2[it];
     std::cout << "point " << it << ":"
               << " x = " << p.x()
               << " y = " << p.y() << std::endl;
    }

    return 0;

}
