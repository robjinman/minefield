#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include "CreditsMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// CreditsMenu::CreditsMenu
//===========================================
CreditsMenu::CreditsMenu(const XmlNode data)
   : Asset(internString("CreditsMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, CreditsMenu);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CreditsMenu; ");
      throw;
   }
}

//===========================================
// CreditsMenu::CreditsMenu
//===========================================
CreditsMenu::CreditsMenu(const CreditsMenu& copy)
   : Asset(internString("CreditsMenu")),
     Entity(copy),
     Menu(copy) {}

//===========================================
// CreditsMenu::CreditsMenu
//===========================================
CreditsMenu::CreditsMenu(const CreditsMenu& copy, long name)
   : Asset(internString("CreditsMenu")),
     Entity(copy, name),
     Menu(copy, name) {}

//===========================================
// CreditsMenu::onMenuItemActivate
//===========================================
void CreditsMenu::onMenuItemActivate(pMenuItem_t item) {
   static long mnuBackToStartMenuStr = internString("mnuCreditsToStartMenu");

   if (item->getName() == mnuBackToStartMenuStr)
      returnToParentMenu();
}

//===========================================
// CreditsMenu::getSize
//===========================================
size_t CreditsMenu::getSize() const {
   return sizeof(CreditsMenu)
      - sizeof(Menu)
      + Menu::getSize();
}

//===========================================
// CreditsMenu::clone
//===========================================
CreditsMenu* CreditsMenu::clone() const {
   return new CreditsMenu(*this);
}

//===========================================
// CreditsMenu::addToWorld
//===========================================
void CreditsMenu::addToWorld() {
   Menu::addToWorld();
}

//===========================================
// CreditsMenu::removeFromWorld
//===========================================
void CreditsMenu::removeFromWorld() {
   Menu::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// CreditsMenu::dbg_print
//===========================================
void CreditsMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "CreditsMenu\n";

   Item::dbg_print(out, tab + 1);
   Menu::dbg_print(out, tab + 1);
}
#endif

//===========================================
// CreditsMenu::update
//===========================================
void CreditsMenu::update() {
   Menu::update();
}

//===========================================
// CreditsMenu::draw
//===========================================
void CreditsMenu::draw() const {
   Menu::draw();
}

//===========================================
// CreditsMenu::assignData
//===========================================
void CreditsMenu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, CreditsMenu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Menu") {
         Menu::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CreditsMenu; ");
      throw;
   }
}

//===========================================
// CreditsMenu::~CreditsMenu
//===========================================
CreditsMenu::~CreditsMenu() {}
