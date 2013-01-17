#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include "StartMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const XmlNode data)
   : Asset(internString("StartMenu")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_active(false) {

   try {
      XML_NODE_CHECK(data, StartMenu);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StartMenu; ");
   }

   init();
}

//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const StartMenu& copy)
   : Asset(internString("StartMenu")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_active(false) {

   init();
}

//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const StartMenu& copy, long name)
   : Asset(internString("StartMenu")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_active(false) {

   init();
}

//===========================================
// StartMenu::init
//===========================================
void StartMenu::init() {
   static long menuItemStr = internString("menuItem");
   static long mnuStartGameStr = internString("mnuStartGame");

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i) {
      (*i)->addToWorld();

      if ((*i)->getTypeName() == menuItemStr) {
         pMenuItem_t mnuItem = boost::dynamic_pointer_cast<MenuItem>(*i);

         // Give focus to 'start game' button
         if (mnuItem->getName() == mnuStartGameStr) {
            mnuItem->setFocus(true);
            m_hasFocus = m_menuItems.size();
         }

         m_menuItems.push_back(mnuItem);
         mnuItem->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &StartMenu::menuItemClick));
         mnuItem->registerCallback(UIEVENT_KEY_DOWN, Functor<void, TYPELIST_2(pEntity_t, int)>(this, &StartMenu::menuItemKeyDown));
         mnuItem->registerCallback(UIEVENT_HOVER_ON, Functor<void, TYPELIST_3(pEntity_t, float32_t, float32_t)>(this, &StartMenu::menuItemHoverOn));
      }
   }
}

//===========================================
// StartMenu::menuItemClick
//===========================================
void StartMenu::menuItemClick(pEntity_t entity) {
   static long mnuStartGameStr = internString("mnuStartGame");

   if (!m_active) return;

   if (entity->getName() == mnuStartGameStr) {
      EventManager eventManager;
      EEvent* event = new EEvent(internString("startGame"));
      eventManager.queueEvent(event);
   }
}

//===========================================
// StartMenu::menuItemHoverOn
//===========================================
void StartMenu::menuItemHoverOn(pEntity_t entity, float32_t x, float32_t y) {
   pMenuItem_t item = boost::dynamic_pointer_cast<MenuItem>(entity);

   if (item != m_menuItems[m_hasFocus]) {
      item->setFocus(true);

      for (uint_t i = 0; i < m_menuItems.size(); ++i) {
         if (m_menuItems[i] == item) {
            m_menuItems[m_hasFocus]->setFocus(false);
            m_hasFocus = i;
         }
      }
   }
}

//===========================================
// StartMenu::prevItem
//===========================================
int StartMenu::prevItem(int idx) const {
   if (idx < 0) return -1;
   if (idx >= static_cast<int>(m_menuItems.size())) return -1;

   return (idx == 0) ? m_menuItems.size() - 1 : idx - 1;
}

//===========================================
// StartMenu::nextItem
//===========================================
int StartMenu::nextItem(int idx) const {
   if (idx < 0) return -1;
   if (idx >= static_cast<int>(m_menuItems.size())) return -1;

   return idx == static_cast<int>(m_menuItems.size()) - 1 ? 0 : idx + 1;
}

//===========================================
// StartMenu::menuItemKeyDown
//===========================================
void StartMenu::menuItemKeyDown(pEntity_t entity, int key) {
   if (!m_active) return;

   assert(m_menuItems[m_hasFocus] == entity);
   assert(boost::dynamic_pointer_cast<MenuItem>(entity)->hasFocus());

   int idx = -1;
   switch (key) {
      case WinIO::KEY_UP:     idx = nextItem(m_hasFocus);   break;
      case WinIO::KEY_DOWN:   idx = prevItem(m_hasFocus);   break;
   }

   if (idx != -1) {
      m_menuItems[m_hasFocus]->setFocus(false);
      m_menuItems[idx]->setFocus(true);
      m_hasFocus = idx;
   }
}

//===========================================
// StartMenu::getSize
//===========================================
size_t StartMenu::getSize() const {
   return sizeof(StartMenu)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize();
}

//===========================================
// StartMenu::clone
//===========================================
StartMenu* StartMenu::clone() const {
   return new StartMenu(*this);
}

//===========================================
// StartMenu::addToWorld
//===========================================
void StartMenu::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// StartMenu::removeFromWorld
//===========================================
void StartMenu::removeFromWorld() {
   Sprite::removeFromWorld();
}

//===========================================
// StartMenu::onEvent
//===========================================
void StartMenu::onEvent(const EEvent* event) {
   if (!m_active) return;

}

#ifdef DEBUG
//===========================================
// StartMenu::dbg_print
//===========================================
void StartMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "StartMenu\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// StartMenu::update
//===========================================
void StartMenu::update() {
   if (!m_active) return;

   Sprite::update();

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i)
      (*i)->update();
}

//===========================================
// StartMenu::draw
//===========================================
void StartMenu::draw() const {
   Sprite::draw();

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i)
      (*i)->draw();
}

//===========================================
// StartMenu::assignData
//===========================================
void StartMenu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, StartMenu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StartMenu; ");
      throw;
   }
}

//===========================================
// StartMenu::~StartMenu
//===========================================
StartMenu::~StartMenu() {}
