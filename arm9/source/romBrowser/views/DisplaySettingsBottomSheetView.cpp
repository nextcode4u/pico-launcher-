#include "common.h"
#include "gui/GraphicsContext.h"
#include "gui/VramContext.h"
#include "gui/IVramManager.h"
#include "hGridIcon.h"
#include "vGridIcon.h"
#include "bannerListIcon.h"
#include "listIcon.h"
#include "sortNameAscendingIcon.h"
#include "sortNameDescendingIcon.h"
#include "iconButtonSelector.h"
#include "largeFolderIcon.h"
#include "coverflowIcon.h"
#include "../IRomBrowserController.h"
#include "gui/input/InputProvider.h"
#include "themes/material/MaterialColorScheme.h"
#include "themes/IFontRepository.h"
#include "DisplaySettingsBottomSheetView.h"

#define TITLE_LABEL_X       20
#define TITLE_LABEL_Y       10

#define LAYOUT_LABEL_X      20
#define LAYOUT_LABEL_Y      36

#define SORTING_LABEL_X     20
#define SORTING_LABEL_Y     66

#define APPEARANCE_LABEL_X  20
#define APPEARANCE_LABEL_Y  96

#define FILTERS_LABEL_X     20
#define FILTERS_LABEL_Y     126

#define ICON_START_X        120
#define ICON_SPACING        34

static RomBrowserLayout sRomBrowserDisplayModes[4] =
{
    [0] = RomBrowserLayout::HorizontalIconGrid,
    [1] = RomBrowserLayout::VerticalIconGrid,
    [2] = RomBrowserLayout::BannerList,
    [3] = RomBrowserLayout::CoverFlow
};

static RomBrowserSortMode sRomBrowserSortModes[4] =
{
    [0] = RomBrowserSortMode::NameAscending,
    [1] = RomBrowserSortMode::NameDescending,
    [2] = RomBrowserSortMode::LastModified
};

DisplaySettingsBottomSheetView::DisplaySettingsBottomSheetView(
    DisplaySettingsViewModel* viewModel, const MaterialColorScheme* materialColorScheme,
    const IFontRepository* fontRepository)
    : _viewModel(viewModel)
    , _titleLabel(128, 16, 25, fontRepository->GetFont(FontType::Medium11))
    , _layoutLabel(64, 16, 25, fontRepository->GetFont(FontType::Regular10))
    , _sortingLabel(64, 16, 25, fontRepository->GetFont(FontType::Regular10))
    , _appearanceLabel(64, 16, 25, fontRepository->GetFont(FontType::Regular10))
    , _filtersLabel(64, 16, 25, fontRepository->GetFont(FontType::Regular10))
    , _materialColorScheme(materialColorScheme)
{
    _titleLabel.SetText(u"Display Settings");
    AddChildTail(&_titleLabel);
    _layoutLabel.SetText(u"Layout");
    AddChildTail(&_layoutLabel);
    _sortingLabel.SetText(u"Sorting");
    AddChildTail(&_sortingLabel);
    _appearanceLabel.SetText(u"Appearance");
    AddChildTail(&_appearanceLabel);
    _filtersLabel.SetText(u"Hidden Items");
    AddChildTail(&_filtersLabel);

    for (auto& layoutOption : _layoutOptions)
    {
        layoutOption = CreateLayoutOptionIconButton();
        AddChildTail(&layoutOption);
    }

    for (auto& sortOption : _sortOptions)
    {
        sortOption = CreateSortOptionIconButton();
        AddChildTail(&sortOption);
    }

    for (auto& appearanceOption : _appearanceOptions)
    {
        appearanceOption = CreateAppearanceOptionIconButton();
        AddChildTail(&appearanceOption);
    }

    for (auto& filterOption : _filterOptions)
    {
        filterOption = CreateFilterOptionIconButton();
        AddChildTail(&filterOption);
    }
}

IconButton2DView DisplaySettingsBottomSheetView::CreateLayoutOptionIconButton()
{
    IconButton2DView layoutOption
    {
        IconButtonView::Type::Tonal,
        IconButtonView::State::ToggleUnselected,
        md::sys::color::surfaceContainerLow,
        _materialColorScheme
    };
    layoutOption.SetAction([] (IconButtonView* sender, void* arg)
    {
        auto self = reinterpret_cast<DisplaySettingsBottomSheetView*>(arg);
        u32 idx = ((IconButton2DView*)sender) - &self->_layoutOptions[0];
        self->_viewModel->SetRomBrowserDisplayMode(sRomBrowserDisplayModes[idx]);
    }, this);
    return layoutOption;
}

IconButton2DView DisplaySettingsBottomSheetView::CreateSortOptionIconButton()
{
    IconButton2DView sortOption
    {
        IconButtonView::Type::Tonal,
        IconButtonView::State::ToggleUnselected,
        md::sys::color::surfaceContainerLow,
        _materialColorScheme
    };
    sortOption.SetAction([] (IconButtonView* sender, void* arg)
    {
        auto self = reinterpret_cast<DisplaySettingsBottomSheetView*>(arg);
        u32 idx = ((IconButton2DView*)sender) - &self->_sortOptions[0];
        self->_viewModel->SetRomBrowserSortMode(sRomBrowserSortModes[idx]);
    }, this);
    return sortOption;
}

IconButton2DView DisplaySettingsBottomSheetView::CreateAppearanceOptionIconButton()
{
    IconButton2DView appearanceOption
    {
        IconButtonView::Type::Tonal,
        IconButtonView::State::ToggleUnselected,
        md::sys::color::surfaceContainerLow,
        _materialColorScheme
    };
    appearanceOption.SetAction([] (IconButtonView* sender, void* arg)
    {
        auto self = reinterpret_cast<DisplaySettingsBottomSheetView*>(arg);
        bool nextDark = !self->_viewModel->GetDarkTheme();
        self->_viewModel->SetDarkTheme(nextDark);
    }, this);
    return appearanceOption;
}

IconButton2DView DisplaySettingsBottomSheetView::CreateFilterOptionIconButton()
{
    IconButton2DView filterOption
    {
        IconButtonView::Type::Tonal,
        IconButtonView::State::ToggleUnselected,
        md::sys::color::surfaceContainerLow,
        _materialColorScheme
    };
    filterOption.SetAction([] (IconButtonView* sender, void* arg)
    {
        auto self = reinterpret_cast<DisplaySettingsBottomSheetView*>(arg);
        self->_viewModel->SetHideFolders(!self->_viewModel->GetHideFolders());
    }, this);
    return filterOption;
}

void DisplaySettingsBottomSheetView::InitVram(const VramContext& vramContext)
{
    BottomSheetView::InitVram(vramContext);

    const auto objVramManager = vramContext.GetObjVramManager();
    if (objVramManager)
    {
        // layout options
        _layoutOptions[0].SetIconVramOffset(LoadIcon(*objVramManager, hGridIconTiles, hGridIconTilesLen));
        _layoutOptions[1].SetIconVramOffset(LoadIcon(*objVramManager, vGridIconTiles, vGridIconTilesLen));
        _layoutOptions[2].SetIconVramOffset(LoadIcon(*objVramManager, bannerListIconTiles, bannerListIconTilesLen));
        _layoutOptions[3].SetIconVramOffset(LoadIcon(*objVramManager, coverflowIconTiles, coverflowIconTilesLen));

        // sort options
        _sortOptions[0].SetIconVramOffset(LoadIcon(*objVramManager, sortNameAscendingIconTiles, sortNameAscendingIconTilesLen));
        _sortOptions[1].SetIconVramOffset(LoadIcon(*objVramManager, sortNameDescendingIconTiles, sortNameDescendingIconTilesLen));
        // _sortOptions[2].SetIconVramOffset(LoadIcon(objVramManager, recentIconTiles, recentIconTilesLen));

        // appearance options
        _appearanceOptions[0].SetIconVramOffset(LoadIcon(*objVramManager, iconButtonSelectorTiles, iconButtonSelectorTilesLen));

        // filter options
        _filterOptions[0].SetIconVramOffset(LoadIcon(*objVramManager, largeFolderIconTiles, largeFolderIconTilesLen));
    }
}

void DisplaySettingsBottomSheetView::UpdateLabels()
{
    _titleLabel.SetPosition(TITLE_LABEL_X, _position.y + TITLE_LABEL_Y);
    _layoutLabel.SetPosition(LAYOUT_LABEL_X, _position.y + LAYOUT_LABEL_Y);
    _sortingLabel.SetPosition(SORTING_LABEL_X, _position.y + SORTING_LABEL_Y);
    _appearanceLabel.SetPosition(APPEARANCE_LABEL_X, _position.y + APPEARANCE_LABEL_Y);
    _filtersLabel.SetPosition(FILTERS_LABEL_X, _position.y + FILTERS_LABEL_Y);
}

void DisplaySettingsBottomSheetView::Update()
{
    BottomSheetView::Update();
    UpdateLabels();
    auto selectedDisplayMode = _viewModel->GetRomBrowserDisplayMode();
    int x = ICON_START_X;
    u32 idx = 0;
    for (auto& layoutOption : _layoutOptions)
    {
        layoutOption.SetPosition(x, _position.y + (LAYOUT_LABEL_Y - 10));
        layoutOption.SetState(sRomBrowserDisplayModes[idx] == selectedDisplayMode
            ? IconButtonView::State::ToggleSelected
            : IconButtonView::State::ToggleUnselected);
        x += ICON_SPACING;
        idx++;
    }
    auto selectedSortMode = _viewModel->GetRomBrowserSortMode();
    x = ICON_START_X;
    idx = 0;
    for (auto& sortOption : _sortOptions)
    {
        sortOption.SetPosition(x, _position.y + (SORTING_LABEL_Y - 8));
        sortOption.SetState(sRomBrowserSortModes[idx] == selectedSortMode
            ? IconButtonView::State::ToggleSelected
            : IconButtonView::State::ToggleUnselected);
        x += ICON_SPACING;
        idx++;
    }
    x = ICON_START_X;
    auto isDarkTheme = _viewModel->GetDarkTheme();
    idx = 0;
    for (auto& appearanceOption : _appearanceOptions)
    {
        appearanceOption.SetPosition(x, _position.y + (APPEARANCE_LABEL_Y - 8));
        appearanceOption.SetState(isDarkTheme
            ? IconButtonView::State::ToggleSelected
            : IconButtonView::State::ToggleUnselected);
        x += ICON_SPACING;
        idx++;
    }
    x = ICON_START_X;
    auto hideFoldersEnabled = _viewModel->GetHideFolders();
    for (auto& filterOption : _filterOptions)
    {
        filterOption.SetPosition(x, _position.y + (FILTERS_LABEL_Y - 8));
        filterOption.SetState(hideFoldersEnabled
            ? IconButtonView::State::ToggleSelected
            : IconButtonView::State::ToggleUnselected);
        x += ICON_SPACING;
    }

}

void DisplaySettingsBottomSheetView::Draw(GraphicsContext& graphicsContext)
{
    graphicsContext.SetClipArea(GetBounds());
    u32 oldPrio = graphicsContext.SetPriority(1);
    {
        _titleLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _titleLabel.SetForegroundColor(_materialColorScheme->onSurface);
        _layoutLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _layoutLabel.SetForegroundColor(_materialColorScheme->onSurfaceVariant);
        _sortingLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _sortingLabel.SetForegroundColor(_materialColorScheme->onSurfaceVariant);
        _appearanceLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _appearanceLabel.SetForegroundColor(_materialColorScheme->onSurfaceVariant);
        _filtersLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _filtersLabel.SetForegroundColor(_materialColorScheme->onSurfaceVariant);
        BottomSheetView::Draw(graphicsContext);
    }
    graphicsContext.SetPriority(oldPrio);
    graphicsContext.ResetClipArea();
}

bool DisplaySettingsBottomSheetView::HandleInput(
    const InputProvider& inputProvider, FocusManager& focusManager)
{
    if (inputProvider.Triggered(InputKey::B))
    {
        _viewModel->Close();
        return true;
    }
    return false;
}

View* DisplaySettingsBottomSheetView::MoveFocus(View* currentFocus,
    FocusMoveDirection direction, View* source)
{
    int idx = 0;
    for (auto& layoutOption : _layoutOptions)
    {
        if (currentFocus == &layoutOption)
        {
            if (direction == FocusMoveDirection::Left)
            {
                if (--idx < 0)
                    idx += _layoutOptions.size();
                return &_layoutOptions[idx];
            }
            else if (direction == FocusMoveDirection::Right)
            {
                if (++idx >= (int)_layoutOptions.size())
                    idx = 0;
                return &_layoutOptions[idx];
            }
            // else if (direction == FocusMoveDirection::Up)
            // {
            //     if (idx >= (int)_filterOptions.size())
            //         idx = _filterOptions.size() - 1;
            //     return &_filterOptions[idx];
            // }
            else //if (direction == FocusMoveDirection::Down)
            {
                if (idx >= (int)_sortOptions.size())
                    idx = _sortOptions.size() - 1;
                return &_sortOptions[idx];
            }
        }
        idx++;
    }
    idx = 0;
    for (auto& sortOption : _sortOptions)
    {
        if (currentFocus == &sortOption)
        {
            if (direction == FocusMoveDirection::Left)
            {
                if (--idx < 0)
                    idx += _sortOptions.size();
                return &_sortOptions[idx];
            }
            else if (direction == FocusMoveDirection::Right)
            {
                if (++idx >= (int)_sortOptions.size())
                    idx = 0;
                return &_sortOptions[idx];
            }
            else if (direction == FocusMoveDirection::Up)
            {
                if (idx >= (int)_layoutOptions.size())
                    idx = _layoutOptions.size() - 1;
                return &_layoutOptions[idx];
            }
            else if (direction == FocusMoveDirection::Down)
            {
                if (idx >= (int)_appearanceOptions.size())
                    idx = _appearanceOptions.size() - 1;
                return &_appearanceOptions[idx];
            }
        }
        idx++;
    }
    idx = 0;
    for (auto& appearanceOption : _appearanceOptions)
    {
        if (currentFocus == &appearanceOption)
        {
            if (direction == FocusMoveDirection::Left)
            {
                if (--idx < 0)
                    idx += _appearanceOptions.size();
                return &_appearanceOptions[idx];
            }
            else if (direction == FocusMoveDirection::Right)
            {
                if (++idx >= (int)_appearanceOptions.size())
                    idx = 0;
                return &_appearanceOptions[idx];
            }
            else if (direction == FocusMoveDirection::Up)
            {
                if (idx >= (int)_sortOptions.size())
                    idx = _sortOptions.size() - 1;
                return &_sortOptions[idx];
            }
            else if (direction == FocusMoveDirection::Down)
            {
                if (idx >= (int)_filterOptions.size())
                    idx = _filterOptions.size() - 1;
                return &_filterOptions[idx];
            }
        }
        idx++;
    }
    idx = 0;
    for (auto& filterOption : _filterOptions)
    {
        if (currentFocus == &filterOption)
        {
            if (direction == FocusMoveDirection::Left)
            {
                if (--idx < 0)
                    idx += _filterOptions.size();
                return &_filterOptions[idx];
            }
            else if (direction == FocusMoveDirection::Right)
            {
                if (++idx >= (int)_filterOptions.size())
                    idx = 0;
                return &_filterOptions[idx];
            }
            else if (direction == FocusMoveDirection::Up)
            {
                if (idx >= (int)_appearanceOptions.size())
                    idx = _appearanceOptions.size() - 1;
                return &_appearanceOptions[idx];
            }
            else if (direction == FocusMoveDirection::Down)
            {
                if (idx >= (int)_layoutOptions.size())
                    idx = _layoutOptions.size() - 1;
                return &_layoutOptions[idx];
            }
        }
        idx++;
    }
    return nullptr;
}

void DisplaySettingsBottomSheetView::SetGraphics(
    const IconButton2DView::VramToken& iconButtonVramToken)
{
    for (auto& layoutOption : _layoutOptions)
        layoutOption.SetGraphics(iconButtonVramToken);
    for (auto& sortOption : _sortOptions)
        sortOption.SetGraphics(iconButtonVramToken);
    for (auto& appearanceOption : _appearanceOptions)
        appearanceOption.SetGraphics(iconButtonVramToken);
    for (auto& filterOption : _filterOptions)
        filterOption.SetGraphics(iconButtonVramToken);
}

u32 DisplaySettingsBottomSheetView::LoadIcon(IVramManager& vramManager,
    const unsigned int* tiles, u32 tilesLength) const
{
    u32 vramOffset = vramManager.Alloc(tilesLength);
    dma_ntrCopy32(3, tiles, vramManager.GetVramAddress(vramOffset), tilesLength);
    return vramOffset;
}
