/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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
 * Since GstStructure is basically a hash it is more natural to
 * automatically convert it as a Ruby Hash object, and vice-versa.
 */

static gboolean
foreach_pair (GQuark field_id, GValue *value, gpointer user_data)
{
    VALUE hash;
    
    hash = *(VALUE *) user_data;
    rb_hash_aset (hash, CSTR2RVAL (g_quark_to_string (field_id)), 
                  GVAL2RVAL (value));
    
    return TRUE;
}

VALUE gst_structure_to_ruby_hash (GstStructure *gst_struct)
{
    VALUE hash;
    
    hash = rb_hash_new ();
    gst_structure_foreach (gst_struct, foreach_pair, &hash);
    return hash;
}

GstStructure *ruby_hash_to_gst_structure_with_name (VALUE hash, 
                                                    const char *name)
{
    GstStructure *gst_struct;
    VALUE ary;
    int i;
    
    Check_Type (hash, T_HASH);

    gst_struct = gst_structure_empty_new (name);
    ary = rb_funcall (hash, rb_intern ("to_a"), 0);
    for (i = 0; i < RARRAY (ary)->len; i++) {
        VALUE pair = RARRAY (ary)->ptr[i];
        GValue value = { 0, };
    
        g_value_init (&value, RVAL2GTYPE (RARRAY (pair)->ptr[1]));
        rbgobj_rvalue_to_gvalue (RARRAY (pair)->ptr[1], &value);
        gst_structure_set_value (gst_struct,
                                 RVAL2CSTR (RARRAY (pair)->ptr[0]),
                                 &value);
        g_value_unset (&value);
    }
    
    return gst_struct; 
}

GstStructure *ruby_hash_to_gst_structure (VALUE hash)
{
    return ruby_hash_to_gst_structure_with_name (hash, "");
}
