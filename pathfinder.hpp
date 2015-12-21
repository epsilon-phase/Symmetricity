#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP
#include <string>
class Agony;
class GraphAnalyzer{
public:
  GraphAnalyzer(const Agony& ag);
  void run_analysis(const std::string& filename)const;
private:
  const Agony& ag;
};
#endif