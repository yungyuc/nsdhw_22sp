//
//  Line.hpp
//  nsd_hw2
//
//  Created by 張秉洋 on 2022/3/9.
//
#include <vector>
#ifndef Line_hpp
#define Line_hpp

struct Coord{
    int x;
    int y;
};

class Line {
    
public:
    Line() = default;
    Line(size_t );
    
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Desctructor.
    ~Line();
    
    size_t size() const;
    
    const int & x(size_t ) const;
    int & x(size_t );
    const int & y(size_t ) const;
    int & y(size_t );
    
private:
    std::vector<Coord> m_coord;
    
    void check_range(size_t ) const;

};

#endif /* Line_hpp */
