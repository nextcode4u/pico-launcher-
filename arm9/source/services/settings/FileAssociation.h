#pragma once
#include "core/String.h"

/// @brief How to hand off the selected ROM to the target application.
///
/// - Argv: pico-launcher passes the ROM path in pload_params_t::arguments (argv[1]).
/// - PathFile: pico-launcher writes the ROM path to a text file before launching the app.
typedef enum
{
    FILE_ASSOC_METHOD_ARGV = 0,
    FILE_ASSOC_METHOD_PATHFILE = 1,
} FileAssociationMethod;

class FileAssociation
{
public:
    FileAssociation() { }

    FileAssociation(const char* extension, const char* applicationPath)
        : extension(extension), applicationPath(applicationPath) { }

    FileAssociation(const char* extension, const char* applicationPath, FileAssociationMethod method, const char* pathFile)
        : extension(extension), applicationPath(applicationPath), method(method), pathFile(pathFile) { }

    String<char, 8> extension;
    String<char, 256> applicationPath;

    /// @brief ROM handoff method (defaults to argv for backwards compatibility).
    FileAssociationMethod method = FILE_ASSOC_METHOD_ARGV;

    /// @brief Path to a text file that will receive the ROM path (only used when method==PathFile).
    String<char, 256> pathFile = "";
};
