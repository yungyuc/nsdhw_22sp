#include <iostream>
#include <vector>
#include <utility>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(size_t size);
    ~Line() {m_size = 0; m_vec.clear(); }
    size_t size() const { return m_size; }
    //float & x(size_t it) const { check_range(it); return m_vec[it].first; }
    float & x(size_t it) { check_range(it); return m_vec[it].first; }
    //float & y(size_t it) const { check_range(it); return m_vec[it].second; }
    float & y(size_t it) { check_range(it); return m_vec[it].second; }


private:
    // private helper
    void check_range(size_t it) const {
        if(it >= m_size) {throw std::out_of_range("Line index out of range");}
    }

    // Member data.
    size_t m_size;
    std::vector<std::pair<float,float>> m_vec;

}; /* end class Line */

Line::Line(size_t size){
    m_size = size;
    m_vec.clear();
    for(int i = 0;i < size;i++){
        std::pair<float,float> tmp_p;
        m_vec.push_back(tmp_p);
    }
}

Line::Line(Line const & other){
    if(other.m_size > 0){
        if(m_size > 0){
            m_vec.clear();
        }
        m_size = other.m_size;
        m_vec.assign(other.m_vec.begin(),other.m_vec.end());
    }
    else{
        m_vec.clear();
        m_size = 0;
    }
}

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
