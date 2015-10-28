//
//  ces_batch_component.h
//  gbCore
//
//  Created by sergey.sergeev on 10/28/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_batch_component_h
#define ces_batch_component_h

#include "ces_base_component.h"

namespace gb
{
    class ces_batch_component : public ces_base_component
    {
    private:
        
    protected:
        
        std::map<std::string, batch_shared_ptr> m_batches;
        
    public:
        
        ces_batch_component();
        ~ces_batch_component();
        
        void add_batch(const batch_shared_ptr& batch, const std::string& guid);
        batch_shared_ptr get_batch(const std::string& technique_name, i32 technique_pass, const std::string& material_guid);
        
        void cleanup();
    };
};

#endif
