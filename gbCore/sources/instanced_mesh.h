//
//  instanced_mesh.h
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef instanced_mesh_h
#define instanced_mesh_h

#include "mesh.h"
#include "instanced_vbo.h"

namespace gb
{
    class instanced_mesh : public mesh
    {
    private:
        
    protected:
        
        i32 m_num_instances;
        std::map<std::string, instanced_vbo_shared_ptr> m_instanced_vbos;
        
    public:
        
        instanced_mesh(const std::string& guid, i32 num_instances = 1, GLenum mode = GL_TRIANGLES);
        
        static std::shared_ptr<instanced_mesh> construct(const std::string& guid,
                                                         const vbo_shared_ptr& vbo,
                                                         const ibo_shared_ptr& ibo,
                                                         const glm::vec3& min_bound,
                                                         const glm::vec3& max_bound,
                                                         i32 num_instances = 1,
                                                         GLenum mode = GL_TRIANGLES);
        
        static std::shared_ptr<instanced_mesh> construct(const std::string& guid,
                                                         const vbo_shared_ptr& vbo,
                                                         const ibo_shared_ptr& ibo,
                                                         i32 num_instances = 1,
                                                         GLenum mode = GL_TRIANGLES);
        ~instanced_mesh();
        
        template<typename T_VALUE> void attach_instanced_vbo(const std::string& instanced_vbo_id)
        {
            const auto& iterator = m_instanced_vbos.find(instanced_vbo_id);
            if(iterator == m_instanced_vbos.end())
            {
                instanced_vbo_shared_ptr vbo = std::make_shared<instanced_vbo>();
                vbo->allocate_data<T_VALUE>(m_num_instances);
                m_instanced_vbos.insert(std::make_pair(instanced_vbo_id, vbo));
            }
            else
            {
                assert(false);
            }
        }
        
        i32 get_num_instances() const;
        void set_num_instances(i32 num_instances);
        
        template<typename T_VALUE> void bind_instanced_vbo(const std::string& attributes_guid, const std::string& instanced_vbo_id, i32 attribute)
        {
            const auto& iterator = m_instanced_vbos.find(instanced_vbo_id);
            if(iterator != m_instanced_vbos.end())
            {
                assert(attributes_guid.length() != 0);
                std::shared_ptr<vao> vao_state = m_vao_states[attributes_guid];
                if(vao_state)
                {
                    vao::bind(vao_state);
                    iterator->second->bind<T_VALUE>(attribute);
                }
                else
                {
                    assert(false);
                }
            }
            else
            {
                assert(false);
            }
        }
        
        void draw() const;
        void draw(ui32 indices) const;
        
        template<typename T_VALUE> void unbind_instanced_vbo(const std::string& attributes_guid, const std::string& instanced_vbo_id, i32 attribute)
        {
            const auto& iterator = m_instanced_vbos.find(instanced_vbo_id);
            if(iterator != m_instanced_vbos.end())
            {
                iterator->second->unbind(attribute);
            }
            else
            {
                assert(false);
            }
        }
        
        template<typename T_VALUE> T_VALUE* lock(const std::string& instanced_attribute_id) const
        {
            const auto& iterator = m_instanced_vbos.find(instanced_attribute_id);
            if(iterator != m_instanced_vbos.end())
            {
                return iterator->second->lock<T_VALUE>();
            }
            else
            {
                assert(false);
                return nullptr;
            }
        }
        
        template<typename T_VALUE> void unlock(const std::string& instanced_attribute_id, ui32 size = 0) const
        {
            const auto& iterator = m_instanced_vbos.find(instanced_attribute_id);
            if(iterator != m_instanced_vbos.end())
            {
                return iterator->second->unlock<T_VALUE>(size);
            }
            else
            {
               assert(false);
            }
        }
    };
};

#endif
