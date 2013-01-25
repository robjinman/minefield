#ifndef __GAME_OPTIONS_HPP__
#define __GAME_OPTIONS_HPP__


#include <dodge/xml/xml.hpp>


struct GameOptions {
   public:
      GameOptions()
         : mines(0), coins(0), nuggets(0), totalGold(0), throwables(0), zombies(0), requiredGold(0), timeLimit(0) {}

      GameOptions(const Dodge::XmlNode data);

      int mines;
      int coins;
      int nuggets;
      int totalGold;
      int throwables;
      int zombies;
      int requiredGold;
      int timeLimit;
};


#endif
