#ifndef __EXIT_HPP__
#define __EXIT_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Exit : public Item, public Dodge::Sprite {
   public:
      Exit(const Dodge::XmlNode data);
      Exit(const Exit& copy);
      Exit(const Exit& copy, long name);

      void open();

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual Exit* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Exit();

   private:
      enum state_t { IS_CLOSED, IS_OPEN };

      state_t m_state;
};

typedef boost::shared_ptr<Exit> pExit_t;


#endif
