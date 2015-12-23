#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
#include <SFML/Window.hpp>
#include "agony.hpp"
#include "designation_menu.hpp"
struct point;
using namespace std;
int main() {
  sf::RenderWindow r(sf::VideoMode(520, 256), "Symmetricity");
  bool menumode = false;
  Agony e;
  sf::View q;
  q.setSize(520, 256);
  q.setCenter(0, 0);
  r.setView(q);
  bool enter_text = false;
  string text;
  sf::Text save_file_display;
  sf::Font font;
  font.loadFromFile("LinBiolinum_RIah.ttf");
  save_file_display.setFont(font);
  Menu menustuff;
  e.setActivityCallback([&menustuff](int a) {menustuff.set_selected_item(a); });
  menustuff.addItem("Dig", [&e]() {
    e.set_designation_type(0);
  });
  menustuff.addItem("Up/Down staircase", [&e]() {e.set_designation_type(1); });
  menustuff.addItem("Down staircase", [&e]() {e.set_designation_type(2); });
  menustuff.addItem("Up staircase", [&e]() {e.set_designation_type(3); });
  menustuff.addItem("Remove Designation", [&e]() {e.set_designation_type(4); });

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
        menustuff.setOrigin(q.getSize().x / 2, 0);
      }
      if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i g = sf::Mouse::getPosition(r);

        auto q = r.mapPixelToCoords(g);
        q.x /= 10;
        q.y /= 10;
        e.mouse_over(q); //tells the thing to display a circle where the mouse is over.
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i g = sf::Mouse::getPosition(r);
        if (event.mouseButton.button == 1) {
          menumode = !menumode;
          continue;
        }
        if (menumode&&menustuff.onclick(r.mapPixelToCoords(g,q))){
          std::cout<<"clicked menu"<<endl;
        } else {
          auto q = r.mapPixelToCoords(g);
          q.x /= 10;
          q.y /= 10;
          e.long_desig(q);
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        e.handle_keyboard(event.key);
      }
      if (event.type == event.TextEntered)
        e.handle_entry(event.text);
    }

    r.clear();
    r.draw(e);
    if (menumode)
      r.draw(menustuff);
    r.display();
  }
  return 0;
}
