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

#include "filestest.h"

#include <iostream>
#include "trjfile.h"
#include "trjfolder.h"

void FilesTest::run()
{
    trj::File existingFile("../../torrijas/nanovg/example/images/image1.jpg");
    std::cout << "existingFile path: " << existingFile.getPath().getCharArray() << std::endl;
    std::cout << "existingFile name: " << existingFile.getName().getCharArray() << std::endl;
    std::cout << "existingFile extension: " << existingFile.getExtension().getCharArray() << std::endl;
    std::cout << "existingFile absolute path: " << existingFile.getAbsolutePath().getCharArray() << std::endl;

    trj::Folder existingFolder = existingFile.getParentFolder();
    std::cout << "existingFolder path: " << existingFolder.getPath().getCharArray() << std::endl;
    std::cout << "existingFolder name: " << existingFolder.getName().getCharArray() << std::endl;
    std::cout << "existingFolder absolute path: " << existingFolder.getAbsolutePath().getCharArray() << std::endl;
    std::cout << "existingFolder parent: " << existingFolder.getParentFolder().getPath().getCharArray() << std::endl;

    trj::File nonExistingFile("../../torrijas/nanovg/example/images/image456.jpg");
    std::cout << "nonExistingFile path: " << nonExistingFile.getPath().getCharArray() << std::endl;

    trj::File noPathFile("torrijas-test");
    std::cout << "noPathFile path: " << noPathFile.getPath().getCharArray() << std::endl;
    std::cout << "noPathFile name: " << noPathFile.getName().getCharArray() << std::endl;
    std::cout << "noPathFile extension: " << noPathFile.getExtension().getCharArray() << std::endl;
    std::cout << "noPathFile absolute path: " << noPathFile.getAbsolutePath().getCharArray() << std::endl;
    std::cout << "noPathFile parent: " << noPathFile.getParentFolder().getPath().getCharArray() << std::endl;

    trj::Folder currentFolder = trj::Folder::getCurrentFolder();
    std::cout << "currentFolder path: " << currentFolder.getPath().getCharArray() << std::endl;

    trj::Folder parentCurrentFolder = currentFolder.getParentFolder();
    std::cout << "parentCurrentFolder path: " << parentCurrentFolder.getPath().getCharArray() << std::endl;

    std::vector<trj::File> files;
    std::vector<trj::Folder> folders;
    parentCurrentFolder.getContent(files, folders);

    for(const trj::File& file : files)
    {
        std::cout << "parentCurrentFolder file path: " << file.getPath().getCharArray() << std::endl;
    }

    for(const trj::Folder& folder : folders)
    {
        std::cout << "parentCurrentFolder folder path: " << folder.getPath().getCharArray() << std::endl;
    }
}
