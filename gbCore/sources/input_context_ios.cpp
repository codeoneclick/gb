//
//  input_context_ios.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/31/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "input_context.h"
#include "ogl_window.h"

#if defined(__IOS__)

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

@interface input_hwnd : UIView

@property(nonatomic, unsafe_unretained) gb::input_context* m_context;

@end

@implementation input_hwnd

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gr_pressed(glm::ivec2(point.x, point.y), gb::e_input_element_mouse_left);
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gr_dragged(glm::ivec2(point.x, point.y), gb::e_input_element_mouse_left);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gr_released(glm::ivec2(point.x, point.y), gb::e_input_element_mouse_left);
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gr_released(glm::ivec2(point.x, point.y), gb::e_input_element_mouse_left);
    }
}

@end

namespace gb
{
    class input_context_ios : public input_context
    {
    private:
        
    protected:
        
    public:
        
        input_context_ios(const std::shared_ptr<ogl_window>& window);
        ~input_context_ios();
    };
    
    std::shared_ptr<input_context> create_input_context_ios(const std::shared_ptr<ogl_window>& window)
    {
        return std::make_shared<input_context_ios>(window);
    };
    
    input_context_ios::input_context_ios(const std::shared_ptr<ogl_window>& window)
    {
        UIView* view = (__bridge UIView*)window->get_hwnd();
        
        input_hwnd* input_view = [[input_hwnd alloc] init];
        input_view.m_context = this;
        input_view.frame = CGRectMake(0.f, 0.f,
                                     view.frame.size.width, view.frame.size.height);
        [view addSubview:input_view];
    }
    
    input_context_ios::~input_context_ios()
    {
        
    }
}
#endif


