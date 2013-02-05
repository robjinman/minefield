#include <sstream>
#include "Counter.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Counter::Counter
//===========================================
Counter::Counter(const Dodge::XmlNode data)
   : Asset(internString("Counter")),
     Entity(data.firstChild().firstChild().firstChild()),
     CTextEntity(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Counter);

      XmlNode node = data.nthChild(1);
      XML_NODE_CHECK(node, value);
      m_value = node.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Counter; ");
      throw;
   }
}

//===========================================
// Counter::Counter
//===========================================
Counter::Counter(const Counter& copy)
   : Asset(internString("Counter")),
     Entity(copy),
     CTextEntity(copy),
     m_value(copy.m_value) {}

//===========================================
// Counter::Counter
//===========================================
Counter::Counter(const Counter& copy, long name)
   : Asset(internString("Counter")),
     Entity(copy, name),
     CTextEntity(copy, name),
     m_value(copy.m_value) {}

//===========================================
// Counter::getSize
//===========================================
size_t Counter::getSize() const {
   return sizeof(Counter)
      - sizeof(CTextEntity)
      + CTextEntity::getSize()
      - sizeof(Asset);
}

//===========================================
// Counter::addToWorld
//===========================================
void Counter::addToWorld() {
   CTextEntity::addToWorld();
   setValue(m_value);
}

//===========================================
// Counter::setValue
//===========================================
void Counter::setValue(int value) {
   m_value = value;

   stringstream str;
   str << m_value;

   setText(str.str());
}

//===========================================
// Counter::clone
//===========================================
Asset* Counter::clone() const {
   return new Counter(*this);
}

#ifdef DEBUG
//===========================================
// Counter::dbg_print
//===========================================
void Counter::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Counter\n";

   CTextEntity::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Counter::assignData
//===========================================
void Counter::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, Counter)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "CTextEntity") {
         CTextEntity::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "value") {
         m_value = node.getInt();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Counter; ");
      throw;
   }
}
