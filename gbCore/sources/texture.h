//
//  texture.h
//  gbCore
//
//  Created by Sergey Sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include "resource.h"

namespace gb
{
    struct texture_transfering_data : public resource_transfering_data
    {
        ui32 m_texture_id;
        ui32 m_width;
        ui32 m_height;
        ui8* m_data;
        GLenum m_format;
        ui32 m_bpp;
        ui32 m_mips;
        bool m_compressed;
        
        texture_transfering_data(void);
        ~texture_transfering_data(void);
    };
    
    class texture : public resource
    {
    private:
        
        std::shared_ptr<texture_transfering_data> m_data;
        
        mutable ui32 m_setted_wrap_mode;
        ui32 m_presetted_wrap_mode;
        
        mutable ui32 m_setted_mag_filter;
        ui32 m_presetted_mag_filter;
        
        mutable ui32 m_setted_min_filter;
        ui32 m_presseted_min_filter;
        
    protected:
        
        void on_transfering_data_serialized(const std::shared_ptr<resource_transfering_data>& data);
        void on_transfering_data_commited(const std::shared_ptr<resource_transfering_data>& data);
        
    public:
        
        texture(const std::string& guid);
        static std::shared_ptr<texture> construct(const std::string& guid,
                                                  ui32 texture_id,
                                                  ui32 width,
                                                  ui32 height);
        ~texture(void);
        
        ui32 get_width(void) const;
        ui32 get_height(void) const;
        
        const ui8* get_data(void) const;
        ui32 get_texture_id(void) const;
        
        GLenum get_format(void) const;
        ui32 get_bpp(void) const;
        ui32 get_num_mips(void) const;
        
        bool is_compressed(void) const;
        
        void set_wrap_mode(ui32 wrap_mode);
        void set_mag_filter(ui32 mag_filter);
        void set_min_filter(ui32 min_filter);
        
        virtual void bind(void) const;
        virtual void unbind(void) const;
    };
};

#endif
