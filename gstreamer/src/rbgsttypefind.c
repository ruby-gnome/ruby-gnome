
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
 *  This class is not documented.
 */

static VALUE signal_have_type(num, values)
    guint num;
    GValue *values;
{
    VALUE args;
    
    assert(num == 2);
    args = rb_ary_new();
    
    rb_ary_push(args, GVAL2RVAL(&values[0]));
    rb_ary_push(args, RGST_CAPS_NEW(g_value_get_pointer(&values[1])));
    return args;
}

void Init_gst_typefind(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_TYPE_FIND, "TypeFind", mGst);
    G_DEF_SIGNAL_FUNC(c, "have_type", signal_have_type);
}

