#include "agony.hpp"
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