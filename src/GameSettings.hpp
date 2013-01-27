#ifndef __GAME_SETTINGS_HPP__
#define __GAME_SETTINGS_HPP__


#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <dodge/Asset.hpp>
#include <dodge/Range.hpp>
#include <dodge/renderer/Colour.hpp>
#include "GameOptions.hpp"


class GameSettings : virtual public Dodge::Asset {
   public:
      GameSettings(const Dodge::XmlNode data);

      virtual GameSettings* clone() const;
      virtual size_t getSize() const;

      Dodge::Colour                 bgColour;
      std::string                   musicTrack;

      long                          startMenuId;
      long                          pauseMenuId;
      long                          gameOptionsMenuId;
      long                          playerId;
      long                          exitId;
      long                          numericTileProtoId;
      long                          mineProtoId;
      long                          soilProtoId;
      long                          coinProtoId;
      long                          nuggetProtoId;
      long                          throwableProtoId;
      long                          zombieProtoId;
      long                          scoreCounterId;
      long                          timeCounterId;
      long                          txtRestartId;

      Dodge::Range                  minefieldBoundary;
      Dodge::Vec2f                  tileSize;

      std::vector<GameOptions>      difficultyModes;

   private:
      void parseGameModes(Dodge::XmlNode data);
};

typedef boost::shared_ptr<GameSettings> pGameSettings_t;


#endif
