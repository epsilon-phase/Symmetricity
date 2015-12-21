#ifndef DESIGNATION_MENU_HPP
#define DESIGNATION_MENU_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
class Menu:public sf::Drawable, public sf::Transformable{
  std::vector<sf::Text> texts;
  std::vector<std::function<void()> > clickfuncs;
  sf::Font menu_font;
public:
  Menu()
  {
    menu_font.loadFromFile("LinBiolinum_RIah.ttf");
  }
  void addItem(const std::string &text,std::function<void()> func);
  void onclick(const sf::Vector2f& coords);
  void set_selected_item(int i);
private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};
#endif