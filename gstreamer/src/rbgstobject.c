
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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
 *  Class: Gst::Object
 *
 *  Basis for the GST object hierarchy.
 */

/*
 *  Method: flags -> aFixnum
 *
 *  Gets the entire set of flags for the object, as a Fixnum.
 */ 
static VALUE rb_gst_object_get_flags(self)
    VALUE self;
{
    GstObject *object = RGST_OBJECT(self);
    return INT2FIX(object->flags);
}

/*
 *  Method: flag?(aFixnum) -> aBoolean
 *
 *  Checks if the given flag is set on the object.
 */
static VALUE rb_gst_object_is_flag_set(self, flag)
    VALUE self, flag;
{
    GstObject *object = RGST_OBJECT(self);
    return CBOOL2RVAL(GST_FLAG_IS_SET(object, FIX2INT(flag)));
}

/*
 *  Method: set_flag(aFixnum) -> self 
 *
 *  Sets the given flag. 
 */
static VALUE rb_gst_object_set_flag(self, flag)
    VALUE self, flag;
{
    GstObject *object = RGST_OBJECT(self);
    GST_FLAG_SET(object, FIX2INT(flag));
    return self;
}

/*
 *  Method: unset_flag(aFixnum) -> self 
 *
 *  Unsets the given flag. 
 */
static VALUE rb_gst_object_unset_flag(self, flag)
    VALUE self, flag;
{
    GstObject *object = RGST_OBJECT(self);
    GST_FLAG_UNSET(object, FIX2INT(flag));
    return self;
}

/*
 *  Constant: FLAG_DESTROYED
 *  The object is flagged for destruction.
 */
static VALUE constFlagDestroyed = INT2FIX(GST_DESTROYED);

/*
 *  Constant: FLAG_FLOATING 
 *  The object is created but has no parent yet to manage it.
 */
static VALUE constFlagFloating = INT2FIX(GST_FLOATING);

/*
 *  Method: destroyed? -> aBoolean
 *
 *  Checks if the Gst::Object::FLAG_DESTROYED flag is set on the object.
 */
static VALUE rb_gst_object_is_destroyed(self)
    VALUE self;
{
    return rb_gst_object_is_flag_set(self, constFlagDestroyed);
}

/*
 *  Method: floating? -> aBoolean
 *
 *  Checks if the Gst::Object::FLAG_FLOATING flag is set on the object.
 */
static VALUE rb_gst_object_is_floating(self)
    VALUE self;
{
    return rb_gst_object_is_flag_set(self, constFlagFloating);
}

void Init_gst_object(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_OBJECT, "Object", mGst);   
 
    /*
     *  Flags
     */

    rb_define_method(c, "flags",      rb_gst_object_get_flags, 0);
    rb_define_method(c, "flag?",      rb_gst_object_is_flag_set, 1);
    rb_define_method(c, "set_flag",   rb_gst_object_set_flag,   1);
    rb_define_method(c, "unset_flag", rb_gst_object_unset_flag, 1);

    rb_define_method(c, "destroyed?", rb_gst_object_is_destroyed, 0);
    rb_define_method(c, "floating?",  rb_gst_object_is_floating,  0);
    
    rb_define_const(c, "FLAG_DESTROYED", constFlagDestroyed);
    rb_define_const(c, "FLAG_FLOATING",  constFlagFloating);
}

