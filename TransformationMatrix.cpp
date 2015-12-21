#include "TransformationMatrix.hpp"
Eigen::Matrix3d make_Reflection(int x,int y,int z){
  Eigen::Matrix3d e;
  e<<1-2*x*x,-2*x*y,-2*x*z,-2*x*y,1-2*y*y,-2*y*z,-2*x*z,-2*y*z,1-2*z*z;
  return e;
}