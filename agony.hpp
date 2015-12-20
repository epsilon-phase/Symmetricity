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
public:
  Agony() : csize(10), cursor_x(0),
            cursor_y(0),current_z(0),
            m_y_sym_on(false), m_x_sym_on(false),
            isDesignating(false), current_activity(0),mouse_is_over(false) {
    m_vertz.setPrimitiveType(sf::Quads);
    fontthing.loadFromFile("LinBiolinum_RIah.ttf");
    zz.setString("");
    zz.setFont(fontthing);
    zz.setColor(sf::Color(255,255,255));
    std::string a="";
    a+=(designations[current_activity]=='\0'?'x':designations[current_activity]);
    zz.setString(a);
  }
  virtual ~Agony() {}
  void set_thing(int x, int y, int z) {
    allowed.insert(std::pair<Eigen::Vector3d, int>(Eigen::Vector3d(x, y, z), designations[current_activity]));
    update();
  }
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
  void add_y_symmetry_at_cursor() {
    if (ysym != -cursor_y || !m_y_sym_on) {
      ysym = -cursor_y;
      m_y_sym_on = true;
    } else {
      m_y_sym_on = false;
    }
  }
  void add_x_symmetry_at_cursor() {
    if (xsym != -cursor_x || !m_x_sym_on) {
      m_xsym = make_Reflection(cursor_x, 1, 1);
      xsym = -cursor_x;
      m_x_sym_on = true;
    } else {
      m_x_sym_on = false;
    }
  }
  void increase_activity() {
    current_activity++;
    current_activity %= 5;
    std::string a="";
    a+=(designations[current_activity]=='\0'?'x':designations[current_activity]);
    zz.setString(a);
  }
  
  void decrease_activity(){
    current_activity--;
    current_activity %= 5;
    std::string a="";
    a+=designations[current_activity]=='\0'?'x':designations[current_activity];
    zz.setString(a);
  }
  void designate(int x, int y, int z) {
    //std::cout<<x<<","<<y<<","<<z<<std::endl;
    Eigen::Vector3d c(x, y, z);
    allowed[c] = designations[current_activity];
    insert_x_symmetry(c);
  }
  void insert_x_symmetry(const Eigen::Vector3d &c) {
    if (m_x_sym_on) {
      Eigen::Vector3d f;
      int diff = std::abs(c.x() - (xsym));
      if (c.x() <= xsym) {
        //std::cout << "low" << std::endl;
        f = Eigen::Vector3d((xsym)+diff, c.y(), c.z());
      } else {
        //std::cout << "high" << std::endl;
        f = Eigen::Vector3d((xsym)-diff, c.y(), c.z());
      }
      allowed[f] = designations[current_activity];
      insert_y_symmetry(f);
    }
    insert_y_symmetry(c);
  }
  void insert_y_symmetry(const Eigen::Vector3d &c) {

    if (m_y_sym_on) {
      Eigen::Vector3d f;
      int diff = std::abs(c.y() - (ysym));
      if (c.y() <= ysym) {
        f = Eigen::Vector3d(c.x(), (ysym)+diff, c.z());
      } else {
        f = Eigen::Vector3d(c.x(), (ysym)-diff, c.z());
      }
      allowed[f] = designations[current_activity];
    }
  }
  void designate() {
    //std::cout << cursor_x << "," << cursor_y << std::endl;
    designate(-cursor_x, -cursor_y, current_z);
    update();
  }
  void mouse_over(const sf::Vector2f& e){
    mouse_is_over=true;
    mouse_place.x=(int)std::round(e.x);
    mouse_place.y=(int)std::round(e.y);
  }
  void long_desig(const sf::Vector2f& e){
    cursor_x=-(int)std::round(e.x);
    cursor_y=-(int)std::round(e.y);
    long_desig();
  }
  void long_desig() {
    if (!isDesignating) {
      m_start = Eigen::Vector3d(cursor_x, cursor_y, current_z);
    } else {
      m_end = Eigen::Vector3d(cursor_x, cursor_y, current_z);
      if (m_end[0] < m_start[0])
        std::swap(m_end[0], m_start[0]);
      if (m_end[1] < m_start[1])
        std::swap(m_end[1], m_start[1]);
      if (m_end[2] < m_start[2])
        std::swap(m_end[2], m_start[2]);
      //std::cout<<allowed.size()<<"/";
      for (int i = m_start[0]; i <= m_end[0]; i++)
        for (int j = m_start[1]; j <= m_end[1]; j++)
          for (int k = m_start[2]; k <= m_end[2]; k++) {
            designate(-i, -j, k);
        }
      update();
    }
    isDesignating = !isDesignating;
  }
  void erase_position(){
    Eigen::Vector3d e(-cursor_x,-cursor_y,current_z);
    allowed.erase(e);
  }
private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void update() ;
};
#endif