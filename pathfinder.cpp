#include "pathfinder.hpp"
#include "agony.hpp"
using namespace std;
map<char, vector<Eigen::Vector3d>> pathables;
GraphAnalyzer::GraphAnalyzer(const Agony &ag) : ag(ag) {
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

void GraphAnalyzer::run_analysis(const std::string &filename) const {
  //TODO write floyd-whatver algorithm to find the distances between all the points
  //Then write the statistics of each node and (max-path-length avg-path-length)
}