/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __GAME_LOGIC_HPP__
#define __GAME_LOGIC_HPP__


#include <map>
#include <dodge/dodge.hpp>
#include "GameData.hpp"
#include "gameState.hpp"


class GameLogic {
   public:
      GameLogic(GameData& gameData);

      void start();

   private:
      typedef enum {
         MOUSE_LEFT_BUTTON    = 0,
         MOUSE_MIDDLE_BUTTON  = 1,
         MOUSE_RIGHT_BUTTON   = 2,

         NUM_MOUSE_BUTTONS = 3
      } mouseBtn_t;

      struct mouseState_t {
         mouseState_t() {
            for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
               btnState[i] = false;
         }

         bool btnState[NUM_MOUSE_BUTTONS];
         Dodge::Vec2i pos;
      };

      void keyDown(int key);
      void keyUp(int key);
      void mouseLeftClick(int x, int y);
      void mouseRightClick(int x, int y);
      void mouseLeftRelease(int x, int y);
      void mouseRightRelease(int x, int y);
      void mouseMove(int x, int y);
      void mouse();
      void keyboard();
      void onWindowResize(int w, int h);

      void draw() const;
      void update();
      void startGame();
      void resetGame();
      void updateTimer();
      void computeFrameRate();

      void deletePending(Dodge::EEvent* event);
      void reqToThrowThrowable(Dodge::EEvent* event);
      void reqGameStateChangeHandler(Dodge::EEvent* event);
      void reqMusicVolumeChangeHandler(Dodge::EEvent* event);
      void reqSfxVolumeChangeHandler(Dodge::EEvent* event);
      void reqGameOptsChangeHandler(Dodge::EEvent* event);
      void animFinishedHandler(Dodge::EEvent* event);
      void playerDeath(Dodge::EEvent* event = NULL);
      void gameOver(Dodge::EEvent* event = NULL);
      void quitGame(Dodge::EEvent* event);
      void updateScore(Dodge::EEvent* event);
      void gameSuccess(const Dodge::EEvent* event);

      Dodge::WorldSpace             m_worldSpace;
      Dodge::Renderer&              m_renderer;
      Dodge::Audio                  m_audio;
      std::map<int, bool>           m_keyState;
      std::vector<int>              m_dirKeyStack;
      int                           m_missedKeypress;
      mouseState_t                  m_mouseState;
      Dodge::WinIO                  m_win;
      Dodge::EventManager           m_eventManager;
      double                        m_frameRate;
      gameState_t                   m_gameState;

      GameData&                     m_data;
};


#endif
