//
//  fabricator.h
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef fabricator_h
#define fabricator_h

#include "main_headers.h"
#include "declarations.h"
#include "configuration_accessor.h"

namespace gb
{
    class fabricator
    {
    private:
        
    protected:
        
        configuration_accessor_shared_ptr m_configuration_accessor;
        resource_accessor_shared_ptr m_resource_accessor;
        
        std::set<camera_shared_ptr> m_cameras_container;
        std::set<global_light_shared_ptr> m_global_lights_container;
        std::set<game_object_shared_ptr> m_game_objects_container;
        std::set<omni_light_shared_ptr> m_omni_lights_container;
        std::set<direction_light_shared_ptr> m_direction_lights_container;
        
    public:
        
        fabricator(const configuration_accessor_shared_ptr& configuration_accessor,
                   const resource_accessor_shared_ptr& resource_accessor);
        ~fabricator();
        
        camera_shared_ptr create_camera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport);
        void destroy_camera(const camera_shared_ptr& camera);
        
        omni_light_shared_ptr create_omni_light();
        void destroy_omni_light(const omni_light_shared_ptr& omni_light);
        
        direction_light_shared_ptr create_direction_light();
        void destroy_direction_light(const direction_light_shared_ptr& direction_light);
        
        global_light_shared_ptr create_global_light(f32 fov, f32 near, f32 far);
        void destroy_global_light(const global_light_shared_ptr& global_light);
        
        model3d_static_shared_ptr create_model3d_static(const std::string& filename);
        model3d_animated_shared_ptr create_model3d_animated(const std::string& filename);
        particle_emitter_shared_ptr create_particle_emitter(const std::string& filename);
        void destroy_game_object(const game_object_shared_ptr& game_object);
    };
};

#endif
