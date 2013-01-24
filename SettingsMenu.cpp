#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/AssetManager.hpp>
#include <dodge/globals.hpp>
#include "SettingsMenu.hpp"
#include "ERequestMusicVolumeChange.hpp"
#include "ERequestSfxVolumeChange.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// SettingsMenu::ProgressBar::ProgressBar
//===========================================
SettingsMenu::ProgressBar::ProgressBar(const Colour& col, const Vec2f& pos, const Vec2f& size) {
   m_gap = 0.005;
   m_value = 1.0;

   m_size = size;

   m_shell = unique_ptr<Quad>(new Quad(Vec2f(0.0, 0.0), Vec2f(m_size.x, 0.0), Vec2f(m_size.x, m_size.y), Vec2f(0.0, m_size.y)));

   m_shell->setLineWidth(2);
   m_shell->setLineColour(col);
   m_shell->setFillColour(Colour(0, 0, 0, 0));
   m_shell->setRenderTransform(pos.x, pos.y, 10);

   m_bar = unique_ptr<Quad>(new Quad(Vec2f(m_gap, m_gap), Vec2f(m_size.x - m_gap, m_gap), Vec2f(m_size.x - m_gap, m_size.y - m_gap), Vec2f(m_gap, m_size.y - m_gap)));

   m_bar->setLineWidth(0);
   m_bar->setFillColour(col);
   m_bar->setRenderTransform(pos.x, pos.y, 11);
}

//===========================================
// SettingsMenu::ProgressBar::setAlpha
//===========================================
void SettingsMenu::ProgressBar::setAlpha(Dodge::float32_t a) {
   // TODO
}

//===========================================
// SettingsMenu::ProgressBar::increaseValue
//===========================================
void SettingsMenu::ProgressBar::increaseValue(Dodge::float32_t val) {
   setValue(m_value + val);
}

//===========================================
// SettingsMenu::ProgressBar::setValue
//===========================================
void SettingsMenu::ProgressBar::setValue(Dodge::float32_t val) {
   if (val < 0.0) val = 0.0;
   if (val > 1.0) val = 1.0;

   const Vec2f& A = m_bar->getVertex(0);
   const Vec2f& B = m_bar->getVertex(1);

   Vec2f C = B;
   C.y += val * (m_size.y - 2.f * m_gap);
   m_bar->setVertex(2, C);

   Vec2f D = A;
   D.y += val * (m_size.y - 2.f * m_gap);
   m_bar->setVertex(3, D);

   m_value = val;
}

//===========================================
// SettingsMenu::ProgressBar::draw
//===========================================
void SettingsMenu::ProgressBar::draw() const {
   m_shell->draw();
   m_bar->draw();
}

//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const XmlNode data)
   : Asset(internString("SettingsMenu")),
     Entity(data.firstChild().firstChild().firstChild()),
     Menu(data.firstChild()) {

   try {
      AssetManager assetManager;

      XML_NODE_CHECK(data, SettingsMenu);

      XmlNode node = data.nthChild(1);
      XML_NODE_CHECK(node, musicVolDownBtn);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      long id = attr.getLong();

      m_musicVolDownBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_musicVolDownBtn)
         throw XmlException("Bad asset id for musicVolDownBtn", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, musicVolUpBtn);

      attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      id = attr.getLong();

      m_musicVolUpBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_musicVolUpBtn)
         throw XmlException("Bad asset id for musicVolUpBtn", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, sfxVolDownBtn);

      attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      id = attr.getLong();

      m_sfxVolDownBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_sfxVolDownBtn)
         throw XmlException("Bad asset id for sfxVolDownBtn", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, sfxVolUpBtn);

      attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      id = attr.getLong();

      m_sfxVolUpBtn = boost::dynamic_pointer_cast<UiButton>(assetManager.getAssetPointer(id));

      if (!m_sfxVolUpBtn)
         throw XmlException("Bad asset id for sfxVolUpBtn", __FILE__, __LINE__);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class SettingsMenu; ");
      throw;
   }

   init();
}

//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const SettingsMenu& copy)
   : Asset(internString("SettingsMenu")),
     Entity(copy),
     Menu(copy) { init(); }

//===========================================
// SettingsMenu::SettingsMenu
//===========================================
SettingsMenu::SettingsMenu(const SettingsMenu& copy, long name)
   : Asset(internString("SettingsMenu")),
     Entity(copy, name),
     Menu(copy, name) { init(); }

//===========================================
// SettingsMenu::init
//===========================================
void SettingsMenu::init() {
   m_musicVolDownBtn->setFocus(true);
   m_musicVolUpBtn->setFocus(true);

   m_musicVolDownBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_musicVolDownBtn->addActivationKey(WinIO::KEY_LEFT);

   m_musicVolUpBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_musicVolUpBtn->addActivationKey(WinIO::KEY_RIGHT);

   m_sfxVolDownBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_sfxVolDownBtn->addActivationKey(WinIO::KEY_LEFT);

   m_sfxVolUpBtn->removeActivationKey(WinIO::KEY_ENTER);
   m_sfxVolUpBtn->addActivationKey(WinIO::KEY_RIGHT);

   m_musicProgBar = unique_ptr<ProgressBar>(new ProgressBar(Colour(0, 0, 0, 0.7), Vec2f(0.85, 0.55), Vec2f(0.03, 0.3)));
   m_sfxProgBar = unique_ptr<ProgressBar>(new ProgressBar(Colour(0, 0, 0, 0.7), Vec2f(0.91, 0.55), Vec2f(0.03, 0.3)));
}

//===========================================
// SettingsMenu::onMenuItemActivate
//===========================================
void SettingsMenu::onMenuItemActivate(pMenuItem_t item) {
   static long mnuSettingsToStartMenuStr = internString("mnuSettingsToStartMenu");

   if (item->getName() == mnuSettingsToStartMenuStr)
      returnToParentMenu();
}

//===========================================
// SettingsMenu::onMenuItemGainFocus
//===========================================
void SettingsMenu::onMenuItemGainFocus(pMenuItem_t item) {
   static long mnuMusicVolumeStr = internString("mnuMusicVolume");
   static long mnuSfxVolumeStr = internString("mnuSfxVolume");

   if (item->getName() == mnuMusicVolumeStr) {
      m_musicVolDownBtn->setFocus(true);
      m_musicVolUpBtn->setFocus(true);
   }
   else if (item->getName() == mnuSfxVolumeStr) {
      m_sfxVolDownBtn->setFocus(true);
      m_sfxVolUpBtn->setFocus(true);
   }
}

//===========================================
// SettingsMenu::onMenuItemLoseFocus
//===========================================
void SettingsMenu::onMenuItemLoseFocus(pMenuItem_t item) {
   static long mnuMusicVolumeStr = internString("mnuMusicVolume");
   static long mnuSfxVolumeStr = internString("mnuSfxVolume");

   if (item->getName() == mnuMusicVolumeStr) {
      m_musicVolDownBtn->setFocus(false);
      m_musicVolUpBtn->setFocus(false);
   }
   else if (item->getName() == mnuSfxVolumeStr) {
      m_sfxVolDownBtn->setFocus(false);
      m_sfxVolUpBtn->setFocus(false);
   }
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

   m_musicVolDownBtn->addToWorld();
   m_musicVolUpBtn->addToWorld();
   m_sfxVolDownBtn->addToWorld();
   m_sfxVolUpBtn->addToWorld();
}

//===========================================
// SettingsMenu::removeFromWorld
//===========================================
void SettingsMenu::removeFromWorld() {
   Menu::removeFromWorld();

   m_musicVolDownBtn->removeFromWorld();
   m_musicVolUpBtn->removeFromWorld();
   m_sfxVolDownBtn->removeFromWorld();
   m_sfxVolUpBtn->removeFromWorld();
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
// SettingsMenu::increaseMusicVolume
//===========================================
void SettingsMenu::increaseMusicVolume() {
   static float32_t duration = 2.f;
   static float32_t fr = gGetTargetFrameRate();

   EventManager eventManager;
   eventManager.immediateDispatch(new ERequestMusicVolumeChange(1.0 / (duration * fr)));

   m_musicProgBar->increaseValue(1.0 / (duration * fr));
}

//===========================================
// SettingsMenu::decreaseMusicVolume
//===========================================
void SettingsMenu::decreaseMusicVolume() {
   static float32_t duration = 2.f;
   static float32_t fr = gGetTargetFrameRate();

   EventManager eventManager;
   eventManager.immediateDispatch(new ERequestMusicVolumeChange(-1.0 / (duration * fr)));

   m_musicProgBar->increaseValue(-1.0 / (duration * fr));
}

//===========================================
// SettingsMenu::increaseSfxVolume
//===========================================
void SettingsMenu::increaseSfxVolume() {
   static float32_t duration = 2.f;
   static float32_t fr = gGetTargetFrameRate();

   EventManager eventManager;
   eventManager.immediateDispatch(new ERequestSfxVolumeChange(1.0 / (duration * fr)));

   m_sfxProgBar->increaseValue(1.0 / (duration * fr));
}

//===========================================
// SettingsMenu::decreaseSfxVolume
//===========================================
void SettingsMenu::decreaseSfxVolume() {
   static float32_t duration = 2.f;
   static float32_t fr = gGetTargetFrameRate();

   EventManager eventManager;
   eventManager.immediateDispatch(new ERequestSfxVolumeChange(-1.0 / (duration * fr)));

   m_sfxProgBar->increaseValue(-1.0 / (duration * fr));
}

//===========================================
// SettingsMenu::update
//===========================================
void SettingsMenu::update() {
   Menu::update();

   m_musicVolDownBtn->update();
   m_musicVolUpBtn->update();
   m_sfxVolDownBtn->update();
   m_sfxVolUpBtn->update();

   if (m_musicVolDownBtn->isPressed()) decreaseMusicVolume();
   if (m_musicVolUpBtn->isPressed()) increaseMusicVolume();
   if (m_sfxVolDownBtn->isPressed()) decreaseSfxVolume();
   if (m_sfxVolUpBtn->isPressed()) increaseSfxVolume();
}

//===========================================
// SettingsMenu::draw
//===========================================
void SettingsMenu::draw() const {
   Menu::draw();

   m_musicVolDownBtn->draw();
   m_musicVolUpBtn->draw();
   m_sfxVolDownBtn->draw();
   m_sfxVolUpBtn->draw();

   m_musicProgBar->draw();
   m_sfxProgBar->draw();
}

//===========================================
// SettingsMenu::assignData
//===========================================
void SettingsMenu::assignData(const XmlNode data) {
   assert(false);
}

//===========================================
// SettingsMenu::~SettingsMenu
//===========================================
SettingsMenu::~SettingsMenu() {}
