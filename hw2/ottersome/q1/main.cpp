#include <vector>
#include <iostream>

class Line{
    public:
        Line(){ //I guess this one can initialize an empty vector
            x_vec.clear();
            y_vec.clear();
        }
        //Copy Constructor
        Line(Line const & old_line){
            for(size_t i = 0;i<old_line.size();i++){
                x_vec.push_back(old_line.x(i));
                y_vec.push_back(old_line.y(i));
            }
        }
        //Move Constructor(TODO)
        Line(Line && old_line){
            x_vec = std::move(old_line.all_x());
            y_vec = std::move(old_line.all_y());
        }
        //Copy Assignment
        Line & operator=(Line const & old_line){
            for(size_t i = 0;i<old_line.size();i++){
                x_vec.push_back(old_line.x(i));
                y_vec.push_back(old_line.y(i));
            }
            return *this;
        }
        std::vector<float> & all_x(){
            return this->x_vec;
        }
        std::vector<float> & all_y(){
            return this->y_vec;
        }
        //Move Assignment
        Line & operator=(Line && old_line){
            //Assign entire vectors maybe ?
            x_vec = std::move(old_line.all_x());
            y_vec = std::move(old_line.all_y());
            return *this;
        }
        //Simple Allocation
        Line(size_t size){
            x_vec.resize(size);
            y_vec.resize(size);
        }
        //Destructor
        ~Line(){
            //Not necessary I believe but still leaving it here
            x_vec.clear();
            y_vec.clear();
        }
        //Get current size 
        size_t size() const{
            return x_vec.size();
        }
        float & x(size_t it) const{
            return x_vec[it];
        }
        float & x(size_t it){
            return x_vec[it];
        }
        float & y(size_t it) const{
            return y_vec[it];
        }
        float & y(size_t it){
            return y_vec[it];
        }

    private:
        //Lets keep an unitialized vector here
        mutable std::vector<float> x_vec;
        mutable std::vector<float> y_vec;

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
