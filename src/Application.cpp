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
#include "PauseMenu.hpp"
#include "SettingsMenu.hpp"
#include "MenuItem.hpp"
#include "CSprite.hpp"
#include "CTextEntity.hpp"
#include "Throwable.hpp"
#include "Zombie.hpp"
#include "Clouds.hpp"
#include "Counter.hpp"
#include "Collectable.hpp"
#include "CreditsMenu.hpp"
#include "GameOptionsMenu.hpp"


#define TARGET_MEM_USAGE 9999999


using namespace std;
using namespace Dodge;


//===========================================
// Application::Application
//===========================================
Application::Application()
   : m_init(false),
     m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
     m_mapLoader(MapLoader::getInstance()),
     m_renderer(Renderer::getInstance()),
     m_logic(m_data) {}

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
   exit(EXIT_SUCCESS);
}

//===========================================
// Application::quit
//===========================================
void Application::quit() throw() {
   try {
      m_eventManager.clear();
      freeAllAssets();
      m_renderer.stop();
      m_audio.quit();
      m_win.destroyWindow();
   }
   catch (...) {}

   m_onExit();
}

//===========================================
// Application::quitReqHandler
//===========================================
void Application::quitReqHandler(EEvent* event) {
   quit();
}

//===========================================
// Application::freeAllAssets
//===========================================
void Application::freeAllAssets() {
   m_assetManager.freeAllAssets();
   m_data.mineField.clear();
   m_data.persistentItems.clear();
   m_data.expendableItems.clear();
   m_worldSpace.removeAndUntrackAll();
   m_mapLoader.freeAllAssets();
   m_data.player.reset();
   m_data.exit.reset();
   m_data.startMenu.reset();
   m_data.timeCounter.reset();
   m_data.scoreCounter.reset();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {
#ifdef DEBUG
   switch (key) {
      case WinIO::KEY_M:
         cout << "Memory usage: " << static_cast<float32_t>(m_mapLoader.dbg_getMemoryUsage()) / 1000.0 << "KB\n";
      break;
   }
#endif
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
      XML_NODE_CHECK(node, gameSettingsId);
      m_gameSettingsId = node.getLong();

      m_win.init("Minefield", winSz.x, winSz.y, false);
      m_renderer.start();

      pCamera_t camera(new Camera(static_cast<float32_t>(winSz.x) / static_cast<float32_t>(winSz.y), 1.f));
      m_renderer.attachCamera(camera);

      const Range& mb = m_mapLoader.getMapBoundary();
      Range boundary(mb.getPosition() - Vec2f(0.1f, 0.1f), mb.getSize() + Vec2f(0.2f, 0.2f));
      m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, boundary)));
   }
   catch (XmlException& e) {
      e.prepend("Error loading map settings; ");
      throw;
   }

   m_init = true;
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

      m_data.expendableItems.erase(entity->getName());

      const Range& mb = m_data.settings->minefieldBoundary;
      const Vec2f& sz = mb.getSize();

      int w = static_cast<int>(floor(sz.x / m_data.settings->tileSize.x + 0.5));
      int h = static_cast<int>(floor(sz.y / m_data.settings->tileSize.y + 0.5));

      Vec2f pos = entity->getTranslation_abs();
      int i = static_cast<int>(floor((pos.x - mb.getPosition().x) / m_data.settings->tileSize.x + 0.5));
      int j = static_cast<int>(floor((pos.y - mb.getPosition().y) / m_data.settings->tileSize.y + 0.5));

      if (entity->getTypeName() == numericTileStr) {
         if (m_data.mineField[i][j] == entity) m_data.mineField[i][j].reset();
      }

      if (entity->getTypeName() == mineStr) {
         if (m_data.mineField[i][j] == entity) m_data.mineField[i][j].reset();

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

            pItem_t item = m_data.mineField[k][l];

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
            pNumericTile_t tile(dynamic_cast<NumericTile*>(m_assetManager.cloneAsset(m_data.settings->numericTileProtoId)));

            if (!tile)
               throw Exception("Error replacing mine with numeric tile; Bad asset id for numeric tile prototype", __FILE__, __LINE__);

            tile->setValue(nMines);
            tile->setTranslation(entity->getTranslation_abs());

            tile->addToWorld();
            m_worldSpace.trackEntity(tile);
            m_data.expendableItems[tile->getName()] = tile;
            m_data.mineField[i][j] = tile;
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

   if (node.name() == "GameSettings") return pAsset_t(new GameSettings(node));
   if (node.name() == "Texture") return pAsset_t(new Texture(node));
   if (node.name() == "Font") return pAsset_t(new Dodge::Font(node));
   if (node.name() == "TextEntity") return pAsset_t(new TextEntity(node));
   if (node.name() == "UiButton") return pAsset_t(new UiButton(node));
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
      if (node.name() == "PauseMenu") item = pItem_t(new PauseMenu(node));
      if (node.name() == "SettingsMenu") item = pItem_t(new SettingsMenu(node));
      if (node.name() == "CreditsMenu") item = pItem_t(new CreditsMenu(node));
      if (node.name() == "GameOptionsMenu") item = pItem_t(new GameOptionsMenu(node));
      if (node.name() == "MenuItem") item = pItem_t(new MenuItem(node));
      if (node.name() == "CSprite") item = pItem_t(new CSprite(node));
      if (node.name() == "CTextEntity") item = pItem_t(new CTextEntity(node));
      if (node.name() == "Throwable") item = pItem_t(new Throwable(node));
      if (node.name() == "Zombie") item = pItem_t(new Zombie(node));
      if (node.name() == "Counter") item = pItem_t(new Counter(node));
      if (node.name() == "Collectable") item = pItem_t(new Collectable(node));
      if (node.name() == "Clouds") item = pItem_t(new Clouds(node));
      // ...
   }

   if (!item)
      throw Exception("Unrecognised item type", __FILE__, __LINE__);

   if (addToWorld) {
      item->addToWorld();
      m_worldSpace.insertAndTrackEntity(item);
      m_data.persistentItems[item->getName()] = item;
   }

   return item;
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
void Application::populateMap(EEvent*) {
   static long numericTileStr = internString("numericTile");
   static long mineStr = internString("mine");

   const Range& mb = m_data.settings->minefieldBoundary;
   const Vec2f& pos = mb.getPosition();
   const Vec2f& sz = mb.getSize();

   int w = static_cast<int>(floor(sz.x / m_data.settings->tileSize.x));
   int h = static_cast<int>(floor(sz.y / m_data.settings->tileSize.y));

   m_data.player->setTranslation(pos);
   m_data.exit->setTranslation(pos.x + (w - 1) * m_data.settings->tileSize.x, pos.y + (h - 1) * m_data.settings->tileSize.y);

   m_data.mineField.clear();
   for (int i = 0; i < w; ++i) {
      m_data.mineField.push_back(std::vector<pItem_t>());
      m_data.mineField[i].resize(h);
   }

   Vec2f plyrPos = m_data.player->getTranslation_abs();
   int plyrI = static_cast<int>(floor((plyrPos.x - mb.getPosition().x) / m_data.settings->tileSize.x + 0.5));
   int plyrJ = static_cast<int>(floor((plyrPos.y - mb.getPosition().y) / m_data.settings->tileSize.y + 0.5));

   Vec2f exitPos = m_data.exit->getTranslation_abs();
   int exitI = static_cast<int>(floor((exitPos.x - mb.getPosition().x) / m_data.settings->tileSize.x + 0.5));
   int exitJ = static_cast<int>(floor((exitPos.y - mb.getPosition().y) / m_data.settings->tileSize.y + 0.5));

   for (int m = 0; m < m_data.gameOpts.mines; ++m) {
      int i = rand() % w;
      int j = rand() % h;

      if (isAdjacentTo(Vec2i(i, j), Vec2i(plyrI, plyrJ))
         || isAdjacentTo(Vec2i(i, j), Vec2i(exitI, exitJ))) {

         --m;
         continue;
      }

      if (m_data.mineField[i][j]) {
         if (m_data.mineField[i][j]->getTypeName() == mineStr) {
            --m;
            continue;
         }
         else if (m_data.mineField[i][j]->getTypeName() == numericTileStr) {
            m_data.mineField[i][j]->setPendingDeletion();
         }
      }

      pMine_t mine(dynamic_cast<Mine*>(m_assetManager.cloneAsset(m_data.settings->mineProtoId)));

      if (!mine)
         throw Exception("Error populating map; Bad mine proto id", __FILE__, __LINE__);

      float32_t x = pos.x + m_data.settings->tileSize.x * static_cast<float32_t>(i);
      float32_t y = pos.y + m_data.settings->tileSize.y * static_cast<float32_t>(j);

      mine->setTranslation(x, y);

      mine->addToWorld();
      m_worldSpace.trackEntity(mine);
      m_data.expendableItems[mine->getName()] = mine;
      m_data.mineField[i][j] = mine;

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

         pItem_t item = m_data.mineField[k][l];

         if (item) {
            if (item->getTypeName() == numericTileStr) {
               pNumericTile_t tile = dynamic_pointer_cast<NumericTile>(item);
               assert(tile);
               tile->setValue(tile->getValue() + 1);
            }
         }
         else {
            pNumericTile_t tile(dynamic_cast<NumericTile*>(m_assetManager.cloneAsset(m_data.settings->numericTileProtoId)));

            if (!tile)
               throw Exception("Error populating map; Bad numeric tile proto id", __FILE__, __LINE__);

            tile->setTranslation(pos.x + static_cast<float32_t>(k) * m_data.settings->tileSize.x, pos.y + static_cast<float32_t>(l) * m_data.settings->tileSize.y);

            tile->addToWorld();
            m_worldSpace.trackEntity(tile);
            m_data.expendableItems[tile->getName()] = tile;
            m_data.mineField[k][l] = tile;
         }
      }
   }

   vector<vector<bool> > coinsAndThrowables;
   for (int i = 0; i < w; ++i) coinsAndThrowables.push_back(vector<bool>(h, false));

   for (int c = 0; c < m_data.gameOpts.coins; ++c) {
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

      pCollectable_t item(dynamic_cast<Collectable*>(m_assetManager.cloneAsset(m_data.settings->coinProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad coin proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_data.settings->tileSize.x, pos.y + static_cast<float32_t>(j) * m_data.settings->tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_data.expendableItems[item->getName()] = item;

      coinsAndThrowables[i][j] = true;
   }

   for (int c = 0; c < m_data.gameOpts.nuggets; ++c) {
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

      if (m_data.mineField[i][j] && m_data.mineField[i][j]->getTypeName() == mineStr) {
         --c;
         continue;
      }

      pCollectable_t item(dynamic_cast<Collectable*>(m_assetManager.cloneAsset(m_data.settings->nuggetProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad nugget proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_data.settings->tileSize.x, pos.y + static_cast<float32_t>(j) * m_data.settings->tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_data.expendableItems[item->getName()] = item;

      coinsAndThrowables[i][j] = true;
   }

   for (int c = 0; c < m_data.gameOpts.throwables; ++c) {
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

      if (m_data.mineField[i][j] && m_data.mineField[i][j]->getTypeName() == mineStr) {
         --c;
         continue;
      }

      pThrowable_t item(dynamic_cast<Throwable*>(m_assetManager.cloneAsset(m_data.settings->throwableProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad throwable proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_data.settings->tileSize.x, pos.y + static_cast<float32_t>(j) * m_data.settings->tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_data.expendableItems[item->getName()] = item;

      coinsAndThrowables[i][j] = true;
   }

   for (int c = 0; c < m_data.gameOpts.zombies; ++c) {
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

      if (m_data.mineField[i][j] && m_data.mineField[i][j]->getTypeName() == mineStr) {
         --c;
         continue;
      }

      if (abs(i - plyrI) < 4 || abs(j - plyrJ) < 4) {
         --c;
         continue;
      }

      pZombie_t item(dynamic_cast<Zombie*>(m_assetManager.cloneAsset(m_data.settings->zombieProtoId)));

      if (!item)
         throw Exception("Error populating map; Bad zombie proto id", __FILE__, __LINE__);

      item->setTranslation(pos.x + static_cast<float32_t>(i) * m_data.settings->tileSize.x, pos.y + static_cast<float32_t>(j) * m_data.settings->tileSize.y);

      item->addToWorld();
      m_worldSpace.trackEntity(item);
      m_data.expendableItems[item->getName()] = item;
   }

   for (int i = 0; i < w; ++i) {
      for (int j = 0; j < h; ++j) {
         if (isAdjacentTo(Vec2i(i, j), Vec2i(plyrI, plyrJ))) continue;
         if (isAdjacentTo(Vec2i(i, j), Vec2i(exitI, exitJ))) continue;

         float32_t x = pos.x + m_data.settings->tileSize.x * static_cast<float32_t>(i);
         float32_t y = pos.y + m_data.settings->tileSize.y * static_cast<float32_t>(j);

         pSoil_t soil(dynamic_cast<Soil*>(m_assetManager.cloneAsset(m_data.settings->soilProtoId)));

         soil->setTranslation(x, y);

         soil->addToWorld();
         m_worldSpace.trackEntity(soil);
         m_data.expendableItems[soil->getName()] = soil;
      }
   }
}

//===========================================
// Application::loadAssets
//===========================================
void Application::loadAssets() {
   m_mapLoader.parseMapFile("data/xml/PersistentAssets.xml");
   m_mapLoader.update(m_renderer.getCamera().getTranslation());

   m_data.settings = boost::dynamic_pointer_cast<GameSettings>(m_assetManager.getAssetPointer(m_gameSettingsId));
   if (!m_data.settings)
      throw Exception("Error loading map; Bad game settings id", __FILE__, __LINE__);

   m_data.player = boost::dynamic_pointer_cast<Player>(m_assetManager.getAssetPointer(m_data.settings->playerId));
   if (!m_data.player)
      throw Exception("Error loading map; Bad player proto id", __FILE__, __LINE__);

   m_data.exit = boost::dynamic_pointer_cast<Exit>(m_assetManager.getAssetPointer(m_data.settings->exitId));
   if (!m_data.exit)
      throw Exception("Error loading map; Bad exit proto id", __FILE__, __LINE__);

   m_data.startMenu = boost::dynamic_pointer_cast<StartMenu>(m_assetManager.getAssetPointer(m_data.settings->startMenuId));
   if (!m_data.startMenu)
      throw Exception("Error loading map; Bad start menu id", __FILE__, __LINE__);

   m_data.pauseMenu = boost::dynamic_pointer_cast<PauseMenu>(m_assetManager.getAssetPointer(m_data.settings->pauseMenuId));
   if (!m_data.pauseMenu)
      throw Exception("Error loading map; Bad pause menu id", __FILE__, __LINE__);

   m_data.gameOptionsMenu = boost::dynamic_pointer_cast<GameOptionsMenu>(m_assetManager.getAssetPointer(m_data.settings->gameOptionsMenuId));
   if (!m_data.gameOptionsMenu)
      throw Exception("Error loading map; Bad game options menu id", __FILE__, __LINE__);

   m_data.scoreCounter = boost::dynamic_pointer_cast<Counter>(m_assetManager.getAssetPointer(m_data.settings->scoreCounterId));
   if (!m_data.scoreCounter)
      throw Exception("Error loading map; Bad score counter id", __FILE__, __LINE__);

   m_data.timeCounter = boost::dynamic_pointer_cast<Counter>(m_assetManager.getAssetPointer(m_data.settings->timeCounterId));
   if (!m_data.timeCounter)
      throw Exception("Error loading map; Bad time counter id", __FILE__, __LINE__);
   m_data.timeCounterColour = m_data.timeCounter->getFillColour();

   m_data.txtRestart = boost::dynamic_pointer_cast<TextEntity>(m_assetManager.getAssetPointer(m_data.settings->txtRestartId));
   if (!m_data.txtRestart)
      throw Exception("Error loading map; Bad txtRestart id", __FILE__, __LINE__);

   m_data.music = pMusicTrack_t(new MusicTrack(m_data.settings->musicTrack));
}

//===========================================
// Application::launch
//===========================================
void Application::launch(int argc, char** argv) {
#ifdef DEBUG
   m_data.dbg_worldSpaceVisible = false;

   if (argc > 0) {
      for (int i = 0; i < argc; ++i) {
         if (strcmp(argv[i], "-novsync") == 0)
            WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
      }
   }
#endif
   m_mapLoader.initialise(Functor<void, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::setMapSettings),
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::constructAsset),
      Functor<void, TYPELIST_1(Dodge::pAsset_t)>(this, &Application::deleteAsset),
      TARGET_MEM_USAGE);

   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   m_eventManager.registerCallback(internString("quitGame"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::quitReqHandler));

   m_eventManager.registerCallback(internString("repopulateMap"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::populateMap));

   m_audio.initialise();
   m_data.soundFx.initialise();

   loadAssets();

   m_logic.start();
}
