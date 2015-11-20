//
//  ces_system_types.h
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_system_types_h
#define ces_system_types_h

namespace gb
{
    enum e_ces_system_type
    {
        e_ces_system_type_undefined = 0,
        e_ces_system_type_render,
        e_ces_system_type_animation,
        e_ces_system_type_input,
        e_ces_system_type_particle_emitter,
        e_ces_system_type_box2d,
        e_ces_system_type_skybox,
        e_ces_system_type_ocean,
        e_ces_system_type_batch,
        e_ces_system_type_terrain,
        e_ces_system_type_max
    };
}

#endif
