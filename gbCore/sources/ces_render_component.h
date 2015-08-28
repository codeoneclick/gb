//
//  ces_render_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_render_component_h
#define ces_render_component_h

#include "ces_base_component.h"
#include "material.h"

namespace gb
{
    class ces_render_component : public ces_base_component
    {
    private:
        
    protected:
        
        std::unordered_map<std::string, material_shared_ptr> m_materials;
        i32 m_z_order;
        
        void bind_custom_shader_uniforms(const material_shared_ptr& material);
        
    public:
        
        ces_render_component(void);
        ~ces_render_component(void);
        
        void add_material(const std::string& technique_name, const material_shared_ptr& material);
        void remove_material(const std::string& technique_name);
        material_shared_ptr get_material(const std::string& technique_name) const;
        
        void set_z_order(i32 z_order);
        i32 get_z_order(void) const;
        
        virtual void bind_transformation_uniforms(const std::string& technique_name,
                                                  const glm::mat4& matrix_m,
                                                  const glm::mat4& matrix_mvp,
                                                  const glm::mat4& matrix_imvp,
                                                  const std::shared_ptr<material>& material = nullptr);
        
        virtual void bind_camera_uniforms(const std::string& technique_name,
                                          const std::shared_ptr<camera>& camera,
                                          const std::shared_ptr<material>& material = nullptr);
        
        virtual void bind_global_light_uniforms(const std::string& technique_name,
                                                const std::shared_ptr<global_light>& global_light,
                                                const std::shared_ptr<material>& material = nullptr);
        
        virtual void bind_skeleton_animation_uniforms(const std::string& technique_name,
                                                      const glm::mat4* transformations, i32 num_transformations,
                                                      const std::shared_ptr<material>& material = nullptr);
        
        virtual material_shared_ptr on_bind(const std::string& technique_name);
        
        virtual void on_draw(const std::string& technique_name, const std::shared_ptr<mesh>& mesh,
                             const std::shared_ptr<material>& material = nullptr);
        
        virtual void on_unbind(const std::string& technique_name,
                               const std::shared_ptr<material>& material = nullptr);
        
        void set_texture(const std::shared_ptr<texture>& texture, e_shader_sampler sampler, const std::string& technique_name = "");
        
        template<typename T_VALUE>
        void set_custom_shader_uniform(T_VALUE value, const std::string& uniform, const std::string& technique_name)
        {
            if(technique_name.length() != 0)
            {
                std::shared_ptr<material> material = ces_render_component::get_material(technique_name);
                assert(material);
                material->set_custom_shader_uniform(value, uniform);
            }
            else
            {
                std::for_each(m_materials.cbegin(), m_materials.cend(), [value, uniform](std::pair<std::string, std::shared_ptr<material>> material) {
                    material.second->set_custom_shader_uniform(value, uniform);
                });
            }
        };
    };
};

#endif
