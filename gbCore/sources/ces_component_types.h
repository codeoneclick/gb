//
//  ces_component_types.h
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_component_types_h
#define ces_component_types_h

namespace gb
{
    enum e_ces_component_type
    {
        e_ces_component_type_undefined = 0,
        e_ces_component_type_render,
        e_ces_component_type_geometry,
        e_ces_component_type_transformation,
        e_ces_component_type_animation,
        e_ces_component_type_touch,
        e_ces_component_type_debug_render,
        e_ces_component_type_particle_emitter,
        e_ces_component_type_box2d,
        e_ces_component_type_skybox,
        e_ces_component_type_ocean,
        e_ces_component_type_batch,
        e_ces_component_type_terrain,
        e_ces_component_type_max
    };
};

#endif
