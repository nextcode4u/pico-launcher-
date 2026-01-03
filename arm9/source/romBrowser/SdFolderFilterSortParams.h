#pragma once
#include "SdFolderSortType.h"
#include "SdFolderSortDirection.h"

class SdFolderFilterSortParams
{
public:
    SdFolderSortType sortType = SdFolderSortType::Name;
    SdFolderSortDirection sortDirection = SdFolderSortDirection::Ascending;
    bool hideFolders = false;

    SdFolderFilterSortParams() { }

    SdFolderFilterSortParams(SdFolderSortType sortType, SdFolderSortDirection sortDirection)
        : sortType(sortType), sortDirection(sortDirection) { }

    SdFolderFilterSortParams(SdFolderSortType sortType, SdFolderSortDirection sortDirection, bool hideFolders)
        : sortType(sortType), sortDirection(sortDirection), hideFolders(hideFolders) { }
};
