#ifndef __CREDITS_MENU_HPP__
#define __CREDITS_MENU_HPP__


#include <vector>
#include <string>
#include "Menu.hpp"


class CreditsMenu : public Menu {
   public:
      CreditsMenu(const Dodge::XmlNode data);
      CreditsMenu(const CreditsMenu& copy);
      CreditsMenu(const CreditsMenu& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual Asset* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~CreditsMenu();

   private:
      void init();
      std::string getVersionNumber() const;
      virtual void onMenuItemActivate(pMenuItem_t item);

      Dodge::pFont_t m_font;
      std::vector<Dodge::pTextEntity_t> m_textEntities;
      Dodge::float32_t m_txtAlpha;
      Dodge::float32_t m_fadeInTime;
};

typedef boost::shared_ptr<CreditsMenu> pCreditsMenu_t;


#endif
