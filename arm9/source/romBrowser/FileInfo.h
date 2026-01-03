#pragma once
#include "fat/ff.h"
#include "fat/FastFileRef.h"
#include "FileType/FileType.h"

class InternalFileInfo;

class FileInfo
{
public:
    FileInfo() { }
    FileInfo(const FileInfo& fileInfo);
    FileInfo(const TCHAR* fileName, const FileType* type, const FastFileRef& fastFileRef, bool hidden);

    FileInfo &operator=(FileInfo&& rhs)
    {
        if (this != &rhs)
        {
            _name = std::move(rhs._name);
            _type = rhs._type;
            _fastFileRef = rhs._fastFileRef;
            _hidden = rhs._hidden;
        }

        return *this;
    }

    const TCHAR* GetFileName() const { return _name.get(); }
    const FileType* GetFileType() const { return _type; }
    u32 GetFileSize() const { return _fastFileRef.GetFileSize(); }
    bool IsHidden() const { return _hidden; }

    InternalFileInfo* CreateInternalFileInfo() const
    {
        return _type->CreateInternalFileInfo(_fastFileRef);
    }

    const FastFileRef& GetFastFileRef() const { return _fastFileRef; }

private:
    std::unique_ptr<TCHAR[]> _name;
    const FileType* _type;
    FastFileRef _fastFileRef;
    bool _hidden = false;
};
