/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_REQUEST_GAME_OPTS_CHANGE_HPP__
#define __E_REQUEST_GAME_OPTS_CHANGE_HPP__


#include <dodge/EEvent.hpp>
#include "GameOptions.hpp"


class ERequestGameOptsChange : public Dodge::EEvent {
   public:
      ERequestGameOptsChange(const GameOptions& options_)
         : EEvent(Dodge::internString("requestGameOptsChange")), options(options_) {}

      GameOptions options;
};


#endif
