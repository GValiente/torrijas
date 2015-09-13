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

#ifndef TRJ_PTR_H
#define TRJ_PTR_H

#include <utility>
#include "trjdebug.h"

namespace trj
{

template<class Type>
class Ptr
{

protected:
    Type* mPtr;

public:
    inline Ptr() noexcept :
        mPtr(nullptr)
    {
    }

    template<typename OtherType>
    inline Ptr(OtherType* ptr) noexcept :
        mPtr(ptr)
    {
    }

    Ptr(const Ptr& other) = delete;
    Ptr& operator=(const Ptr& other) = delete;

    inline Ptr(Ptr&& other) noexcept :
        mPtr(other.release())
    {
    }

    inline Ptr& operator=(Ptr&& other) noexcept
    {
        if(this != &other)
        {
            reset(other.release());
        }

        return *this;
    }

    template<typename OtherType>
    inline Ptr(Ptr<OtherType>&& other) noexcept :
        mPtr(other.release())
    {
    }

    template<typename OtherType>
    inline Ptr& operator=(Ptr<OtherType>&& other) noexcept
    {
        if(this != &other)
        {
            reset(other.release());
        }

        return *this;
    }

    inline ~Ptr()
    {
        reset();
    }

    inline operator bool() const
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

    inline void reset(Type* ptr = nullptr) noexcept
    {
        delete mPtr;
        mPtr = ptr;
    }

    inline Type* release() noexcept
    {
        Type* ptr = mPtr;
        mPtr = nullptr;
        return ptr;
    }

    template<typename OtherType>
    void swap(Ptr<OtherType>& other) noexcept
    {
        Type* tempPtr = mPtr;
        mPtr = other.mPtr;
        other.mPtr = tempPtr;
    }
};


template<class Type1, class Type2>
inline void swap(Ptr<Type1>& a, Ptr<Type2>& b) noexcept
{
    a.swap(b);
}

template<class Type1, class Type2>
inline bool operator==(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return a.get() == b.get();
}

template<class Type1, class Type2>
inline bool operator!=(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return ! (a == b);
}

template<class Type1, class Type2>
inline bool operator<(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return a.get() < b.get();
}

template<class Type1, class Type2>
inline bool operator>(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return b < a;
}

template<class Type1, class Type2>
inline bool operator<=(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return ! (b < a);
}

template<class Type1, class Type2>
inline bool operator>=(const Ptr<Type1>& a, const Ptr<Type2>& b) noexcept
{
    return ! (a < b);
}

}

#endif
