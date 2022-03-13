#include <iostream>
#include <vector>
#include <algorithm>

class Line
{
public:
    Line() = default;
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line() = default;
    size_t size() const {return m_coord.size()/2;};
    float & x(size_t it) const {check_range(it);float temp = m_coord[it*2];return temp;}
    float & x(size_t it)       {check_range(it);return m_coord.at(it * 2);}
    float & y(size_t it) const {check_range(it);float temp = m_coord[it*2+1];return temp;}
    float & y(size_t it)	   {check_range(it);return m_coord.at(it * 2 + 1);}
private:
    // Member data.
    std::vector<float> m_coord;
	
	void check_range(size_t it) const
	{
		if (it >= m_coord.size()/2 + 1)
		{
			throw std::out_of_range("Line index out of range");
		}
	}

}; /* end class Line */

/* Define the copy constructure */
Line::Line(Line const & other)
{
    if (!other.m_coord.empty())
    {
		m_coord = std::vector<float>();
		for(float const element: other.m_coord)
		{
			m_coord.push_back(element);
		}
    }
    else // the other object is empty.
    {
    	m_coord = std::vector<float>();
    }
}

/* Define the move constructure */
Line::Line(Line && other)
{
    std::swap(other.m_coord, m_coord);
}

/* constructure with size*/
Line::Line(size_t size)
{
	for(size_t it=0; it < 2*size; ++it){m_coord.push_back(0);}
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    if (other.m_coord.empty())
    {
		m_coord = std::vector<float>();
		for (float const element: other.m_coord)
		{
			m_coord.push_back(element);
		}
    }
    else
    {
		m_coord = std::vector<float>();
    }
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
	
	std::vector<float> temp;
	for (float const element: other.m_coord)
	{
		temp.push_back(element);
	}
	other.m_coord = std::vector<float>();
	for (float const element: m_coord)
	{
		other.m_coord.push_back(element);
	}
	m_coord = std::vector<float>();
	for (float const element: temp)
	{
		m_coord.push_back(element);
	}
    
	return *this;
}
