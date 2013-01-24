#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include <dodge/globals.hpp>
#include "GameOptionsMenu.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// GameOptionsMenu::GameOptionsMenu
//===========================================
GameOptionsMenu::GameOptionsMenu(const XmlNode data)
   : Asset(internString("GameOptionsMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      AssetManager assetManager;

      XML_NODE_CHECK(data, GameOptionsMenu);

      XmlNode node = data.nthChild(1);
      XML_NODE_CHECK(node, difficultyDownBtn);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      long id = attr.getLong();

      m_difficultyDownBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_difficultyDownBtn)
         throw XmlException("Bad asset id for difficultyDownBtn", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, difficultyUpBtn);

      attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      id = attr.getLong();

      m_difficultyUpBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_difficultyUpBtn)
         throw XmlException("Bad asset id for difficultyUpBtn", __FILE__, __LINE__);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class GameOptionsMenu; ");
      throw;
   }

   init();
}

//===========================================
// GameOptionsMenu::GameOptionsMenu
//===========================================
GameOptionsMenu::GameOptionsMenu(const GameOptionsMenu& copy)
   : Asset(internString("GameOptionsMenu")),
     Entity(copy),
     Menu(copy) { init(); }

//===========================================
// GameOptionsMenu::GameOptionsMenu
//===========================================
GameOptionsMenu::GameOptionsMenu(const GameOptionsMenu& copy, long name)
   : Asset(internString("GameOptionsMenu")),
     Entity(copy, name),
     Menu(copy, name) { init(); }

//===========================================
// GameOptionsMenu::init
//===========================================
void GameOptionsMenu::init() {
   m_difficultyDownBtn->setFocus(true);
   m_difficultyUpBtn->setFocus(true);

   m_difficultyDownBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_difficultyDownBtn->addActivationKey(WinIO::KEY_LEFT);

   m_difficultyUpBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_difficultyUpBtn->addActivationKey(WinIO::KEY_RIGHT);
}

//===========================================
// GameOptionsMenu::onMenuItemActivate
//===========================================
void GameOptionsMenu::onMenuItemActivate(pMenuItem_t item) {
   static long mnuGameOptionsToStartMenuStr = internString("mnuGameOptionsToStartMenu");

   if (item->getName() == mnuGameOptionsToStartMenuStr)
      returnToParentMenu();
}

//===========================================
// GameOptionsMenu::onMenuItemGainFocus
//===========================================
void GameOptionsMenu::onMenuItemGainFocus(pMenuItem_t item) {
   static long mnuDifficultyStr = internString("mnuDifficulty");

   if (item->getName() == mnuDifficultyStr) {
      m_difficultyDownBtn->setFocus(true);
      m_difficultyUpBtn->setFocus(true);
   }
}

//===========================================
// GameOptionsMenu::onMenuItemLoseFocus
//===========================================
void GameOptionsMenu::onMenuItemLoseFocus(pMenuItem_t item) {
   static long mnuDifficultyStr = internString("mnuDifficulty");

   if (item->getName() == mnuDifficultyStr) {
      m_difficultyDownBtn->setFocus(false);
      m_difficultyUpBtn->setFocus(false);
   }
}

//===========================================
// GameOptionsMenu::getSize
//===========================================
size_t GameOptionsMenu::getSize() const {
   return sizeof(GameOptionsMenu)
      - sizeof(Menu)
      + Menu::getSize();
}

//===========================================
// GameOptionsMenu::clone
//===========================================
GameOptionsMenu* GameOptionsMenu::clone() const {
   return new GameOptionsMenu(*this);
}

//===========================================
// GameOptionsMenu::addToWorld
//===========================================
void GameOptionsMenu::addToWorld() {
   Menu::addToWorld();

   m_difficultyDownBtn->addToWorld();
   m_difficultyUpBtn->addToWorld();
}

//===========================================
// GameOptionsMenu::removeFromWorld
//===========================================
void GameOptionsMenu::removeFromWorld() {
   Menu::removeFromWorld();

   m_difficultyDownBtn->removeFromWorld();
   m_difficultyUpBtn->removeFromWorld();
}

#ifdef DEBUG
//===========================================
// GameOptionsMenu::dbg_print
//===========================================
void GameOptionsMenu::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "GameOptionsMenu\n";

   Item::dbg_print(out, tab + 1);
   Menu::dbg_print(out, tab + 1);
}
#endif

//===========================================
// GameOptionsMenu::update
//===========================================
void GameOptionsMenu::update() {
   Menu::update();

   m_difficultyDownBtn->update();
   m_difficultyUpBtn->update();
}

//===========================================
// GameOptionsMenu::draw
//===========================================
void GameOptionsMenu::draw() const {
   Menu::draw();

   m_difficultyDownBtn->draw();
   m_difficultyUpBtn->draw();
}

//===========================================
// GameOptionsMenu::assignData
//===========================================
void GameOptionsMenu::assignData(const XmlNode data) {
   assert(false);
}

//===========================================
// GameOptionsMenu::~GameOptionsMenu
//===========================================
GameOptionsMenu::~GameOptionsMenu() {}
