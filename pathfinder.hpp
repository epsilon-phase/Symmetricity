#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP
#include <string>
#include <Eigen/Dense>
#include <unordered_map>
#include "utility.hpp"
class Agony;
class GraphAnalyzer{
public:
  GraphAnalyzer(const Agony& ag);
  void run_analysis(const std::string& filename);
  void run_step();
  bool is_done()const{return m_done;}
  int get_progress()const;
private:
  void set_up();
  void output();
  const Agony& ag;
  bool m_done;
  int top_cycles=0;
  std::string filename;
  std::unordered_map<Eigen::Vector3d,std::unordered_map<Eigen::Vector3d,double> > costs;
  std::unordered_map<Eigen::Vector3d,char>::const_iterator k;
};
#endif