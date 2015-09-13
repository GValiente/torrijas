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

#ifndef TRJ_CONFIG_H
#define TRJ_CONFIG_H

namespace trj
{
    // Maximum difference between two floats to be considered the same:
    constexpr float kEpsilon = 1e-6f;
}

#if defined(TRJ_CFG_GLES3)
    #undef TRJ_CFG_GL3
    #undef TRJ_CFG_GLES2
    #undef TRJ_CFG_GL2
#elif defined(TRJ_CFG_GL3)
    #undef TRJ_CFG_GLES2
    #undef TRJ_CFG_GL2
#elif defined(TRJ_CFG_GLES2)
    #undef TRJ_CFG_GL2
#elif defined(TRJ_CFG_GL2)
#else
    #define TRJ_CFG_GL2
#endif

#ifndef __APPLE__
    #undef TRJ_CFG_ENABLE_GLEW
    #define TRJ_CFG_ENABLE_GLEW
#endif

#endif
