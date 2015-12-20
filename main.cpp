#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
#include <SFML/Window.hpp>
#include "agony.hpp"
struct point;
using namespace std;
int main() {
  sf::RenderWindow r(sf::VideoMode(520, 256), "Symmetricity");
  Agony e;
  sf::View q;
  q.setSize(520, 256);
  q.setCenter(0, 0);
  r.setView(q);
  /*e.set_thing(1, 1, 0);
  e.set_thing(1, 2, 0);
  e.set_thing(1, 1, -1);
  e.set_thing(5,5,0);*/
  while (r.isOpen()) {
    sf::Event event;
    while (r.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        r.close();
      if (event.type == sf::Event::Resized) {
        q.setSize(r.getSize().x, r.getSize().y);
        r.setView(q);
      }
      if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i g = sf::Mouse::getPosition(r);
        auto q = r.mapPixelToCoords(g);
        q.x /= 10;
        q.y /= 10;
        e.mouse_over(q);//tells the thing to display a circle where the mouse is over.
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i g = sf::Mouse::getPosition(r);
        auto q = r.mapPixelToCoords(g);
        q.x /= 10;
        q.y /= 10;
        e.long_desig(q);
      }
      if (event.type == sf::Event::KeyPressed) {
        int times = 1;
        if (event.key.shift)
          times = 10;
        switch (event.key.code) {
        case sf::Keyboard::Left:
          for (int i = 0; i < times; i++)
            e.increment_cursor_x();
          break;
        case sf::Keyboard::Right:
          for (int i = 0; i < times; i++)
            e.decrement_cursor_x();
          break;
        case sf::Keyboard::Down:
          for (int i = 0; i < times; i++)
            e.decrement_cursor_y();
          break;
        case sf::Keyboard::Up:
          for (int i = 0; i < times; i++)
            e.increment_cursor_y();
          break;
        case sf::Keyboard::Period:
          e.increase_z();
          break;
        case sf::Keyboard::Comma:
          e.decrease_z();
          break;
        case sf::Keyboard::X:
          e.add_x_symmetry_at_cursor();
          break;
        case sf::Keyboard::Y:
          e.add_y_symmetry_at_cursor();
          break;
        case sf::Keyboard::Subtract:
        case sf::Keyboard::Dash:
          e.decrease_activity();
          break;
        case sf::Keyboard::Add:
        case sf::Keyboard::Equal:
          e.increase_activity();
          break;
        case sf::Keyboard::Space:
          e.designate();
          break;
        case sf::Keyboard::Return:
          e.long_desig();
          break;
        case sf::Keyboard::F5: //TODO Write a method to select a file to write to.
          e.write_file_output("output.csv");
        }
      }
    }
    r.clear();
    r.draw(e);
    r.display();
  }
  return 0;
}
