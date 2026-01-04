#pragma once
#include <memory>
#include "core/String.h"
#include "RomBrowserDisplaySettings.h"
#include "FileAssociation.h"

struct ThemeDarkModeSetting
{
    String<char, 64> themeName;
    bool darkMode = false;
};

class AppSettings
{
public:
    String<char, 16> language = "english";
    String<char, 64> theme = "material";
    String<char, 256> lastUsedFilePath = "";
    RomBrowserDisplaySettings romBrowserDisplaySettings;
    bool showHiddenItemsToggle = false;

    std::unique_ptr<FileAssociation[]> fileAssociations;
    u32 numberOfFileAssociations = 0;

    std::unique_ptr<ThemeDarkModeSetting[]> themeDarkModes;
    u32 numberOfThemeDarkModes = 0;

    bool TryGetThemeDarkMode(const char* themeName, bool& darkModeOut) const
    {
        for (u32 i = 0; i < numberOfThemeDarkModes; i++)
        {
            if (strcmp(themeDarkModes[i].themeName.GetString(), themeName) == 0)
            {
                darkModeOut = themeDarkModes[i].darkMode;
                return true;
            }
        }
        return false;
    }

    void SetThemeDarkMode(const char* themeName, bool darkMode)
    {
        for (u32 i = 0; i < numberOfThemeDarkModes; i++)
        {
            if (strcmp(themeDarkModes[i].themeName.GetString(), themeName) == 0)
            {
                themeDarkModes[i].darkMode = darkMode;
                return;
            }
        }

        auto newModes = std::make_unique_for_overwrite<ThemeDarkModeSetting[]>(numberOfThemeDarkModes + 1);
        for (u32 i = 0; i < numberOfThemeDarkModes; i++)
        {
            newModes[i] = themeDarkModes[i];
        }
        newModes[numberOfThemeDarkModes].themeName = themeName;
        newModes[numberOfThemeDarkModes].darkMode = darkMode;
        themeDarkModes = std::move(newModes);
        numberOfThemeDarkModes++;
    }
};
