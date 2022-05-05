#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_rasterizer_outline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_outline_image.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGR24 
//#define AGG_RGB24
//#define AGG_BGRA32 
//#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"


enum flip_y_e { flip_y = true };


class spiral
{
public:
    spiral(double x, double y, double r1, double r2, double step, double start_angle=0) :
        m_x(x), 
        m_y(y), 
        m_r1(r1), 
        m_r2(r2), 
        m_step(step), 
        m_start_angle(start_angle),
        m_angle(start_angle),
        m_da(agg::deg2rad(8.0)),
        m_dr(m_step / 45.0)
    {
    }

    void rewind(unsigned) 
    { 
        m_angle = m_start_angle; 
        m_curr_r = m_r1; 
        m_start = true; 
    }

    unsigned vertex(double* x, double* y)
    {
        if(m_curr_r > m_r2) return agg::path_cmd_stop;

        *x = m_x + cos(m_angle) * m_curr_r;
        *y = m_y + sin(m_angle) * m_curr_r;
        m_curr_r += m_dr;
        m_angle += m_da;
        if(m_start) 
        {
            m_start = false;
            return agg::path_cmd_move_to;
        }
        return agg::path_cmd_line_to;
    }

private:
    double m_x;
    double m_y;
    double m_r1;
    double m_r2;
    double m_step;
    double m_start_angle;

    double m_angle;
    double m_curr_r;
    double m_da;
    double m_dr;
    bool   m_start;
};


struct roundoff
{
    void transform(double* x, double* y) const
    { 
        *x = floor(*x);
        *y = floor(*y);
    }
};


class the_application : public agg::platform_support
{
    double m_start_angle;


public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_primitives<renderer_base> renderer_prim;
    typedef agg::rasterizer_outline<renderer_prim> rasterizer_outline;
    
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_start_angle(0.0)
    {
		
    }



    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        renderer_base ren_base(pf);
        renderer_prim ren_prim(ren_base);
		rasterizer_outline ras_al(ren_prim);
	
		spiral s1(width() / 5, height() / 4 + 50, 5, 70, 8, m_start_angle);
		roundoff rn;
		agg::conv_transform<spiral, roundoff> trans(s1, rn);
		ren_prim.line_color(agg::rgba(0.4, 0.3, 0.1));
		ras_al.add_path(trans);        
    }


    virtual void on_idle()
    {
    }

    virtual void on_ctrl_change()
    {
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Line Join");

    if(app.init(500, 450, 0))
    {
        return app.run();
    }

    return 1;
}


