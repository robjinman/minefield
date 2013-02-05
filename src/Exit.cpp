#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include <dodge/EventManager.hpp>
#include "Exit.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Exit::Exit
//===========================================
Exit::Exit(const XmlNode data)
   : Asset(internString("Exit")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)),
     m_state(IS_CLOSED) {

   try {
      XML_NODE_CHECK(data, Exit);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Exit; ");
      throw;
   }

   init();
}

//===========================================
// Exit::Exit
//===========================================
Exit::Exit(const Exit& copy)
   : Asset(internString("Exit")),
     Entity(copy),
     Item(copy),
     Sprite(copy),
     m_state(IS_CLOSED) { init(); }

//===========================================
// Exit::Exit
//===========================================
Exit::Exit(const Exit& copy, long name)
   : Asset(internString("Exit")),
     Entity(copy, name),
     Item(copy, name),
     Sprite(copy, name),
     m_state(IS_CLOSED) { init(); }

//===========================================
// Exit::init
//===========================================
void Exit::init() {
   m_originalTexSection = getTextureSection();
}

//===========================================
// Exit::getSize
//===========================================
size_t Exit::getSize() const {
   return sizeof(Exit)
      - sizeof(Item)
      - sizeof(Sprite)
      + Item::getSize()
      + Sprite::getSize();
}

//===========================================
// Exit::clone
//===========================================
Asset* Exit::clone() const {
   return new Exit(*this);
}

//===========================================
// Exit::addToWorld
//===========================================
void Exit::addToWorld() {
   Sprite::addToWorld();
}

//===========================================
// Exit::removeFromWorld
//===========================================
void Exit::removeFromWorld() {
   Sprite::removeFromWorld();
   m_state = IS_CLOSED;
}

//===========================================
// Exit::onEvent
//===========================================
void Exit::onEvent(const EEvent* event) {
   static long hitFromAboveStr = internString("hitFromAbove");
   static long successStr = internString("success");

   Sprite::onEvent(event);

   if (event->getType() == hitFromAboveStr) {
      if (m_state == IS_OPEN) {
         EventManager eventManager;
         eventManager.queueEvent(new EEvent(successStr));
      }
   }
}

#ifdef DEBUG
//===========================================
// Exit::dbg_print
//===========================================
void Exit::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Exit\n";

   Item::dbg_print(out, tab + 1);
   Sprite::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Exit::update
//===========================================
void Exit::update() {
   Sprite::update();
}

//===========================================
// Exit::draw
//===========================================
void Exit::draw() const {
   Sprite::draw();
}

//===========================================
// Exit::open
//===========================================
void Exit::open() {
   static long openIdleStr = internString("openIdle");

   if (m_state != IS_OPEN) {
      playAnimation(openIdleStr, true);
      m_state = IS_OPEN;
   }
}

//===========================================
// Exit::close
//===========================================
void Exit::close() {
   m_state = IS_CLOSED;
   stopAnimation();
   setTextureSection(m_originalTexSection);
}

//===========================================
// Exit::assignData
//===========================================
void Exit::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Exit)

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
      e.prepend("Error parsing XML for instance of class Exit; ");
      throw;
   }
}

//===========================================
// Exit::~Exit
//===========================================
Exit::~Exit() {}
