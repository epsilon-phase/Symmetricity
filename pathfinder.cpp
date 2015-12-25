#include "pathfinder.hpp"
#include "agony.hpp"
#include <limits>
#include <fstream>
#include <queue>
#include "ppm.hpp"
#include <unordered_set>

using namespace std;
map<char, vector<Eigen::Vector3i>> pathables;
GraphAnalyzer::GraphAnalyzer(const Agony &ag) : ag(ag) {
  m_done = true;
  if (pathables.size() == 0) {
    pathables['d'].push_back(Eigen::Vector3i(1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3i(-1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3i(1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3i(-1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3i(0, 1, 0));
    pathables['d'].push_back(Eigen::Vector3i(0, -1, 0));
    pathables['d'].push_back(Eigen::Vector3i(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3i(1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3i(-1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3i(1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3i(-1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3i(0, 1, 0));
    pathables['u'].push_back(Eigen::Vector3i(0, -1, 0));
    pathables['u'].push_back(Eigen::Vector3i(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3i(0, 0, 1));

    pathables['j'].push_back(Eigen::Vector3i(1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3i(-1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3i(1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3i(-1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3i(0, 1, 0));
    pathables['j'].push_back(Eigen::Vector3i(0, -1, 0));
    pathables['j'].push_back(Eigen::Vector3i(-1, -1, 0));
    pathables['j'].push_back(Eigen::Vector3i(0, 0, -1));

    pathables['i'].push_back(Eigen::Vector3i(1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3i(-1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3i(1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3i(-1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3i(0, 1, 0));
    pathables['i'].push_back(Eigen::Vector3i(0, -1, 0));
    pathables['i'].push_back(Eigen::Vector3i(-1, -1, 0));
    pathables['i'].push_back(Eigen::Vector3i(0, 0, -1));
    pathables['i'].push_back(Eigen::Vector3i(0, 0, 1));
  }
}
void GraphAnalyzer::run_analysis(const std::string &filename) {
  this->filename = filename;
  this->costs.clear();
  m_done = false;
  this->k = ag.allowed.begin();
  this->i=ag.allowed.begin();
  set_up();
}
void GraphAnalyzer::bfs(const Eigen::Vector3i& start){
  int ccost=0;
  int nodes_to_go=1;
  int next_level=0;
  queue<Eigen::Vector3i> q;
  q.push(start);
  costs[start][start]=0;
  unordered_set<Eigen::Vector3i> visited;
  while(!q.empty()){
    auto f=q.front();
    q.pop();
    nodes_to_go--;
    auto pf=pathables[this->ag.allowed.find(f)->second];
    for(int z=0;z<pf.size();z++){
      auto zz=Eigen::Vector3i(pf[z][0]+f[0],pf[z][1]+f[1],pf[z][2]+f[2]);
      if(ag.allowed.end()!=ag.allowed.find(zz)&&visited.find(zz)==visited.end()){
        costs[start][zz]=ccost;
        q.push(zz);
        next_level++;
        visited.insert(zz);
      }
    }
    if(nodes_to_go<=0){
      nodes_to_go=next_level;
      next_level=0;
      ccost++;
    }
  }
}
void GraphAnalyzer::set_up() {
  top_cycles = 0;
/*  for (auto i : q) { //set all adjacent things to 1
    auto z = i.first;
    for (auto v : pathables[i.second]) { //TODO fix stair connections to ensure that they are mutually connective.
      auto current = Eigen::Vector3i(z[0] + v[0], z[1] + v[1], z[2] + v[2]);
      if (q.find(current) != q.end())
        costs[i.first][current] = 1;
    }
  }*/
  
  m_set_up = true;
}
void GraphAnalyzer::run_step() {

  auto q = ag.allowed;
  if (k != ag.allowed.end()) {
    bfs(k->first);
    k++;
    top_cycles++;
  } else {
    this->m_done = true;
    output();
  }
}
float GraphAnalyzer::get_progress() const {
  return top_cycles / float(ag.allowed.size()) * 100;
}
void GraphAnalyzer::output() {
  std::unordered_map<Eigen::Vector3i, double> maxes;
  for (auto i : costs) {
    auto z = i.first;
    double max = 0;
    for (auto c : i.second) {
      //if (c.second > max && c.second != numeric_limits<double>::infinity())
        max += c.second;
    }
    maxes[i.first] = max;
  }
  auto c = ag.getBoundaries();
  write_ppm_to_file(filename, maxes,
                    c);
  costs.clear();
  m_set_up = false;
}
void GraphAnalyzer::stop(){
  m_set_up=false;
  m_done=false;
  costs.clear();
  k=ag.allowed.begin();
  i=ag.allowed.begin();
  
}