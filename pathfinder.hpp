#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP
#include <string>
#include <eigen3/Eigen/Dense>
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
  float get_progress()const;
  void toggle_pause(){m_paused=!m_paused;}
  bool is_set_up()const{return m_set_up;}
  int get_top_cycle()const{return top_cycles;}
  void stop();
private:
  void bfs(const Eigen::Vector3i& start);
  void set_up();
  void output();
  const Agony& ag;
  bool m_done,m_paused=false,m_set_up=false;
  int top_cycles=0;
  std::string filename;
  std::unordered_map<Eigen::Vector3i,std::unordered_map<Eigen::Vector3i,double> > costs;
  std::unordered_map<Eigen::Vector3i,char>::const_iterator k,i;
};
#endif