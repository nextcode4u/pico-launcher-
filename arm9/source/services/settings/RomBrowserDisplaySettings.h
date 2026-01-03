#pragma once
#include "RomBrowserLayout.h"
#include "RomBrowserSortMode.h"

class RomBrowserDisplaySettings
{
public:
    bool hideFolders = false;
    RomBrowserLayout layout = RomBrowserLayout::HorizontalIconGrid;
    RomBrowserSortMode sortMode = RomBrowserSortMode::NameAscending;
};
