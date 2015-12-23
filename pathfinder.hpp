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
  bool is_paused()const{return m_paused;}
  int get_progress()const;
  void toggle_pause(){m_paused=!m_paused;}
  bool is_set_up()const{return m_set_up;}
private:
  void set_up();
  void output();
  const Agony& ag;
  bool m_done,m_paused=false,m_set_up=false;
  int top_cycles=0;
  std::string filename;
  std::unordered_map<Eigen::Vector3d,std::unordered_map<Eigen::Vector3d,double> > costs;
  std::unordered_map<Eigen::Vector3d,char>::const_iterator k,i;
};
#endif