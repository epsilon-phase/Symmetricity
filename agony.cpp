#include "agony.hpp"
Agony::Agony() : csize(10), cursor_x(0),
                 cursor_y(0), current_z(0),
                 m_y_sym_on(false), m_x_sym_on(false),
                 isDesignating(false), current_activity(0), mouse_is_over(false) {
  m_vertz.setPrimitiveType(sf::Quads);
  fontthing.loadFromFile("LinLibertine_DRah.ttf");
  zz.setString("");
  zz.setFont(fontthing);
  zz.setColor(sf::Color(255, 255, 255));
  std::string a = "";
  a += (designations[current_activity] == '\0' ? 'x' : designations[current_activity]);
  zz.setString(a);
}
Agony::~Agony(){}
void Agony::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = NULL;
  target.draw(m_vertz, states);
  //draw the cursor
  sf::RectangleShape r(sf::Vector2f(csize, csize));
  r.setFillColor(sf::Color(0, 0, 0, 0));
  r.setOutlineColor(sf::Color(255, 255, 0, 255));
  r.setOutlineThickness(1.0);
  r.setOrigin(csize * cursor_x, csize * cursor_y);
  //this is the x axis symmetry thing
  sf::RectangleShape xSym(sf::Vector2f(2, target.getSize().y));
  xSym.setOrigin(csize * -xsym - csize / 2.0, target.getSize().y / 2);
  xSym.setFillColor(sf::Color(0, 255, 0, 255));
  //this is the y axis symmetry thing
  sf::RectangleShape ySym(sf::Vector2f(target.getSize().x, 2));
  ySym.setOrigin(target.getSize().x / 2, -ysym * csize);
  ySym.setFillColor(sf::Color(255, 0, 0, 255));
  if (isDesignating) {
    sf::CircleShape start_desig(csize / 2);
    start_desig.setFillColor(sf::Color(255, 0, 255));
    start_desig.setOrigin(m_start.x() * csize, m_start.y() * csize);
    target.draw(start_desig);
  }
  if (mouse_is_over) {
    sf::CircleShape q(csize / 2);
    q.setOrigin((int)-mouse_place.x * csize, -mouse_place.y * csize);
    q.setFillColor(sf::Color(0, 0, 0, 0));
    q.setOutlineColor(sf::Color(255, 255, 255, 255));
    q.setOutlineThickness(1);
    target.draw(q);
  }
  if (m_y_sym_on)
    target.draw(ySym);
  if (m_x_sym_on)
    target.draw(xSym);
  target.draw(r);
  zz.setOrigin(target.getSize().x / 2, target.getSize().y / 2);
  target.draw(zz);
}
void Agony::update() {
  std::vector<Eigen::Vector3d> things;
  std::vector<Eigen::Vector3d> torem;
  for (auto i : allowed) {
    if (std::round(i.first.z()) == current_z) {
      if (i.second != '\0') {
        things.push_back(i.first);
      } else {
        torem.push_back(i.first);
      }
    }
  }
  for (auto i : torem)
    allowed.erase(i);
  m_vertz.resize(things.size() * 4);

  m_vertz.setPrimitiveType(sf::PrimitiveType::Quads);
  for (int i = 0; i < things.size(); i++) {
    sf::Vertex *current = &m_vertz[i * 4];
    auto g = things[i];

    current[0].position = sf::Vector2f(g[0] * csize, g[1] * csize);
    current[1].position = sf::Vector2f((g[0] + 1) * csize, g[1] * csize);
    current[2].position = sf::Vector2f((1 + g[0]) * csize, (1 + g[1]) * csize);
    current[3].position = sf::Vector2f(g[0] * csize, (1 + g[1]) * csize);
    sf::Color curcol;
    switch (allowed[things[i]]) {
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
}
void Agony::long_desig() {
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
void Agony::insert_x_symmetry(const Eigen::Vector3d &c) {
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
void Agony::insert_y_symmetry(const Eigen::Vector3d &c) {

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
void Agony::designate(int x, int y, int z) {
  //std::cout<<x<<","<<y<<","<<z<<std::endl;
  Eigen::Vector3d c(x, y, z);
  allowed[c] = designations[current_activity];
  insert_x_symmetry(c);
}
void Agony::designate() {
  //std::cout << cursor_x << "," << cursor_y << std::endl;
  designate(-cursor_x, -cursor_y, current_z);
  update();
}
void Agony::mouse_over(const sf::Vector2f &e) {
  mouse_is_over = true;
  mouse_place.x = (int)std::round(e.x);
  mouse_place.y = (int)std::round(e.y);
}
void Agony::long_desig(const sf::Vector2f &e) {
  cursor_x = -(int)std::round(e.x);
  cursor_y = -(int)std::round(e.y);
  long_desig();
}
void Agony::increase_activity() {
  current_activity++;
  current_activity %= 5;
  std::string a = "";
  a += (designations[current_activity] == '\0' ? 'x' : designations[current_activity]);
  zz.setString(a);
}
void Agony::decrease_activity() {
  current_activity--;
  current_activity %= 5;
  std::string a = "";
  a += designations[current_activity] == '\0' ? 'x' : designations[current_activity];
  zz.setString(a);
}
void Agony::add_x_symmetry_at_cursor() {
  if (xsym != -cursor_x || !m_x_sym_on) {
    m_xsym = make_Reflection(cursor_x, 1, 1);
    xsym = -cursor_x;
    m_x_sym_on = true;
  } else {
    m_x_sym_on = false;
  }
}
void Agony::add_y_symmetry_at_cursor() {
  if (ysym != -cursor_y || !m_y_sym_on) {
    ysym = -cursor_y;
    m_y_sym_on = true;
  } else {
    m_y_sym_on = false;
  }
}
void Agony::set_thing(int x, int y, int z) {
  allowed.insert(std::pair<Eigen::Vector3d, int>(Eigen::Vector3d(x, y, z), designations[current_activity]));
  update();
}
void Agony::erase_position() {
  Eigen::Vector3d e(-cursor_x, -cursor_y, current_z);
  allowed.erase(e);
}