/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_REQUEST_GAME_STATE_CHANGE_HPP__
#define __E_REQUEST_GAME_STATE_CHANGE_HPP__


#include <dodge/EEvent.hpp>
#include "gameState.hpp"


class ERequestGameStateChange : public Dodge::EEvent {
   public:
      ERequestGameStateChange(gameState_t state_)
         : EEvent(Dodge::internString("requestGameStateChange")), state(state_) {}

      gameState_t state;
};


#endif
