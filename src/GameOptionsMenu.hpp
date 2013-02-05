#ifndef __GAME_OPTIONS_MENU_HPP__
#define __GAME_OPTIONS_MENU_HPP__


#include <vector>
#include <dodge/TextEntity.hpp>
#include "GameOptions.hpp"
#include "Menu.hpp"
#include "ProgressBar.hpp"


class GameOptionsMenu : public Menu {
   public:
      GameOptionsMenu(const Dodge::XmlNode data);
      GameOptionsMenu(const GameOptionsMenu& copy);
      GameOptionsMenu(const GameOptionsMenu& copy, long name);

      void setDifficultyModes(const std::vector<pGameOptions_t>& modes);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual Asset* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~GameOptionsMenu();

   private:
      void init();
      void updateText();

      virtual void onMenuItemActivate(pMenuItem_t item);
      virtual void onMenuItemGainFocus(pMenuItem_t item);
      virtual void onMenuItemLoseFocus(pMenuItem_t item);

      void decreaseDifficulty(Dodge::pEntity_t);
      void increaseDifficulty(Dodge::pEntity_t);

      Dodge::pUiButton_t m_difficultyDownBtn;
      Dodge::pUiButton_t m_difficultyUpBtn;

      std::vector<pGameOptions_t> m_difficultyModes;
      unsigned int m_currentMode;

      std::unique_ptr<ProgressBar> m_slider;

      Dodge::pFont_t m_font;

      Dodge::pTextEntity_t m_txtMines;
      Dodge::pTextEntity_t m_txtGold;
      Dodge::pTextEntity_t m_txtThrowables;
      Dodge::pTextEntity_t m_txtZombies;
      Dodge::pTextEntity_t m_txtReqScore;
      Dodge::pTextEntity_t m_txtTime;
      Dodge::pTextEntity_t m_txtBestTime;
};

typedef boost::shared_ptr<GameOptionsMenu> pGameOptionsMenu_t;


#endif
