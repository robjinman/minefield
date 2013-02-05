#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include "Soil.hpp"
#include "EBlast.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const XmlNode data)
   : Asset(internString("Soil")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Soil);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Soil; ");
      throw;
   }

   init();
}

//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const Soil& copy)
   : Asset(internString("Soil")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {

   init();
}

//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const Soil& copy, long name)
   : Asset(internString("Soil")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {

   init();
}

//===========================================
// Soil::init
//===========================================
void Soil::init() {
   EventManager eventManager;
   eventManager.registerCallback(internString("blast"), Functor<void, TYPELIST_1(EEvent*)>(this, &Soil::blastHandler));
}

//===========================================
// Soil::getSize
//===========================================
size_t Soil::getSize() const {
   return sizeof(Soil)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize();
}

//===========================================
// Soil::clone
//===========================================
Asset* Soil::clone() const {
   return new Soil(*this);
}

//===========================================
// Soil::addToWorld
//===========================================
void Soil::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// Soil::removeFromWorld
//===========================================
void Soil::removeFromWorld() {
   Sprite::removeFromWorld();
}

//===========================================
// Soil::onEvent
//===========================================
void Soil::onEvent(const EEvent* event) {
   static long hitFromLeftStr = internString("hitFromLeft");
   static long hitFromTopStr = internString("hitFromTop");
   static long hitFromRightStr = internString("hitFromRight");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long dissolveStr = internString("dissolve");
   static long animFinishedStr = internString("animFinished");

   Sprite::onEvent(event);

   if (event->getType() == hitFromLeftStr
      || event->getType() == hitFromRightStr
      || event->getType() == hitFromTopStr
      || event->getType() == hitFromBottomStr) {

      playAnimation(dissolveStr);
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == dissolveStr)
         setPendingDeletion();
   }
}

//===========================================
// Soil::blastHandler
//===========================================
void Soil::blastHandler(EEvent* event) {
   static long dissolveStr = internString("dissolve");

   EBlast* e = static_cast<EBlast*>(event);
   Vec2f pos = getTranslation_abs() + (getOnScreenSize() / 2.f);
   Vec2f diff = pos - e->pos;

   if (diff.x * diff.x + diff.y * diff.y < e->radius * e->radius)
      playAnimation(dissolveStr);
}

#ifdef DEBUG
//===========================================
// Soil::dbg_print
//===========================================
void Soil::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Soil\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Soil::update
//===========================================
void Soil::update() {
   Sprite::update();
}

//===========================================
// Soil::draw
//===========================================
void Soil::draw() const {
   Sprite::draw();
}

//===========================================
// Soil::assignData
//===========================================
void Soil::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Soil)

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
      e.prepend("Error parsing XML for instance of class Soil; ");
      throw;
   }
}

//===========================================
// Soil::~Soil
//===========================================
Soil::~Soil() {
   EventManager eventManager;
   eventManager.unregisterCallback(internString("blast"), Functor<void, TYPELIST_1(EEvent*)>(this, &Soil::blastHandler)); 
}
