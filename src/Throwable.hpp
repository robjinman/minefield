#ifndef __THROWABLE_HPP__
#define __THROWABLE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Throwable : public Item, public Dodge::Sprite {
   public:
      Throwable(const Dodge::XmlNode data);
      Throwable(const Throwable& copy);
      Throwable(const Throwable& copy, long name);

      void throwTo(Dodge::float32_t x, Dodge::float32_t y);

      virtual void draw() const;

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void update();

      virtual size_t getSize() const;
      virtual Asset* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Throwable() {}
};

typedef boost::shared_ptr<Throwable> pThrowable_t;


#endif
