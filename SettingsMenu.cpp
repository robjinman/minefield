#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include "SettingsMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const XmlNode data)
   : Asset(internString("SettingsMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, SettingsMenu);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class SettingsMenu; ");
      throw;
   }
}

//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const SettingsMenu& copy)
   : Asset(internString("SettingsMenu")),
     Entity(copy),
     Menu(copy) {}

//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const SettingsMenu& copy, long name)
   : Asset(internString("SettingsMenu")),
     Entity(copy, name),
     Menu(copy, name) {}

//===========================================
// SettingsMenu::onMenuItemActivate
//===========================================
void SettingsMenu::onMenuItemActivate(pMenuItem_t item) {
   static long mnuSettingsToStartMenuStr = internString("mnuSettingsToStartMenu");

   if (item->getName() == mnuSettingsToStartMenuStr)
      returnToParentMenu();
}

//===========================================
// SettingsMenu::getSize
//===========================================
size_t SettingsMenu::getSize() const {
   return sizeof(SettingsMenu)
      - sizeof(Menu)
      + Menu::getSize();
}

//===========================================
// SettingsMenu::clone
//===========================================
SettingsMenu* SettingsMenu::clone() const {
   return new SettingsMenu(*this);
}

//===========================================
// SettingsMenu::addToWorld
//===========================================
void SettingsMenu::addToWorld() {
   Menu::addToWorld();
}

//===========================================
// SettingsMenu::removeFromWorld
//===========================================
void SettingsMenu::removeFromWorld() {
   Menu::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// SettingsMenu::dbg_print
//===========================================
void SettingsMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "SettingsMenu\n";

   Item::dbg_print(out, tab + 1);
   Menu::dbg_print(out, tab + 1);
}
#endif

//===========================================
// SettingsMenu::update
//===========================================
void SettingsMenu::update() {
   Menu::update();
}

//===========================================
// SettingsMenu::draw
//===========================================
void SettingsMenu::draw() const {
   Menu::draw();
}

//===========================================
// SettingsMenu::assignData
//===========================================
void SettingsMenu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, SettingsMenu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Menu") {
         Menu::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class SettingsMenu; ");
      throw;
   }
}

//===========================================
// SettingsMenu::~SettingsMenu
//===========================================
SettingsMenu::~SettingsMenu() {}
