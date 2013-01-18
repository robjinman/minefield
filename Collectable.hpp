#ifndef __COLLECTABLE_HPP__
#define __COLLECTABLE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Collectable : public Item, public Dodge::Sprite {
   public:
      Collectable(const Dodge::XmlNode data);
      Collectable(const Collectable& copy);
      Collectable(const Collectable& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual Collectable* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Collectable();

   private:
      int m_value;
};

typedef boost::shared_ptr<Collectable> pCollectable_t;


#endif
