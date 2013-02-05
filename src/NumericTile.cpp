#include "NumericTile.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// NumericTile::NumericTile
//===========================================
NumericTile::NumericTile(const Dodge::XmlNode data)
   : Asset(Dodge::internString("NumericTile")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_value(1) {

   try {
      XML_NODE_CHECK(data, NumericTile);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class NumericTile; ");
      throw;
   }

   m_tex = getTextureSection().getPosition();
}

//===========================================
// NumericTile::NumericTile
//===========================================
NumericTile::NumericTile(const NumericTile& copy)
   : Asset(Dodge::internString("NumericTile")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {

   m_value = copy.m_value;
   m_tex = copy.m_tex;
}

//===========================================
// NumericTile::NumericTile
//===========================================
NumericTile::NumericTile(const NumericTile& copy, long name)
   : Asset(Dodge::internString("NumericTile")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {

   m_value = copy.m_value;
   m_tex = copy.m_tex;
}

//===========================================
// NumericTile::update
//===========================================
void NumericTile::update() {
   Sprite::update();
}

//===========================================
// NumericTile::getSize
//===========================================
size_t NumericTile::getSize() const {
   return sizeof(NumericTile)
      - sizeof(Item)
      + Item::getSize()
      - sizeof(Sprite)
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// NumericTile::clone
//===========================================
Asset* NumericTile::clone() const {
   return new NumericTile(*this);
}

#ifdef DEBUG
//===========================================
// NumericTile::dbg_print
//===========================================
void NumericTile::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "NumericTile\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// NumericTile::setValue
//===========================================
void NumericTile::setValue(unsigned int value) {
   m_value = value;

   const Range& r = getTextureSection();
   float32_t w = r.getSize().x;
   float32_t h = r.getSize().y;

   assert(value >= 1 && value <= 8);

   setTextureSection(static_cast<float32_t>(m_tex.x + (m_value - 1) * w), static_cast<float32_t>(m_tex.y), w, h);
}

//===========================================
// NumericTile::assignData
//===========================================
void NumericTile::assignData(const Dodge::XmlNode data) {
   if (data.isNull() || data.name() != "NumericTile") return;

   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "Item") {
      Item::assignData(node);
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "Sprite") {
      Sprite::assignData(node);
   }

   m_tex = getTextureSection().getPosition();
}
