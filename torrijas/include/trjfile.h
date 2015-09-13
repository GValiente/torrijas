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

#ifndef TRJ_FILE_H
#define TRJ_FILE_H

#include "trjfolder.h"

namespace trj
{

class File
{

protected:
    String mPath;

public:
    static bool isFilePath(const String& path);

    File(String path);

    File(const Folder& folder, const String& name);
    
    String getName() const;
    
    String getNameWithoutExtension() const;
    
    String getExtension() const;
    
    Folder getParentFolder() const;

    const String& getPath() const noexcept
	{
		return mPath;
	}

    String getAbsolutePath() const;

	bool exists() const;

    String getContent() const;

    bool remove() const;

    bool operator==(const File& other) const noexcept
	{
		return mPath == other.mPath;
	}

    bool operator!=(const File& other) const noexcept
	{
		return mPath != other.mPath;
    }
};

}

#endif
