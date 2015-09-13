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

#ifndef TRJ_FOLDER_H
#define TRJ_FOLDER_H

#include <vector>
#include "trjstring.h"

namespace trj
{
	
class File;

class Folder
{
    
protected:
    String mPath;
    
public:
    static Folder getCurrentFolder();

    static bool isFolderPath(const String& path);

    Folder(String path);
    
    Folder(const Folder& folder, const String& name);

    String getName() const;

    Folder getParentFolder() const;

    const String& getPath() const noexcept
    {
        return mPath;
    }

    String getAbsolutePath() const;
    
    bool exists() const;

    std::vector<File> getChildFiles() const;

    std::vector<Folder> getChildFolders() const;
    
    void getContent(std::vector<File>& files, std::vector<Folder>& folders) const;
    
    bool create();
    
    bool operator==(const Folder& other) const noexcept
    {
        return mPath == other.mPath;
    }
    
    bool operator!=(const Folder& other) const noexcept
    {
        return mPath != other.mPath;
    }
};
    
}

#endif
