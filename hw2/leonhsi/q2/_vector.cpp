#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;

float caculate_angle(vector<float> cor1, vector<float> cor2){

	//caculate ||u|| and ||v||
	float len1 = sqrt(pow(abs(cor1[0]), 2) + pow(abs(cor1[1]), 2));
	float len2 = sqrt(pow(abs(cor2[0]), 2) + pow(abs(cor2[1]), 2));

	//caculate u dot v
	float dot = cor1[0] * cor2[0] + cor1[1] + cor2[1];

	//theta = arccos(dot / ||u|| * ||v||)
	float theta = acos(dot / (len1 * len2));

	return theta;
}

PYBIND11_MODULE(_vector, m){
	m.def("caculate_angle", &caculate_angle, "caculate angle between two vectors");
}
