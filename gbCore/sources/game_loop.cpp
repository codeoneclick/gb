//
//  game_loop.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_loop.h"
#include "thread_operations_pool.h"

namespace gb
{
    game_loop_interface::game_loop_interface() :
    m_paused(false)
    {
        
    }
    
    void game_loop_interface::set_paused(bool paused)
    {
        m_paused = paused;
    }
    
    bool game_loop_interface::is_paused() const
    {
        return m_paused;
    }
    
    game_loop::~game_loop()
    {
        m_listeners.clear();
    }
    
    void game_loop::on_update()
    {
        static std::chrono::steady_clock::time_point previous_timestamp = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point current_timestamp = std::chrono::steady_clock::now();
        f32 deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - previous_timestamp).count() / 1000.0f;
        previous_timestamp = current_timestamp;
        for(const auto& listener : m_listeners)
        {
            if(!listener->is_paused())
            {
                listener->on_update(deltatime);
            }
        }
        thread_operations_pool::shared_instance()->update();
    }
    
    void game_loop::add_listener(const std::shared_ptr<game_loop_interface>& listener)
    {
        m_listeners.insert(listener);
    }
    
    void game_loop::remove_listener(const std::shared_ptr<game_loop_interface>& listener)
    {
        m_listeners.erase(listener);
    }
}