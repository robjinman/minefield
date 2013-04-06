#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <dodge/Sprite.hpp>
#include <dodge/xml/xml.hpp>
#include <dodge/StringId.hpp>
#include <dodge/WorldSpace.hpp>
#include "Item.hpp"
#include "Throwable.hpp"


class Player : public Item, public Dodge::Sprite {
   public:
      enum state_t { ALIVE, DEAD };
      enum mode_t { NORMAL_MODE, THROWING_MODE };
      enum dir_t { LEFT, RIGHT, UP, DOWN };

      explicit Player(const Dodge::XmlNode data);
      Player(const Player& copy);
      Player(const Player& copy, long name);

      inline void confineToRegion(const Dodge::Range& region);

      bool moveLeft();
      bool moveRight();
      bool moveUp();
      bool moveDown();

      inline dir_t facingDir() const;

      virtual void addToWorld() { Sprite::addToWorld(); }
      virtual void removeFromWorld() { Sprite::removeFromWorld(); }

      void revive();
      void die();

      inline state_t getState() const;
      inline mode_t getMode() const;

      virtual void draw() const;
      virtual void update();
      virtual void onEvent(const Dodge::EEvent* event);
      virtual size_t getSize() const;
      virtual Asset* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Player();

   private:
      Dodge::Range m_region;

      Dodge::WorldSpace m_worldSpace;
      Dodge::Quad m_footSensor;
      Dodge::Quad m_headSensor;
      Dodge::Quad m_leftSensor;
      Dodge::Quad m_rightSensor;
      Dodge::Quad m_midSensor;
      pItem_t m_crosshairs;
      pThrowable_t m_throwable;

      Dodge::Range m_originalTexSection;

      dir_t m_facing;
      state_t m_state;
      mode_t m_mode;

      bool move(dir_t direction);
      void deepCopy(const Player& copy);
      void init();
      void checkForCollisions();
      void checkForCollisionWithThrowable();
      void enterThrowingMode(pThrowable_t throwable);
      void enterNormalMode();
      void explosionHandler(Dodge::EEvent* event);

      void mouseMove(int x, int y);
      void mouseLeftClick(int x, int y);
};

typedef boost::shared_ptr<Player> pPlayer_t;

//===========================================
// Player::getState
//===========================================
Player::state_t Player::getState() const {
   return m_state;
}

//===========================================
// Player::getMode
//===========================================
Player::mode_t Player::getMode() const {
   return m_mode;
}

//===========================================
// Player::confineToRegion
//===========================================
void Player::confineToRegion(const Dodge::Range& region) {
   m_region = region;
}

//===========================================
// Player::facingDir
//===========================================
inline Player::dir_t Player::facingDir() const {
   return m_facing;
}


#endif
