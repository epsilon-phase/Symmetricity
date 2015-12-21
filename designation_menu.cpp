#include "designation_menu.hpp"
#include <iostream>
void Menu::onclick(const sf::Vector2f &coords){
  for(int i=0;i<texts.size();i++)
    if(texts[i].getGlobalBounds().contains(coords)){
      clickfuncs[i]();
      std::cout<<texts[i].getString().toAnsiString()<<std::endl;
    }
}
void Menu::addItem(const std::string &text, std::function<void ()> func){
  sf::Text f;
  f.setFont(menu_font);
  f.setOrigin(getOrigin());
  if(!texts.empty())
    for(auto i:texts)
    f.move(0,i.getLocalBounds().height);
  f.setString(text);
  f.setColor(sf::Color(255,255,255));
  f.setCharacterSize(16);
  texts.push_back(f);
  clickfuncs.push_back(func);
}
void Menu::draw(sf::RenderTarget& target, sf::RenderStates states)const{
  for(auto f:texts)
    target.draw(f);
}
void Menu::set_selected_item(int i){
  for(int z=0;z<texts.size();z++)
    texts[z].setColor(sf::Color(255,255,255));
  texts[i].setColor(sf::Color(255,0,0));
}