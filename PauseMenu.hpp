#ifndef __PAUSE_MENU_HPP__
#define __PAUSE_MENU_HPP__


#include "Menu.hpp"
#include "CSprite.hpp"


class PauseMenu : public Menu {
   public:
      PauseMenu(const Dodge::XmlNode data);
      PauseMenu(const PauseMenu& copy);
      PauseMenu(const PauseMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual PauseMenu* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~PauseMenu();

   private:
      pCSprite_t m_flare;

      virtual void onMenuItemActivate(pMenuItem_t item);
};

typedef boost::shared_ptr<PauseMenu> pPauseMenu_t;


#endif
