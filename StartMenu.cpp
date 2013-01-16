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

   const set<pEntity_t>& children = getChildren();
   for (auto i = children.begin(); i != children.end(); ++i) {
      (*i)->addToWorld();

      if ((*i)->getTypeName() == menuItemStr) {
         pMenuItem_t mnuItem = boost::dynamic_pointer_cast<MenuItem>(*i);

         m_menuItems.push_back(mnuItem);
         mnuItem->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &StartMenu::menuItemClick));
      }
   }

   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &StartMenu::keyDown));
}

//===========================================
// StartMenu::menuItemClick
//===========================================
void StartMenu::menuItemClick(pEntity_t entity) {
   static long mnuStartGameStr = internString("mnuStartGame");
cout << "StartMenu::menuItemClick()\n";
   if (!m_active) return;

   if (entity->getName() == mnuStartGameStr) {
      EventManager eventManager;
      EEvent* event = new EEvent(internString("startGame"));
      eventManager.queueEvent(event);
   }
}

//===========================================
// StartMenu::keyDown
//===========================================
void StartMenu::keyDown(int key) { // TODO
   if (!m_active) return;

   EventManager eventManager;
   EEvent* event = new EEvent(internString("startGame"));
   eventManager.queueEvent(event);
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

   // TODO
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
StartMenu::~StartMenu() {
   m_win.unregisterCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &StartMenu::keyDown));
}
