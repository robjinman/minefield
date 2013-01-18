#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include "StartMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const XmlNode data)
   : Asset(internString("StartMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, StartMenu);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StartMenu; ");
      throw;
   }
}

//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const StartMenu& copy)
   : Asset(internString("StartMenu")),
     Entity(copy),
     Menu(copy) {}

//===========================================
// StartMenu::StartMenu
//===========================================
StartMenu::StartMenu(const StartMenu& copy, long name)
   : Asset(internString("StartMenu")),
     Entity(copy, name),
     Menu(copy, name) {}

//===========================================
// StartMenu::onMenuItemActivate
//===========================================
void StartMenu::onMenuItemActivate(pMenuItem_t item) {
   static long settingsMenuStr = internString("settingsMenu");
   static long mnuStartGameStr = internString("mnuStartGame");
   static long mnuSettingsStr = internString("mnuSettings");
   static long mnuQuitStr = internString("mnuQuit");

   if (item->getName() == mnuStartGameStr) {
      EventManager eventManager;
      EEvent* event = new EEvent(internString("startGame"));
      eventManager.queueEvent(event);
   }
   else if (item->getName() == mnuSettingsStr) {
      activateSubmenu(settingsMenuStr);
   }
   else if (item->getName() == mnuQuitStr) {
      EventManager eventManager;
      EEvent* event = new EEvent(internString("quitGame"));
      eventManager.queueEvent(event);
   }
}

//===========================================
// StartMenu::getSize
//===========================================
size_t StartMenu::getSize() const {
   return sizeof(StartMenu)
      - sizeof(Menu)
      + Menu::getSize();
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
   Menu::addToWorld();
}

//===========================================
// StartMenu::removeFromWorld
//===========================================
void StartMenu::removeFromWorld() {
   Menu::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// StartMenu::dbg_print
//===========================================
void StartMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "StartMenu\n";

   Item::dbg_print(out, tab + 1);
   Menu::dbg_print(out, tab + 1);
}
#endif

//===========================================
// StartMenu::update
//===========================================
void StartMenu::update() {
   Menu::update();
}

//===========================================
// StartMenu::draw
//===========================================
void StartMenu::draw() const {
   Menu::draw();
}

//===========================================
// StartMenu::assignData
//===========================================
void StartMenu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, StartMenu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Menu") {
         Menu::assignData(node);
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
