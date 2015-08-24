//
//  declarations.h
//  gbCore
//
//  Created by sergey.sergeev on 8/24/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef declarations_h
#define declarations_h

namespace gb
{
    class configuration;
    typedef std::shared_ptr<configuration> configuration_shared_ptr;
    
    class resource;
    typedef std::shared_ptr<resource> resource_shared_ptr;
    
    class resource_transfering_data;
    typedef std::shared_ptr<resource_transfering_data> resource_transfering_data_shared_ptr;
    
    class shader;
    typedef std::shared_ptr<shader> shader_shared_ptr;
    
    class shader_transfering_data;
    typedef std::shared_ptr<shader_transfering_data> shader_transfering_data_shared_ptr;
    
    class texture;
    typedef std::shared_ptr<texture> texture_shared_ptr;
    
    class texture_transfering_data;
    typedef std::shared_ptr<texture_transfering_data> texture_transfering_data_shared_ptr;
    
    class resource_serializer;
    typedef std::shared_ptr<resource_serializer> resource_serializer_shared_ptr;
    
    class resource_commiter;
    typedef std::shared_ptr<resource_commiter> resource_commiter_shared_ptr;
    
    class resource_loading_operation;
    typedef std::shared_ptr<resource_loading_operation> resource_loading_operation_shared_ptr;
    
    class resource_loading_interface;
    typedef std::shared_ptr<resource_loading_interface> resource_loading_interface_shared_ptr;
    
    class shader_loading_operation;
    typedef std::shared_ptr<shader_loading_operation> shader_loading_operation_shared_ptr;
    
    class texture_loading_operation;
    typedef std::shared_ptr<texture_loading_operation> texture_loading_operation_shared_ptr;
    
    class resource_accessor;
    typedef std::shared_ptr<resource_accessor> resource_accessor_shared_ptr;
    
    class configuration_accessor;
    typedef std::shared_ptr<configuration_accessor> configuration_accessor_shared_ptr;
    
    class material;
    typedef std::shared_ptr<material> material_shared_ptr;
};

#endif
