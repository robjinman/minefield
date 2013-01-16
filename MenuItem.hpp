#ifndef __MENU_ITEM_HPP__
#define __MENU_ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/ui/UiButton.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class MenuItem : public Item, public Dodge::UiButton {
   public:
      MenuItem(const Dodge::XmlNode data);
      MenuItem(const MenuItem& copy);
      MenuItem(const MenuItem& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual MenuItem* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~MenuItem();
};

typedef boost::shared_ptr<MenuItem> pMenuItem_t;


#endif
