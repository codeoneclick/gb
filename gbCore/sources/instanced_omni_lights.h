//
//  instanced_omni_lights.h
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef instanced_omni_lights_h
#define instanced_omni_lights_h

#include "ces_entity.h"

namespace gb
{
    class instanced_omni_lights : public ces_entity
    {
    private:
        
        std::weak_ptr<scene_graph> m_scene_graph;
        std::vector<glm::vec4> m_parameters;
        i32 m_num_instances;
        
    protected:
        
        void set_scene_graph(const scene_graph_shared_ptr& scene_graph);
        scene_graph_shared_ptr get_scene_graph() const;
        
    public:
        
        instanced_omni_lights(i32 num_instances);
        ~instanced_omni_lights();
        
        void set_position(const glm::vec3& position, i32 instance_id);
        glm::vec3 get_position(i32 instance_id) const;
        
        void set_radius(f32 radius, i32 instance_id);
        f32 get_radius(i32 instance_id) const;
        
        void on_added_to_scene(const scene_graph_shared_ptr& scene_graph);
        void on_removed_from_scene();
        
        void add_material(const std::string& technique_name, const material_shared_ptr& material);
        void remove_material(const std::string& technique_name);
        material_shared_ptr get_material(const std::string& technique_name) const;
        
        void set_mesh(const instanced_mesh_shared_ptr& mesh);
    };
};

#endif
