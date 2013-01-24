#include "Mine.hpp"
#include "EExplosion.hpp"
#include "EBlast.hpp"
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

      XmlNode node = data.nthChild(2);
      XML_NODE_CHECK(node, explosionRadius);
      m_explosionRadius = node.getFloat();

      node = node.nextSibling();
      XML_NODE_CHECK(node, blastRadius);
      m_blastRadius = node.getFloat();
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
     m_state(IDLE),
     m_explosionRadius(copy.m_explosionRadius),
     m_blastRadius(copy.m_blastRadius) {}

//===========================================
// Mine::Mine
//===========================================
Mine::Mine(const Mine& copy, long name)
   : Asset(Dodge::internString("Mine")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_state(IDLE),
     m_explosionRadius(copy.m_explosionRadius),
     m_blastRadius(copy.m_blastRadius) {}

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
   static long hitFromAboveStr = internString("hitFromAbove");
   static long animFinishedStr = internString("animFinished");
   static long explodeStr = internString("explode");

   if (event->getType() == hitFromAboveStr) {
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
      setZ(getZ() + 1);

      stopAnimation();
      playAnimation(explodeStr);

      EventManager eventManager;

      EExplosion* explosion = new EExplosion(getTranslation_abs() + (getOnScreenSize() / 2.f), m_explosionRadius);
      EBlast* blast = new EBlast(getTranslation_abs() + (getOnScreenSize() / 2.f), m_blastRadius);

      eventManager.queueEvent(explosion);
      eventManager.queueEvent(blast);

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
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "explosionRadius") {
      m_explosionRadius = node.getFloat();
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "blastRadius") {
      m_blastRadius = node.getFloat();
   }
}
