#include "Mine.hpp"
#include "EExplosion.hpp"
#include <dodge/EventManager.hpp>
#include <dodge/EAnimFinished.hpp>


using namespace std;
using namespace Dodge;


//===========================================
// Mine::Mine
//===========================================
Mine::Mine(const Dodge::XmlNode data)
   : Asset(Dodge::internString("Mine")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_state(IDLE) {

   try {
      XML_NODE_CHECK(data, Mine);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Mine; ");
      throw;
   }
}

//===========================================
// Mine::Mine
//===========================================
Mine::Mine(const Mine& copy)
   : Asset(Dodge::internString("Mine")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_state(IDLE) {}

//===========================================
// Mine::Mine
//===========================================
Mine::Mine(const Mine& copy, long name)
   : Asset(Dodge::internString("Mine")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_state(IDLE) {}

//===========================================
// Mine::update
//===========================================
void Mine::update() {
   Sprite::update();
}

//===========================================
// Mine::getSize
//===========================================
size_t Mine::getSize() const {
   return sizeof(Mine)
      - sizeof(Item)
      + Item::getSize()
      - sizeof(Sprite)
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// Mine::clone
//===========================================
Mine* Mine::clone() const {
   return new Mine(*this);
}

#ifdef DEBUG
//===========================================
// Mine::dbg_print
//===========================================
void Mine::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Mine\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Mine::onEvent
//===========================================
void Mine::onEvent(const EEvent* event) {
   static long hitFromLeftStr = internString("hitFromLeft");
   static long hitFromRightStr = internString("hitFromRight");
   static long hitFromTopStr = internString("hitFromTop");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long animFinishedStr = internString("animFinished");
   static long explodeStr = internString("explode");

   if (event->getType() == hitFromLeftStr
      || event->getType() == hitFromRightStr
      || event->getType() == hitFromTopStr
      || event->getType() == hitFromBottomStr) {

      explode();
   }

   if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == explodeStr)
         setPendingDeletion();
   }
}

//===========================================
// Mine::explode
//===========================================
void Mine::explode() {
   static long explodeStr = internString("explode");

   if (m_state != EXPLODING) {
      stopAnimation();
      playAnimation(explodeStr);

      EventManager eventManager;
      EExplosion* event = new EExplosion(getTranslation_abs() + (getOnScreenSize() / 2.f), 0.25);
      eventManager.queueEvent(event);

      m_state = EXPLODING;
   }
}

//===========================================
// Mine::assignData
//===========================================
void Mine::assignData(const Dodge::XmlNode data) {
   if (data.isNull() || data.name() != "Mine") return;

   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "Item") {
      Item::assignData(node);
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "Sprite") {
      Sprite::assignData(node);
   }
}
