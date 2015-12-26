#ifndef TRANSFORMATIONMATRIX_HPP
#define TRANSFORMATIONMATRIX_HPP
#include <eigen3/Eigen/Dense>
Eigen::Matrix3d make_Reflection(int x,int y,int z);
Eigen::Matrix3d make_Rotation(float degree1, float degree2);
#endif