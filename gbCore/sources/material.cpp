//
//  material.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "material.h"

namespace gb
{
    material_cached_parameters::material_cached_parameters(void) :
    m_shader(nullptr)
    {
        std::for_each(m_textures.begin(), m_textures.end(), [](std::shared_ptr<texture>& iterator){
            iterator = nullptr;
        });
    }
    
    material_cached_parameters::~material_cached_parameters(void)
    {
        
    }
    
    std::shared_ptr<material_cached_parameters> material::m_cached_parameters = nullptr;
    std::once_flag g_cached_parameters_created;
    std::shared_ptr<material_cached_parameters> material::get_cached_parameters(void)
    {
        std::call_once(g_cached_parameters_created, []{
            m_cached_parameters = std::make_shared<material_cached_parameters>();
            m_cached_parameters->m_is_depth_test = true;
            m_cached_parameters->m_is_depth_mask = true;
            m_cached_parameters->m_is_culling = false;
            m_cached_parameters->m_is_blending = false;
        });
        return m_cached_parameters;
    }
    
    material::material(void) :
    m_parameters(std::make_shared<material_cached_parameters>())
    {
        
    }
    
    material::~material(void)
    {
        
    }
    
    std::shared_ptr<material> material::construct(const std::shared_ptr<material_configuration> &configuration)
    {
        std::shared_ptr<material> material = std::make_shared<gb::material>();
        assert(configuration != nullptr);
        
        material->set_culling(configuration->get_culling());
        material->set_culling_mode(configuration->get_culling_mode());
        
        material->set_blending(configuration->get_blending());
        material->set_blending_function_source(configuration->get_blending_function_source());
        material->set_blending_function_destination(configuration->get_blending_function_destination());
        
        material->set_depth_test(configuration->get_depth_test());
        material->set_depth_mask(configuration->get_depth_mask());
        
        material->set_clipping(configuration->get_clipping());
        material->set_clipping_plane(glm::vec4(configuration->get_clipping_x(),
                                               configuration->get_clipping_y(),
                                               configuration->get_clipping_z(),
                                               configuration->get_clipping_w()));
        
        material->set_reflecting(configuration->get_reflecting());
        material->set_shadowing(configuration->get_shadowing());
        material->set_debugging(configuration->get_debugging());
        
        return material;
    }
    
    
    bool material::is_culling(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_is_culling;
    }
    
    GLenum material::get_culling_mode(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_culling_mode;
    }
    
    bool material::is_blending(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_is_blending;
    }
    
    GLenum material::get_blending_function_source(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_blending_function_source;
    }
    
    GLenum material::get_blending_function_destination(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_blending_function_destination;
    }
    
    bool material::isDepthTest(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isDepthTest;
    }
    
    bool material::isDepthMask(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isDepthMask;
    }
    
    bool material::isClipping(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isClipping;
    }
    
    glm::vec4 material::getClippingPlane(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_clippingPlane;
    }
    
    bool material::isReflecting(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isReflecting;
    }
    
    bool material::isShadowing(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isShadowing;
    }
    
    bool material::isDebugging(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_isDebugging;
    }
    
    CSharedShader material::getShader(void) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_shader;
    }
    
    CSharedTexture material::getTexture(E_SHADER_SAMPLER sampler) const
    {
        assert(m_parameters != nullptr);
        return m_parameters->m_textures.at(sampler);
    }
    
    E_SHADER_SAMPLER material::getSamplerIndex(CSharedTextureRef texture) const
    {
        assert(m_parameters != nullptr);
        for(ui32 i = 0; i < m_parameters->m_textures.size(); ++i)
        {
            if(texture == m_parameters->m_textures.at(i))
            {
                return static_cast<E_SHADER_SAMPLER>(i);
            }
        }
        assert(false);
        return E_SHADER_SAMPLER_01;
    }
    
    void material::setCulling(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isCulling = value;
    }
    
    void material::setCullingMode(GLenum value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_cullingMode = value;
    }
    
    void material::setBlending(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isBlending = value;
    }
    
    void material::setBlendingFunctionSource(GLenum value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_blendingFunctionSource = value;
    }
    
    void material::setBlendingFunctionDestination(GLenum value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_blendingFunctionDestination = value;
    }
    
    void material::setDepthTest(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isDepthTest = value;
    }
    
    void material::setDepthMask(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isDepthMask = value;
    }
    
    void material::setClipping(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isClipping = value;
    }
    
    void material::setClippingPlane(const glm::vec4& value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_clippingPlane = value;
    }
    
    void material::setReflecting(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isReflecting = value;
    }
    
    void material::setShadowing(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isShadowing = value;
    }
    
    void material::setDebugging(bool value)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_isDebugging = value;
    }
    
    void material::setShader(CSharedShaderRef shader)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_shader = shader;
    }
    
    void material::setTexture(CSharedTextureRef texture,
                               E_SHADER_SAMPLER sampler)
    {
        assert(m_parameters != nullptr);
        m_parameters->m_textures.at(sampler) = texture;
    }
    
    void material::setCustomShaderUniform(const glm::mat4x4& matrix, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT4X4);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setMatrix4x4(matrix);
    }
    
    void material::setCustomShaderUniform(const glm::mat3x3& matrix, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT3X3);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setMatrix3x3(matrix);
    }
    
    void material::setCustomShaderUniform(const glm::vec4& vector, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR4);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setVector4(vector);
    }
    
    void material::setCustomShaderUniform(const glm::vec3& vector, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR3);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setVector3(vector);
    }
    
    void material::setCustomShaderUniform(const glm::vec2& vector, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR2);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setVector2(vector);
    }
    
    void CMaterial::setCustomShaderUniform(f32 value, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_FLOAT);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setFloat(value);
    }
    
    void CMaterial::setCustomShaderUniform(i32 value, const std::string& uniform)
    {
        const auto& iterator = m_customShaderUniforms.find(uniform);
        CSharedShaderUniform currentUniform = nullptr;
        if(iterator != m_customShaderUniforms.end())
        {
            currentUniform = iterator->second;
        }
        else
        {
            currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_INT);
            m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
        }
        currentUniform->setInt(value);
    }
    
    const std::map<std::string, CSharedShaderUniform>& CMaterial::getCustomUniforms(void) const
    {
        return m_customShaderUniforms;
    }
    
    bool CMaterial::isLoaded(void) const
    {
        bool value = false;
        if(m_parameters != nullptr)
        {
            for(const auto& texture : m_parameters->m_textures)
            {
                if(texture != nullptr)
                {
                    value = texture->isLoaded();
                    if(!value)
                    {
                        return value;
                    }
                }
            }
            value = m_parameters->m_shader->isLoaded();
        }
        return value;
    }
    
    bool CMaterial::isCommited(void) const
    {
        bool value = false;
        if(m_parameters != nullptr)
        {
            for(const auto& texture : m_parameters->m_textures)
            {
                if(texture != nullptr)
                {
                    value = texture->isCommited();
                    if(!value)
                    {
                        return value;
                    }
                }
            }
            value = m_parameters->m_shader->isCommited();
        }
        return value;
    }
    
    void CMaterial::bind(void)
    {
        assert(m_parameters != nullptr);
        assert(m_parameters->m_shader != nullptr);
        
        m_parameters->m_shader->bind();
        
        for(ui32 i = 0; i < E_SHADER_SAMPLER_MAX; ++i)
        {
            if(m_parameters->m_textures[i] != nullptr)
            {
                m_parameters->m_shader->setTexture(m_parameters->m_textures[i], static_cast<E_SHADER_SAMPLER>(i));
            }
        }
        
        if(m_parameters->m_isDepthTest &&
           getCachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
        {
            ieEnable(GL_DEPTH_TEST);
            ieDepthFunc(GL_LEQUAL);
            getCachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
        }
        else if(getCachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
        {
            ieDisable(GL_DEPTH_TEST);
            getCachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
        }
        
        if(m_parameters->m_isDepthMask &&
           getCachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
        {
            ieDepthMask(GL_TRUE);
            getCachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
        }
        else if(getCachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
        {
            ieDepthMask(GL_FALSE);
            getCachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
        }
        
        if(m_parameters->m_isCulling &&
           getCachedParameters()->m_isCulling != m_parameters->m_isCulling)
        {
            ieEnable(GL_CULL_FACE);
            ieCullFace(m_parameters->m_cullingMode);
            getCachedParameters()->m_isCulling = m_parameters->m_isCulling;
        }
        else if(getCachedParameters()->m_isCulling != m_parameters->m_isCulling)
        {
            ieDisable(GL_CULL_FACE);
            getCachedParameters()->m_isCulling = m_parameters->m_isCulling;
        }
        
        if(m_parameters->m_isBlending &&
           getCachedParameters()->m_isBlending != m_parameters->m_isBlending)
        {
            ieEnable(GL_BLEND);
            ieBlendFunc(m_parameters->m_blendingFunctionSource, m_parameters->m_blendingFunctionDestination);
            getCachedParameters()->m_isBlending = m_parameters->m_isBlending;
        }
        else if(getCachedParameters()->m_isBlending != m_parameters->m_isBlending)
        {
            ieDisable(GL_BLEND);
            getCachedParameters()->m_isBlending = m_parameters->m_isBlending;
        }
        
        if(m_parameters->m_isClipping &&
           getCachedParameters()->m_isClipping != m_parameters->m_isClipping)
        {
#if defined(__IOS__) && defined(GL_APPLE_clip_distance)
            ieEnable(GL_CLIP_DISTANCE0_APPLE);
#else
            ieEnable(GL_CLIP_DISTANCE0);
#endif
            getCachedParameters()->m_isClipping = m_parameters->m_isClipping;
        }
        else if(getCachedParameters()->m_isClipping != m_parameters->m_isClipping)
        {
#if defined(__IOS__) && defined(GL_APPLE_clip_distance)
            ieDisable(GL_CLIP_DISTANCE0_APPLE);
#else
            ieDisable(GL_CLIP_DISTANCE0);
#endif
            getCachedParameters()->m_isClipping = m_parameters->m_isClipping;
        }
    }
    
    void CMaterial::unbind(void)
    {
        assert(m_parameters != nullptr);
        assert(m_parameters->m_shader != nullptr);
        m_parameters->m_shader->unbind();
    }
    
#if defined(__EDITOR__)
    
    void CMaterial::setEnabled(bool value)
    {
        m_isEnabled = value;
    }
    
    bool CMaterial::getEnabled(void) const
    {
        return m_isEnabled;
    }

}