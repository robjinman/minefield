#include "Zombie.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Dodge::XmlNode data)
   : Asset(internString("Zombie")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Zombie);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Zombie; ");
      throw;
   }
}

//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Zombie& copy)
   : Asset(internString("Zombie")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {}

//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Zombie& copy, long name)
   : Asset(internString("Zombie")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {}

//===========================================
// Zombie::Zombie
//===========================================
size_t Zombie::getSize() const {
   return sizeof(Zombie)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// Zombie::draw
//===========================================
void Zombie::draw() const {
   Sprite::draw();
}

//===========================================
// Zombie::update
//===========================================
void Zombie::update() {
   Sprite::update();
}

//===========================================
// Zombie::clone
//===========================================
Zombie* Zombie::clone() const {
   return new Zombie(*this);
}

//===========================================
// Zombie::addToWorld
//===========================================
void Zombie::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// Zombie::removeFromWorld
//===========================================
void Zombie::removeFromWorld() {
   Sprite::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// Zombie::dbg_print
//===========================================
void Zombie::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Zombie\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Zombie::assignData
//===========================================
void Zombie::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, Zombie)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Zombie; ");
      throw;
   }
}
