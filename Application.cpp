/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <sstream>
#include <iostream>
#include <cstdlib>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "NumericTile.hpp"
#include "Mine.hpp"
#include "Player.hpp"
#include "Soil.hpp"
#include "Exit.hpp"
#include "StartMenu.hpp"
#include "SettingsMenu.hpp"
#include "MenuItem.hpp"
#include "CSprite.hpp"
#include "CTextEntity.hpp"
#include "Counter.hpp"
#include "Collectable.hpp"
#include "CreditsMenu.hpp"
#include "EUpdateScore.hpp"


#define TARGET_MEM_USAGE 9999999


using namespace std;
using namespace Dodge;


//===========================================
// Application::Application
//===========================================
Application::Application()
   : m_init(false),
     m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
     m_renderer(Dodge::Renderer::getInstance()),
     m_frameRate(60.0) {

   for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
      m_mouseState[i] = false;
}

//===========================================
// Application::onExit
//===========================================
void Application::onExit(Functor<void, TYPELIST_0()> callBack) {
   m_onExit = callBack;
}

//===========================================
// Application::exitDefault
//===========================================
void Application::exitDefault() {
   exit(0);
}

//===========================================
// Application::quit
//===========================================
void Application::quit() { // TODO Some monostate/singletons may not have been initialised!
   m_eventManager.clear();
   freeAllAssets();
   m_renderer.stop();
   m_win.destroyWindow();

   m_onExit();
}

//===========================================
// Application::freeAllAssets
//===========================================
void Application::freeAllAssets() {
   m_assetManager.freeAllAssets();
   m_mineField.clear();
   m_items.clear();
   m_worldSpace.removeAndUntrackAll();
   m_mapLoader.freeAllAssets();
   m_player.reset();
   m_exit.reset();
   m_startMenu.reset();
   m_timeCounter.reset();
   m_scoreCounter.reset();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {

   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
#ifdef DEBUG
      case WinIO::KEY_F:
         cout << "Frame rate (main thread): " << m_frameRate << "fps\n";
         cout << "Frame rate (renderer): " << m_renderer.getFrameRate() << "fps\n";
      break;
      case WinIO::KEY_M:
         cout << "Memory usage: " << static_cast<float32_t>(m_mapLoader.dbg_getMemoryUsage()) / 1000.0 << "KB\n";
      break;
#endif
   }

   m_keyState[key] = true;
}

//===========================================
// Application::keyUp
//===========================================
void Application::keyUp(int key) {
   m_keyState[key] = false;
}

//===========================================
// Application::mouseLeftClick
//===========================================
void Application::mouseLeftClick(int x, int y) {
   m_mouseState[MOUSE_LEFT_BUTTON] = true;
}

//===========================================
// Application::mouseRightClick
//===========================================
void Application::mouseRightClick(int x, int y) {
   m_mouseState[MOUSE_RIGHT_BUTTON] = true;
}

//===========================================
// Application::mouseLeftRelease
//===========================================
void Application::mouseLeftRelease(int x, int y) {
   m_mouseState[MOUSE_LEFT_BUTTON] = false;
}

//===========================================
// Application::mouseRightRelease
//===========================================
void Application::mouseRightRelease(int x, int y) {
   m_mouseState[MOUSE_RIGHT_BUTTON] = false;
}

//===========================================
// Application::mouseMove
//===========================================
void Application::mouseMove(int x, int y) {

}

//===========================================
// Application::keyboard
//===========================================
void Application::keyboard() {
   switch (m_gameState) {
      case ST_RUNNING:
         switch (m_player->getState()) {
            case Player::ALIVE: {
               Vec2f mapPos = m_minefieldBoundary.getPosition();
               Vec2f mapSz = m_minefieldBoundary.getSize();

               Vec2f pos = m_player->getTranslation_abs();

               float32_t w = m_tileSize.x;
               float32_t h = m_tileSize.y;

               float32_t dx = w / 2.f;
               float32_t dy = h / 2.f;

               if (m_keyState[WinIO::KEY_LEFT])    if (pos.x > mapPos.x + dx)                m_player->moveLeft();
               if (m_keyState[WinIO::KEY_RIGHT])   if (pos.x + w < mapPos.x + mapSz.x - dx)  m_player->moveRight();
               if (m_keyState[WinIO::KEY_DOWN])    if (pos.y > mapPos.y + dy)                m_player->moveDown();
               if (m_keyState[WinIO::KEY_UP])      if (pos.y + h < mapPos.y + mapSz.y - dy)  m_player->moveUp();
            }
            break;
            case Player::DEAD: {
               if (m_keyState[WinIO::KEY_ENTER]) resetGame();
            }
            break;
         }
      break;
      case ST_START_MENU:
      break;
   }
}

//===========================================
// Application::resetGame
//===========================================
void Application::resetGame() {
   m_eventManager.clear();
   freeAllAssets();
   loadAssets();

   m_gameState = ST_START_MENU;
}

//===========================================
// Application::computeFrameRate
//===========================================
void Application::computeFrameRate() {
   static Timer timer;

   m_frameRate = 1.0 / timer.getTime();
   timer.reset();
}

//===========================================
// Application::setMapSettings
//===========================================
void Application::setMapSettings(const XmlNode data) {
   if (m_init) return;

   try {
      XML_NODE_CHECK(data, customSettings);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, windowSize);
      Vec2i winSz(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, bgColour);
      m_bgColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, minefieldBoundary);
      m_minefieldBoundary = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, tileSize);
      m_tileSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, startMenuId);
      m_startMenuId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, playerProtoId);
      m_playerProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, exitProtoId);
      m_exitProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, numericTileProtoId);
      m_numericTileProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, mineProtoId);
      m_mineProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, soilProtoId);
      m_soilProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, collectableProtoId);
      m_collectableProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, throwableProtoId);
      m_throwableProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, timeCounterId);
      m_timeCounterId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, scoreCounterId);
      m_scoreCounterId = node.getLong();

      m_win.init("Minefield", winSz.x, winSz.y, false);
      m_renderer.start();

      pCamera_t camera(new Camera(static_cast<float32_t>(winSz.x) / static_cast<float32_t>(winSz.y), 1.f));
      m_renderer.attachCamera(camera);

      const Range& mb = m_mapLoader.getMapBoundary();
      Range boundary(mb.getPosition() - Vec2f(0.1, 0.1), mb.getSize() + Vec2f(0.2, 0.2));
      m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, boundary)));

      m_init = true;
   }
   catch (XmlException& e) {
      m_init = false;

      e.prepend("Error loading map settings; ");
      throw;
   }
}

//===========================================
// Application::deletePending
//===========================================
void Application::deletePending(EEvent* event) {
   static long pendingDeletionStr = internString("pendingDeletion");

   if (event->getType() == pendingDeletionStr) {
      EPendingDeletion* e = static_cast<EPendingDeletion*>(event);

      deleteAsset(e->item);
   }
}

//===========================================
// Application::deleteAsset
//===========================================
void Application::deleteAsset(pAsset_t asset) {
   static long mineStr = internString("mine");
   static long numericTileStr = internString("numericTile");

   pEntity_t entity = dynamic_pointer_cast<Entity>(asset);

   if (entity) {
      m_worldSpace.removeAndUntrackEntity(entity);
      entity->removeFromWorld();
      m_items.erase(entity->getName());

      const Range& mb = m_minefieldBoundary;
      const Vec2f& sz = mb.getSize();

      int w = floor(sz.x / m_tileSize.x + 0.5);
      int h = floor(sz.y / m_tileSize.y + 0.5);

      Vec2f pos = entity->getTranslation_abs();
      int i = floor((pos.x - mb.getPosition().x) / m_tileSize.x + 0.5);
      int j = floor((pos.y - mb.getPosition().y) / m_tileSize.y + 0.5);

      if (entity->getTypeName() == numericTileStr) {
         if (m_mineField[i][j] == entity) m_mineField[i][j].reset();
      }

      if (entity->getTypeName() == mineStr) {
         if (m_mineField[i][j] == entity) m_mineField[i][j].reset();

         int nMines = 0;
         for (int n = 0; n < 8; ++n) {
            int k, l;

            switch (n) {
               case 0: k = i + 1; l = j - 1; break;
               case 1: k = i + 1; l = j + 0; break;
               case 2: k = i + 1; l = j + 1; break;
               case 3: k = i + 0; l = j - 1; break;
               case 4: k = i + 0; l = j + 1; break;
               case 5: k = i - 1; l = j - 1; break;
               case 6: k = i - 1; l = j + 0; break;
               case 7: k = i - 1; l = j + 1; break;
            }

            if (k < 0 || k >= w || l < 0 || l >= h) continue;

            pItem_t item = m_mineField[k][l];

            if (item) {
               if (item->getTypeName() == numericTileStr) {

                  pNumericTile_t tile = dynamic_pointer_cast<NumericTile>(item);
                  assert(tile);

                  int val = tile->getValue();

                  if (val > 1)
                     tile->setValue(val - 1);
                  else
                     tile->setPendingDeletion();
               }
               else if (item->getTypeName() == mineStr) {
                  ++nMines;
               }
            }
         }

         if (nMines > 0) {
            pNumericTile_t tile(dynamic_cast<NumericTile*>(m_assetManager.cloneAsset(m_numericTileProtoId)));

            if (!tile)
               throw Exception("Error replacing mine with numeric tile; Bad asset id for numeric tile prototype", __FILE__, __LINE__);

            tile->setValue(nMines);
            tile->setTranslation(entity->getTranslation_abs());

            tile->addToWorld();
            m_worldSpace.trackEntity(tile);
            m_items[tile->getName()] = tile;
            m_mineField[i][j] = tile;
         }
      }
   }

   m_assetManager.freeAsset(asset->getAssetId());
}

//===========================================
// Application::constructAsset
//===========================================
pAsset_t Application::constructAsset(const XmlNode data) {
   long proto = -1;
   bool addToWorld = false;

   XmlAttribute attr = data.firstAttribute();
   attr = attr.nextAttribute();

   if (!attr.isNull() && attr.name() == "proto") {
      proto = attr.getLong();
      attr = attr.nextAttribute();
   }

   if (!attr.isNull() && attr.name() == "addToWorld") {
      addToWorld = attr.getBool();
   }

   XmlNode node = data.firstChild();


   // Construct non-Item assets

   if (node.name() == "Texture") return pAsset_t(new Texture(node));
   if (node.name() == "Font") return pAsset_t(new Dodge::Font(node));
   // ...


   // Construct Items

   pItem_t item;
   if (proto != -1) {
      item = pItem_t(dynamic_cast<Item*>(m_assetManager.cloneAsset(proto)));
      item->assignData(node);
   }
   else {
      if (node.name() == "Item") item = pItem_t(new Item(node));
      if (node.name() == "Mine") item = pItem_t(new Mine(node));
      if (node.name() == "NumericTile") item = pItem_t(new NumericTile(node));
      if (node.name() == "Player") item = pItem_t(new Player(node));
      if (node.name() == "Soil") item = pItem_t(new Soil(node));
      if (node.name() == "Exit") item = pItem_t(new Exit(node));
      if (node.name() == "StartMenu") item = pItem_t(new StartMenu(node));
      if (node.name() == "SettingsMenu") item = pItem_t(new SettingsMenu(node));
      if (node.name() == "CreditsMenu") item = pItem_t(new CreditsMenu(node));
      if (node.name() == "MenuItem") item = pItem_t(new MenuItem(node));
      if (node.name() == "CSprite") item = pItem_t(new CSprite(node));
      if (node.name() == "CTextEntity") item = pItem_t(new CTextEntity(node));
      if (node.name() == "Counter") item = pItem_t(new Counter(node));
      if (node.name() == "Collectable") item = pItem_t(new Collectable(node));
      // ...
   }

   if (!item)
      throw Exception("Unrecognised item type", __FILE__, __LINE__);

   if (addToWorld) {
      item->addToWorld();
      m_worldSpace.insertAndTrackEntity(item);
      m_items[item->getName()] = item;
   }

   return item;
}

//===========================================
// Application::update
//===========================================
void Application::update() {
   switch (m_gameState) {
      case ST_RUNNING:
         for (auto i = m_items.begin(); i != m_items.end(); ++i)
            i->second->update();

         m_player->update();
         m_exit->update();
      break;
      case ST_START_MENU:
         m_startMenu->update();
      break;
   }
}

//===========================================
// Application::onWindowResize
//===========================================
void Application::onWindowResize(int w, int h) {
   m_renderer.onWindowResize(w, h);
   m_renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

//===========================================
// Application::playerDeath
//===========================================
void Application::playerDeath(EEvent* event) {
   // TODO
}

//===========================================
// Application::gameSuccess
//===========================================
void Application::gameSuccess(const EEvent* event) {
   // TODO
}

//===========================================
// Application::draw
//===========================================
void Application::draw() const {
   switch (m_gameState) {
      case ST_RUNNING:
         for (auto i = m_items.begin(); i != m_items.end(); ++i)
            i->second->draw();

         m_player->draw();
         m_exit->draw();
      break;
      case ST_START_MENU:
         m_startMenu->draw();
      break;
   }

#ifdef DEBUG
   if (dbg_worldSpaceVisible)
      m_worldSpace.dbg_draw(Colour(1.f, 1.f, 1.f, 1.f), 2, 9);
#endif
}

//===========================================
// Application::isAdjacentTo
//===========================================
bool Application::isAdjacentTo(const Vec2i& a, const Vec2i& b) const {
   return a.x <= b.x + 1 && a.x >= b.x - 1
      && a.y <= b.y + 1 && a.y >= b.y - 1;
}

//===========================================
// Application::populateMap
//===========================================
void Application::populateMap() {
   static long numericTileStr = internString("numericTile");
   static long mineStr = internString("mine");

   const Range& mb = m_minefieldBoundary;
   const Vec2f& pos = mb.getPosition();
   const Vec2f& sz = mb.getSize();

   m_player = boost::dynamic_pointer_cast<Player>(m_assetManager.getAssetPointer(m_playerProtoId));

   if (!m_player)
      throw Exception("Error populating map; Bad player proto id", __FILE__, __LINE__);

   m_player->setTranslation(pos);
   m_player->addToWorld();
   m_worldSpace.trackEntity(m_player);

   m_exit = boost::dynamic_pointer_cast<Exit>(m_assetManager.getAssetPointer(m_exitProtoId));

   if (!m_exit)
      throw Exception("Error populating map; Bad exit proto id", __FILE__, __LINE__);

   m_exit->setTranslation(pos + sz - m_tileSize);
   m_exit->addToWorld();
   m_worldSpace.trackEntity(m_exit);

   int w = floor(sz.x / m_tileSize.x + 0.5);
   int h = floor(sz.y / m_tileSize.y + 0.5);

   m_mineField.clear();
   for (int i = 0; i < w; ++i) {
      m_mineField.push_back(std::vector<pItem_t>());
      m_mineField[i].resize(h);
   }

   Vec2f plyrPos = m_player->getTranslation_abs();
   int plyrI = floor((plyrPos.x - mb.getPosition().x) / m_tileSize.x + 0.5);
   int plyrJ = floor((plyrPos.y - mb.getPosition().y) / m_tileSize.y + 0.5);

   Vec2f exitPos = m_exit->getTranslation_abs();
   int exitI = floor((exitPos.x - mb.getPosition().x) / m_tileSize.x + 0.5);
   int exitJ = floor((exitPos.y - mb.getPosition().y) / m_tileSize.y + 0.5);

   for (int m = 0; m < m_numMines; ++m) {
      int i = rand() % w;
      int j = rand() % h;

      if (isAdjacentTo(Vec2i(i, j), Vec2i(plyrI, plyrJ))
         || isAdjacentTo(Vec2i(i, j), Vec2i(exitI, exitJ))) {

         --m;
         continue;
      }

      if (m_mineField[i][j]) {
         if (m_mineField[i][j]->getTypeName() == mineStr) {
            --m;
            continue;
         }
         else if (m_mineField[i][j]->getTypeName() == numericTileStr) {
            m_mineField[i][j]->setPendingDeletion();
         }
      }

      pMine_t mine(dynamic_cast<Mine*>(m_assetManager.cloneAsset(m_mineProtoId)));

      float32_t x = pos.x + m_tileSize.x * static_cast<float32_t>(i);
      float32_t y = pos.y + m_tileSize.y * static_cast<float32_t>(j);

      mine->setTranslation(x, y);

      mine->addToWorld();
      m_worldSpace.trackEntity(mine);
      m_items[mine->getName()] = mine;
      m_mineField[i][j] = mine;

      for (int n = 0; n < 8; ++n) {
         int k, l;

         switch (n) {
            case 0: k = i + 1; l = j - 1; break;
            case 1: k = i + 1; l = j + 0; break;
            case 2: k = i + 1; l = j + 1; break;
            case 3: k = i + 0; l = j - 1; break;
            case 4: k = i + 0; l = j + 1; break;
            case 5: k = i - 1; l = j - 1; break;
            case 6: k = i - 1; l = j + 0; break;
            case 7: k = i - 1; l = j + 1; break;
         }

         if (k < 0 || k >= w || l < 0 || l >= h) continue;

         pItem_t item = m_mineField[k][l];

         if (item) {
            if (item->getTypeName() == numericTileStr) {
               pNumericTile_t tile = dynamic_pointer_cast<NumericTile>(item);
               assert(tile);
               tile->setValue(tile->getValue() + 1);
            }
         }
         else {
            pNumericTile_t tile(dynamic_cast<NumericTile*>(m_assetManager.cloneAsset(m_numericTileProtoId)));

            tile->setTranslation(pos.x + static_cast<float32_t>(k) * m_tileSize.x, pos.y + static_cast<float32_t>(l) * m_tileSize.y);

            tile->addToWorld();
            m_worldSpace.trackEntity(tile);
            m_items[tile->getName()] = tile;
            m_mineField[k][l] = tile;
         }
      }
   }

   vector<vector<bool> > coinsAndThrowables;
   for (int i = 0; i < w; ++i) coinsAndThrowables.push_back(vector<bool>(h, false));

   for (int c = 0; c < m_numCollectables; ++c) {
      int i = rand() % w;
      int j = rand() % h;

      if (i == exitI && j == exitJ) {
         --c;
         continue;
      }

      if (i == plyrI && j == plyrJ) {
         --c;
         continue;
      }

      if (coinsAndThrowables[i][j]) {
         --c;
         continue;
      }

      pCollectable_t item(dynamic_cast<Collectable*>(m_assetManager.cloneAsset(m_collectableProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad collectable proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_tileSize.x, pos.y + static_cast<float32_t>(j) * m_tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_items[item->getName()] = item;

      coinsAndThrowables[i][j] = true;
   }

   for (int c = 0; c < m_numThrowables; ++c) {
      int i = rand() % w;
      int j = rand() % h;

      if (i == exitI && j == exitJ) {
         --c;
         continue;
      }

      if (i == plyrI && j == plyrJ) {
         --c;
         continue;
      }

      if (coinsAndThrowables[i][j]) {
         --c;
         continue;
      }

      pCSprite_t item(dynamic_cast<CSprite*>(m_assetManager.cloneAsset(m_throwableProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad throwable proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_tileSize.x, pos.y + static_cast<float32_t>(j) * m_tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_items[item->getName()] = item;

      coinsAndThrowables[i][j] = true;
   }

   for (int i = 0; i < w; ++i) {
      for (int j = 0; j < h; ++j) {
         if (isAdjacentTo(Vec2i(i, j), Vec2i(plyrI, plyrJ))) continue;
         if (isAdjacentTo(Vec2i(i, j), Vec2i(exitI, exitJ))) continue;

         float32_t x = pos.x + m_tileSize.x * static_cast<float32_t>(i);
         float32_t y = pos.y + m_tileSize.y * static_cast<float32_t>(j);

         pSoil_t soil(dynamic_cast<Soil*>(m_assetManager.cloneAsset(m_soilProtoId)));

         soil->setTranslation(x, y);

         soil->addToWorld();
         m_worldSpace.trackEntity(soil);
         m_items[soil->getName()] = soil;
      }
   }
}

//===========================================
// Application::startGame
//===========================================
void Application::startGame(EEvent* event) {
   m_startMenu->removeFromWorld();

   // TODO:
   m_numMines = 10;
   m_numCollectables = 8;
   m_numThrowables = 3;
   m_requiredScore = 5;
   m_timeLimit = 100;

   populateMap();

   m_scoreCounter->setValue(m_requiredScore);
   m_timeCounter->setValue(m_timeLimit);
   m_gameState = ST_RUNNING;
}

//===========================================
// Application::loadAssets
//===========================================
void Application::loadAssets() {
   m_mapLoader.parseMapFile("data/xml/map0.xml");

   m_mapLoader.update(m_renderer.getCamera().getTranslation());

   m_startMenu = boost::dynamic_pointer_cast<StartMenu>(m_assetManager.getAssetPointer(m_startMenuId));
   if (!m_startMenu)
      throw Exception("Error loading map; Bad start menu id", __FILE__, __LINE__);

   m_startMenu->addToWorld();

   m_scoreCounter = boost::dynamic_pointer_cast<Counter>(m_assetManager.getAssetPointer(m_scoreCounterId));
   if (!m_scoreCounter)
      throw Exception("Error loading map; Bad score counter id", __FILE__, __LINE__);

   m_timeCounter = boost::dynamic_pointer_cast<Counter>(m_assetManager.getAssetPointer(m_timeCounterId));
   if (!m_timeCounter)
      throw Exception("Error loading map; Bad time counter id", __FILE__, __LINE__);
}

//===========================================
// Application::quitGame
//===========================================
void Application::quitGame(EEvent* event) {
   quit();
}

//===========================================
// Application::updateScore
//===========================================
void Application::updateScore(EEvent* event) {
   EUpdateScore* e = static_cast<EUpdateScore*>(event);

   if (m_scoreCounter->getValue() > 0)
      m_scoreCounter->subtract(e->value);

   if (m_scoreCounter->getValue() == 0)
      m_exit->open();
}

//===========================================
// Application::updateTimer
//===========================================
void Application::updateTimer() {
   static Timer timer;

   if (!m_player) return;
   if (m_player->getState() == Player::DEAD) return;

   if (timer.getTime() > 1.0) {
      timer.reset();

      m_timeCounter->decrement();

      if (m_timeCounter->getValue() == 0) {
         m_player->die();
      }
   }
}

//===========================================
// Application::launch
//===========================================
void Application::launch(int argc, char** argv) {
#ifdef DEBUG
   dbg_worldSpaceVisible = false;

   if (argc > 0) {
      for (int i = 0; i < argc; ++i) {
         if (strcmp(argv[i], "-novsync") == 0)
            WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
      }
   }
#endif
   gInitialise();

   m_mapLoader.initialise(Functor<void, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::setMapSettings),
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::constructAsset),
      Functor<void, TYPELIST_1(Dodge::pAsset_t)>(this, &Application::deleteAsset),
      TARGET_MEM_USAGE);

   loadAssets();

   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_MOUSEMOVE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseMove));
   m_win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseLeftClick));
   m_win.registerCallback(WinIO::EVENT_BTN3PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseRightClick));
   m_win.registerCallback(WinIO::EVENT_BTN1RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseLeftRelease));
   m_win.registerCallback(WinIO::EVENT_BTN3RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseRightRelease));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   m_eventManager.registerCallback(internString("success"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::gameSuccess));

   m_eventManager.registerCallback(internString("startGame"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::startGame));

   m_eventManager.registerCallback(internString("quitGame"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::quitGame));

   m_eventManager.registerCallback(internString("updateScore"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::updateScore));

   m_eventManager.registerCallback(internString("playerDeath"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::playerDeath));

   m_gameState = ST_START_MENU;

   while (1) {
      LOOP_START

      switch (m_gameState) {
         case ST_START_MENU:
            computeFrameRate();
            m_win.doEvents();
            m_eventManager.doEvents();
            keyboard();
            update();
            draw();
            m_renderer.tick(m_bgColour);
            m_win.swapBuffers();
         break;
         case ST_RUNNING:
            computeFrameRate();
            m_win.doEvents();
            m_eventManager.doEvents();
            keyboard();
            update();
            updateTimer();
            draw();
            m_renderer.tick(m_bgColour);
            m_win.swapBuffers();
         break;
      }

      LOOP_END
   }
}
