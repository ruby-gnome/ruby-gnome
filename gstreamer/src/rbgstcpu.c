
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/*
 *  Module: Gst::Cpu
 *
 *  This module is typically used to enable special optimisations based on the 
 *  features of the CPU.
 */

/*
 *  Class method: has_mmx? -> aBoolean
 *
 *  Checks if the CPU is mmx enabled.
 */
VALUE rb_gst_cpu_is_mmx(self)
    VALUE self;
{
    return CBOOL2RVAL((gst_cpu_get_flags() & GST_CPU_FLAG_MMX)); 
}

/*
 *  Class method: has_sse? -> aBoolean
 *
 *  Checks if the CPU is sse enabled.
 */
VALUE rb_gst_cpu_is_sse(self)
    VALUE self;
{
    return CBOOL2RVAL((gst_cpu_get_flags() & GST_CPU_FLAG_SSE)); 
}

/*
 *  Class method: has_mmxext? -> aBoolean
 *
 *  Checks if the CPU has extended mmx instructions.
 */
VALUE rb_gst_cpu_is_mmxext(self)
    VALUE self;
{
    return CBOOL2RVAL((gst_cpu_get_flags() & GST_CPU_FLAG_MMXEXT)); 
}

/*
 *  Class method: has_3dnow? -> aBoolean
 *
 *  Checks if the CPU is 3DNOW enabled.
 */
VALUE rb_gst_cpu_is_3dnow(self)
    VALUE self;
{
    return CBOOL2RVAL((gst_cpu_get_flags() & GST_CPU_FLAG_3DNOW)); 
}

/*
 *  Creates the Gst::Cpu module 
 */
void Init_gst_cpu(void) {
    VALUE m = rb_define_module_under(mGst, "Cpu");
    rb_define_module_function(m, "has_mmx?",    rb_gst_cpu_is_mmx,    0);
    rb_define_module_function(m, "has_sse?",    rb_gst_cpu_is_sse,    0);
    rb_define_module_function(m, "has_mmxext?", rb_gst_cpu_is_mmxext, 0);
    rb_define_module_function(m, "has_3dnow?",  rb_gst_cpu_is_3dnow,  0);
}

