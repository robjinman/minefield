#include <dodge/EventManager.hpp>
#include <dodge/StringId.hpp>
#include "SoundFx.hpp"


using namespace Dodge;


pSound_t SoundFx::m_bang;
pSound_t SoundFx::m_pickUpCoin;
pSound_t SoundFx::m_throw;


//===========================================
// Application::initialise
//===========================================
void SoundFx::initialise() {
   EventManager eventManager;

   eventManager.registerCallback(internString("explosion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &SoundFx::processMessages));

   eventManager.registerCallback(internString("updateScore"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &SoundFx::processMessages));

   eventManager.registerCallback(internString("requestToThrowThrowable"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &SoundFx::processMessages));

   m_bang = pSound_t(new Sound("data/sounds/bang.wav"));
   m_pickUpCoin = pSound_t(new Sound("data/sounds/pickUpCoin.wav"));
   m_throw = pSound_t(new Sound("data/sounds/throw.wav"));
}

//===========================================
// Application::processMessages
//===========================================
void SoundFx::processMessages(Dodge::EEvent* event) {
   static long explosionStr = internString("explosion");
   static long updateScoreStr = internString("updateScore");
   static long requestToThrowThrowableStr = internString("requestToThrowThrowable");

   if (event->getType() == explosionStr) m_bang->play();
   else if (event->getType() == updateScoreStr) m_pickUpCoin->play();
   else if (event->getType() == requestToThrowThrowableStr) m_throw->play();
}
