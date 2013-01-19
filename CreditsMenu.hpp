#ifndef __CREDITS_MENU_HPP__
#define __CREDITS_MENU_HPP__


#include "Menu.hpp"


class CreditsMenu : public Menu {
   public:
      CreditsMenu(const Dodge::XmlNode data);
      CreditsMenu(const CreditsMenu& copy);
      CreditsMenu(const CreditsMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual CreditsMenu* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~CreditsMenu();

   private:
      virtual void onMenuItemActivate(pMenuItem_t item);
};

typedef boost::shared_ptr<CreditsMenu> pCreditsMenu_t;


#endif
