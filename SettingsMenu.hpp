#ifndef __SETTINGS_MENU_HPP__
#define __SETTINGS_MENU_HPP__


#include "Menu.hpp"


class SettingsMenu : public Menu {
   public:
      SettingsMenu(const Dodge::XmlNode data);
      SettingsMenu(const SettingsMenu& copy);
      SettingsMenu(const SettingsMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual SettingsMenu* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~SettingsMenu();

   private:
      virtual void onMenuItemActivate(pMenuItem_t item);
};

typedef boost::shared_ptr<SettingsMenu> pSettingsMenu_t;


#endif
