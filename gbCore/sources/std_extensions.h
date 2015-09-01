//
//  std_extensions.h
//  gbCore
//
//  Created by sergey.sergeev on 9/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef std_extensions_h
#define std_extensions_h

#include "main_headers.h"

#if defined(__IOS__) || defined(__OSX__)

#include <mach/mach.h>
#include <mach/mach_time.h>

#endif

namespace std
{
    ui64 get_tick_count()
    {
#if defined(__IOS__) || defined(__OSX__)
        static mach_timebase_info_data_t timebase_data;
        uint64_t mach_time = mach_absolute_time();
        if (timebase_data.denom == 0 )
        {
            (void)mach_timebase_info(&timebase_data);
        }
        uint64_t milliseconds = ((mach_time / 1000000) * timebase_data.numer) / timebase_data.denom;
        return milliseconds;
#endif
        return 0;
    }
    
    f32 get_random_f(f32 min, f32 max)
    {
#if defined(__IOS__) || defined(__OSX__)
        f32 random = (((f32)arc4random()/0x100000000)*(max - min) + min);
        return random;
#endif
        return 0;
    };
}

#endif
