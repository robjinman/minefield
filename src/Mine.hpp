#ifndef __MINE_HPP__
#define __MINE_HPP__


#include <dodge/Sprite.hpp>
#include <dodge/xml/xml.hpp>
#include <dodge/StringId.hpp>
#include "Item.hpp"


class Mine : public Item, public Dodge::Sprite {
   public:
      explicit Mine(const Dodge::XmlNode data);
      Mine(const Mine& copy);
      Mine(const Mine& copy, long name);

      virtual void addToWorld() { Sprite::addToWorld(); }
      virtual void removeFromWorld() { Sprite::removeFromWorld(); }

      virtual void update();
      virtual void draw() const { Sprite::draw(); }
      virtual void onEvent(const Dodge::EEvent* event);
      virtual size_t getSize() const;
      virtual Asset* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);

      void explode();

   private:
      enum state_t { IDLE, EXPLODING };

      state_t m_state;
      Dodge::float32_t m_explosionRadius;
      Dodge::float32_t m_blastRadius;
};

typedef boost::shared_ptr<Mine> pMine_t;


#endif
