/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include "GameLogic.hpp"
#include "EUpdateScore.hpp"
#include "ERequestToThrowThrowable.hpp"
#include "ERequestGameStateChange.hpp"
#include "ERequestMusicVolumeChange.hpp"
#include "ERequestSfxVolumeChange.hpp"
#include "ERequestGameOptsChange.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// GameLogic::GameLogic
//===========================================
GameLogic::GameLogic(GameData& gameData)
   : m_renderer(Renderer::getInstance()),
     m_missedKeypress(-1),
     m_frameRate(60.0),
     m_data(gameData) {}

//===========================================
// GameLogic::keyDown
//===========================================
void GameLogic::keyDown(int key) {
   if (key == WinIO::KEY_LEFT || key == WinIO::KEY_RIGHT
      || key == WinIO::KEY_UP || key == WinIO::KEY_DOWN) {

      m_dirKeyStack.push_back(key);
   }

#ifdef DEBUG
   switch (key) {
      case WinIO::KEY_1: m_data.dbg_worldSpaceVisible = !m_data.dbg_worldSpaceVisible; break;
      case WinIO::KEY_F:
         cout << "Frame rate (main thread): " << m_frameRate << "fps\n";
         cout << "Frame rate (renderer): " << m_renderer.getFrameRate() << "fps\n";
      break;
      default: break;
   }
#endif

   switch (m_gameState) {
      case ST_RUNNING:
         switch (key) {
            case WinIO::KEY_ESCAPE:
               m_eventManager.queueEvent(new ERequestGameStateChange(ST_PAUSED));
            break;
         }
      break;
      case ST_PAUSED:
      break;
      case ST_START_MENU:
      break;
      // ...
   }

   m_keyState[key] = true;
}

//===========================================
// GameLogic::keyUp
//===========================================
void GameLogic::keyUp(int key) {
   m_keyState[key] = false;

   // dirKeyStack is the list of currently depressed directional
   // keys in the order they were pressed.
   for (uint_t i = 0; i < m_dirKeyStack.size(); ++i) {
      if (m_keyState[m_dirKeyStack[i]] == false) {
         m_dirKeyStack.erase(m_dirKeyStack.begin() + i);
         --i;
      }
   }
}

//===========================================
// GameLogic::mouseLeftClick
//===========================================
void GameLogic::mouseLeftClick(int x, int y) {
   m_mouseState.btnState[MOUSE_LEFT_BUTTON] = true;
   m_mouseState.pos = Vec2i(x, y);

   if (m_gameState == ST_RUNNING) {
      if (m_data.player->getState() == Player::DEAD) {
         resetGame();
         startGame();
         m_mouseState.btnState[MOUSE_LEFT_BUTTON] = false;
      }
      else if (m_data.player->getState() == Player::ALIVE
         && m_data.player->getMode() == Player::THROWING_MODE) {

         m_mouseState.btnState[MOUSE_LEFT_BUTTON] = false;
      }
   }
}

//===========================================
// GameLogic::mouseRightClick
//===========================================
void GameLogic::mouseRightClick(int x, int y) {
   m_mouseState.btnState[MOUSE_RIGHT_BUTTON] = true;
   m_mouseState.pos = Vec2i(x, y);
}

//===========================================
// GameLogic::mouseLeftRelease
//===========================================
void GameLogic::mouseLeftRelease(int x, int y) {
   m_mouseState.btnState[MOUSE_LEFT_BUTTON] = false;
   m_mouseState.pos = Vec2i(x, y);
}

//===========================================
// GameLogic::mouseRightRelease
//===========================================
void GameLogic::mouseRightRelease(int x, int y) {
   m_mouseState.btnState[MOUSE_RIGHT_BUTTON] = false;
   m_mouseState.pos = Vec2i(x, y);
}

//===========================================
// GameLogic::mouseMove
//===========================================
void GameLogic::mouseMove(int x, int y) {
   m_mouseState.pos = Vec2i(x, y);
}

//===========================================
// GameLogic::mouse
//===========================================
void GameLogic::mouse() {
   Vec2f viewPos = m_renderer.getCamera().getTranslation();

   int x = m_mouseState.pos.x;
   int y = m_win.getWindowHeight() - m_mouseState.pos.y;

   float32_t wx = viewPos.x + static_cast<float32_t>(x) * gGetPixelSize().x;
   float32_t wy = viewPos.y + static_cast<float32_t>(y) * gGetPixelSize().y;

   switch (m_gameState) {
      case ST_RUNNING:
         if (m_mouseState.btnState[MOUSE_LEFT_BUTTON]
            && m_data.player->getState() == Player::ALIVE
            && m_data.player->getMode() == Player::NORMAL_MODE) {

            Vec2f sz = m_data.player->getOnScreenSize();
            Vec2f plyr = m_data.player->getTranslation_abs() + (sz / 2.0);

            float32_t dx = wx - plyr.x;
            float32_t dy = wy - plyr.y;

            if (fabs(dx) > sz.x / 2.0 || fabs(dy) > sz.y / 2.0) {
               if (fabs(dx) > fabs(dy)) {
                  if (dx < 0.f)
                     m_data.player->moveLeft();
                  else
                     m_data.player->moveRight();
               }
               else {
                  if (dy < 0.f)
                     m_data.player->moveDown();
                  else
                     m_data.player->moveUp();
               }
            }
         }
      break;
      case ST_START_MENU:
      break;
      case ST_PAUSED:
      break;
   }
}

//===========================================
// GameLogic::keyboard
//===========================================
void GameLogic::keyboard() {
   switch (m_gameState) {
      case ST_RUNNING:
         switch (m_data.player->getState()) {
            case Player::ALIVE: {
               // Attempt to move in direction of missed keypress
               if (m_missedKeypress != -1) {
                  switch (m_missedKeypress) {
                     case WinIO::KEY_RIGHT:  if (m_data.player->moveRight()) m_missedKeypress = -1; break;
                     case WinIO::KEY_LEFT:   if (m_data.player->moveLeft())  m_missedKeypress = -1; break;
                     case WinIO::KEY_UP:     if (m_data.player->moveUp())    m_missedKeypress = -1; break;
                     case WinIO::KEY_DOWN:   if (m_data.player->moveDown())  m_missedKeypress = -1; break;
                  }
               }
               else {
                  // Move in the direction of most recently pressed key
                  if (m_dirKeyStack.size() > 0) {
                     switch (m_dirKeyStack.back()) {
                        case WinIO::KEY_RIGHT:
                           if (!m_data.player->moveRight()) {                       // If the player is already moving remember this keypress
                              if (m_data.player->facingDir() != Player::RIGHT)
                                 m_missedKeypress = WinIO::KEY_RIGHT;
                           }
                           break;
                        case WinIO::KEY_LEFT:
                           if (!m_data.player->moveLeft()) {
                              if (m_data.player->facingDir() != Player::LEFT)
                                 m_missedKeypress = WinIO::KEY_LEFT;
                           }
                           break;
                        case WinIO::KEY_UP:
                           if (!m_data.player->moveUp()) {
                              if (m_data.player->facingDir() != Player::UP)
                                 m_missedKeypress = WinIO::KEY_UP;
                           }
                           break;
                        case WinIO::KEY_DOWN:
                           if (!m_data.player->moveDown()) {
                              if (m_data.player->facingDir() != Player::DOWN)
                                 m_missedKeypress = WinIO::KEY_DOWN;
                           }
                           break;
                     }
                  }
               }
            }
            break;
            case Player::DEAD: {
               if (m_keyState[WinIO::KEY_ENTER]) {
                  resetGame();
                  startGame();
               }
            }
            break;
         }
      break;
      case ST_START_MENU:
      break;
      case ST_PAUSED:
      break;
   }
}

//===========================================
// GameLogic::resetGame
//===========================================
void GameLogic::resetGame() {
   m_data.txtRestart->removeFromWorld();

   m_eventManager.clear();

   m_data.mineField.clear();
   m_data.expendableItems.clear();

   m_worldSpace.removeAndUntrackAll();
   for (auto i = m_data.persistentItems.begin(); i != m_data.persistentItems.end(); ++i)
      m_worldSpace.insertAndTrackEntity(i->second);

   m_data.startMenu->addToWorld();
   m_gameState = ST_START_MENU;
}

//===========================================
// GameLogic::computeFrameRate
//===========================================
void GameLogic::computeFrameRate() {
   static Timer timer;

   m_frameRate = 1.0 / timer.getTime();
   timer.reset();
}

//===========================================
// GameLogic::update
//===========================================
void GameLogic::update() {
   switch (m_gameState) {
      case ST_RUNNING:
         for (auto i = m_data.persistentItems.begin(); i != m_data.persistentItems.end(); ++i)
            i->second->update();

         for (auto i = m_data.expendableItems.begin(); i != m_data.expendableItems.end(); ++i)
            i->second->update();

         if (m_data.player->getState() == Player::DEAD)
            m_data.txtRestart->update();
      break;
      case ST_START_MENU:
         m_data.startMenu->update();
      break;
      case ST_PAUSED:
         m_data.pauseMenu->update();
      break;
   }
}

//===========================================
// GameLogic::draw
//===========================================
void GameLogic::draw() const {
   switch (m_gameState) {
      case ST_RUNNING:
         for (auto i = m_data.persistentItems.begin(); i != m_data.persistentItems.end(); ++i)
            i->second->draw();

         for (auto i = m_data.expendableItems.begin(); i != m_data.expendableItems.end(); ++i)
            i->second->draw();

         if (m_data.player->getState() == Player::DEAD)
            m_data.txtRestart->draw();
      break;
      case ST_START_MENU:
         m_data.startMenu->draw();
      break;
      case ST_PAUSED:
         m_data.pauseMenu->draw();
      break;
   }

#ifdef DEBUG
   if (m_data.dbg_worldSpaceVisible)
      m_worldSpace.dbg_draw(Colour(1.f, 1.f, 1.f, 1.f), 2, 9);
#endif
}

//===========================================
// GameLogic::onWindowResize
//===========================================
void GameLogic::onWindowResize(int w, int h) {
   m_renderer.onWindowResize(w, h);
   m_renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

//===========================================
// GameLogic::gameOver
//===========================================
void GameLogic::gameOver(EEvent* event) {
   m_data.txtRestart->addToWorld();
}

//===========================================
// GameLogic::playerDeath
//===========================================
void GameLogic::playerDeath(EEvent* event) {

}

//===========================================
// GameLogic::animFinished
//===========================================
void GameLogic::animFinishedHandler(EEvent* event) {
   static long enterPortalStr = internString("enterPortal");

   EAnimFinished* e = dynamic_cast<EAnimFinished*>(event);
   assert(e);

   if (e->animation->getName() == enterPortalStr) {
      m_eventManager.queueEvent(new EEvent(internString("gameOver")));
      m_eventManager.queueEvent(new ERequestGameStateChange(ST_START_MENU));
   }
}

//===========================================
// GameLogic::gameSuccess
//===========================================
void GameLogic::gameSuccess(const EEvent* event) {
   m_data.player->stopAnimation();
   m_data.player->stopTransformations();
   m_data.player->playAnimation(internString("enterPortal"));

   int secs = m_data.gameOpts->timeLimit - m_data.timeCounter->getValue();
   int& best = m_data.gameOpts->bestTime;

   if (best == -1 || secs < best)
      best = secs;
}

//===========================================
// GameLogic::reqToThrowThrowable
//===========================================
void GameLogic::reqToThrowThrowable(EEvent* event) {
   ERequestToThrowThrowable* e = static_cast<ERequestToThrowThrowable*>(event);

   int toX = static_cast<int>(e->to.x / m_data.settings->tileSize.x);
   int toY = static_cast<int>(e->to.y / m_data.settings->tileSize.y);
   Vec2f dest(toX * m_data.settings->tileSize.x, toY * m_data.settings->tileSize.y);

   e->item->throwTo(dest.x, dest.y);
}

//===========================================
// GameLogic::startGame
//===========================================
void GameLogic::startGame() {
   m_data.startMenu->removeFromWorld();

   m_data.timeCounter->setFillColour(m_data.timeCounterColour);
   m_data.player->revive();
   m_data.exit->close();

   m_eventManager.immediateDispatch(new EEvent(internString("repopulateMap")));

   m_data.scoreCounter->setValue(m_data.gameOpts->requiredGold);
   m_data.timeCounter->setValue(m_data.gameOpts->timeLimit);
   m_gameState = ST_RUNNING;

   m_dirKeyStack.clear();
   m_missedKeypress = -1;
}

//===========================================
// GameLogic::updateScore
//===========================================
void GameLogic::updateScore(EEvent* event) {
   EUpdateScore* e = static_cast<EUpdateScore*>(event);

   m_data.scoreCounter->subtract(e->value);

   if (m_data.scoreCounter->getValue() < 0)
      m_data.scoreCounter->setValue(0);

   if (m_data.scoreCounter->getValue() == 0)
      m_data.exit->open();
}

//===========================================
// GameLogic::updateTimer
//===========================================
void GameLogic::updateTimer() {
   static long tenSecondsRemainingStr = internString("tenSecondsRemaining");
   static float32_t fr = gGetTargetFrameRate();
   static float32_t i = 0;

   if (!m_data.player) return;
   if (m_data.player->getState() == Player::DEAD) return;

   if (i > fr) {
      i = -1.f;

      m_data.timeCounter->decrement();

      if (m_data.timeCounter->getValue() == 10) {
         m_data.timeCounter->setFillColour(Colour(1, 0, 0, 1));
         m_eventManager.queueEvent(new EEvent(tenSecondsRemainingStr));
      }

      if (m_data.timeCounter->getValue() == 0) {
         m_data.player->die();
      }
   }

   i += 1.f;
}

//===========================================
// GameLogic::reqGameStateChangeHandler
//===========================================
void GameLogic::reqGameStateChangeHandler(EEvent* event) {
   ERequestGameStateChange* e = dynamic_cast<ERequestGameStateChange*>(event);
   assert(e);

   switch (m_gameState) {
      case ST_START_MENU:
         switch (e->state) {
            case ST_START_MENU: break;
            case ST_RUNNING:
               startGame();
            break;
            case ST_PAUSED: break;
         }
      break;
      case ST_RUNNING:
         switch (e->state) {
            case ST_START_MENU:
               resetGame();
            break;
            case ST_RUNNING: break;
            case ST_PAUSED:
               m_data.pauseMenu->addToWorld();
               m_gameState = ST_PAUSED;
            break;
         }
      break;
      case ST_PAUSED:
         switch (e->state) {
            case ST_START_MENU:
               m_data.pauseMenu->removeFromWorld();
               resetGame();
            break;
            case ST_RUNNING:
               m_data.pauseMenu->removeFromWorld();
               m_gameState = ST_RUNNING;
            break;
            case ST_PAUSED: break;
         }
      break;
   }
}

//===========================================
// GameLogic::reqMusicVolumeChangeHandler
//===========================================
void GameLogic::reqMusicVolumeChangeHandler(EEvent* event) {
   ERequestMusicVolumeChange* e = dynamic_cast<ERequestMusicVolumeChange*>(event);
   assert(e);

   float32_t v = m_data.music->getVolume();
   v += e->volume;

   if (v < 0.0) v = 0.0;
   if (v > 1.0) v = 1.0;

   m_data.music->setVolume(v);
}

//===========================================
// GameLogic::reqSfxVolumeChangeHandler
//===========================================
void GameLogic::reqSfxVolumeChangeHandler(EEvent* event) {
   ERequestSfxVolumeChange* e = dynamic_cast<ERequestSfxVolumeChange*>(event);
   assert(e);

   float32_t v = m_audio.getSoundVolume();
   v += e->volume;

   if (v < 0.0) v = 0.0;
   if (v > 1.0) v = 1.0;

   m_audio.setSoundVolume(v);
}

//===========================================
// GameLogic::reqGameOptsChangeHandler
//===========================================
void GameLogic::reqGameOptsChangeHandler(EEvent* event) {
   ERequestGameOptsChange* e = dynamic_cast<ERequestGameOptsChange*>(event);
   assert(e);

   m_data.gameOpts = e->options;
}

//===========================================
// GameLogic::start
//===========================================
void GameLogic::start() {
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &GameLogic::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &GameLogic::keyUp));
   m_win.registerCallback(WinIO::EVENT_MOUSEMOVE, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::mouseMove));
   m_win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::mouseLeftClick));
   m_win.registerCallback(WinIO::EVENT_BTN3PRESS, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::mouseRightClick));
   m_win.registerCallback(WinIO::EVENT_BTN1RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::mouseLeftRelease));
   m_win.registerCallback(WinIO::EVENT_BTN3RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::mouseRightRelease));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &GameLogic::onWindowResize));

   m_eventManager.registerCallback(internString("success"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::gameSuccess));

   m_eventManager.registerCallback(internString("updateScore"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::updateScore));

   m_eventManager.registerCallback(internString("playerDeath"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::playerDeath));

   m_eventManager.registerCallback(internString("requestToThrowThrowable"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::reqToThrowThrowable));

   m_eventManager.registerCallback(internString("requestGameStateChange"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::reqGameStateChangeHandler));

   m_eventManager.registerCallback(internString("requestMusicVolumeChange"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::reqMusicVolumeChangeHandler));

   m_eventManager.registerCallback(internString("requestSfxVolumeChange"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::reqSfxVolumeChangeHandler));

   m_eventManager.registerCallback(internString("requestGameOptsChange"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::reqGameOptsChangeHandler));

   m_eventManager.registerCallback(internString("animFinished"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::animFinishedHandler));

   m_eventManager.registerCallback(internString("gameOver"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &GameLogic::gameOver));

   m_data.music->play(true);
   m_data.startMenu->addToWorld();

   m_data.gameOptionsMenu->setDifficultyModes(m_data.settings->difficultyModes);
   m_data.gameOpts = m_data.settings->difficultyModes.front();

   m_gameState = ST_START_MENU;

   while (1) {
      LOOP_START

      switch (m_gameState) {
         case ST_PAUSED:
         case ST_START_MENU:
            computeFrameRate();
            m_win.doEvents();
            m_eventManager.doEvents();
            keyboard();
            mouse();
            update();
            draw();
            m_renderer.tick(m_data.settings->bgColour);
         break;
         case ST_RUNNING:
            computeFrameRate();
            m_win.doEvents();
            m_eventManager.doEvents();
            keyboard();
            mouse();
            update();
            updateTimer();
            draw();
            m_renderer.tick(m_data.settings->bgColour);
         break;
      }

      LOOP_END
   }
}
