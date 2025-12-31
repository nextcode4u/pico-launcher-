#pragma once
#include "FileType.h"
#include "services/settings/FileAssociation.h"
#include "../Theme/IThemeFileIconFactory.h"
#include "fat/File.h"
#include <string.h>

static inline bool writeTextFile(const char* path, const char* text)
{
    if (!path || !path[0])
        return false;

    File f;
    if (f.Open(path, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
        return false;

    u32 written = 0;
    const u32 len = (u32)strlen(text);
    if (f.Write(text, len, written) != FR_OK)
        return false;

    f.Sync();
    return written == len;
}

/// @brief Class representing a custom (user provided) file type.
class CustomFileType : public FileType
{
public:
    CustomFileType()
        : FileType(nullptr, FileTypeClassification::Unknown) { }

    explicit CustomFileType(const FileAssociation* fileAssociation)
        : CustomFileType(fileAssociation, nullptr) { }

    CustomFileType(const FileAssociation* fileAssociation, const FileType* baseFileType)
        : FileType(
            baseFileType != nullptr ? baseFileType->GetShortName() : fileAssociation->extension.GetString(),
            baseFileType != nullptr ? baseFileType->GetClassification() : FileTypeClassification::Misc)
        , _fileAssociation(fileAssociation), _baseFileType(baseFileType) { }

    std::unique_ptr<FileIcon> CreateFileIcon(const TCHAR* fileName,
        const IThemeFileIconFactory* themeFileIconFactory) const override
    {
        return _baseFileType != nullptr
            ? _baseFileType->CreateFileIcon(fileName, themeFileIconFactory)
            : themeFileIconFactory->CreateGenericFileIcon(fileName);
    }

    FileCover* CreateFileCover(const TCHAR* fileName) const override
    {
        return _baseFileType != nullptr
            ? _baseFileType->CreateFileCover(fileName)
            : FileType::CreateFileCover(fileName);
    };

    bool HasInternalFileInfo() const override
    {
        return _baseFileType != nullptr
            && _baseFileType->HasInternalFileInfo();
    }

    InternalFileInfo* CreateInternalFileInfo(const FastFileRef& fastFileRef) const override
    {
        return _baseFileType != nullptr
            ? _baseFileType->CreateInternalFileInfo(fastFileRef)
            : nullptr;
    }

    bool TrySetLaunchParameters(pload_params_t* launchParameters, const char* filePath) const override
    {
        // The "application" for a file association is always the .nds we boot.
        StringUtil::Copy(launchParameters->romPath, _fileAssociation->applicationPath, sizeof(launchParameters->romPath));

        if (_fileAssociation->method == FILE_ASSOC_METHOD_PATHFILE)
        {
            // Method A: pathfile handoff
            // Write the selected ROM path into a text file the emulator can read on boot.
            // Then launch the emulator without argv.
            if (!writeTextFile(_fileAssociation->pathFile.GetString(), filePath))
                return false;

            launchParameters->arguments[0] = 0;
            launchParameters->argumentsLength = 0;
            return true;
        }

        // Method B: argv handoff
        u32 length = StringUtil::Copy(launchParameters->arguments, filePath, sizeof(launchParameters->arguments));
        launchParameters->argumentsLength = length + 1;
        return true;
    }

private:
    const FileAssociation* _fileAssociation;
    const FileType* _baseFileType = nullptr;
};
