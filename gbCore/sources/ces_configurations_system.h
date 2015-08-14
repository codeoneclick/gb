//
//  ces_configurations_system.h
//  gbCore
//
//  Created by sergey.sergeev on 8/14/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_configurations_system_h
#define ces_configurations_system_h

#include "ces_system.h"
#include "ces_entity.h"

namespace gb
{
    class configuration_accessor;
    class ces_configurations_system : public ces_system
    {
    private:
        
        std::shared_ptr<configuration_accessor> m_configuration_accessor;
        
    protected:
        
        void on_feed(const std::shared_ptr<ces_entity>& entity);
        
    public:
        
        ces_configurations_system(void);
        ~ces_configurations_system(void);
    };
};

#endif
