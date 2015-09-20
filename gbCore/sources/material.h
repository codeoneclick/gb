//
//  material.h
//  gbCore
//
//  Created by sergey.sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef material_h
#define material_h

#include "texture.h"
#include "shader.h"
#include "material_configuration.h"
#include "resource_accessor.h"
#include "declarations.h"

namespace gb
{
    class material_cached_parameters
    {
    private:
        
    protected:
        
    public:
        
        material_cached_parameters(void);
        ~material_cached_parameters(void);
        
        bool m_is_culling;
        GLenum m_culling_mode;
        
        bool m_is_blending;
        GLenum m_blending_function_source;
        GLenum m_blending_function_destination;
        
        bool m_is_depth_test;
        bool m_is_depth_mask;
        
        bool m_is_clipping;
        glm::vec4 m_clipping_plane;
        
        bool m_is_reflecting;
        bool m_is_shadowing;
        bool m_is_debugging;
        
        std::shared_ptr<shader> m_shader;
        std::array<std::shared_ptr<texture>, e_shader_sampler_max> m_textures;
    };
    
    class material
    {
    private:
        
    protected:
        
        std::map<std::string, std::shared_ptr<shader_uniform>> m_custom_shader_uniforms;
        std::shared_ptr<material_cached_parameters> m_parameters;
        static std::shared_ptr<material_cached_parameters> m_cached_parameters;
        static std::shared_ptr<material_cached_parameters> get_cached_parameters(void);
        
    public:
        
        material(void);
        ~material(void);
        
        static std::shared_ptr<material> construct(const std::shared_ptr<material_configuration>& configuration);
        
        static void set_shader(const material_shared_ptr& material,
                               const std::shared_ptr<material_configuration>& configuration,
                               const resource_accessor_shared_ptr& resource_accessor,
                               const resource_loading_interface_shared_ptr& listener = nullptr);
        
        static void set_textures(const material_shared_ptr& material,
                                 const std::shared_ptr<material_configuration>& configuration,
                                 const resource_accessor_shared_ptr& resource_accessor,
                                 const resource_loading_interface_shared_ptr& listener = nullptr);
        
        bool is_culling(void) const;
        GLenum get_culling_mode(void) const;
        
        bool is_blending(void) const;
        GLenum get_blending_function_source(void) const;
        GLenum get_blending_function_destination(void) const;
        
        bool is_depth_test(void) const;
        bool is_depth_mask(void) const;
        
        bool is_clipping(void) const;
        glm::vec4 get_clipping_plane(void) const;
        
        bool is_reflecting(void) const;
        bool is_shadowing(void) const;
        bool is_debugging(void) const;
        
        std::shared_ptr<shader> get_shader(void) const;
        std::shared_ptr<texture> get_texture(e_shader_sampler sampler) const;
        e_shader_sampler get_sampler_index(const std::shared_ptr<texture>& texture) const;
        
        void set_culling(bool value);
        void set_culling_mode(GLenum value);
        
        void set_blending(bool value);
        void set_blending_function_source(GLenum value);
        void set_blending_function_destination(GLenum value);
        
        void set_depth_test(bool value);
        void set_depth_mask(bool value);
        
        void set_clipping(bool value);
        void set_clipping_plane(const glm::vec4& value);
        
        void set_reflecting(bool value);
        void set_shadowing(bool value);
        void set_debugging(bool value);
        
        void set_shader(const std::shared_ptr<shader>& shader);
        void set_texture(const std::shared_ptr<texture>& texture, e_shader_sampler sampler);
        
        void set_custom_shader_uniform(const glm::mat4& matrix, const std::string& uniform);
        void set_custom_shader_uniform(const glm::mat3& matrix, const std::string& uniform);
        void set_custom_shader_uniform(const glm::vec4& vector, const std::string& uniform);
        void set_custom_shader_uniform(const glm::vec3& vector, const std::string& uniform);
        void set_custom_shader_uniform(const glm::vec2& vector, const std::string& uniform);
        void set_custom_shader_uniform(f32 value, const std::string& uniform);
        void set_custom_shader_uniform(i32 value, const std::string& uniform);
        
        const std::map<std::string, std::shared_ptr<shader_uniform>>& get_custom_uniforms(void) const;
        
        void bind(void);
        void unbind(void);
    };
};

#endif
