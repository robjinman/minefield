#ifndef __START_MENU_HPP__
#define __START_MENU_HPP__


#include "Menu.hpp"


class StartMenu : public Menu {
   public:
      StartMenu(const Dodge::XmlNode data);
      StartMenu(const StartMenu& copy);
      StartMenu(const StartMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual StartMenu* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~StartMenu();

   private:
      virtual void onMenuItemActivate(pMenuItem_t item);
};

typedef boost::shared_ptr<StartMenu> pStartMenu_t;


#endif
