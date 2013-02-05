#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include "ERequestGameStateChange.hpp"
#include "PauseMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// PauseMenu::PauseMenu
//===========================================
PauseMenu::PauseMenu(const XmlNode data)
   : Asset(internString("PauseMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      AssetManager assetManager;

      XML_NODE_CHECK(data, PauseMenu);

      XmlNode node = data.nthChild(1);
      XML_NODE_CHECK(node, flare);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      long id = attr.getLong();

      m_flare = boost::dynamic_pointer_cast<CSprite>(assetManager.getAssetPointer(id));

      if (!m_flare)
         throw XmlException("Bad asset id for flare item", __FILE__, __LINE__);

      m_flare->addToWorld();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class PauseMenu; ");
      throw;
   }
}

//===========================================
// PauseMenu::PauseMenu
//===========================================
PauseMenu::PauseMenu(const PauseMenu& copy)
   : Asset(internString("PauseMenu")),
     Entity(copy),
     Menu(copy) {}

//===========================================
// PauseMenu::PauseMenu
//===========================================
PauseMenu::PauseMenu(const PauseMenu& copy, long name)
   : Asset(internString("PauseMenu")),
     Entity(copy, name),
     Menu(copy, name) {}

//===========================================
// PauseMenu::onMenuItemActivate
//===========================================
void PauseMenu::onMenuItemActivate(pMenuItem_t item) {
   static long pauseMnuQuitStr = internString("pauseMnuQuit");
   static long settingsMenuStr = internString("settingsMenu");
   static long pauseMnuSettingsStr = internString("pauseMnuSettings");
   static long pauseMnuResumeStr = internString("pauseMnuResume");

   if (item->getName() == pauseMnuSettingsStr) {
      activateSubmenu(settingsMenuStr);
   }
   else if (item->getName() == pauseMnuResumeStr) {
      EventManager eventManager;
      EEvent* event = new ERequestGameStateChange(ST_RUNNING);
      eventManager.queueEvent(event);
   }
   else if (item->getName() == pauseMnuQuitStr) {
      EventManager eventManager;
      EEvent* event = new ERequestGameStateChange(ST_START_MENU);
      eventManager.queueEvent(event);
   }
}

//===========================================
// PauseMenu::getSize
//===========================================
size_t PauseMenu::getSize() const {
   return sizeof(PauseMenu)
      - sizeof(Menu)
      + Menu::getSize();
}

//===========================================
// PauseMenu::clone
//===========================================
Asset* PauseMenu::clone() const {
   return new PauseMenu(*this);
}

//===========================================
// PauseMenu::addToWorld
//===========================================
void PauseMenu::addToWorld() {
   Menu::addToWorld();
   m_flare->addToWorld();
}

//===========================================
// PauseMenu::removeFromWorld
//===========================================
void PauseMenu::removeFromWorld() {
   Menu::removeFromWorld();
   m_flare->removeFromWorld();
}

#ifdef DEBUG
//===========================================
// PauseMenu::dbg_print
//===========================================
void PauseMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "PauseMenu\n";

   Item::dbg_print(out, tab + 1);
   Menu::dbg_print(out, tab + 1);
}
#endif

//===========================================
// PauseMenu::update
//===========================================
void PauseMenu::update() {
   static long rotateStr = internString("rotate");

   Menu::update();
   m_flare->update();

   if (m_flare) {
      if (m_flare->numActiveTransformations() == 0)
         m_flare->playTransformation(rotateStr);
   }
}

//===========================================
// PauseMenu::draw
//===========================================
void PauseMenu::draw() const {
   Menu::draw();
   m_flare->draw();
}

//===========================================
// PauseMenu::assignData
//===========================================
void PauseMenu::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, PauseMenu)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Menu") {
         Menu::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class PauseMenu; ");
      throw;
   }
}

//===========================================
// PauseMenu::~PauseMenu
//===========================================
PauseMenu::~PauseMenu() {}
