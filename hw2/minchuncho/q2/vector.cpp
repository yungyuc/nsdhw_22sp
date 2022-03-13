#include "vector.hpp"

Vec::Vec(Vec const &obj){
	x_ = obj.x_;
	y_ = obj.y_;
}

double Vec::angle(Vec const& v1, Vec const& v2){
	double d1, d2, inner_prd, angle;
	d1 = std::sqrt(v1.x_*v1.x_ + v1.y_*v1.y_);
	d2 = std::sqrt(v2.x_*v2.x_ + v2.y_*v2.y_);
	inner_prd = v1.x_*v2.x_ + v1.y_*v2.y_;

	angle = acos(inner_prd/(d1*d2)) * 180.0/PI;

	return angle;
}

void Vec::show(){
	std::cout << this->x_ << ", " << this->y_ << std::endl;
}