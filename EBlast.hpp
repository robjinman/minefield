/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_BLAST_HPP__
#define __E_BLAST_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/math/Vec2f.hpp>


// Blasts are non-deadly, whereas explosions are.
class EBlast : public Dodge::EEvent {
   public:
      EBlast(const Dodge::Vec2f& pos_, Dodge::float32_t radius_)
         : EEvent(Dodge::internString("blast")), pos(pos_), radius(radius_) {}

      Dodge::Vec2f pos;
      Dodge::float32_t radius;
};


#endif
