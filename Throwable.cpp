#include <dodge/EventManager.hpp>
#include <dodge/WorldSpace.hpp>
#include <dodge/math/math.hpp>
#include <dodge/EAnimFinished.hpp>
#include "Throwable.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Throwable::Throwable
//===========================================
Throwable::Throwable(const Dodge::XmlNode data)
   : Asset(internString("Throwable")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Throwable);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Throwable; ");
      throw;
   }
}

//===========================================
// Throwable::Throwable
//===========================================
Throwable::Throwable(const Throwable& copy)
   : Asset(internString("Throwable")),
     Entity(copy),
     Item(copy),
     Sprite(copy) {}

//===========================================
// Throwable::Throwable
//===========================================
Throwable::Throwable(const Throwable& copy, long name)
   : Asset(internString("Throwable")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name) {}

//===========================================
// Throwable::Throwable
//===========================================
size_t Throwable::getSize() const {
   return sizeof(Throwable)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// Throwable::draw
//===========================================
void Throwable::draw() const {
   Sprite::draw();
}

//===========================================
// Throwable::update
//===========================================
void Throwable::update() {
   Sprite::update();
}

//===========================================
// Throwable::throwTo
//===========================================
void Throwable::throwTo(float32_t x, float32_t y) {
   static long throwStr = internString("throw");

   Vec2f pos = getTranslation_abs();
   Vec2f dest(x, y);

   vector<TransPart> parts;
   parts.push_back(TransPart(0.5f, dest - pos, -360.0, Vec2f(1.f, 1.f)));

   pTransformation_t trans(new Transformation(throwStr, parts));

   addTransformation(trans);
   playTransformation(throwStr);
}

//===========================================
// Throwable::onEvent
//===========================================
void Throwable::onEvent(const Dodge::EEvent* event) {
   static long transFinishedStr = internString("transFinished");
   static long animFinishedStr = internString("animFinished");
   static long hitFromAboveStr = internString("hitFromAbove");
   static long throwStr = internString("throw");
   static long dissolveStr = internString("dissolve");

   Sprite::onEvent(event);

   if (event->getType() == transFinishedStr) {
      WorldSpace worldSpace;

      const ETransFinished* e = static_cast<const ETransFinished*>(event);

      if (e->transformation->getName() == throwStr) {
         vector<pEntity_t> vec;
         worldSpace.getEntities(getBoundary(), vec);

         for (uint_t i = 0; i < vec.size(); ++i) {
            Vec2f A = getTranslation_abs();
            Vec2f B = vec[i]->getTranslation_abs();

            if (vec[i]->hasShape() && Math::overlap(getShape(), A, vec[i]->getShape(), B)) {
               EEvent* event = new EEvent(hitFromAboveStr);
               vec[i]->onEvent(event);
               delete event;

               playAnimation(dissolveStr);
            }
         }
      }
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == dissolveStr)
         setPendingDeletion();
   }
}

//===========================================
// Throwable::clone
//===========================================
Throwable* Throwable::clone() const {
   return new Throwable(*this);
}

//===========================================
// Throwable::addToWorld
//===========================================
void Throwable::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// Throwable::removeFromWorld
//===========================================
void Throwable::removeFromWorld() {
   Sprite::removeFromWorld();
}

#ifdef DEBUG
//===========================================
// Throwable::dbg_print
//===========================================
void Throwable::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Throwable\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Throwable::assignData
//===========================================
void Throwable::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, Throwable)

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
      e.prepend("Error parsing XML for instance of class Throwable; ");
      throw;
   }
}
