//
//  renderable_interface.h
//  gbCore
//
//  Created by sergey.sergeev on 10/12/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef renderable_interface_h
#define renderable_interface_h

#include "ces_entity.h"

namespace gb
{
    class renderable_interface : public ces_entity
    {
    private:
        
        std::weak_ptr<scene_graph> m_scene_graph;
        
    protected:
        
        void set_scene_graph(const scene_graph_shared_ptr& scene_graph);
        scene_graph_shared_ptr get_scene_graph() const;
        
    public:
        
        renderable_interface();
        virtual ~renderable_interface();
        
        virtual void on_added_to_scene(const scene_graph_shared_ptr& scene_graph);
        virtual void on_removed_from_scene();
        
        virtual void add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material);
        virtual void remove_material(const std::string& technique_name, i32 technique_pass);
        virtual material_shared_ptr get_material(const std::string& technique_name, i32 technique_pass) const;
    };
};

#endif
