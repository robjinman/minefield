#ifndef __CLOUDS_HPP__
#define __CLOUDS_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Clouds : public Item, public Dodge::Sprite {
   public:
      Clouds(const Dodge::XmlNode data);
      Clouds(const Clouds& copy);
      Clouds(const Clouds& copy, long name);

      virtual void draw() const;

      virtual void update();

      virtual size_t getSize() const;
      virtual Clouds* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Clouds() {}
};

typedef boost::shared_ptr<Clouds> pClouds_t;


#endif
