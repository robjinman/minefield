#include "Player.hpp"
#include "EExplosion.hpp"
#include <dodge/EventManager.hpp>
#include <dodge/math/math.hpp>
#include <dodge/EAnimFinished.hpp>


using namespace std;
using namespace Dodge;


//===========================================
// Player::Player
//===========================================
Player::Player(const Dodge::XmlNode data)
   : Asset(Dodge::internString("Player")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_state(ALIVE) {

   try {
      XML_NODE_CHECK(data, Player);

      XmlNode node = data.nthChild(2);
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

      node = node.nextSibling();
      XML_NODE_CHECK(node, midSensor);
      m_midSensor = Quad(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Player; ");
      throw;
   }

   init();
}

//===========================================
// Player::Player
//===========================================
Player::Player(const Player& copy)
   : Asset(Dodge::internString("Player")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_state(ALIVE) {

   deepCopy(copy);
   init();
}

//===========================================
// Player::Player
//===========================================
Player::Player(const Player& copy, long name)
   : Asset(Dodge::internString("Player")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_state(ALIVE) {

   deepCopy(copy);
   init();
}

//===========================================
// Player::init
//===========================================
void Player::init() {
   EventManager eventManager;
   eventManager.registerCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Player::explosionHandler));
}

//===========================================
// Player::deepCopy
//===========================================
void Player::deepCopy(const Player& copy) {
   m_footSensor = copy.m_footSensor;
   m_headSensor = copy.m_headSensor;
   m_leftSensor = copy.m_leftSensor;
   m_rightSensor = copy.m_rightSensor;
}

//===========================================
// Player::update
//===========================================
void Player::update() {
   Sprite::update();
}

//===========================================
// Player::getSize
//===========================================
size_t Player::getSize() const {
   return sizeof(Player)
      - sizeof(Item)
      + Item::getSize()
      - sizeof(Sprite)
      + Sprite::getSize()
      - sizeof(Asset)
      - sizeof(Quad) + m_footSensor.getSize()
      - sizeof(Quad) + m_headSensor.getSize()
      - sizeof(Quad) + m_leftSensor.getSize()
      - sizeof(Quad) + m_rightSensor.getSize();
}

//===========================================
// Player::clone
//===========================================
Player* Player::clone() const {
   return new Player(*this);
}

#ifdef DEBUG
//===========================================
// Player::dbg_print
//===========================================
void Player::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Player\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Player::onEvent
//===========================================
void Player::onEvent(const EEvent* event) {
   static long entityTranslationStr = internString("entityTranslation");
   static long transPartFinishedStr = internString("transPartFinished");
   static long animFinishedStr = internString("animFinished");
   static long explodeStr = internString("explode");
   static long moveLeftStr = internString("moveLeft");
   static long moveRightStr = internString("moveRight");
   static long moveUpStr = internString("moveUp");
   static long moveDownStr = internString("moveDown");
   static long playerDeathStr = internString("playerDeath");

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

      if (anim->getName() == explodeStr) {
         EventManager eventManager;
         EEvent* eDeath = new EEvent(playerDeathStr);
         eventManager.queueEvent(eDeath);
      }
   }
   else if (event->getType() == entityTranslationStr) {
      checkForCollisions();
   }
}

//===========================================
// Player::explosionHandler
//===========================================
void Player::explosionHandler(EEvent* event) {
   EExplosion* e = static_cast<EExplosion*>(event);
   Vec2f pos = getTranslation_abs() + (getOnScreenSize() / 2.f);
   Vec2f diff = pos - e->pos;

   if (diff.x * diff.x + diff.y * diff.y < e->radius * e->radius) {
      if (m_state == ALIVE) die();
   }
}

//===========================================
// Player::die
//===========================================
void Player::die() {
   static long explodeStr = internString("explode");

   stopAnimation();
   stopTransformations();
   playAnimation(explodeStr);
   m_state = DEAD;
}

//===========================================
// Player::enterThrowingMode
//===========================================
void Player::enterThrowingMode(pEntity_t throwable) {
   cout << "Player::enterThrowingMode()\n";
}

//===========================================
// Player::checkForCollisions
//===========================================
void Player::checkForCollisions() {
   static long throwableStr = internString("throwable");
   static long hitFromRightStr = internString("hitFromRight");
   static long hitFromLeftStr = internString("hitFromLeft");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long hitFromTopStr = internString("hitFromTop");
   static long hitFromAboveStr = internString("hitFromAbove");

   vector<pEntity_t> vec;
   m_worldSpace.getEntities(getBoundary(), vec);

   for (uint_t i = 0; i < vec.size(); ++i) {
      if (vec[i].get() == this) continue;
      if (!vec[i]->hasShape()) continue;

      Vec2f A = getTranslation_abs();
      Vec2f B = vec[i]->getTranslation_abs();

      bool l = false;
      bool r = false;
      bool t = false;
      bool b = false;

      if (Math::overlap(m_leftSensor, A, vec[i]->getShape(), B)) {
         EEvent* event = new EEvent(hitFromRightStr);
         vec[i]->onEvent(event);
         delete event;

         l = true;
      }

      if (Math::overlap(m_rightSensor, A, vec[i]->getShape(), B)) {
         EEvent* event = new EEvent(hitFromLeftStr);
         vec[i]->onEvent(event);
         delete event;

         r = true;
      }

      if (Math::overlap(m_headSensor, A, vec[i]->getShape(), B)) {
         EEvent* event = new EEvent(hitFromBottomStr);
         vec[i]->onEvent(event);
         delete event;

         t = true;
      }

      if (Math::overlap(m_footSensor, A, vec[i]->getShape(), B)) {
         EEvent* event = new EEvent(hitFromTopStr);
         vec[i]->onEvent(event);
         delete event;

         b = true;
      }

      if (l && r && t && b) {
         EEvent* event = new EEvent(hitFromAboveStr);
         vec[i]->onEvent(event);
         delete event;

         l = true;
      }

      // Check if the player has stumbled upon a throwable item
      if (Math::overlap(m_midSensor, A, vec[i]->getShape(), B)) {
         if (vec[i]->getTypeName() == throwableStr && numActiveTransformations() == 0)
            enterThrowingMode(vec[i]);
      }
   }
}

//===========================================
// Player::move
//===========================================
void Player::move(dir_t dir) {
   static long moveLeftStr = internString("moveLeft");
   static long moveRightStr = internString("moveRight");
   static long moveUpStr = internString("moveUp");
   static long moveDownStr = internString("moveDown");

   long anim = 0;

   switch (dir) {
      case MOVE_LEFT:   anim = moveLeftStr;  break;
      case MOVE_RIGHT:  anim = moveRightStr; break;
      case MOVE_DOWN:   anim = moveDownStr;  break;
      case MOVE_UP:     anim = moveUpStr;    break;
   }

   playAnimation(anim, true);
   pauseAnimation();

   if (numActiveTransformations() == 0)
      playTransformation(anim);
}

//===========================================
// Player::assignData
//
// I'm not using this function
//===========================================
void Player::assignData(const Dodge::XmlNode data) {
   assert(false);
}

//===========================================
// Player::~Player
//===========================================
Player::~Player() {
   EventManager eventManager;
   eventManager.unregisterCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Player::explosionHandler));
}
