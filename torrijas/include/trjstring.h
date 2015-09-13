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

#ifndef TRJ_STRING_H
#define TRJ_STRING_H

#include <string>
#include "trjoptional.h"

namespace trj
{

class String
{

protected:
    Optional<std::string> mString;
    const char* mCharArray = nullptr;
    int mCharArraySize = 0;

    String(const char* charArray, int size) noexcept :
        mCharArray(charArray),
        mCharArraySize(size)
    {
    }

public:
    String() :
        mCharArray("")
    {
    }

    template<int Size>
    inline String(const char(&charArray)[Size]) noexcept :
		mCharArray(charArray),
        mCharArraySize(Size - 1)
	{
        static_assert(Size > 0, "Invalid size");
    }

    String(std::string string) :
        mString(std::move(string))
	{
	}

	String(int value);

	String(long value);

	String(float value);

    String(double value);

    String(const String& other);

    String& operator=(const String& other);
    
    String(String&& other) = default;
    
    String& operator=(String&& other) = default;

    bool isStatic() const noexcept
	{
        return mString;
	}

    const char* getCharArray() const noexcept
	{
		if(mString)
		{
			return mString->c_str();
		}
		
		return mCharArray;
	}

    std::string& getStdString()
	{
		if(! mString)
		{
            mString.reset(std::string(mCharArray, mCharArraySize));
		}
		
		return *mString;
	}

    int getSize() const noexcept
	{
		if(mString)
		{
			return (int) mString->size();
		}
		
        return mCharArraySize;
	}

    bool isEmpty() const noexcept
	{
	    return (getSize() == 0);
	}

    char getCharAt(int index) const noexcept;

    bool startsWith(char character) const noexcept
	{
		if(isEmpty())
		{
			return false;
		}

		return getCharAt(0) == character;
	}

    bool endsWith(char character) const noexcept
	{
		int size = getSize();
		if(size == 0)
		{
			return false;
		}

		return getCharAt(size - 1) == character;
	}

    void append(const String& other)
    {
        getStdString() += other.getCharArray();
    }

	bool toNumber(int& result) const;

	bool toNumber(long& result) const;

	bool toNumber(float& result) const;

	bool toNumber(double& result) const;

	bool operator==(const String& other) const;

    bool operator!=(const String& other) const
	{
		return ! (operator==(other));
	}

	bool operator<(const String& other) const;

    bool operator>(const String& other) const
    {
        return other < *this;
    }

    bool operator<=(const String& other) const
    {
        return ! (other < *this);
    }

    bool operator>=(const String& other) const
    {
        return ! (*this < other);
    }

    String& operator+=(const String& other)
    {
        append(other);
        return *this;
    }
};

String operator+(const String& a, const String& b);

}

#endif
