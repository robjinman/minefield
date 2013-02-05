#ifndef __GAME_OPTIONS_HPP__
#define __GAME_OPTIONS_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/xml/xml.hpp>


struct GameOptions {
   public:
      GameOptions()
         : mines(0),
           coins(0),
           nuggets(0),
           totalGold(0),
           throwables(0),
           zombies(0),
           requiredGold(0),
           timeLimit(0),
           bestTime(-1) {}

      GameOptions(const Dodge::XmlNode data);

      int mines;
      int coins;
      int nuggets;
      int totalGold;
      int throwables;
      int zombies;
      int requiredGold;
      int timeLimit;

      int bestTime;
};

typedef boost::shared_ptr<GameOptions> pGameOptions_t;


#endif
