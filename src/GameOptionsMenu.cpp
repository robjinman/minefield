#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include <dodge/globals.hpp>
#include "GameOptionsMenu.hpp"
#include "ERequestGameOptsChange.hpp"


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
      XML_NODE_CHECK(node, font);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);

      long id = attr.getLong();
      m_font = boost::dynamic_pointer_cast<Dodge::Font>(assetManager.getAssetPointer(id));

      if (!m_font)
         throw XmlException("Bad font asset id", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, difficultyDownBtn);

      attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      id = attr.getLong();

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

   m_difficultyDownBtn->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &GameOptionsMenu::decreaseDifficulty));
   m_difficultyUpBtn->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &GameOptionsMenu::increaseDifficulty));

   m_slider = unique_ptr<ProgressBar>(new ProgressBar(Colour(0.f, 0.f, 0.f, 0.7f), Vec2f(0.65f, 0.55f), Vec2f(0.03f, 0.3f)));

   m_currentMode = 0;
   m_slider->setValue(0.f);

   pTextEntity_t lblMines(new TextEntity(internString("text"), m_font, "Land mines: ", Vec2f(0.022f, 0.044f)));
   lblMines->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblMines->setTranslation(0.75f, 0.85f);
   lblMines->setZ(9.f);

   pTextEntity_t lblGold(new TextEntity(internString("text"), m_font, "Total gold: ", Vec2f(0.022f, 0.044f)));
   lblGold->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblGold->setTranslation(0.75f, 0.8f);
   lblGold->setZ(9.f);

   pTextEntity_t lblThrowables(new TextEntity(internString("text"), m_font, "Sticks: ", Vec2f(0.022f, 0.044f)));
   lblThrowables->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblThrowables->setTranslation(0.75f, 0.75f);
   lblThrowables->setZ(9.f);

   pTextEntity_t lblZombies(new TextEntity(internString("text"), m_font, "Wanderers: ", Vec2f(0.022f, 0.044f)));
   lblZombies->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblZombies->setTranslation(0.75f, 0.7f);
   lblZombies->setZ(9.f);

   pTextEntity_t lblReqScore(new TextEntity(internString("text"), m_font, "Gold required: ", Vec2f(0.022f, 0.044f)));
   lblReqScore->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblReqScore->setTranslation(0.75f, 0.65f);
   lblReqScore->setZ(9.f);

   pTextEntity_t lblTime(new TextEntity(internString("text"), m_font, "Time available: ", Vec2f(0.022f, 0.044f)));
   lblTime->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblTime->setTranslation(0.75f, 0.6f);
   lblTime->setZ(9.f);

   pTextEntity_t lblBestTime(new TextEntity(internString("text"), m_font, "Best time: ", Vec2f(0.022f, 0.044f)));
   lblBestTime->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   lblBestTime->setTranslation(0.75f, 0.5f);
   lblBestTime->setZ(9.f);

   addChild(lblMines);
   addChild(lblGold);
   addChild(lblThrowables);
   addChild(lblZombies);
   addChild(lblReqScore);
   addChild(lblTime);
   addChild(lblBestTime);

   m_txtMines = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtMines->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtMines->setTranslation(1.1f, 0.85f);
   m_txtMines->setZ(9.f);

   m_txtGold = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtGold->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtGold->setTranslation(1.1f, 0.8f);
   m_txtGold->setZ(9.f);

   m_txtThrowables = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtThrowables->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtThrowables->setTranslation(1.1f, 0.75f);
   m_txtThrowables->setZ(9.f);

   m_txtZombies = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtZombies->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtZombies->setTranslation(1.1f, 0.7f);
   m_txtZombies->setZ(9.f);

   m_txtReqScore = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtReqScore->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtReqScore->setTranslation(1.1f, 0.65f);
   m_txtReqScore->setZ(9.f);

   m_txtTime = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtTime->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtTime->setTranslation(1.1f, 0.6f);
   m_txtTime->setZ(9.f);

   m_txtBestTime = pTextEntity_t(new TextEntity(internString("text"), m_font, "", Vec2f(0.022f, 0.044f)));
   m_txtBestTime->setFillColour(Colour(0.f, 0.f, 0.f, 1.f));
   m_txtBestTime->setTranslation(1.1f, 0.5f);
   m_txtBestTime->setZ(9.f);

   addChild(m_txtMines);
   addChild(m_txtGold);
   addChild(m_txtThrowables);
   addChild(m_txtZombies);
   addChild(m_txtReqScore);
   addChild(m_txtTime);
   addChild(m_txtBestTime);
}

//===========================================
// GameOptionsMenu::updateText
//===========================================
void GameOptionsMenu::updateText() {
   stringstream str;

   str.str("");
   str << m_difficultyModes[m_currentMode]->mines;
   m_txtMines->setText(str.str());

   str.str("");
   str << m_difficultyModes[m_currentMode]->totalGold;
   m_txtGold->setText(str.str());

   str.str("");
   str << m_difficultyModes[m_currentMode]->throwables;
   m_txtThrowables->setText(str.str());

   str.str("");
   str << m_difficultyModes[m_currentMode]->zombies;
   m_txtZombies->setText(str.str());

   str.str("");
   str << m_difficultyModes[m_currentMode]->requiredGold;
   m_txtReqScore->setText(str.str());

   str.str("");
   str << m_difficultyModes[m_currentMode]->timeLimit;
   m_txtTime->setText(str.str());

   int best = m_difficultyModes[m_currentMode]->bestTime;

   if (best != -1) {
      str.str("");
      str << best;
      m_txtBestTime->setText(str.str());
   }
   else
      m_txtBestTime->setText("n/a");
}

//===========================================
// GameOptionsMenu::setDifficultyModes
//===========================================
void GameOptionsMenu::setDifficultyModes(const std::vector<pGameOptions_t>& modes) {
   m_difficultyModes = modes;
   updateText();
}

//===========================================
// GameOptionsMenu::decreaseDifficulty
//===========================================
void GameOptionsMenu::decreaseDifficulty(Dodge::pEntity_t) {
   if (m_currentMode > 0) {
      --m_currentMode;

      float32_t fCurrentMode = static_cast<float32_t>(m_currentMode);
      float32_t fNumModes = static_cast<float32_t>(m_difficultyModes.size());

      m_slider->setValue(fCurrentMode / (fNumModes - 1.f));

      EventManager eventManager;
      eventManager.queueEvent(new ERequestGameOptsChange(m_difficultyModes[m_currentMode]));

      updateText();
   }
}

//===========================================
// GameOptionsMenu::increaseDifficulty
//===========================================
void GameOptionsMenu::increaseDifficulty(Dodge::pEntity_t) {
   if (m_currentMode + 1 < m_difficultyModes.size()) {
      ++m_currentMode;

      float32_t fCurrentMode = static_cast<float32_t>(m_currentMode);
      float32_t fNumModes = static_cast<float32_t>(m_difficultyModes.size());

      m_slider->setValue(fCurrentMode / (fNumModes - 1.f));

      EventManager eventManager;
      eventManager.queueEvent(new ERequestGameOptsChange(m_difficultyModes[m_currentMode]));

      updateText();
   }
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
Asset* GameOptionsMenu::clone() const {
   return new GameOptionsMenu(*this);
}

//===========================================
// GameOptionsMenu::addToWorld
//===========================================
void GameOptionsMenu::addToWorld() {
   Menu::addToWorld();

   m_difficultyDownBtn->addToWorld();
   m_difficultyUpBtn->addToWorld();

   updateText();
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

   m_slider->draw();
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
