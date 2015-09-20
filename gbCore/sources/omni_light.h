//
//  omni_light.h
//  gbCore
//
//  Created by sergey.sergeev on 9/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef omni_light_h
#define omni_light_h

#include "ces_entity.h"

namespace gb
{
    class omni_light : public ces_entity
    {
    private:
        
        std::weak_ptr<scene_graph> m_scene_graph;
        
        f32 m_radius;
        
    protected:
        
        void set_scene_graph(const scene_graph_shared_ptr& scene_graph);
        scene_graph_shared_ptr get_scene_graph() const;
        
    public:
        
        omni_light();
        ~omni_light();
        
        void set_position(const glm::vec3& position);
        glm::vec3 get_position() const;
        
        void set_radius(f32 radius);
        f32 get_radius() const;
        
        virtual void on_added_to_scene(const scene_graph_shared_ptr& scene_graph);
        virtual void on_removed_from_scene();
        
        void add_material(const std::string& technique_name, const material_shared_ptr& material);
        void remove_material(const std::string& technique_name);
        material_shared_ptr get_material(const std::string& technique_name) const;
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};


#endif
