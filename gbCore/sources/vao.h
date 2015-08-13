//
//  vao.h
//  gbCore
//
//  Created by sergey.sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef vao_h
#define vao_h

#include "shader.h"

namespace gb
{
    class vbo;
    class ibo;
    class vao
    {
    private:
        
    protected:
        
        ui32 m_handle;
        
        std::shared_ptr<vbo> m_vbo;
        std::shared_ptr<ibo> m_ibo;
        
    public:
        
        vao(const std::shared_ptr<vbo>& vbo,
            const std::shared_ptr<ibo>& ibo);
        ~vao(void);
        
        void init(const std::array<i32, e_shader_attribute_max>& attributes);
        
        static void bind(const std::shared_ptr<vao>& state);
        static void unbind(void);
    };
};
#endif
