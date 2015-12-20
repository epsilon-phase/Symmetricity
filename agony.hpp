#ifndef AGONY_HPP
#define AGONY_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <map>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <string>
#include "ReflectionMatrix.hpp"
namespace std {
template <>
struct less<Eigen::Vector3d> {
  bool operator()(const Eigen::Vector3d &a, const Eigen::Vector3d &b) {
    return a[0] == b[0] ? (a[1] == b[1] ? (a[2] < b[2]) : a[1] < b[1]) : a[0] < b[0];
  }
};
}
static const char designations[] = {'d', 'i', 'j', 'u', '\0'};
class Agony : public sf::Drawable, public sf::Transformable {

public:
  Agony() ;
  virtual ~Agony();
  void set_thing(int x, int y, int z);
  void increase_z() {
    current_z++;
    update();
  }
  void decrease_z() {
    current_z--;
    update();
  }
  void increment_cursor_x() { cursor_x++; }
  void increment_cursor_y() { cursor_y++; }
  void decrement_cursor_x() { cursor_x--; }
  void decrement_cursor_y() { cursor_y--; }
  void add_y_symmetry_at_cursor();
  void add_x_symmetry_at_cursor() ;
  void increase_activity() ;
  
  void decrease_activity();
  void designate(int x, int y, int z) ;
  void insert_x_symmetry(const Eigen::Vector3d &c);
  void insert_y_symmetry(const Eigen::Vector3d &c);
  void designate();
  void mouse_over(const sf::Vector2f& e);
  void long_desig(const sf::Vector2f& e);
  void long_desig();
  void erase_position();
private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void update() ;
  
  int csize;
  int cursor_x, cursor_y;
  int current_z;
  //std::set<Eigen::Vector3d> allowed;
  std::map<Eigen::Vector3d, char> allowed;
  Eigen::Vector3d m_start, m_end;
  sf::VertexArray m_vertz;
  sf::Font fontthing;
  bool m_x_sym_on, m_y_sym_on, m_z_sym_on;
  Eigen::Matrix3d m_xsym, m_ysym;
  sf::Vector2f mouse_place;
  bool mouse_is_over;
  int xsym, ysym, zsym;
  bool isDesignating;
  int current_activity;
  mutable sf::Text zz;
};
#endif