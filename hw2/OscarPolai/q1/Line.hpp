#include <cstdint>
#include <utility>
#include <iostream>
#include <vector>

class Line{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(size_t size);//costom constructor
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    
    // Desctructor.
    ~Line() = default;
    // Accessors.
    size_t size() const{
        return m_size; 
    }
    
    float & x(size_t it){ 
        check_range(it); 
        return l[it].first; 
    }
    
    float & y(size_t it){
        check_range(it); 
        return l[it].second; 
    }

private:
    // Private helper.
    void check_range(size_t it) const{ 
        if (it >= m_size){ 
            throw std::out_of_range("Line index out of range"); 
        } 
    }
    // Member data.
    std::vector<std::pair<float, float>> l;
    size_t m_size = 0; // number of points.
    //float *m_coord = nullptr; // memory buffer for the points.
}; /* end class Line */

