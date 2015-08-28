//
//  game_loop.h
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_loop_h
#define game_loop_h

#include "main_headers.h"

namespace gb
{
    class game_loop_interface
    {
    private:
        
        friend class game_loop;
        bool m_paused;
        
    protected:
        
        virtual void on_update(f32 deltatime) = 0;
        
    public:
        
        game_loop_interface(void);
        virtual ~game_loop_interface(void) = default;
        
        void set_paused(bool paused);
        bool is_paused(void) const;
    };
    
    class game_loop
    {
    private:
        
    protected:
        
        std::set<std::shared_ptr<game_loop_interface>> m_listeners;
        
    public:
        
        game_loop(void) = default;
        ~game_loop(void);
        
        void on_update(void);
        
        void add_listener(const std::shared_ptr<game_loop_interface>& listener);
        void remove_listener(const std::shared_ptr<game_loop_interface>& listener);
    };
    
    void add_listener_to_game_loop(const std::shared_ptr<game_loop_interface>& listener);
    void remove_listener_from_game_loop(const std::shared_ptr<game_loop_interface>& listener);
    
#if defined(__OSX__)
    
    void terminate_game_loop(void);

#endif
    
};
#endif
