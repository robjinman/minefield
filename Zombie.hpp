#ifndef __ZOMBIE_HPP__
#define __ZOMBIE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Zombie : public Item, public Dodge::Sprite {
   public:
      Zombie(const Dodge::XmlNode data);
      Zombie(const Zombie& copy);
      Zombie(const Zombie& copy, long name);

      virtual void draw() const;

      virtual void onEvent(const Dodge::EEvent* event) { Sprite::onEvent(event); }
      virtual void update();

      virtual size_t getSize() const;
      virtual Zombie* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Zombie() {}
};

typedef boost::shared_ptr<Zombie> pZombie_t;


#endif
