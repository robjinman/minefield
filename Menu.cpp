#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include "Menu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Menu::Menu
//===========================================
Menu::Menu(const XmlNode data)
   : Asset(internString("Menu")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_active(false),
     m_activeSubmenu(-1),
     m_parent(NULL),
     m_hasFocus(-1) {

   try {
      XML_NODE_CHECK(data, Menu);

      XmlNode node = data.nthChild(2);
      XML_NODE_CHECK(node, submenus);

      XmlNode node_ = node.firstChild();
      m_submenus.clear();
      while (!node_.isNull() && node_.name() == "submenu") {
         XmlAttribute attr = node_.firstAttribute();
         XML_ATTR_CHECK(attr, ptr);

         AssetManager assetManager;
         pMenu_t submenu = boost::dynamic_pointer_cast<Menu>(assetManager.getAssetPointer(attr.getLong()));

         if (!submenu)
            throw XmlException("Bad submenu asset id", __FILE__, __LINE__);

         submenu->setParent(this);
         m_submenus.push_back(submenu);

         node_ = node_.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Menu; ");
      throw;
   }

   init();
}

//===========================================
// Menu::Menu
//===========================================
Menu::Menu(const Menu& copy)
   : Asset(internString("Menu")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_active(false),
     m_activeSubmenu(-1),
     m_parent(NULL),
     m_hasFocus(-1) {

   init();
}

//===========================================
// Menu::Menu
//===========================================
Menu::Menu(const Menu& copy, long name)
   : Asset(internString("Menu")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_active(false),
     m_activeSubmenu(-1),
     m_parent(NULL),
     m_hasFocus(-1) {

   init();
}

//===========================================
// Menu::init
//===========================================
void Menu::init() {
   static long menuItemStr = internString("menuItem");

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i) {
      if ((*i)->getTypeName() == menuItemStr) {
         pMenuItem_t mnuItem = boost::dynamic_pointer_cast<MenuItem>(*i);

         if (!mnuItem)
            throw Exception("Error constructing Menu; Menu item is not of class MenuItem", __FILE__, __LINE__);

         m_menuItems.push_back(mnuItem);
         mnuItem->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &Menu::menuItemClick));
         mnuItem->registerCallback(UIEVENT_KEY_DOWN, Functor<void, TYPELIST_2(pEntity_t, int)>(this, &Menu::menuItemKeyDown));
         mnuItem->registerCallback(UIEVENT_HOVER_ON, Functor<void, TYPELIST_3(pEntity_t, float32_t, float32_t)>(this, &Menu::menuItemHoverOn));
      }
   }

   if (m_menuItems.size() > 0)
      setFocus(m_menuItems.back()->getName());
}

//===========================================
// Menu::activateSubmenu
//===========================================
void Menu::activateSubmenu(long name) {
   for (uint_t i = 0; i < m_submenus.size(); ++i) {
      if (m_submenus[i]->getName() == name) {
         if (m_activeSubmenu != -1) m_submenus[m_activeSubmenu]->removeFromWorld();

         m_submenus[i]->addToWorld();
         removeFromWorld();
         m_activeSubmenu = i;
         break;
      }
   }
}

//===========================================
// Menu::returnToParentMenu
//===========================================
void Menu::returnToParentMenu() {
   if (m_parent) m_parent->returnFromSubmenu();
}

//===========================================
// Menu::setParent
//===========================================
void Menu::setParent(Menu* parent) {
   m_parent = parent;
}

//===========================================
// Menu::returnFromSubmenu
//===========================================
void Menu::returnFromSubmenu() {
   if (m_activeSubmenu != -1) m_submenus[m_activeSubmenu]->removeFromWorld();

   addToWorld();
   m_activeSubmenu = -1;
}

//===========================================
// Menu::setFocus
//===========================================
void Menu::setFocus(long name) {
   for (uint_t i = 0; i < m_menuItems.size(); ++i) {
      if (m_menuItems[i]->getName() == name) {
         if (static_cast<int>(i) != m_hasFocus) {
            m_menuItems[i]->setFocus(true);
            if (m_hasFocus != -1) m_menuItems[m_hasFocus]->setFocus(false);
            m_hasFocus = i;
         }
         break;
      }
   }
}

//===========================================
// Menu::menuItemClick
//===========================================
void Menu::menuItemClick(pEntity_t entity) {
   if (!m_active) return;

   pMenuItem_t item = boost::dynamic_pointer_cast<MenuItem>(entity);
   assert(item);

   onMenuItemActivate(item);
}

//===========================================
// Menu::menuItemHoverOn
//===========================================
void Menu::menuItemHoverOn(pEntity_t entity, float32_t x, float32_t y) {
   if (!m_active) return;

   pMenuItem_t item = boost::dynamic_pointer_cast<MenuItem>(entity);
   assert(item);

   setFocus(item->getName());
}

//===========================================
// Menu::prevItem
//===========================================
int Menu::prevItem(int idx) const {
   if (idx < 0) return -1;
   if (idx >= static_cast<int>(m_menuItems.size())) return -1;

   return (idx == 0) ? m_menuItems.size() - 1 : idx - 1;
}

//===========================================
// Menu::nextItem
//===========================================
int Menu::nextItem(int idx) const {
   if (idx < 0) return -1;
   if (idx >= static_cast<int>(m_menuItems.size())) return -1;

   return idx == static_cast<int>(m_menuItems.size()) - 1 ? 0 : idx + 1;
}

//===========================================
// Menu::menuItemKeyDown
//===========================================
void Menu::menuItemKeyDown(pEntity_t entity, int key) {
   if (!m_active) return;

   pMenuItem_t item = boost::dynamic_pointer_cast<MenuItem>(entity);

   assert(item);
   assert(m_menuItems[m_hasFocus] == item);
   assert(item->hasFocus());

   int idx = -1;
   switch (key) {
      case WinIO::KEY_UP:     idx = nextItem(m_hasFocus);   break;
      case WinIO::KEY_DOWN:   idx = prevItem(m_hasFocus);   break;
   }

   if (idx != -1) {
      item->setFocus(false);
      m_menuItems[idx]->setFocus(true);
      m_hasFocus = idx;
   }

   onMenuItemKeyDown(item, key);
}

//===========================================
// Menu::getSize
//===========================================
size_t Menu::getSize() const {
   return sizeof(Menu)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize();
}

//===========================================
// Menu::addToWorld
//===========================================
void Menu::addToWorld() {
   m_active = true;

   Sprite::addToWorld();

   for (uint_t i = 0; i < m_menuItems.size(); ++i)
      m_menuItems[i]->addToWorld();
}

//===========================================
// Menu::removeFromWorld
//===========================================
void Menu::removeFromWorld() {
   Sprite::removeFromWorld();

   for (uint_t i = 0; i < m_menuItems.size(); ++i)
      m_menuItems[i]->removeFromWorld();

   m_active = false;
}

//===========================================
// Menu::onEvent
//===========================================
void Menu::onEvent(const EEvent* event) {
   if (!m_active) return;

}

#ifdef DEBUG
//===========================================
// Menu::dbg_print
//===========================================
void Menu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Menu\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Menu::update
//===========================================
void Menu::update() {
   if (m_activeSubmenu != -1)
      m_submenus[m_activeSubmenu]->update();

   if (!m_active) return;

   Sprite::update();

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i)
      (*i)->update();
}

//===========================================
// Menu::draw
//===========================================
void Menu::draw() const {
   if (m_activeSubmenu != -1)
      m_submenus[m_activeSubmenu]->draw();

   if (!m_active) return;

   Sprite::draw();

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i)
      (*i)->draw();
}

//===========================================
// Menu::assignData
//===========================================
void Menu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Menu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "submenu") {

         XmlNode node_ = node.firstChild();
         m_submenus.clear();
         while (!node_.isNull() && node_.name() == "submenu") {
            XmlAttribute attr = node_.firstAttribute();
            XML_ATTR_CHECK(attr, ptr);

            AssetManager assetManager;
            pMenu_t submenu = boost::dynamic_pointer_cast<Menu>(assetManager.getAssetPointer(attr.getLong()));

            if (!submenu)
               throw XmlException("Bad submenu asset id", __FILE__, __LINE__);

            m_submenus.push_back(submenu);

            node_ = node_.nextSibling();
         }
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Menu; ");
      throw;
   }
}

//===========================================
// Menu::~Menu
//===========================================
Menu::~Menu() {}
