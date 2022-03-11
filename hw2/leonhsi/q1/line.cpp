#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Line
{
public:
	//constructor
    Line(){};
    Line(size_t size) : m_x(size, 0), m_y(size, 0){};
	//copy constructor
    Line(Line const & line) : m_x(line.m_x), m_y(line.m_y){};
	//move constructor
	//use std::move since line.m_x is lvalue, need to convert it to rvalue to save copy time
    Line(Line && line) : m_x(std::move(line.m_x)), m_y(std::move(line.m_y)){};
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
	//destructor
    ~Line(){m_x.clear(); m_y.clear();};

    size_t size() const{return m_x.size();};

	//getter and setter
    const float & x(size_t it) const {return m_x.at(it);};
    float & x(size_t it) {return m_x.at(it);};
    const float & y(size_t it) const {return m_y.at(it);};
    float & y(size_t it) {return m_y.at(it);};
private:
	vector<float> m_x;
	vector<float> m_y;
}; 

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
