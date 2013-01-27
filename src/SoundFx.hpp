#ifndef __SOUND_FX_HPP__
#define __SOUND_FX_HPP__


#include <dodge/audio/Audio.hpp>


class SoundFx {
   public:
      void initialise();

   private:
      static Dodge::pSound_t m_bang;
      static Dodge::pSound_t m_pickUpCoin;
      static Dodge::pSound_t m_throw;
      static Dodge::pSound_t m_enterPortal;
      static Dodge::pSound_t m_scream;
      static Dodge::pSound_t m_tickTock;

      void processMessages(Dodge::EEvent* event);
};


#endif
