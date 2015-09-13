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

#include "trjstring.h"

#include <algorithm>
#include <cstring>
#include <sstream>

namespace trj
{
	
String::String(int value)
{
	std::stringstream stream;
	stream << value;
	mString.reset(stream.str());
}

String::String(long value)
{
	std::stringstream stream;
	stream << value;
	mString.reset(stream.str());
}

String::String(float value)
{
	std::stringstream stream;
	stream << value;
	mString.reset(stream.str());
}

String::String(double value)
{
	std::stringstream stream;
	stream << value;
	mString.reset(stream.str());
}

String::String(const String& other) :
    String(other.mCharArray, other.mCharArraySize)
{
    if(other.mString)
    {
        std::string string = *(other.mString);
        mString.reset(std::move(string));
    }
}

String& String::operator=(const String& other)
{
    if(this != &other)
    {
        mCharArray = other.mCharArray;
        mCharArraySize = other.mCharArraySize;

        if(other.mString)
        {
            std::string string = *(other.mString);
            mString.reset(std::move(string));
        }
    }

    return *this;
}

char String::getCharAt(int index) const noexcept
{
    TRJ_ASSERT(index >= 0 && index < getSize(), "Invalid index");

    return getCharArray()[index];
}

bool String::toNumber(int& result) const
{
    std::stringstream stream(getCharArray());
    return ! ((stream >> result).fail() || ! (stream >> std::ws).eof());
}

bool String::toNumber(long& result) const
{
    std::stringstream stream(getCharArray());
    return ! ((stream >> result).fail() || ! (stream >> std::ws).eof());
}

bool String::toNumber(float& result) const
{
    std::stringstream stream(getCharArray());
    return ! ((stream >> result).fail() || ! (stream >> std::ws).eof());
}

bool String::toNumber(double& result) const
{
    std::stringstream stream(getCharArray());
    return ! ((stream >> result).fail() || ! (stream >> std::ws).eof());
}

bool String::operator==(const String& other) const
{
	const char* charArray = getCharArray();
	const char* otherCharArray = other.getCharArray();
	if(charArray == otherCharArray)
	{
		return true;
	}

	int size = getSize();
	if(size != other.getSize())
	{
		return false;
	}

	return (memcmp(charArray, otherCharArray, size) == 0);
}

bool String::operator<(const String& other) const
{
	const char* charArray = getCharArray();
	int size = getSize();

	const char* otherCharArray = other.getCharArray();
	int otherSize = getSize();
    int minSize = std::min(size, otherSize);
	
	int diff = memcmp(charArray, otherCharArray, minSize);
	if(diff == 0)
	{
		return (size < otherSize);
	}

	return (diff < 0);
}

String operator+(const String& a, const String& b)
{
    String result = a;
    result += b;
    return result;
}

}
