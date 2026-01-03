#pragma once
#include "../IRomBrowserController.h"
#include "services/settings/RomBrowserDisplaySettings.h"

/// @brief View model for the display settings screen.
class DisplaySettingsViewModel
{
public:
    explicit DisplaySettingsViewModel(IRomBrowserController* romBrowserController)
        : _romBrowserController(romBrowserController)
        , _romBrowserDisplaySettings(_romBrowserController->GetRomBrowserDisplaySettings()) { }

    constexpr RomBrowserLayout GetRomBrowserDisplayMode() const
    {
        return _romBrowserDisplaySettings.layout;
    }

    void SetRomBrowserDisplayMode(RomBrowserLayout romBrowserDisplayMode)
    {
        if (_romBrowserDisplaySettings.layout != romBrowserDisplayMode)
        {
            _romBrowserDisplaySettings.layout = romBrowserDisplayMode;
            _romBrowserController->SetRomBrowserDisplaySettings(_romBrowserDisplaySettings);
        }
    }

    constexpr RomBrowserSortMode GetRomBrowserSortMode() const
    {
        return _romBrowserDisplaySettings.sortMode;
    }

    void SetRomBrowserSortMode(RomBrowserSortMode romBrowserSortMode)
    {
        if (_romBrowserDisplaySettings.sortMode != romBrowserSortMode)
        {
            _romBrowserDisplaySettings.sortMode = romBrowserSortMode;
            _romBrowserController->SetRomBrowserDisplaySettings(_romBrowserDisplaySettings);
        }
    }

    constexpr bool GetDarkTheme() const
    {
        return _romBrowserDisplaySettings.darkTheme;
    }

    void SetDarkTheme(bool darkTheme)
    {
        if (_romBrowserDisplaySettings.darkTheme != darkTheme)
        {
            _romBrowserDisplaySettings.darkTheme = darkTheme;
            _romBrowserController->SetRomBrowserDisplaySettings(_romBrowserDisplaySettings);
        }
    }

    constexpr bool GetHideFolders() const
    {
        return _romBrowserDisplaySettings.hideFolders;
    }

    void SetHideFolders(bool hideFolders)
    {
        if (_romBrowserDisplaySettings.hideFolders != hideFolders)
        {
            _romBrowserDisplaySettings.hideFolders = hideFolders;
            _romBrowserController->SetRomBrowserDisplaySettings(_romBrowserDisplaySettings);
        }
    }

    void Close()
    {
        _romBrowserController->HideDisplaySettings();
    }

private:
    IRomBrowserController* _romBrowserController;
    RomBrowserDisplaySettings _romBrowserDisplaySettings;
};
