/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_REQUEST_SFX_VOLUME_CHANGE_HPP__
#define __E_REQUEST_SFX_VOLUME_CHANGE_HPP__


#include <dodge/EEvent.hpp>


class ERequestSfxVolumeChange : public Dodge::EEvent {
   public:
      ERequestSfxVolumeChange(float dv)
         : EEvent(Dodge::internString("requestSfxVolumeChange")), volume(dv) {}

      float volume;
};


#endif
