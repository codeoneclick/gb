//
//  resource_loader.h
//  gbCore
//
//  Created by sergey.sergeev on 8/24/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef resource_loader_h
#define resource_loader_h

#include "main_headers.h"
#include "declarations.h"
#include "game_loop.h"

namespace gb
{
    class resource_loader : public game_loop_interface
    {
    private:
        
    protected:
        
        std::unordered_map<std::string, resource_loading_operation_shared_ptr> m_operationsQueue;
        std::unordered_map<std::string, resource_shared_ptr> m_resources;
        
        std::mutex m_mutex;
        std::thread m_thread;
        
        void onGameLoopUpdate(f32 deltatime);
        void onThreadUpdate(void);
        
        void _OnGameLoopUpdate(f32 _deltatime);
        
    public:
        
        resource_loader(void);
        ~resource_loader(void);
        
        CSharedTexture startTextureLoadingOperation(const std::string& filename, bool sync = false);
        
        CSharedShader startShaderLoadingOperation(const std::string& vsFilename,
                                                  const std::string& fsFilename);
        
        CSharedMesh startMeshLoadingOperation(const std::string& filename);
        
        CSharedAnimationSequence startAnimationSequenceLoadingOperation(const std::string& filename);
        
        void removeLoadingDependecies(ISharedResourceLoadingHandler handler);
        
        void unloadResource(ISharedResourceRef resource);
    };

};

#endif
