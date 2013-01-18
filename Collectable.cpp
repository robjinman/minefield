#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include "EUpdateScore.hpp"
#include "Collectable.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Collectable::Collectable
//===========================================
Collectable::Collectable(const XmlNode data)
   : Asset(internString("Collectable")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Collectable);

      XmlNode node = data.nthChild(2);
      XML_NODE_CHECK(node, value);
      m_value = node.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Collectable; ");
      throw;
   }
}

//===========================================
// Collectable::Collectable
//===========================================
Collectable::Collectable(const Collectable& copy)
   : Asset(internString("Collectable")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_value(copy.m_value) {}

//===========================================
// Collectable::Collectable
//===========================================
Collectable::Collectable(const Collectable& copy, long name)
   : Asset(internString("Collectable")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_value(copy.m_value) {}

//===========================================
// Collectable::getSize
//===========================================
size_t Collectable::getSize() const {
   return sizeof(Collectable)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize();
}

//===========================================
// Collectable::clone
//===========================================
Collectable* Collectable::clone() const {
   return new Collectable(*this);
}

//===========================================
// Collectable::addToWorld
//===========================================
void Collectable::addToWorld() {
   static long idleStr = internString("idle");

   Sprite::addToWorld();
   playAnimation(idleStr, true);

   pauseAnimation();
   int r = rand() % 50;
   for (int i = 0; i < r; ++i) stepAnimation();
   playAnimation(true);
}

//===========================================
// Collectable::removeFromWorld
//===========================================
void Collectable::removeFromWorld() {
   Sprite::removeFromWorld();
}

//===========================================
// Collectable::onEvent
//===========================================
void Collectable::onEvent(const EEvent* event) {
   static long hitFromLeftStr = internString("hitFromLeft");
   static long hitFromTopStr = internString("hitFromTop");
   static long hitFromRightStr = internString("hitFromRight");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long animFinishedStr = internString("animFinished");
   static long collectStr = internString("collect");

   Sprite::onEvent(event);

   if (event->getType() == hitFromLeftStr
      || event->getType() == hitFromRightStr
      || event->getType() == hitFromTopStr
      || event->getType() == hitFromBottomStr) {

      stopAnimation();
      playAnimation(collectStr);

      EventManager eventManager;
      EEvent* e = new EUpdateScore(m_value);
      eventManager.queueEvent(e);
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == collectStr)
         setPendingDeletion();
   }
}

#ifdef DEBUG
//===========================================
// Collectable::dbg_print
//===========================================
void Collectable::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Collectable\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Collectable::update
//===========================================
void Collectable::update() {
   Sprite::update();
}

//===========================================
// Collectable::draw
//===========================================
void Collectable::draw() const {
   Sprite::draw();
}

//===========================================
// Collectable::assignData
//===========================================
void Collectable::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Collectable)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "value") {
         m_value = node.getInt();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Collectable; ");
      throw;
   }
}

//===========================================
// Collectable::~Collectable
//===========================================
Collectable::~Collectable() {}
