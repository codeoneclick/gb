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
    
    class mesh;
    typedef std::shared_ptr<mesh> mesh_shared_ptr;
    
    class mesh_transfering_data;
    typedef std::shared_ptr<mesh_transfering_data> mesh_transfering_data_shared_ptr;
    
    class skeleton_transfering_data;
    typedef std::shared_ptr<skeleton_transfering_data> skeleton_transfering_data_shared_ptr;
    
    class sequence;
    typedef std::shared_ptr<sequence> sequence_shared_ptr;
    
    class sequence_transfering_data;
    typedef std::shared_ptr<sequence_transfering_data> sequence_transfering_data_shared_ptr;
    
    class vbo;
    typedef std::shared_ptr<vbo> vbo_shared_ptr;
    
    class ibo;
    typedef std::shared_ptr<ibo> ibo_shared_ptr;
    
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
    
    class mesh_loading_operation;
    typedef std::shared_ptr<mesh_loading_operation> mesh_loading_operation_shared_ptr;
    
    class resource_accessor;
    typedef std::shared_ptr<resource_accessor> resource_accessor_shared_ptr;
    
    class configuration_accessor;
    typedef std::shared_ptr<configuration_accessor> configuration_accessor_shared_ptr;
    
    class material;
    typedef std::shared_ptr<material> material_shared_ptr;
    
    class camera;
    typedef std::shared_ptr<camera> camera_shared_ptr;
    
    class frustum;
    typedef std::shared_ptr<frustum> frustum_shared_ptr;
    
    class global_light;
    typedef std::shared_ptr<global_light> global_light_shared_ptr;
    
    class scene_graph;
    typedef std::shared_ptr<scene_graph> scene_graph_shared_ptr;
    
    class fabricator;
    typedef std::shared_ptr<fabricator> fabricator_shared_ptr;
    
    class ces_systems_feeder;
    typedef std::shared_ptr<ces_systems_feeder> ces_systems_feeder_shared_ptr;
    
    class ces_transformation_component;
    typedef std::shared_ptr<ces_transformation_component> ces_transformation_component_shared_ptr;
    
    class ces_camera_component;
    typedef std::shared_ptr<ces_camera_component> ces_camera_component_shared_ptr;
    
    class ces_frustum_culling_component;
    typedef std::shared_ptr<ces_frustum_culling_component> ces_frustum_culling_component_shared_ptr;
    
    class ces_global_light_component;
    typedef std::shared_ptr<ces_global_light_component> ces_global_light_component_shared_ptr;
    
    class ces_geometry_component;
    typedef std::shared_ptr<ces_geometry_component> ces_geometry_component_shared_ptr;

    class ces_render_component;
    typedef std::shared_ptr<ces_render_component> ces_render_component_shared_ptr;
    
    class game_object;
    typedef std::shared_ptr<game_object> game_object_shared_ptr;
    
    class model3d_static;
    typedef std::shared_ptr<model3d_static> model3d_static_shared_ptr;
};

#endif
