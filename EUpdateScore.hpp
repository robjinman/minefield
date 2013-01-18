/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_UPDATE_SCORE_HPP__
#define __E_UPDATE_SCORE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>


class EUpdateScore : public Dodge::EEvent {
   public:
      EUpdateScore(int value_)
         : EEvent(Dodge::internString("updateScore")), value(value_) {}

      int value;
};


#endif
