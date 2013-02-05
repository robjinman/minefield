#ifndef __SETTINGS_MENU_HPP__
#define __SETTINGS_MENU_HPP__


#include "Menu.hpp"
#include "ProgressBar.hpp"


class SettingsMenu : public Menu {
   public:
      SettingsMenu(const Dodge::XmlNode data);
      SettingsMenu(const SettingsMenu& copy);
      SettingsMenu(const SettingsMenu& copy, long name);

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
      virtual ~SettingsMenu();

   private:
      void init();

      void increaseMusicVolume();
      void decreaseMusicVolume();
      void increaseSfxVolume();
      void decreaseSfxVolume();

      virtual void onMenuItemActivate(pMenuItem_t item);
      virtual void onMenuItemGainFocus(pMenuItem_t item);
      virtual void onMenuItemLoseFocus(pMenuItem_t item);

      Dodge::pUiButton_t m_musicVolDownBtn;
      Dodge::pUiButton_t m_musicVolUpBtn;
      Dodge::pUiButton_t m_sfxVolDownBtn;
      Dodge::pUiButton_t m_sfxVolUpBtn;

      std::unique_ptr<ProgressBar> m_musicProgBar;
      std::unique_ptr<ProgressBar> m_sfxProgBar;
};

typedef boost::shared_ptr<SettingsMenu> pSettingsMenu_t;


#endif
