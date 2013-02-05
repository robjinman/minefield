#ifndef __NUMERIC_TILE_HPP__
#define __NUMERIC_TILE_HPP__


#include <dodge/Sprite.hpp>
#include <dodge/xml/xml.hpp>
#include <dodge/StringId.hpp>
#include "Item.hpp"


class NumericTile : public Item, public Dodge::Sprite {
   public:
      explicit NumericTile(const Dodge::XmlNode data);
      NumericTile(const NumericTile& copy);
      NumericTile(const NumericTile& copy, long name);

      virtual void update();
      virtual void draw() const { Sprite::draw(); }

      virtual void addToWorld() { Sprite::addToWorld(); }
      virtual void removeFromWorld() { Sprite::removeFromWorld(); }
      virtual void onEvent(const Dodge::EEvent* event) { Sprite::onEvent(event); }

      virtual size_t getSize() const;
      virtual Asset* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);

      inline unsigned int getValue() const;
      void setValue(unsigned int value);

   private:
      unsigned int m_value;
      Dodge::Vec2i m_tex;
};

typedef boost::shared_ptr<NumericTile> pNumericTile_t;

//===========================================
// NumericTile::getValue
//===========================================
inline unsigned int NumericTile::getValue() const {
   return m_value;
}


#endif
