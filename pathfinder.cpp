#include "pathfinder.hpp"
#include "agony.hpp"
#include <limits>
#include <fstream>
#include "ppm.hpp"

using namespace std;
map<char, vector<Eigen::Vector3d>> pathables;
GraphAnalyzer::GraphAnalyzer(const Agony &ag) : ag(ag) {
  m_done = true;
  if (pathables.size() == 0) {
    pathables['d'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, 0, 1));

    pathables['j'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, 0, -1));

    pathables['i'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, 0, -1));
    pathables['i'].push_back(Eigen::Vector3d(0, 0, 1));
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
void GraphAnalyzer::set_up() {
  top_cycles = 0;
  auto q = ag.allowed;
  for (auto j : q)
    for (auto i : q)
      costs[j.first][i.first] = numeric_limits<double>::infinity();
  for (auto i : q) { //the distance between all vertices and themselves are zero.
    auto c = i.first;
    costs[c][c] = 0;
  }
  for (auto i : q) { //set all adjacent things to 1
    auto z = i.first;
    for (auto v : pathables[i.second]) { //TODO fix stair connections to ensure that they are mutually connective.
      auto current = Eigen::Vector3d(z[0] + v[0], z[1] + v[1], z[2] + v[2]);
      if (q.find(current) != q.end())
        costs[i.first][current] = 1;
    }
  }
  m_set_up = true;
}
void GraphAnalyzer::run_step() {

  auto q = ag.allowed;
  if (k != ag.allowed.end()) {
    for (auto j : q) {
      if (costs[i->first][j.first] > costs[i->first][k->first] + costs[k->first][j.first])
        costs[i->first][j.first] = costs[i->first][k->first] + costs[k->first][j.first];
    }
    i++;
    if (i == ag.allowed.end()) {
      i=ag.allowed.begin();
      k++;
      top_cycles++;
    }
  } else {
    this->m_done = true;
    output();
  }
}
int GraphAnalyzer::get_progress() const {
  return top_cycles / double(ag.allowed.size()) * 100;
}
void GraphAnalyzer::output() {
  ofstream output(filename);
  std::unordered_map<Eigen::Vector3d, double> maxes;
  for (auto i : costs) {
    auto z = i.first;
    output << z[0] << " " << z[1] << " " << z[2] << " ";
    double max = 0;
    for (auto c : i.second) {
      if (c.second > max && c.second != numeric_limits<double>::infinity())
        max += c.second / double(i.second.size());
    }
    maxes[i.first] = max;
    output << max << endl;
  }
  auto c = ag.getBoundaries();
  write_ppm_to_file(filename, maxes,
                    c);
  costs.clear();
  m_set_up = false;
}