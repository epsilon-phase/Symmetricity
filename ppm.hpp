#ifndef PPM_HPP
#define PPM_HPP
#include <unordered_map>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include "agony.hpp"
#include <string>
#include <sstream>
#include <cmath>
inline void write_ppm_to_file(const std::string& filename,
                          const std::unordered_map<Eigen::Vector3d,double> &a,
                          std::tuple<int,int,int,int,int,int> bounds){
  auto getValueBetweenTwoFixedColors=[](float value, int &red, int &green, int &blue)
{
  int aR = 0;   int aG = 0; int aB=255;  // RGB for our 1st color (blue in this case).
  int bR = 255; int bG = 0; int bB=0;    // RGB for our 2nd color (red in this case).
 
  red   = (float)(bR - aR) * value + aR;      // Evaluated as -255*value + 255.
  green = (float)(bG - aG) * value + aG;      // Evaluates as 0.
  blue  = (float)(bB - aB) * value + aB;      // Evaluates as 255*value + 0.
};
  using namespace std;
  int minx=std::get<0>(bounds),
      miny=std::get<1>(bounds),
      minz=std::get<2>(bounds),
      maxx=std::get<3>(bounds),
      maxy=std::get<4>(bounds),
      maxz=std::get<5>(bounds);
  double maxval=0;
  for(auto q:a)
    if(maxval<q.second&&q.second!=numeric_limits<double>::infinity())
      maxval=q.second;
  stringstream q;
  for(int i=minz;i<=maxz;i++){
    q.str("");
    q<<filename<<i<<".ppm";
    ofstream output(q.str());
    output<<"P3\n"<<maxx-minx+1<<" "<<maxy-miny+1<<"\n";
    output<<255<<"\n";
    for(int j=miny;j<=maxy;j++){
      for(int k=minx;k<=maxx;k++){
        output.width(5);
        auto f=a.find(Eigen::Vector3d(k,j,i));
        if(f!=a.end()){
          int r,g,b;
          getValueBetweenTwoFixedColors(f->second/maxval,r,g,b);
          
          output<<r<<" "<<g<<" "<<b<<" ";
        }
        else
          output<<0<<" "<<0<<" "<<0<<" ";
      }
      output<<"\n";
    }
  }
}
#endif