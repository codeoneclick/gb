//
//  ces_configuration_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/14/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_configuration_component_h
#define ces_configuration_component_h

#include "ces_base_component.h"
#include "configuration_types.h"

namespace gb
{
    class configuration;
    class ces_configuration_component_interface : public ces_base_component_interface
    {
    private:
        
    protected:
        
    public:
        
        ces_configuration_component_interface(void) = default;
        ~ces_configuration_component_interface(void) = default;
        
        virtual void on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success);
    };
    
    class ces_configuration_component : public ces_base_component
    {
    private:
        
        friend class ces_configurations_system;
        
        std::string m_configuration_filename;
        e_configuration_type m_configuration_type;
        std::shared_ptr<configuration> m_configuration;
        
    protected:
        
        void on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success);
        
    public:
        
        ces_configuration_component(const std::string& configuration_filename, e_configuration_type configuration_type);
        ~ces_configuration_component(void);
        
        std::string get_configuration_filename(void) const;
        e_configuration_type get_configuration_type(void) const;
        std::shared_ptr<configuration> get_configuration(void) const;
    };
};

#endif
