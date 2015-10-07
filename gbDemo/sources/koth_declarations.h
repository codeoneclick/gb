//
//  koth_declarations.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/7/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef koth_declarations_h
#define koth_declarations_h

namespace koth
{
    class level;
    typedef std::shared_ptr<level> level_shared_ptr;
    
    class game_object_navigator;
    typedef std::shared_ptr<game_object_navigator> game_object_navigator_shared_ptr;
};

#endif
