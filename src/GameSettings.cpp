#include "GameSettings.hpp"
#include <dodge/StringId.hpp>


using namespace std;
using namespace Dodge;


//===========================================
// GameSettings::GameSettings
//===========================================
GameSettings::GameSettings(const XmlNode data)
   : Asset(internString("GameSettings")) {

   try {
      XML_NODE_CHECK(data, GameSettings);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, soundTrack);
      musicTrack = node.getString();

      node = node.nextSibling();
      XML_NODE_CHECK(node, bgColour);
      bgColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, minefieldBoundary);
      minefieldBoundary = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, tileSize);
      tileSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, startMenuId);
      startMenuId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, pauseMenuId);
      pauseMenuId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, gameOptionsMenuId);
      gameOptionsMenuId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, playerId);
      playerId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, exitId);
      exitId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, numericTileProtoId);
      numericTileProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, mineProtoId);
      mineProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, soilProtoId);
      soilProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, coinProtoId);
      coinProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, nuggetProtoId);
      nuggetProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, throwableProtoId);
      throwableProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, zombieProtoId);
      zombieProtoId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, timeCounterId);
      timeCounterId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, scoreCounterId);
      scoreCounterId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, txtRestartId);
      txtRestartId = node.getLong();

      node = node.nextSibling();
      XML_NODE_CHECK(node, gameModes);
      parseGameModes(node);
   }
   catch (XmlException& e) {
      e.prepend("Error loading game settings; ");
      throw;
   }
}

//===========================================
// GameSettings::parseGameModes
//===========================================
void GameSettings::parseGameModes(XmlNode data) {
   XmlNode node = data.firstChild();

   while (!node.isNull() && node.name() == "GameOptions") {
      difficultyModes.push_back(pGameOptions_t(new GameOptions(node)));
      node = node.nextSibling();
   }
}

//===========================================
// GameSettings::clone
//===========================================
Asset* GameSettings::clone() const {
   return new GameSettings(*this);
}

//===========================================
// GameSettings::getSize
//===========================================
size_t GameSettings::getSize() const {
   return sizeof(GameSettings);
}
