//
//  ces_base_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_base_component_h
#define ces_base_component_h

#include "main_headers.h"
#include "declarations.h"
#include "ces_component_types.h"

namespace gb
{
    class ces_base_component : public std::enable_shared_from_this<ces_base_component>
    {
    private:
        
    protected:
        
        e_ces_component_type m_type;

        ces_base_component();
        
    public:
        
        virtual ~ces_base_component() = default;
        
        e_ces_component_type get_type() const;
    };

#define unsafe_get_render_component(entity) static_cast<ces_render_component*>(entity->get_component(e_ces_component_type_render).get())
#define unsafe_get_render_component_from_this() static_cast<ces_render_component*>(ces_entity::get_component(e_ces_component_type_render).get())
    
#define unsafe_get_transformation_component(entity) static_cast<ces_transformation_component*>(entity->get_component(e_ces_component_type_transformation).get())
#define unsafe_get_transformation_component_from_this() static_cast<ces_transformation_component*>(ces_entity::get_component(e_ces_component_type_transformation).get())
    
};

#endif
