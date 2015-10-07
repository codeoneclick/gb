//
//  ui_to_in_game_scene_commands.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/7/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ui_to_in_game_scene_commands_h
#define ui_to_in_game_scene_commands_h

#include "main_headers.h"
#include "declarations.h"

namespace koth
{
    namespace keyboard_on_key_down
    {
        typedef std::function<void(i32)> t_command;
        const std::string guid = "keyboard_on_key_down";
    };
    
    namespace keyboard_on_key_up
    {
        typedef std::function<void(i32)> t_command;
        const std::string guid = "keyboard_on_key_up";
    };
};

#endif
