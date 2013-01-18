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

      virtual void update();
      virtual void onEvent(const Dodge::EEvent* event);
      virtual size_t getSize() const;
      virtual Mine* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);

      void explode();

   private:
      enum state_t { IDLE, EXPLODING };

      state_t m_state;
      unsigned int m_value;
};

typedef boost::shared_ptr<Mine> pMine_t;


#endif
