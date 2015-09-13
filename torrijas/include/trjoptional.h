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

#ifndef TRJ_OPTIONAL_H
#define TRJ_OPTIONAL_H

#include <new>
#include "trjdebug.h"

namespace trj
{
	
template<typename Type, int Alignment = sizeof(void*)>
class Optional
{

protected:
    alignas(Alignment) char mPool[sizeof(Type)];
    Type* mPtr;

public:
    inline Optional() noexcept :
		mPtr(nullptr)
	{
	}

    inline explicit Optional(Type&& var) noexcept :
        mPtr(new(mPool) Type(std::move(var)))
    {
	}

    Optional(const Optional& other) = delete;
    Optional& operator=(const Optional& other) = delete;
    
    inline Optional(Optional&& other) noexcept :
        mPtr(nullptr)
    {
        if(other.mPtr)
        {
            mPtr = new(mPool) Type(std::move(*(other.mPtr)));
            other.reset();
        }
    }
    
    inline Optional& operator=(Optional&& other) noexcept
    {
        if(this != &other)
        {
            reset();

            if(other.mPtr)
            {
                mPtr = new(mPool) Type(std::move(*(other.mPtr)));
                other.reset();
            }
        }

        return *this;
    }

    inline ~Optional()
    {
        reset();
    }

    inline operator bool() const noexcept
	{
    	return mPtr != nullptr;
	}

    inline const Type& operator*() const noexcept
    {
        TRJ_ASSERT(mPtr, "Pointer is null");

        return *mPtr;
    }

    inline Type& operator*() noexcept
    {
        TRJ_ASSERT(mPtr, "Pointer is null");

        return *mPtr;
    }

    inline const Type* operator->() const noexcept
    {
        TRJ_ASSERT(mPtr, "Pointer is null");

        return mPtr;
    }

    inline Type* operator->() noexcept
    {
        TRJ_ASSERT(mPtr, "Pointer is null");

        return mPtr;
    }

    inline const Type* get() const noexcept
    {
        return mPtr;
    }

    inline Type* get() noexcept
    {
        return mPtr;
    }
    
    inline void reset() noexcept
    {
		if(mPtr)
		{
            mPtr->~Type();
			mPtr = nullptr;
		}
    }

    inline void reset(Type&& var) noexcept
    {
        reset();

        mPtr = new(mPool) Type(std::move(var));
    }

    template<typename... Args>
    inline void reset(Args&&... args) noexcept
    {
        reset();

        mPtr = new(mPool) Type(std::forward<Args>(args)...);
    }

    inline Type release() noexcept
    {
        TRJ_ASSERT(mPtr, "Pointer is null");

        Type result = std::move(*mPtr);
        reset();

        return result;
    }
};

}

#endif
