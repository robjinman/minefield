#ifndef __GAME_OPTIONS_MENU_HPP__
#define __GAME_OPTIONS_MENU_HPP__


#include "Menu.hpp"


class GameOptionsMenu : public Menu {
   public:
      GameOptionsMenu(const Dodge::XmlNode data);
      GameOptionsMenu(const GameOptionsMenu& copy);
      GameOptionsMenu(const GameOptionsMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual GameOptionsMenu* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~GameOptionsMenu();

   private:
      void init();

      virtual void onMenuItemActivate(pMenuItem_t item);
      virtual void onMenuItemGainFocus(pMenuItem_t item);
      virtual void onMenuItemLoseFocus(pMenuItem_t item);

      Dodge::pUiButton_t m_difficultyDownBtn;
      Dodge::pUiButton_t m_difficultyUpBtn;
};

typedef boost::shared_ptr<GameOptionsMenu> pGameOptionsMenu_t;


#endif
