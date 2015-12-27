#ifndef AGONY_HPP
#define AGONY_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
//#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>

#include "pathfinder.hpp"

static const char designations[] = {'d', 'i', 'j', 'u', '\0'};
static const std::vector<std::string> buildables = {"b", "d", "p", "t", "c", "f", "Cw", "Cf", "CF", "\0"};
static const std::unordered_map<std::string, sf::Color> build_colors = {
    {"d", sf::Color(255, 0, 255)}, {"b", sf::Color(0, 0, 255)}, {"p", sf::Color(152, 110, 20)}, {"Cw", sf::Color(40, 255, 0)}, {"Cf", sf::Color(255, 40, 0)}, {"CF", sf::Color(0, 40, 255)}, {"t", sf::Color(29, 133, 132)}, {"c", sf::Color(111, 79, 89)}};
/**
* A set containing the features that are likely to need to be queried and rooms expanded.
*/
static const std::unordered_set<std::string> room_things = {
    "b", "t", "c"};
static const std::array<std::string, 3> query_preference_order = {"b", "t", "c"};
/**
* Contains the constructions that block the expansions of rooms.
*/
static const std::unordered_set<std::string> blocking_constructions = {
    "Cw", "d"};
class Agony : public sf::Drawable, public sf::Transformable {

public:
  friend class GraphAnalyzer;
  Agony();
  virtual ~Agony();
  void set_thing(int x, int y, int z);
  void increase_z() {
    if (!is_file_entry) {
      current_z++;
      update();
    }
  }
  void decrease_z() {
    if (!is_file_entry) {
      current_z--;
      update();
    }
  }
  void increment_cursor_x() { cursor_x++; }
  void increment_cursor_y() { cursor_y++; }
  void decrement_cursor_x() { cursor_x--; }
  void decrement_cursor_y() { cursor_y--; }
  void move_over(int x, int y);
  void add_y_symmetry_at_cursor();
  void add_x_symmetry_at_cursor();
  void add_radial_symmetry_at_cursor();
  void increase_activity();
  void set_circle();
  void decrease_activity();
  void designate(int x, int y, int z);
  void insert_x_symmetry(const Eigen::Vector3i &c);
  void insert_y_symmetry(const Eigen::Vector3i &c);
  void insert_radial_symmetry(const Eigen::Vector3i &c);
  void designate();
  void mouse_over(const sf::Vector2f &e);
  void long_desig(const sf::Vector2f &e);
  void long_desig();
  void erase_position();
  void write_file_output(const std::string &output_name) const;
  std::string getStatus() const;
  void set_designation_type(int i);
  void setActivityCallback(std::function<void(int)> a);
  void serialize(const std::string &f) const;
  void deserialize(const std::string &f);
  void handle_entry(sf::Event::TextEvent a);
  void start_save();
  void start_serialize();
  void start_load();
  void finish_entry();
  bool is_entry() const { return is_file_entry; }
  bool line_thing();
  void handle_keyboard(sf::Event::KeyEvent);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void update();
  void draw_circle();
  void update_text();
  void abort_entry();
  void add_rot_90_symmetry_at_cursor();
  void insert_rot_90_symmetry(const Eigen::Vector3i& a);
  std::tuple<int, int, int, int, int, int> getBoundaries() const;
  int csize;
  int cursor_x, cursor_y;
  int current_z;
  //std::set<Eigen::Vector3i> allowed;
  std::unordered_map<Eigen::Vector3i, char> allowed;
  std::unordered_map<Eigen::Vector3i, std::string> builds;
  std::unordered_multimap<std::string, Eigen::Vector3i> furniture;
  Eigen::Vector3i m_start, m_end;
  sf::VertexArray m_vertz;
  sf::VertexArray start_vertz;
  sf::Font fontthing;
  bool m_x_sym_on, m_y_sym_on, m_z_sym_on;
  Eigen::Matrix3d m_xsym, m_ysym;
  Eigen::Vector2i start = Eigen::Vector2i(0, 0);
  sf::Vector2f mouse_place;
  bool mouse_is_over;
  int xsym, ysym, zsym;
  bool isDesignating, isCircle;
  int current_desig, current_build = 0;
  mutable sf::Text zz;
  ///Radial symmetry location
  int r_sym_x, r_sym_y;
  bool m_radial=false;
  bool m_rot_90_ccw=false;
  int rot_90_ccw_x,rot_90_ccw_y;
  std::function<void(int)> on_designation_change;
  std::string save_prompt;
  std::string previous_save;
  bool serializing = false;
  bool is_saving = false;
  bool is_file_entry = false;
  bool line_start = false;
  bool is_build = false;
  bool generate_query = true;
  mutable GraphAnalyzer running;
  mutable int max_path_ticks_per_frame = 1;
  mutable bool runtime_tick = false;
};
#endif