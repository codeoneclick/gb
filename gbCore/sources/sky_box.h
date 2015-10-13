//
//  sky_box.h
//  gbCore
//
//  Created by sergey.sergeev on 10/13/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef sky_box_h
#define sky_box_h

#include "game_object.h"

namespace gb
{
    class sky_box : public game_object
    {
    private:
        
    protected:
        
    public:
        
        sky_box();
        ~sky_box();
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};

#endif