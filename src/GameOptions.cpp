#include "GameOptions.hpp"


using namespace Dodge;


GameOptions::GameOptions(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, GameOptions);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, mines);
      mines = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, coins);
      coins = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, nuggets);
      nuggets = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, totalGold);
      totalGold = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, throwables);
      throwables = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, zombies);
      zombies = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, requiredGold);
      requiredGold = node.getInt();

      node = node.nextSibling();
      XML_NODE_CHECK(node, timeLimit);
      timeLimit = node.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class GameOptions");
      throw;
   }
}
