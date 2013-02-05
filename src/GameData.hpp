/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __GAME_DATA_HPP__
#define __GAME_DATA_HPP__


#include <map>
#include <dodge/dodge.hpp>
#include "Player.hpp"
#include "Exit.hpp"
#include "StartMenu.hpp"
#include "PauseMenu.hpp"
#include "Counter.hpp"
#include "SoundFx.hpp"
#include "GameOptionsMenu.hpp"
#include "GameSettings.hpp"


struct GameData {
   pGameSettings_t               settings;

   std::map<long, pItem_t>       persistentItems;
   std::map<long, pItem_t>       expendableItems;

   std::vector<std::vector<pItem_t> > mineField;

   pPlayer_t                     player;
   pExit_t                       exit;

   pCounter_t                    scoreCounter;
   pCounter_t                    timeCounter;
   Dodge::Colour                 timeCounterColour;
   Dodge::pTextEntity_t          txtRestart;

   pGameOptions_t                gameOpts;

   pStartMenu_t                  startMenu;
   pPauseMenu_t                  pauseMenu;
   pGameOptionsMenu_t            gameOptionsMenu;

   Dodge::pMusicTrack_t          music;
   SoundFx                       soundFx;

#ifdef DEBUG
   bool                          dbg_worldSpaceVisible;
#endif
};


#endif
