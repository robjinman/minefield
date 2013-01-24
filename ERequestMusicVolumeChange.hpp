/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_REQUEST_MUSIC_VOLUME_CHANGE_HPP__
#define __E_REQUEST_MUSIC_VOLUME_CHANGE_HPP__


#include <dodge/EEvent.hpp>


class ERequestMusicVolumeChange : public Dodge::EEvent {
   public:
      ERequestMusicVolumeChange(float dv)
         : EEvent(Dodge::internString("requestMusicVolumeChange")), volume(dv) {}

      float volume;
};


#endif
