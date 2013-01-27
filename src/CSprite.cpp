#include "CSprite.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// CSprite::CSprite
//===========================================
CSprite::CSprite(const Dodge::XmlNode data)
   : Asset(internString("CSprite")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CSprite);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CSprite; ");
      throw;
   }
}

//===========================================
// CSprite::CSprite
//===========================================
CSprite::CSprite(const CSprite& copy)
   : Asset(internString("CSprite")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {}

//===========================================
// CSprite::CSprite
//===========================================
CSprite::CSprite(const CSprite& copy, long name)
   : Asset(internString("CSprite")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {}

//===========================================
// CSprite::CSprite
//===========================================
size_t CSprite::getSize() const {
   return sizeof(CSprite)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// CSprite::draw
//===========================================
void CSprite::draw() const {
   Sprite::draw();
}

//===========================================
// CSprite::update
//===========================================
void CSprite::update() {
   Sprite::update();
}

//===========================================
// CSprite::clone
//===========================================
CSprite* CSprite::clone() const {
   return new CSprite(*this);
}

//===========================================
// CSprite::addToWorld
//===========================================
void CSprite::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// CSprite::removeFromWorld
//===========================================
void CSprite::removeFromWorld() {
   Sprite::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// CSprite::dbg_print
//===========================================
void CSprite::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "CSprite\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// CSprite::assignData
//===========================================
void CSprite::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, CSprite)

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
      e.prepend("Error parsing XML for instance of class CSprite; ");
      throw;
   }
}
