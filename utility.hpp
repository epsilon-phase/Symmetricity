#ifndef UTILITY_HPP
#define UTILITY_HPP
#include "city.h"
#include <eigen3/Eigen/Dense>
namespace std {
template <>
struct less<Eigen::Vector3i> {
  bool operator()(const Eigen::Vector3i &a, const Eigen::Vector3i &b) const {
    //return a[0] == b[0] ? (a[1] == b[1] ? (a[2] < b[2]) : a[1] < b[1]) : a[0] < b[0];
    return a[2] == b[2] ? (a[0] == b[0] ? a[1] < b[1] : a[0] < b[0]) : a[2] < b[2];
  }
};
template<> struct hash<Eigen::Vector3i> {
  std::size_t operator()(const Eigen::Vector3i& a)const{
    if(sizeof(std::size_t)==8){
      return CityHash64((const char*)&a,sizeof(a));
    }
  }
};
}
#endif