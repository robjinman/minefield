#include <dodge/EventManager.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/math/math.hpp>
#include "Zombie.hpp"
#include "EExplosion.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Dodge::XmlNode data)
   : Asset(internString("Zombie")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_state(EMERGING),
     m_active(false) {

   try {
      XML_NODE_CHECK(data, Zombie);

      XmlNode node = data.nthChild(2);
      XML_NODE_CHECK(node, alertRadius);
      float32_t f = node.getFloat();
      m_alertRadiusSq = f * f;

      node = node.nextSibling();
      XML_NODE_CHECK(node, footSensor);
      m_footSensor = Quad(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, headSensor);
      m_headSensor = Quad(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, leftSensor);
      m_leftSensor = Quad(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, rightSensor);
      m_rightSensor = Quad(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Zombie; ");
      throw;
   }
}

//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Zombie& copy)
   : Asset(internString("Zombie")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_state(EMERGING),
     m_active(false),
     m_alertRadiusSq(copy.m_alertRadiusSq) {

   deepCopy(copy);
}

//===========================================
// Zombie::Zombie
//===========================================
Zombie::Zombie(const Zombie& copy, long name)
   : Asset(internString("Zombie")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_state(EMERGING),
     m_active(false),
     m_alertRadiusSq(copy.m_alertRadiusSq) {

   deepCopy(copy);
}

//===========================================
// Zombie::deepCopy
//===========================================
void Zombie::deepCopy(const Zombie& copy) {
   m_footSensor = copy.m_footSensor;
   m_headSensor = copy.m_headSensor;
   m_leftSensor = copy.m_leftSensor;
   m_rightSensor = copy.m_rightSensor;
}

//===========================================
// Zombie::getSize
//===========================================
size_t Zombie::getSize() const {
   return sizeof(Zombie)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize()
      - sizeof(Asset);
}

//===========================================
// Zombie::draw
//===========================================
void Zombie::draw() const {
   if (m_active) Sprite::draw();
}

//===========================================
// Zombie::checkForCollisions
//===========================================
void Zombie::checkForCollisions() {
   static long hitFromAboveStr = internString("hitFromAbove");

   vector<pEntity_t> vec;
   m_worldSpace.getEntities(getBoundary(), vec);

   for (uint_t i = 0; i < vec.size(); ++i) {
      if (vec[i].get() == this) continue;
      if (!vec[i]->hasShape()) continue;

      Vec2f A = getTranslation_abs();
      Vec2f B = vec[i]->getTranslation_abs();

      if (Math::overlap(m_leftSensor, A, vec[i]->getShape(), B)
         && Math::overlap(m_rightSensor, A, vec[i]->getShape(), B)
         && Math::overlap(m_headSensor, A, vec[i]->getShape(), B)
         && Math::overlap(m_footSensor, A, vec[i]->getShape(), B)) {

            EEvent* event = new EEvent(hitFromAboveStr);
            vec[i]->onEvent(event);
            delete event;
      }
   }
}

//===========================================
// Zombie::onEvent
//===========================================
void Zombie::onEvent(const EEvent* event) {
   static long entityTranslationStr = internString("entityTranslation");
   static long transPartFinishedStr = internString("transPartFinished");
   static long moveLeftStr = internString("moveLeft");
   static long moveRightStr = internString("moveRight");
   static long moveUpStr = internString("moveUp");
   static long moveDownStr = internString("moveDown");
   static long animFinishedStr = internString("animFinished");
   static long explodeStr = internString("explode");
   static long ariseStr = internString("arise");

   Sprite::onEvent(event);

   if (event->getType() == transPartFinishedStr) {
      const ETransPartFinished* e = static_cast<const ETransPartFinished*>(event);
      pTransformation_t trans = e->transformation;

      if (trans->getName() == moveLeftStr
         || trans->getName() == moveUpStr
         || trans->getName() == moveRightStr
         || trans->getName() == moveDownStr) stepAnimation();
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);
      pAnimation_t anim = e->animation;

      if (anim->getName() == explodeStr)
         setPendingDeletion();
      else if (anim->getName() == ariseStr) {
         if (m_state == EMERGING)
            m_state = ALIVE;
      }
   }
   else if (event->getType() == entityTranslationStr) {
      checkForCollisions();
   }
}

//===========================================
// Zombie::move
//===========================================
void Zombie::move(dir_t dir) {
   static long moveLeftStr = internString("moveLeft");
   static long moveRightStr = internString("moveRight");
   static long moveUpStr = internString("moveUp");
   static long moveDownStr = internString("moveDown");

   long anim = 0;

   switch (dir) {
      case LEFT:   anim = moveLeftStr;  break;
      case RIGHT:  anim = moveRightStr; break;
      case DOWN:   anim = moveDownStr;  break;
      case UP:     anim = moveUpStr;    break;
   }

   stopAnimation();
   playAnimation(anim, true);
   pauseAnimation();

   if (numActiveTransformations() == 0)
      playTransformation(anim);
}

//===========================================
// Zombie::update
//===========================================
void Zombie::update() {
   static long idleStr = internString("idle");

   Sprite::update();

   if (m_state == IDLE)
      playAnimation(idleStr);

   if (m_active && m_state == ALIVE) {
      if (numActiveTransformations() == 0) {
         Vec2f pos = getTranslation_abs();

         float32_t dx = m_lastKnownPos.x - pos.x;
         float32_t dy = m_lastKnownPos.y - pos.y;

         if (fabs(dx) > fabs(dy)) {
            if (dx < 0.f)
               move(LEFT);
            else
               move(RIGHT);
         }
         else {
            if (dy < 0.f)
               move(DOWN);
            else
               move(UP);
         }
      }
   }
}

//===========================================
// Zombie::playerDeathHandler
//===========================================
void Zombie::playerDeathHandler(EEvent* event) {
   if (m_state != DEAD) m_state = IDLE;
}

//===========================================
// Zombie::explosionHandler
//===========================================
void Zombie::explosionHandler(EEvent* event) {
   static long explodeStr = internString("explode");

   EExplosion* e = static_cast<EExplosion*>(event);
   Vec2f pos = getTranslation_abs() + (getOnScreenSize() / 2.f);
   Vec2f diff = pos - e->pos;

   if (diff.x * diff.x + diff.y * diff.y < e->radius * e->radius) {
      if (m_state == ALIVE) {
         stopAnimation();
         stopTransformations();
         playAnimation(explodeStr);
         m_state = DEAD;
      }
   }
}

//===========================================
// Zombie::clone
//===========================================
Zombie* Zombie::clone() const {
   return new Zombie(*this);
}

//===========================================
// Zombie::addToWorld
//===========================================
void Zombie::addToWorld() {
   Sprite::addToWorld();

   EventManager eventManager;
   eventManager.registerCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::entityMovedHandler));
   eventManager.registerCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::explosionHandler));
   eventManager.registerCallback(internString("playerDeath"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::playerDeathHandler));
}

//===========================================
// Zombie::removeFromWorld
//===========================================
void Zombie::removeFromWorld() {
   Sprite::removeFromWorld();

   EventManager eventManager;
   eventManager.unregisterCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::entityMovedHandler));
   eventManager.unregisterCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::explosionHandler));
   eventManager.unregisterCallback(internString("playerDeath"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::playerDeathHandler));
}

//===========================================
// Zombie::entityMovedHandler
//===========================================
void Zombie::entityMovedHandler(EEvent* event) {
   static long playerStr = internString("player");
   static long ariseStr = internString("arise");

   if (m_state == DEAD) return;

   EEntityTranslation* e = static_cast<EEntityTranslation*>(event);

   if (e->entity->getName() == playerStr) {
      Vec2f plyr = e->entity->getTranslation_abs();
      Vec2f zomb = getTranslation_abs();

      m_lastKnownPos = plyr;

      if ((zomb.x - plyr.x) * (zomb.x - plyr.x) + (zomb.y - plyr.y) * (zomb.y - plyr.y) <= m_alertRadiusSq) {
         m_active = true;

         if (m_state == EMERGING)
            playAnimation(ariseStr);
      }
   }
}

#ifdef DEBUG
//===========================================
// Zombie::dbg_print
//===========================================
void Zombie::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Zombie\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Zombie::assignData
//===========================================
void Zombie::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, Zombie)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "alertRadius") {
         float32_t f = node.getFloat();
         m_alertRadiusSq = f * f;
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Zombie; ");
      throw;
   }
}

//===========================================
// Zombie::~Zombie
//===========================================
Zombie::~Zombie() {
   EventManager eventManager;
   eventManager.unregisterCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::entityMovedHandler));
   eventManager.unregisterCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::explosionHandler));
   eventManager.unregisterCallback(internString("playerDeath"), Functor<void, TYPELIST_1(EEvent*)>(this, &Zombie::playerDeathHandler));
}
