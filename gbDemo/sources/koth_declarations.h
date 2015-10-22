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
    
    class level_node;
    typedef std::shared_ptr<level_node> level_node_shared_ptr;
    
    class game_object_navigator;
    typedef std::shared_ptr<game_object_navigator> game_object_navigator_shared_ptr;
    
    class character_controller;
    typedef std::shared_ptr<character_controller> character_controller_shared_ptr;
    
    class ai_character_controller;
    typedef std::shared_ptr<ai_character_controller> ai_character_controller_shared_ptr;
    
    class ai_action;
    typedef std::shared_ptr<ai_action> ai_action_shared_ptr;
    
    class ai_actions_processor;
    typedef std::shared_ptr<ai_actions_processor> ai_actions_processor_shared_ptr;
    
    class ai_move_action;
    typedef std::shared_ptr<ai_move_action> ai_move_action_shared_ptr;
};

#endif
