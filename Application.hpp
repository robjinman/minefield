/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Player.hpp"
#include "Exit.hpp"
#include "StartMenu.hpp"
#include "PauseMenu.hpp"
#include "Counter.hpp"
#include "SoundFx.hpp"
#include "gameState.hpp"
#include "GameOptionsMenu.hpp"


class Application {
   public:
      Application();

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void launch(int argc, char** argv);
      void quit();

   private:
      typedef enum {
         MOUSE_LEFT_BUTTON    = 0,
         MOUSE_MIDDLE_BUTTON  = 1,
         MOUSE_RIGHT_BUTTON   = 2,

         NUM_MOUSE_BUTTONS = 3
      } mouseBtn_t;

      void keyDown(int key);
      void keyUp(int key);
      void mouseLeftClick(int x, int y);
      void mouseRightClick(int x, int y);
      void mouseLeftRelease(int x, int y);
      void mouseRightRelease(int x, int y);
      void mouseMove(int x, int y);
      void keyboard();
      void onWindowResize(int w, int h);
      void deletePending(Dodge::EEvent* event);
      void deleteAsset(Dodge::pAsset_t asset);
      Dodge::pAsset_t constructAsset(const Dodge::XmlNode data);
      void setMapSettings(const Dodge::XmlNode data);
      void draw() const;
      void update();
      void populateMap();
      void playerDeath(Dodge::EEvent* event = NULL);
      void freeAllAssets();
      bool isAdjacentTo(const Dodge::Vec2i& a, const Dodge::Vec2i& b) const;
      void gameSuccess(const Dodge::EEvent* event);
      void startGame();
      void loadAssets();
      void resetGame();
      void quitGame(Dodge::EEvent* event);
      void updateScore(Dodge::EEvent* event);
      void updateTimer();
      void reqToThrowThrowable(Dodge::EEvent* event);
      void reqGameStateChangeHandler(Dodge::EEvent* event);
      void reqMusicVolumeChangeHandler(Dodge::EEvent* event);
      void reqSfxVolumeChangeHandler(Dodge::EEvent* event);
      void reqGameOptsChangeHandler(Dodge::EEvent* event);
      void parseGameModes(Dodge::XmlNode data);

      void exitDefault();

      void computeFrameRate();

      bool m_init;

      Functor<void, TYPELIST_0()>   m_onExit;

      Dodge::Renderer&              m_renderer;
      Dodge::Audio                  m_audio;
      std::map<int, bool>           m_keyState;
      bool                          m_mouseState[NUM_MOUSE_BUTTONS];
      Dodge::WinIO                  m_win;
      Dodge::EventManager           m_eventManager;
      Dodge::Range                  m_viewArea;
      gameState_t                   m_gameState;
#ifdef DEBUG
      bool                          dbg_worldSpaceVisible;
#endif
      double                        m_frameRate;

      Dodge::AssetManager           m_assetManager;
      Dodge::WorldSpace             m_worldSpace;
      std::map<long, pItem_t>       m_persistentItems;
      std::map<long, pItem_t>       m_expendableItems;

      Dodge::Colour                 m_bgColour;

      Dodge::MapLoader              m_mapLoader;

      std::vector<std::vector<pItem_t> > m_mineField;

      Dodge::Range                  m_minefieldBoundary;
      Dodge::Vec2f                  m_tileSize;

      long                          m_startMenuId;
      long                          m_pauseMenuId;
      long                          m_gameOptionsMenuId;
      long                          m_playerId;
      long                          m_exitId;
      long                          m_numericTileProtoId;
      long                          m_mineProtoId;
      long                          m_soilProtoId;
      long                          m_coinProtoId;
      long                          m_nuggetProtoId;
      long                          m_throwableProtoId;
      long                          m_zombieProtoId;
      long                          m_scoreCounterId;
      long                          m_timeCounterId;
      long                          m_txtRestartId;

      std::vector<GameOptions>      m_difficultyModes;

      pPlayer_t                     m_player;
      pExit_t                       m_exit;

      pCounter_t                    m_scoreCounter;
      pCounter_t                    m_timeCounter;
      Dodge::pTextEntity_t          m_txtRestart;

      GameOptions                   m_gameOpts;

      pStartMenu_t                  m_startMenu;
      pPauseMenu_t                  m_pauseMenu;
      pGameOptionsMenu_t            m_gameOptionsMenu;

      Dodge::pMusicTrack_t          m_music;
      SoundFx                       m_soundFx;
};


#endif
