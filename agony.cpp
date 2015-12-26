#include "agony.hpp"
#include <fstream>
#include <chrono>
#include <cmath>
using namespace std;
Agony::Agony() : csize(10), cursor_x(0),
                 cursor_y(0), current_z(0),
                 m_y_sym_on(false), m_x_sym_on(false),
                 isDesignating(false), current_desig(0),
                 mouse_is_over(false), isCircle(false),
                 m_radial(false), running(*this) {
  m_vertz.setPrimitiveType(sf::Quads);
  fontthing.loadFromFile("LinLibertine_DRah.ttf");
  zz.setString("");
  zz.setFont(fontthing);
  zz.setColor(sf::Color(255, 200, 0));
  std::string a = "Designating:";
  a += (designations[current_desig] == '\0' ? 'x' : designations[current_desig]);
  zz.setString(a);
}
Agony::~Agony() {}
void Agony::draw(sf::RenderTarget &target, sf::RenderStates states) const {

  states.transform *= getTransform();
  states.texture = NULL;
  target.draw(m_vertz, states);
  //draw the cursor
  sf::RectangleShape r(sf::Vector2f(csize, csize));
  r.setOrigin(this->getOrigin());
  r.setFillColor(sf::Color(0, 0, 0, 0));
  r.setOutlineColor(sf::Color(255, 255, 0, 255));
  r.setOutlineThickness(1.0);
  r.move(csize * cursor_x, csize * cursor_y);
  //this is the x axis symmetry thing
  sf::RectangleShape xSym(sf::Vector2f(2, target.getSize().y));
  xSym.setOrigin(getOrigin());

  xSym.move(csize * xsym + csize / 2, int(target.getSize().y) / -2);
  xSym.setFillColor(sf::Color(0, 255, 0, 255));
  //this is the y axis symmetry thing
  sf::RectangleShape ySym(sf::Vector2f(target.getSize().x, 2));
  ySym.setOrigin(getOrigin());
  ySym.move(target.getView().getSize().x / -2, ysym * csize + csize / 2);
  ySym.setFillColor(sf::Color(255, 0, 0, 255));
  sf::CircleShape rsym(csize / 2);
  rsym.setOrigin(getOrigin());
  rsym.move(r_sym_x * csize, csize * r_sym_y);
  rsym.setFillColor(sf::Color(255, 200, 0, 150));
  rsym.setOutlineColor(sf::Color(0, 0, 255, 255));
  rsym.setOutlineThickness(1);
  if (isDesignating) {
    sf::CircleShape start_desig(csize / 2);
    start_desig.setFillColor(sf::Color(255, 0, 255));
    start_desig.setOrigin(getOrigin());
    start_desig.move(m_start.x() * csize, m_start.y() * csize);
    target.draw(start_desig);
  }
  if (mouse_is_over) {
    sf::CircleShape q(csize / 2);
    q.setOrigin(getOrigin());
    q.move((int)mouse_place.x * csize, mouse_place.y * csize);
    q.setFillColor(sf::Color(0, 0, 0, 0));
    q.setOutlineColor(sf::Color(255, 255, 255, 255));
    q.setOutlineThickness(1);
    target.draw(q);
  }
  if (m_y_sym_on)
    target.draw(ySym);
  if (m_x_sym_on)
    target.draw(xSym);
  if (m_radial)
    target.draw(rsym);
  target.draw(r);
  if (!running.is_done()) {
    if (!running.is_paused()) {
      if (!runtime_tick) {
        auto f = chrono::system_clock::now();
        running.run_step();
        auto g = chrono::system_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(g - f).count();
        if (dur == 0)
          dur = 1;
        if (dur >= 10)
          dur = 10;
        this->max_path_ticks_per_frame = (int)(1000/30.0) / dur;
        cout<<"Ticks per frame:"<<max_path_ticks_per_frame<<endl;
        runtime_tick = true;
      } else {
        for (int i = 0; i < max_path_ticks_per_frame; i++) {
          running.run_step();
        }
      }
    }
    zz.Text::setString(std::to_string(running.get_progress()) + (running.is_paused() ? "(paused)" : ""));
  }
  zz.setOrigin(target.getSize().x / 2, target.getSize().y / 2);
  target.draw(zz);
  target.draw(start_vertz, states);
}
void Agony::update() {
  std::vector<Eigen::Vector3i> designations;
  std::vector<Eigen::Vector3i> buildings;
  std::vector<Eigen::Vector3i> torem_desig, torem_build;
  for (auto i : allowed) {
    if (i.first.z() == current_z) {
      if (i.second != '\0') {
        if (builds.find(i.first) != builds.end()) {
          if (builds[i.first] != "\0")
            buildings.push_back(i.first);
          else
            torem_build.push_back(i.first);
        }
        designations.push_back(i.first);
      } else {
        torem_desig.push_back(i.first);
      }
    }
  }
  for(auto i:builds){
    if(allowed.find(i.first)==allowed.end())
      torem_build.push_back(i.first);
  }
  for (auto i : torem_desig)
    allowed.erase(i);
  for (auto i : torem_build)
    builds.erase(i);
  m_vertz.resize(designations.size() * 4 + buildings.size() * 4);

  m_vertz.setPrimitiveType(sf::PrimitiveType::Quads);
  for (int i = 0; i < designations.size(); i++) {
    sf::Vertex *current = &m_vertz[i * 4];
    auto g = designations[i];

    current[0].position = sf::Vector2f(g[0] * csize, g[1] * csize);
    current[1].position = sf::Vector2f((g[0] + 1) * csize, g[1] * csize);
    current[2].position = sf::Vector2f((1 + g[0]) * csize, (1 + g[1]) * csize);
    current[3].position = sf::Vector2f(g[0] * csize, (1 + g[1]) * csize);

    sf::Color curcol;
    switch (allowed[designations[i]]) {
    case 'd':
      curcol = sf::Color(255, 255, 255);
      break;
    case 'j':
      curcol = sf::Color(255, 255, 0);
      break;
    case 'i':
      curcol = sf::Color(0, 255, 0, 255);
      break;
    case 'u':
      curcol = sf::Color(255, 0, 0, 255);
      break;
    case '\0':
      curcol = sf::Color(0, 0, 0);
      break;
    }
    current[0].color = curcol;
    current[1].color = curcol;
    current[2].color = curcol;
    current[3].color = curcol;
  }
  int i = designations.size();
  for (auto z : buildings) {
    auto current = &m_vertz[(i + 1) * 4];
    current[0].position = sf::Vector2f((.25+z[0]) * csize, (.25+z[1]) * csize);
    current[1].position = sf::Vector2f((z[0] + 0.75) * csize, (.25+z[1]) * csize);
    current[2].position = sf::Vector2f((.75 + z[0]) * csize, (0.75 + z[1]) * csize);
    current[3].position = sf::Vector2f((z[0]+.25) * csize, (0.75 + z[1]) * csize);
    sf::Color build_color = build_colors.find(builds.find(z)->second)->second;
    current[0].color = build_color;
    current[1].color = build_color;
    current[2].color = build_color;
    current[3].color = build_color;
    i++;
  }
  start_vertz.resize(3);
  start_vertz.setPrimitiveType(sf::PrimitiveType::Triangles);
  start_vertz[0].position = sf::Vector2f((start.x() + .5) * csize, start.y() * csize);
  start_vertz[1].position = sf::Vector2f((1 + start.x()) * csize, (1 + start.y()) * csize);
  start_vertz[2].position = sf::Vector2f(start.x() * csize, (1 + start.y()) * csize);
  start_vertz[0].color = sf::Color::Blue;
  start_vertz[1].color = sf::Color::Blue;
  start_vertz[2].color = sf::Color::Blue;
}
void Agony::long_desig() {
  if (!isDesignating) {
    m_start = Eigen::Vector3i(cursor_x, cursor_y, current_z);
  } else {
    m_end = Eigen::Vector3i(cursor_x, cursor_y, current_z);
    if (isCircle) {
      draw_circle();
      isCircle = false;

    } else {
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
            designate(i, j, k);
          }
      update();
    }
  }
  update_text();
  isDesignating = !isDesignating;
}
void Agony::insert_x_symmetry(const Eigen::Vector3i &c) {
  if (m_x_sym_on) {
    Eigen::Vector3i f;
    int diff = std::abs(c.x() - (xsym));
    if (c.x() <= xsym) {
      //std::cout << "low" << std::endl;
      f = Eigen::Vector3i((xsym)+diff, c.y(), c.z());
    } else {
      //std::cout << "high" << std::endl;
      f = Eigen::Vector3i((xsym)-diff, c.y(), c.z());
    }
    if (!is_build)
      allowed[f] = designations[current_desig];
    else
      builds[f] = buildables[current_build];
    insert_y_symmetry(f);
  }
  insert_y_symmetry(c);
}
void Agony::insert_y_symmetry(const Eigen::Vector3i &c) {

  if (m_y_sym_on) {
    Eigen::Vector3i f;
    int diff = std::abs(c.y() - (ysym));
    if (c.y() <= ysym) {
      f = Eigen::Vector3i(c.x(), (ysym)+diff, c.z());
    } else {
      f = Eigen::Vector3i(c.x(), (ysym)-diff, c.z());
    }
    if (!is_build)
      allowed[f] = designations[current_desig];
    else
      builds[f] = buildables[current_build];
  }
}
void Agony::insert_radial_symmetry(const Eigen::Vector3i &c) {
  if (m_radial) {
    int diff_x = c.x() - r_sym_x;
    int diff_y = c.y() - r_sym_y;
    auto f = Eigen::Vector3i(r_sym_x - diff_x, r_sym_y - diff_y, c.z());
    if (!is_build)
      allowed[f] = designations[current_desig];
    else
      builds[f] = buildables[current_build];
    insert_x_symmetry(f);
  }
  insert_x_symmetry(c);
}
void Agony::designate(int x, int y, int z) {
  //std::cout<<x<<","<<y<<","<<z<<std::endl;
  Eigen::Vector3i c(x, y, z);
  if (!is_build)
    allowed[c] = designations[current_desig];
  else
    builds[c] = buildables[current_build];
  insert_radial_symmetry(c);
}
void Agony::designate() {
  //std::cout << cursor_x << "," << cursor_y << std::endl;
  designate(cursor_x, cursor_y, current_z);
  update();
}
void Agony::mouse_over(const sf::Vector2f &e) {
  mouse_is_over = true;
  mouse_place.x = (int)std::round(e.x);
  mouse_place.y = (int)std::round(e.y);
}
void Agony::long_desig(const sf::Vector2f &e) {
  cursor_x = (int)std::round(e.x);
  cursor_y = (int)std::round(e.y);
  long_desig();
}
void Agony::increase_activity() {

  if (is_build) {
    current_build++;
    current_build %= buildables.size();
  } else {
    current_desig++;
    current_desig %= 5;
  }
  update_text();
}
void Agony::decrease_activity() {
  if (is_build) {
    if (current_build == 0)
      current_build = buildables.size() - 1;
    else
      current_build--;
  } else {
    if (current_desig == 0)
      current_desig = 4;
    else
      current_desig--;
    current_desig %= 5;
  }
  update_text();
}
void Agony::update_text() {
  std::string a;
  if (!is_file_entry) {
    if (!is_build) {
      a = "Designating";
    } else {
      a = "Building";
    }
    if (isCircle) {
      a += "(Circle)";
    }
    a += ":";
    if (!is_build) {
      a += designations[current_desig] == '\0' ? 'x' : designations[current_desig];
      on_designation_change(current_desig);
    } else {
      if (buildables[current_build] != "\0")
        a += buildables[current_build];
      else
        a += "removing";
    }
  } else {
    if (is_saving) {
      if (serializing)
        a = "Save to:";
      else
        a = "Export to:";
    } else {
      a = "Load from:";
    }
    a += this->save_prompt;
  }
  zz.setString(a);
}
void Agony::add_x_symmetry_at_cursor() {
  if (xsym != cursor_x || !m_x_sym_on) {
    m_xsym = make_Reflection(cursor_x, 1, 1);
    xsym = cursor_x;
    m_x_sym_on = true;
  } else {
    m_x_sym_on = false;
  }
}
void Agony::add_y_symmetry_at_cursor() {
  if (ysym != cursor_y || !m_y_sym_on) {
    ysym = cursor_y;
    m_y_sym_on = true;
  } else {
    m_y_sym_on = false;
  }
}
void Agony::add_radial_symmetry_at_cursor() {
  if (this->m_radial || (r_sym_x != cursor_x && r_sym_y != cursor_y)) {
    m_radial = true;
    r_sym_x = cursor_x;
    r_sym_y = cursor_y;
  } else
    m_radial = false;
}
void Agony::set_thing(int x, int y, int z) {
  allowed.insert(std::pair<Eigen::Vector3i, int>(Eigen::Vector3i(x, y, z), designations[current_desig]));
  update();
}
void Agony::erase_position() {
  Eigen::Vector3i e(cursor_x, cursor_y, current_z);
  allowed.erase(e);
}
std::tuple<int, int, int, int, int, int> Agony::getBoundaries() const {
  int minx = 200000, miny = 200000, minz = 200000;
  int maxx = -200000, maxy = -200000, maxz = -200000;
  for (auto i : allowed) {
    auto z = i.first;
    if (z[0] < minx)
      minx = z[0];
    if (z[1] < miny)
      miny = z[1];
    if (z[2] < minz)
      minz = z[2];
    if (z[0] > maxx)
      maxx = z[0];
    if (z[1] > maxy)
      maxy = z[1];
    if (z[2] > maxz)
      maxz = z[2];
  }
  return std::make_tuple(minx, miny, minz, maxx, maxy, maxz);
}
void Agony::write_file_output(const std::string &output_name) const {
  auto boundaries = getBoundaries();
  ofstream out(output_name + "-dig.csv");
  out << "#dig start(" << abs(start[0] - get<0>(boundaries))+1 << ";" << abs(start[1] - get<1>(boundaries))+1 << ")" << endl;
  for (int z = std::get<5>(boundaries); z >= std::get<2>(boundaries); z--) {
    for (int y = std::get<1>(boundaries); y <= std::get<4>(boundaries); y++) {
      for (int x = std::get<0>(boundaries); x <= std::get<3>(boundaries); x++) {
        Eigen::Vector3i h = Eigen::Vector3i(x, y, z);
        if (allowed.find(h) != allowed.end()) {
          out << allowed.find(h)->second;
        } else {
          out << " ";
        }
        out << ",";
      }
      out << "#" << endl;
    }
    if (z > std::get<2>(boundaries))
      out << "#>" << endl;
  }
  out.close();
  if (builds.size() > 0) {
    out.open(output_name + "-build.csv");
    out << "#build start(" << abs(start[0] - get<0>(boundaries))+1 << ";" << abs(start[1] - get<1>(boundaries))+1 << ")" << endl;
    for (int z = std::get<5>(boundaries); z >= std::get<2>(boundaries); z--) {
      for (int y = std::get<1>(boundaries); y <= std::get<4>(boundaries); y++) {
        for (int x = std::get<0>(boundaries); x <= std::get<3>(boundaries); x++) {
          Eigen::Vector3i h = Eigen::Vector3i(x, y, z);
          if (builds.find(h) != builds.end()) {
            out << builds.find(h)->second;
          } else {
            out << " ";
          }
          out << ",";
        }
        out << "#" << endl;
      }
      if (z > std::get<2>(boundaries))
        out << "#>" << endl;
    }
  }
}
void Agony::set_circle() {
  this->isCircle = true;
  update_text();
}
float distance(const Eigen::Vector3i &a, const Eigen::Vector3i &b) {
  auto c = Eigen::Vector3i(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
  return std::sqrt(c.x() * c.x() + c.y() * c.y() + c.z() * c.z());
}
void Agony::draw_circle() {
  float xx = (m_start[0] - m_end[0]),
        yy = (m_start[1] - m_end[1]);

  int r = sqrt(xx * xx + yy * yy);
  //std::cout<<r<<endl;
  float hy = m_start[1];
  float hx = m_start[0];
  std::cout << "h=" << hx << ",k=" << hy << endl;
  for (int x = hx - r - 1; x <= hx + r + 1; x++) {
    for (int y = hy - r - 1; y <= hy + r + 1; y++) {
      for (int z = m_start[2]; z <= m_end[2]; z++) {
        cout << x << "," << y << endl;
        float v = ((x - hx) * (x - hx)) / (r * r) +
                  ((y - hy) * (y - hy)) / (r * r);
        if (v <= 1) {
          designate(x, y, z);
        }
      }
    }
  }
  update();
}
void Agony::move_over(int x, int y) {
  auto g = getOrigin();
  g.x /= csize;
  g.y /= csize;
  setOrigin(g.x * csize + x * csize, g.y * csize + y * csize);
}
void Agony::set_designation_type(int i) {
  current_desig = i;
  update_text();
}
void Agony::setActivityCallback(std::function<void(int)> a) {
  this->on_designation_change = a;
}
void Agony::serialize(const std::string &f) const {
  ofstream e(f);
  for (auto i : allowed) {
    auto z = i.first;
    auto j = i.second;
    e << z.x() << " " << z.y() << " " << z.z() << " " << j << endl;
  }
  for (auto i : builds) {
    auto z = i.first;
    auto j = i.second;
    e << z.x() << " " << z.y() << " " << z.z() << " b " << j << endl;
  }
  e.close();
}
void Agony::deserialize(const std::string &f) {
  allowed.clear();
  builds.clear();
  ifstream q(f);
  double x, y, z;
  char c;
  while (q >> x >> y >> z >> c) {
    if (c != 'b')
      allowed[Eigen::Vector3i(x, y, z)] = c;
    else {
      string building;
      q >> building;
      builds[Eigen::Vector3i(x, y, z)] = building;
    }
  }
  update();
}
void Agony::handle_entry(sf::Event::TextEvent a) {
  if (is_file_entry && a.unicode < 128 && a.unicode != 8) { //filter out backspace.
    save_prompt += static_cast<char>(a.unicode);
  }
  update_text();
}
void Agony::finish_entry() {
  if (is_file_entry) {
    if (!is_saving) {
      deserialize(this->save_prompt);
    } else {
      if (serializing)
        serialize(save_prompt);
      else {
        write_file_output(save_prompt);
      }
    }
    previous_save = save_prompt;
  }
  is_file_entry = false;
  update_text();
}
void Agony::abort_entry() {
  is_file_entry = false;
  is_saving = false;
  serializing = false;
  update_text();
}
void Agony::start_save() {
  save_prompt = "";
  is_file_entry = true;
  is_saving = true;
}
void Agony::start_serialize() {
  save_prompt = "";
  is_file_entry = true;
  is_saving = true;
  serializing = true;
  std::cout << "write" << endl;
}
void Agony::start_load() {
  save_prompt = "";
  is_file_entry = true;
  is_saving = false;
  serializing = true;
}
void Agony::handle_keyboard(sf::Event::KeyEvent a) {
  if (is_file_entry && !(a.code == sf::Keyboard::Escape || a.code == sf::Keyboard::Return || a.code == sf::Keyboard::BackSpace)) {
    return;
  }
  int times = 1;
  if (a.shift)
    times = 10;
  switch (a.code) {
  case sf::Keyboard::Escape:
    abort_entry();
    break;
  case sf::Keyboard::Right:
    if (a.control) {
      move_over(1, 0);
    } else
      for (int i = 0; i < times; i++)
        increment_cursor_x();
    break;
  case sf::Keyboard::Left:
    if (a.control)
      move_over(-1, 0);
    else
      for (int i = 0; i < times; i++)
        decrement_cursor_x();
    break;
  case sf::Keyboard::Up:
    if (a.control)
      move_over(0, -1);
    else
      for (int i = 0; i < times; i++)
        decrement_cursor_y();
    break;
  case sf::Keyboard::Down:
    if (a.control)
      move_over(0, 1);
    else
      for (int i = 0; i < times; i++)
        increment_cursor_y();
    break;
  case sf::Keyboard::Period:
    decrease_z();
    break;
  case sf::Keyboard::Comma:
    increase_z();
    break;
  case sf::Keyboard::X:
    add_x_symmetry_at_cursor();
    break;
  case sf::Keyboard::Y:
    add_y_symmetry_at_cursor();
    break;
  case sf::Keyboard::Subtract:
  case sf::Keyboard::Dash:
    decrease_activity();
    break;
  case sf::Keyboard::Add:
  case sf::Keyboard::Equal:
    increase_activity();
    break;
  case sf::Keyboard::B:
    is_build = !is_build;
  case sf::Keyboard::Space:
    designate();
    break;
  case sf::Keyboard::R:
    if (a.shift)
      m_radial = false;
    else
      add_radial_symmetry_at_cursor();
    break;
  case sf::Keyboard::Return:
    if (is_entry()) {
      finish_entry();
      cout << "Ended" << endl;
    } else {
      if (a.shift)
        set_circle();
      long_desig();
    }
    break;
  case sf::Keyboard::F5:
    if (is_entry())
      finish_entry();
    if (a.shift)
      start_save();
    else
      start_serialize();
    break;
  case sf::Keyboard::E:
    if (save_prompt.size() != 0) {
      write_file_output(save_prompt);
    }
    break;
  case sf::Keyboard::F6:
    start_load();
    break;
  case sf::Keyboard::F7:
    if (!running.is_set_up())
      running.run_analysis(this->previous_save);
    else {
      running.toggle_pause();
    }
    runtime_tick=false;
    break;
  case sf::Keyboard::F8:
    if (running.is_set_up() && !running.is_done())
      cout << running.get_top_cycle() << " cycles" << endl;
    cout << allowed.size() << endl;
    break;
  case sf::Keyboard::BackSpace:
    if (is_file_entry) {
      save_prompt.pop_back();
    }
    break;
  }
}
