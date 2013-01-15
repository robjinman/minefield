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
   m_midSensor = copy.m_midSensor;
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
      - sizeof(Quad) + m_rightSensor.getSize()
      - sizeof(Quad) + m_midSensor.getSize();
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
   static long transPartFinishedStr = internString("transPartFinished");
   static long moveLeftStr = internString("moveLeft");
   static long moveRightStr = internString("moveRight");
   static long moveUpStr = internString("moveUp");
   static long moveDownStr = internString("moveDown");

   Sprite::onEvent(event);

   if (event->getType() == transPartFinishedStr) {
      const ETransPartFinished* e = static_cast<const ETransPartFinished*>(event);
      pTransformation_t trans = e->transformation;

      if (trans->getName() == moveLeftStr
         || trans->getName() == moveUpStr
         || trans->getName() == moveRightStr
         || trans->getName() == moveDownStr) stepAnimation();
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
      if (m_state == ALIVE) {
         stopAnimation();
         stopTransformations();
         playAnimation(internString("explode"));
         m_state = DEAD;
      }
   }
}

//===========================================
// Player::move
//===========================================
void Player::move(dir_t dir) {
   static long moveLeftStr = internString("moveLeft");
   static long hitFromRightStr = internString("hitFromRight");
   static long moveRightStr = internString("moveRight");
   static long hitFromLeftStr = internString("hitFromLeft");
   static long moveUpStr = internString("moveUp");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long moveDownStr = internString("moveDown");
   static long hitFromTopStr = internString("hitFromTop");

   Shape* sensor = NULL;

   long plyrAnim = 0, eventType = 0;
   switch (dir) {
      case MOVE_LEFT:
         sensor = &m_leftSensor;
         plyrAnim = moveLeftStr;
         eventType = hitFromRightStr;
         break;
      case MOVE_UP:
         sensor = &m_headSensor;
         plyrAnim = moveUpStr;
         eventType = hitFromBottomStr;
         break;
      case MOVE_RIGHT:
         sensor = &m_rightSensor;
         plyrAnim = moveRightStr;
         eventType = hitFromLeftStr;
         break;
      case MOVE_DOWN:
         sensor = &m_footSensor;
         plyrAnim = moveDownStr;
         eventType = hitFromTopStr;
         break;
   }

   playAnimation(plyrAnim, true);
   pauseAnimation();

   if (numActiveTransformations() == 0) {
      playTransformation(plyrAnim);

      Vec2f min = sensor->getMinimum();
      Vec2f pos = getTranslation_abs() + min;
      Range range(pos, sensor->getMaximum() - min);

      vector<pEntity_t> vec;
      m_worldSpace.getEntities(range, vec);
      for (uint_t i = 0; i < vec.size(); ++i) {
         if (vec[i].get() == this) continue;
         if (!vec[i]->hasShape()) continue;

         if (Math::overlap(*sensor, getTranslation_abs(), vec[i]->getShape(), vec[i]->getTranslation_abs())) {
            EEvent* event = new EEvent(eventType);
            vec[i]->onEvent(event);
         }
      }
   }
}

//===========================================
// Player::assignData
//===========================================
void Player::assignData(const Dodge::XmlNode data) {
   if (data.isNull() || data.name() != "Player") return;

   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "Item") {
      Item::assignData(node);
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "Sprite") {
      Sprite::assignData(node);
   }
}

//===========================================
// Player::~Player
//===========================================
Player::~Player() {
   EventManager eventManager;
   eventManager.unregisterCallback(internString("explosion"), Functor<void, TYPELIST_1(EEvent*)>(this, &Player::explosionHandler));
}
