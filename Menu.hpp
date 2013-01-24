#ifndef __MENU_HPP__
#define __MENU_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include <dodge/WinIO.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"
#include "MenuItem.hpp"


class Menu;
typedef boost::shared_ptr<Menu> pMenu_t;


class Menu : public Item, public Dodge::Sprite {
   public:
      Menu(const Dodge::XmlNode data);
      Menu(const Menu& copy);
      Menu(const Menu& copy, long name);

      void setActive(bool b);
      void setFocus(long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual Menu* clone() const = 0;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Menu() = 0;

   protected:
      virtual void onMenuItemKeyDown(pMenuItem_t item, int key) {}
      virtual void onMenuItemKeyUp(pMenuItem_t item, int key) {}
      virtual void onMenuItemActivate(pMenuItem_t item) {}
      virtual void onMenuItemGainFocus(pMenuItem_t item) {}
      virtual void onMenuItemLoseFocus(pMenuItem_t item) {}

      void activateSubmenu(long name);
      void returnToParentMenu();

      bool isActive() const;

   private:
      void init();

      void menuItemKeyDown(Dodge::pEntity_t entity, int key);
      void menuItemKeyUp(Dodge::pEntity_t entity, int key);
      void menuItemClick(Dodge::pEntity_t entity);
      void menuItemHoverOn(Dodge::pEntity_t, Dodge::float32_t x, Dodge::float32_t y);

      void setParent(Menu* parent);
      void returnFromSubmenu();

      int prevItem(int idx) const;
      int nextItem(int idx) const;

      Dodge::WinIO m_win;
      bool m_active;
      std::vector<pMenuItem_t> m_menuItems;
      std::vector<pMenu_t> m_submenus;
      int m_activeSubmenu;

      Menu* m_parent;

      int m_hasFocus; // idx of menu item with focus (-1 indicates no item has focus)
};


#endif
