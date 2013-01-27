#include "CTextEntity.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// CTextEntity::CTextEntity
//===========================================
CTextEntity::CTextEntity(const Dodge::XmlNode data)
   : Asset(internString("CTextEntity")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     TextEntity(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CTextEntity);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CTextEntity; ");
      throw;
   }
}

//===========================================
// CTextEntity::CTextEntity
//===========================================
CTextEntity::CTextEntity(const CTextEntity& copy)
   : Asset(internString("CTextEntity")),
     Entity(copy),
     Item(copy),
     TextEntity(copy) {}

//===========================================
// CTextEntity::CTextEntity
//===========================================
CTextEntity::CTextEntity(const CTextEntity& copy, long name)
   : Asset(internString("CTextEntity")),
     Entity(copy, name),
     Item(copy, name),
     TextEntity(copy, name) {}

//===========================================
// CTextEntity::CTextEntity
//===========================================
size_t CTextEntity::getSize() const {
   return sizeof(CTextEntity)
      - sizeof(Item)
      - sizeof(TextEntity)
      + Item::getSize()
      + TextEntity::getSize()
      - sizeof(Asset);
}

//===========================================
// CTextEntity::draw
//===========================================
void CTextEntity::draw() const {
   TextEntity::draw();
}

//===========================================
// CTextEntity::update
//===========================================
void CTextEntity::update() {
   TextEntity::update();
}

//===========================================
// CTextEntity::clone
//===========================================
CTextEntity* CTextEntity::clone() const {
   return new CTextEntity(*this);
}

//===========================================
// CTextEntity::addToWorld
//===========================================
void CTextEntity::addToWorld() {
   TextEntity::addToWorld();
}

//===========================================
// CTextEntity::removeFromWorld
//===========================================
void CTextEntity::removeFromWorld() {
   TextEntity::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// CTextEntity::dbg_print
//===========================================
void CTextEntity::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "CTextEntity\n";

   Item::dbg_print(out, tab + 1);
   TextEntity::dbg_print(out, tab + 1);
}
#endif

//===========================================
// CTextEntity::assignData
//===========================================
void CTextEntity::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, CTextEntity)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "TextEntity") {
         TextEntity::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CTextEntity; ");
      throw;
   }
}
