//
//  direction_light.h
//  gbCore
//
//  Created by sergey.sergeev on 9/22/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef direction_light_h
#define direction_light_h

#include "ces_entity.h"

namespace gb
{
    class direction_light : public ces_entity
    {
    private:
        
        std::weak_ptr<scene_graph> m_scene_graph;
        
        glm::vec3 m_direction;
        
    protected:
        
        void set_scene_graph(const scene_graph_shared_ptr& scene_graph);
        scene_graph_shared_ptr get_scene_graph() const;
        
    public:
        
        direction_light();
        ~direction_light();
        
        void set_direction(const glm::vec3& direction);
        glm::vec3 get_direction() const;
        
        void on_added_to_scene(const scene_graph_shared_ptr& scene_graph);
        void on_removed_from_scene();
        
        void add_material(const std::string& technique_name, const material_shared_ptr& material);
        void remove_material(const std::string& technique_name);
        material_shared_ptr get_material(const std::string& technique_name) const;
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};

#endif
