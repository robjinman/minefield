#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include "MenuItem.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MenuItem::MenuItem
//===========================================
MenuItem::MenuItem(const XmlNode data)
   : Asset(internString("MenuItem")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     UiButton(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, MenuItem);

      XmlAttribute attr = data.firstAttribute();

      if (!attr.isNull() && attr.name() == "position")
         m_position = attr.getInt();
      else
         m_position = -1;
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class MenuItem; ");
      throw;
   }
}

//===========================================
// MenuItem::MenuItem
//===========================================
MenuItem::MenuItem(const MenuItem& copy)
   : Asset(internString("MenuItem")),
     Entity(copy),
     Item(copy),
     UiButton(copy) {}

//===========================================
// MenuItem::MenuItem
//===========================================
MenuItem::MenuItem(const MenuItem& copy, long name)
   : Asset(internString("MenuItem")),
     Entity(copy, name),
     Item(copy, name),
     UiButton(copy, name) {}

//===========================================
// MenuItem::getSize
//===========================================
size_t MenuItem::getSize() const {
   return sizeof(MenuItem)
      - sizeof(Item)
      - sizeof(UiButton)
      + Item::getSize()
      + UiButton::getSize();
}

//===========================================
// MenuItem::clone
//===========================================
Asset* MenuItem::clone() const {
   return new MenuItem(*this);
}

//===========================================
// MenuItem::addToWorld
//===========================================
void MenuItem::addToWorld() {
   UiButton::addToWorld();
}

//===========================================
// MenuItem::removeFromWorld
//===========================================
void MenuItem::removeFromWorld() {
   UiButton::removeFromWorld();
}

//===========================================
// MenuItem::onEvent
//===========================================
void MenuItem::onEvent(const EEvent* event) {
 // TODO
}

#ifdef DEBUG
//===========================================
// MenuItem::dbg_print
//===========================================
void MenuItem::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "MenuItem\n";

   Item::dbg_print(out, tab + 1);
   UiButton::dbg_print(out, tab + 1);
}
#endif

//===========================================
// MenuItem::update
//===========================================
void MenuItem::update() {
   UiButton::update();
}

//===========================================
// MenuItem::draw
//===========================================
void MenuItem::draw() const {
   UiButton::draw();
}

//===========================================
// MenuItem::assignData
//===========================================
void MenuItem::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, MenuItem)

      XmlAttribute attr = data.firstAttribute();
      if (!attr.isNull() && attr.name() == "position")
         m_position = attr.getInt();

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "UiButton") {
         UiButton::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class MenuItem; ");
      throw;
   }
}

//===========================================
// MenuItem::~MenuItem
//===========================================
MenuItem::~MenuItem() {}
