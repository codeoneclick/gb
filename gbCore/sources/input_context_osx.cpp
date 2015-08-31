//
//  input_context_osx.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/31/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "input_context.h"
#include "ogl_window.h"

#if defined(__OSX__)

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface input_hwnd : NSView

@property(nonatomic, unsafe_unretained) gb::input_context* m_context;

@end

@implementation input_hwnd

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        
    }
    return self;
}

- (void)mouseDown:(NSEvent*)event
{
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self window] makeFirstResponder:self];
    
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_pressed(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_left);
}

- (void)rightMouseDown:(NSEvent *)event;
{
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self window] makeFirstResponder:self];
    
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_pressed(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_right);
}

- (void)mouseUp:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_released(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_left);
}

- (void)rightMouseUp:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_released(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_right);
}

- (void)mouseDragged:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_dragged(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_left);
}

- (void)rightMouseDragged:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_dragged(glm::ivec2(point.x, self.frame.size.height - point.y), gb::e_input_element_mouse_right);
}

- (void)mouseMoved:(NSEvent *)event
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gr_moved(glm::ivec2(point.x, self.frame.size.height - point.y));
}

@end

namespace gb
{
    class input_context_osx : public input_context
    {
    private:
        
    protected:
        
    public:
        
        input_context_osx(const std::shared_ptr<ogl_window>& window);
        ~input_context_osx();
    };
    
    std::shared_ptr<input_context> create_input_context_osx(const std::shared_ptr<ogl_window>& window)
    {
        return std::make_shared<input_context_osx>(window);
    };
    
    input_context_osx::input_context_osx(const std::shared_ptr<ogl_window>& window)
    {
        NSView* view = (__bridge NSView*)window->get_hwnd();
        
        input_hwnd* input_view = [[input_hwnd alloc] init];
        input_view.m_context = this;
        input_view.frame = CGRectMake(0.f, 0.f,
                                      view.frame.size.width, view.frame.size.height);
        [view addSubview:input_view];
    }
    
    input_context_osx::~input_context_osx()
    {
        
    }
}

#endif