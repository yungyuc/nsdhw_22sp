#include <iostream>
#include <vector>

class Line
{
public:
    Line(){};
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line(){};
    size_t size() const;
    const float & x(size_t it) const;
    float & x(size_t it);
    const float & y(size_t it) const;
    float & y(size_t it);
private:
	size_t m_size = 0;
    std::vector<float> m_xs;
	std::vector<float> m_ys;
}; /* end class Line */

Line::Line(Line const & tar){
	*this = tar;	
}

Line::Line(Line		  && tar){
	*this = tar;
}

Line & Line::operator=(Line const & tar){
	m_size = tar.size();
	m_xs.resize(m_size);
	m_ys.resize(m_size);
	for(size_t i=0; i<m_size; i++){
		m_xs[i] = tar.x(i);
		m_ys[i] = tar.y(i);
	}
	return *this;
}

Line & Line::operator=(Line 	  && tar){
	m_size = tar.size();
	m_xs.resize(m_size);
	m_ys.resize(m_size);
	for(size_t i=0; i<m_size; i++){
		m_xs[i] = tar.x(i);
		m_ys[i] = tar.y(i);
	}
	return *this;
}

Line::Line(size_t size){
	m_size = size;
	m_xs.resize(size);
	m_ys.resize(size);
}

size_t Line::size() const{
	return m_size;
}

const float & Line::x(size_t it) const{
	return m_xs[it];
}

float & Line::x(size_t it){
	return m_xs[it];
}

const float & Line::y(size_t it) const{
	return m_ys[it];
}

float & Line::y(size_t it){
	return m_ys[it];
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
