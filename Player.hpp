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

      explicit Player(const Dodge::XmlNode data);
      Player(const Player& copy);
      Player(const Player& copy, long name);

      inline void moveLeft();
      inline void moveRight();
      inline void moveUp();
      inline void moveDown();

      void revive();
      void die();

      inline state_t getState() const;

      virtual void draw() const;
      virtual void update();
      virtual void onEvent(const Dodge::EEvent* event);
      virtual size_t getSize() const;
      virtual Player* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Player();

   private:
      enum dir_t { MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN };
      enum mode_t { NORMAL_MODE, THROWING_MODE };

      Dodge::WorldSpace m_worldSpace;
      Dodge::Quad m_footSensor;
      Dodge::Quad m_headSensor;
      Dodge::Quad m_leftSensor;
      Dodge::Quad m_rightSensor;
      Dodge::Quad m_midSensor;
      pItem_t m_crosshairs;
      pThrowable_t m_throwable;

      Dodge::Range m_originalTexSection;

      state_t m_state;
      mode_t m_mode;

      void move(dir_t direction);
      void deepCopy(const Player& copy);
      void init();
      void checkForCollisions();
      void enterThrowingMode(pThrowable_t throwable);
      void enterNormalMode();
      void explosionHandler(Dodge::EEvent* event);

      void mouseMove(int x, int y);
      void mouseLeftClick(int x, int y);
};

typedef boost::shared_ptr<Player> pPlayer_t;

//===========================================
// Player::move*
//===========================================
void Player::moveLeft() { move(MOVE_LEFT); }
void Player::moveRight() { move(MOVE_RIGHT); }
void Player::moveUp() { move(MOVE_UP); }
void Player::moveDown() { move(MOVE_DOWN); }

//===========================================
// Player::getState
//===========================================
Player::state_t Player::getState() const {
   return m_state;
}


#endif
