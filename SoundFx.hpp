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

      void processMessages(Dodge::EEvent* event);
};


#endif
