#include "Clouds.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Clouds::Clouds
//===========================================
Clouds::Clouds(const Dodge::XmlNode data)
   : Asset(internString("Clouds")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Clouds);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Clouds; ");
      throw;
   }
}

//===========================================
// Clouds::Clouds
//===========================================
Clouds::Clouds(const Clouds& copy)
   : Asset(internString("Clouds")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {}

//===========================================
// Clouds::Clouds
//===========================================
Clouds::Clouds(const Clouds& copy, long name)
   : Asset(internString("Clouds")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {}

//===========================================
// Clouds::Clouds
//===========================================
size_t Clouds::getSize() const {
   return sizeof(Clouds)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// Clouds::draw
//===========================================
void Clouds::draw() const {
   Sprite::draw();
}

//===========================================
// Clouds::update
//===========================================
void Clouds::update() {
   static long idleStr = internString("idle");

   playAnimation(idleStr);
   if (numActiveTransformations() == 0) playTransformation(idleStr);

   Sprite::update();
}

//===========================================
// Clouds::clone
//===========================================
Asset* Clouds::clone() const {
   return new Clouds(*this);
}

//===========================================
// Clouds::addToWorld
//===========================================
void Clouds::addToWorld() {
   static long idleStr = internString("idle");

   Sprite::addToWorld();

   playTransformation(idleStr);
}

//===========================================
// Clouds::removeFromWorld
//===========================================
void Clouds::removeFromWorld() {
   Sprite::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// Clouds::dbg_print
//===========================================
void Clouds::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Clouds\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Clouds::assignData
//===========================================
void Clouds::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, Clouds)

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
      e.prepend("Error parsing XML for instance of class Clouds; ");
      throw;
   }
}
