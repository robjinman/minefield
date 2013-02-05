/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <dodge/dodge.hpp>
#include "GameData.hpp"
#include "GameLogic.hpp"


class Application {
   public:
      Application();

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void launch(int argc, char** argv);
      void quit() throw();

   private:
      void quitReqHandler(Dodge::EEvent* event);
      void deletePending(Dodge::EEvent* event);
      void deleteAsset(Dodge::pAsset_t asset);
      Dodge::pAsset_t constructAsset(const Dodge::XmlNode data);
      void setMapSettings(const Dodge::XmlNode data);
      void populateMap(Dodge::EEvent* event = NULL);
      void freeAllAssets();
      bool isAdjacentTo(const Dodge::Vec2i& a, const Dodge::Vec2i& b) const;
      void loadAssets();
      void parseGameModes(Dodge::XmlNode data);
      void keyDown(int key);

      void exitDefault();

      bool m_init;

      Functor<void, TYPELIST_0()>   m_onExit;
      Dodge::AssetManager           m_assetManager;
      Dodge::WorldSpace             m_worldSpace;
      Dodge::MapLoader&             m_mapLoader;
      Dodge::Renderer&              m_renderer;
      Dodge::Audio                  m_audio;
      Dodge::WinIO                  m_win;
      Dodge::EventManager           m_eventManager;

      long                          m_gameSettingsId;

      GameData                      m_data;
      GameLogic                     m_logic;
};


#endif
