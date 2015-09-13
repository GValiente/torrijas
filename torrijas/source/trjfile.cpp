//
// Copyright (c) 2015 Gustavo Valiente gustavovalient@gmail.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "trjfile.h"

#include <algorithm>
#include <fstream>
#include "trjexception.h"
#include "trjdebug.h"

#ifdef _WIN32
	#include <Shlobj.h>
	#include <direct.h>

    #ifndef S_ISDIR
        #define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
    #endif
#else
    #include <cstring>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <dirent.h>
#endif

namespace trj
{

namespace
{
    String getNameImpl(const String& path)
    {
        String name;
        const char* nameCharArray = strrchr(path.getCharArray(), '/');
        if(nameCharArray)
        {
            ++nameCharArray;
            name = std::string(nameCharArray);
        }
        else
        {
            name = path;
        }

        return name;
    }

    Folder getParentFolderImpl(const String& path)
    {
        const char* pathCharArray = path.getCharArray();
        const char* name = strrchr(pathCharArray, '/');
        if(name)
        {
            std::string folderPath(pathCharArray, name - pathCharArray);
            return Folder(std::move(folderPath));
        }

        return Folder::getCurrentFolder();
    }

    String getAbsolutePathImpl(const String& path)
    {
        #ifdef _WIN32
            char absolutePathCharArray[MAX_PATH];
            if(! GetFullPathName(path.getCharArray(), MAX_PATH, absolutePathCharArray, NULL))
            {
                throw trj::Exception(__FILE__, __LINE__, "GetFullPathName failed");
            }

            std::string absolutePath(absolutePathCharArray);
            std::replace(absolutePath.begin(), absolutePath.end(), '\\', '/');
            return absolutePath;
        #else
            char* absolutePathCharArray = realpath(path.getCharArray(), nullptr);
            if(! absolutePathCharArray)
            {
                throw trj::Exception(__FILE__, __LINE__, "realpath failed");
            }

            std::string absolutePath(absolutePathCharArray);
            free(absolutePathCharArray);
            return absolutePath;
        #endif
    }
}

bool File::isFilePath(const String& path)
{
    struct stat pathStat;
    if(stat(path.getCharArray(), &pathStat))
    {
        return false;
    }

    return ! S_ISDIR(pathStat.st_mode);
}

File::File(String path) :
    mPath(std::move(path))
{
    TRJ_ASSERT(! mPath.isEmpty(), "Path is empty");
    TRJ_ASSERT(! Folder::isFolderPath(mPath), "Path is from a folder");
}

File::File(const Folder& folder, const String& name)
{
    TRJ_ASSERT(! name.isEmpty(), "Name is empty");

    std::string& stdString = mPath.getStdString();
    stdString += folder.getPath().getCharArray();
    stdString += name.getCharArray();

    TRJ_ASSERT(! Folder::isFolderPath(mPath), "Path is from a folder");
}
    
String File::getName() const
{
    return getNameImpl(mPath);
}
    
String File::getNameWithoutExtension() const
{
    String name = getName();
    const char* nameCharArray = name.getCharArray();
    const char* nameWithoutExtension = strchr(nameCharArray, '.');
    if(nameWithoutExtension)
    {
        name = std::string(nameCharArray, nameWithoutExtension - nameCharArray);
    }
    
    return name;
}
    
String File::getExtension() const
{
    String extension;
    const char* extensionCharArray = strrchr(mPath.getCharArray(), '.');
    if(extensionCharArray)
    {
        ++extensionCharArray;
        extension = std::string(extensionCharArray);
    }

    return extension;
}
    
Folder File::getParentFolder() const
{
    return getParentFolderImpl(mPath);
}

String File::getAbsolutePath() const
{
    return getAbsolutePathImpl(mPath);
}

bool File::exists() const
{
    std::ifstream fileStream(mPath.getCharArray());
    return fileStream.good();
}

bool File::remove() const
{
    int returnCode = ::remove(mPath.getCharArray());
    return (returnCode == 0);
}
    
String File::getContent() const
{
    std::ifstream fileStream(mPath.getCharArray());
    String output;
    if(fileStream.good())
    {
        output = std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    }

    return output;
}

Folder Folder::getCurrentFolder()
{
    #ifdef _WIN32
        char* currentPathCharArray = _getcwd(nullptr, 0);
    #else
        char* currentPathCharArray = getcwd(nullptr, 0);
    #endif

    if(! currentPathCharArray)
    {
        throw Exception(__FILE__, __LINE__, "getCurrentFolder failed");
    }

    std::string currentPath(currentPathCharArray);
    free(currentPathCharArray);

    #ifdef _WIN32
        std::replace(currentPath.begin(), currentPath.end(), '\\', '/');
    #endif

    return Folder(currentPath);
}

bool Folder::isFolderPath(const String& path)
{
    struct stat pathStat;
    if(stat(path.getCharArray(), &pathStat))
    {
        return false;
    }

    return S_ISDIR(pathStat.st_mode);
}
    
Folder::Folder(String path) :
    mPath(std::move(path))
{
    TRJ_ASSERT(! mPath.isEmpty(), "Path is empty");
    TRJ_ASSERT(! File::isFilePath(mPath), "Path is from a file");

    if(! mPath.endsWith('/'))
	{
        mPath.getStdString() += '/';
    }
}

Folder::Folder(const Folder& folder, const String& name)
{
    TRJ_ASSERT(! name.isEmpty(), "Name is empty");

    std::string& stdString = mPath.getStdString();
    stdString += folder.getPath().getCharArray();
    stdString += name.getCharArray();

    TRJ_ASSERT(! File::isFilePath(mPath), "Path is from a file");

    if(! mPath.endsWith('/'))
    {
        stdString += '/';
    }
}

String Folder::getName() const
{
    std::string path = mPath.getCharArray();
    path.pop_back();

    return getNameImpl(path);
}

Folder Folder::getParentFolder() const
{
    std::string path = mPath.getCharArray();
    path.pop_back();

    return getParentFolderImpl(path);
}

String Folder::getAbsolutePath() const
{
    String absolutePath = getAbsolutePathImpl(mPath);
    if(! absolutePath.endsWith('/'))
    {
        absolutePath.getStdString() += '/';
    }

    return absolutePath;
}
    
bool Folder::exists() const
{
    #ifdef _WIN32
        return (_chdir(mPath.getCharArray()) == 0);
	#else
		struct stat pathStat;
        return (! stat(mPath.getCharArray(), &pathStat));
    #endif
}

std::vector<File> Folder::getChildFiles() const
{
    std::vector<File> files;
    std::vector<Folder> folders;
    getContent(files, folders);
    return files;
}

std::vector<Folder> Folder::getChildFolders() const
{
    std::vector<File> files;
    std::vector<Folder> folders;
    getContent(files, folders);
    return folders;
}
    
void Folder::getContent(std::vector<File>& files, std::vector<Folder>& folders) const
{
    #ifdef _WIN32
        std::string windowsPath(mPath.getCharArray());
        std::replace(windowsPath.begin(), windowsPath.end(), '/', '\\');
        windowsPath += "*.*";

        WIN32_FIND_DATAA findData;
        HANDLE findHandle = ::FindFirstFileA(windowsPath.c_str(), &findData);
        if(findHandle != INVALID_HANDLE_VALUE)
        {
            int folderIndex = 0;

            do
            {
                if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(folderIndex > 1)
                    {
                        folders.push_back(Folder(*this, findData.cFileName));
                    }

                    ++folderIndex;
                }
                else
                {
                    files.push_back(File(*this, findData.cFileName));
                }
            }
            while(::FindNextFileA(findHandle, &findData));

            ::FindClose(findHandle);
        }
    #else
        const char* pathCharArray = mPath.getCharArray();
        DIR* dir = opendir(pathCharArray);
        if(dir)
        {
            struct dirent* ent = readdir(dir);
            while(ent)
            {
                const char* nameCharArray = ent->d_name;
                std::string name(nameCharArray);
                if(name.front() != '.')
                {
                    std::string path = pathCharArray;
                    path += name;

                    if(isFolderPath(path))
                    {
                        folders.push_back(Folder(std::move(path)));
                    }
                    else
                    {
                        files.push_back(File(std::move(path)));
                    }
                }

                ent = readdir(dir);
            }

            closedir(dir);
        }
    #endif
}
    
bool Folder::create()
{
    int returnCode;

    #ifdef _WIN32
        returnCode = _mkdir(mPath.getCharArray());
    #else
        returnCode = mkdir(mPath.getCharArray(), 0775);
    #endif

    return (returnCode == 0);
}

}
