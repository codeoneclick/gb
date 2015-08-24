//
//  resource_commiter.h
//  gbCore
//
//  Created by sergey.sergeev on 8/18/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef resource_commiter_h
#define resource_commiter_h

#include "main_headers.h"

namespace gb
{
    enum e_commiter_status
    {
        e_commiter_status_undefined = 0,
        e_commiter_status_in_progress,
        e_commiter_status_failure,
        e_commiter_status_success
    };
    
    class resource;
    class resource_transfering_data;
    class resource_commiter  : public std::enable_shared_from_this<resource_commiter>
    {
    private:
        
    protected:
        
        std::string m_guid;
        std::shared_ptr<resource> m_resource;
        e_commiter_status m_status;
        
        void on_transfering_data_commited(const std::shared_ptr<resource_transfering_data>& data);
        
    public:
        
        resource_commiter(const std::string& guid, const std::shared_ptr<resource>& resource);
        virtual ~resource_commiter(void);
        
        std::string get_guid(void) const;
        e_commiter_status get_status(void) const;
        
        virtual void commit(void) = 0;
    };
}

#endif
