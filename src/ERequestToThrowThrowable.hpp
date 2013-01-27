/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_REQUEST_TO_THROW_THROWABLE_HPP__
#define __E_REQUEST_TO_THROW_THROWABLE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/math/Vec2f.hpp>
#include "Throwable.hpp"


class ERequestToThrowThrowable : public Dodge::EEvent {
   public:
      ERequestToThrowThrowable(pThrowable_t item_, Dodge::float32_t x, Dodge::float32_t y)
         : EEvent(Dodge::internString("requestToThrowThrowable")), item(item_), to(x, y) {}

      pThrowable_t item;
      Dodge::Vec2f to;
};


#endif
