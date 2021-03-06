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

#include "trjexception.h"

#include <sstream>

namespace trj
{

Exception::Exception(const char* file, int line, String message) :
    mMessage(std::move(message)),
    mFile(file),
    mLine(line)
{
    std::stringstream stringStream;
    stringStream << file << "::" << line;

    if(! mMessage.isEmpty())
    {
        stringStream << ": ";
        stringStream << mMessage.getCharArray();
    }

    mWhat = stringStream.str();
}

ApplicationClosedException::ApplicationClosedException(const char* file, int line) :
    Exception(file, line, "Application was closed")
{
}

FrameBufferException::FrameBufferException(const char* file, int line) :
    Exception(file, line, "FrameBuffer build failed")
{
}

}

