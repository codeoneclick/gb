//
//  ces_entity.h
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_entity_h
#define ces_entity_h

#include "ces_base_component.h"

namespace gb
{
    class ces_entity : public std::enable_shared_from_this<ces_entity>
    {
    private:
        
    protected:
        
        std::array<ces_base_component_shared_ptr, e_ces_component_type_max> m_components;
        
    public:
        
        ces_entity();
        virtual ~ces_entity();
        
        void add_component(const std::shared_ptr<ces_base_component>& component);
        void remove_component(const std::shared_ptr<ces_base_component>& component);
        void remove_components();
        bool is_component_exist(e_ces_component_type type) const;
        ces_base_component_shared_ptr get_component(e_ces_component_type type) const;
    };
};

#endif
