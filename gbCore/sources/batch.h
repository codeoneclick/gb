//
//  batch.h
//  gbCore
//
//  Created by sergey.sergeev on 10/28/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef batch_h
#define batch_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class batch
    {
    private:
        
    protected:
    
        mesh_shared_ptr m_mesh;
        std::vector<std::pair<mesh_shared_ptr, glm::mat4>> m_data;
        
        i32 m_num_vertices;
        i32 m_num_indices;
        
        bool m_is_processed;
        
    public:
        
        static const ui32 k_max_num_vertices;
        static const ui32 k_max_num_indices;
        
        batch();
        ~batch();
        
        mesh_shared_ptr get_mesh() const;
        
        bool get_is_processed() const;
        void set_is_processed(bool value);
        
        void add_data(const mesh_shared_ptr& mesh, const glm::mat4& mat_m);
        
        void cleanup();
        void generate();
        
    };
};


#endif
