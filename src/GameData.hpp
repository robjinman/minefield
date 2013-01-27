/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __GAME_DATA_HPP__
#define __GAME_DATA_HPP__


#include <map>
#include <vector>
#include <dodge/dodge.hpp>
#include "Player.hpp"
#include "Exit.hpp"
#include "StartMenu.hpp"
#include "PauseMenu.hpp"
#include "Counter.hpp"
#include "SoundFx.hpp"
#include "GameOptionsMenu.hpp"


struct GameData {
   std::map<long, pItem_t>       persistentItems;
   std::map<long, pItem_t>       expendableItems;

   Dodge::Colour                 bgColour;

   std::vector<std::vector<pItem_t> > mineField;

   Dodge::Range                  minefieldBoundary;
   Dodge::Vec2f                  tileSize;

   long                          startMenuId;
   long                          pauseMenuId;
   long                          gameOptionsMenuId;
   long                          playerId;
   long                          exitId;
   long                          numericTileProtoId;
   long                          mineProtoId;
   long                          soilProtoId;
   long                          coinProtoId;
   long                          nuggetProtoId;
   long                          throwableProtoId;
   long                          zombieProtoId;
   long                          scoreCounterId;
   long                          timeCounterId;
   long                          txtRestartId;

   std::vector<GameOptions>      difficultyModes;

   pPlayer_t                     player;
   pExit_t                       exit;

   pCounter_t                    scoreCounter;
   pCounter_t                    timeCounter;
   Dodge::Colour                 timeCounterColour;
   Dodge::pTextEntity_t          txtRestart;

   GameOptions                   gameOpts;

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
