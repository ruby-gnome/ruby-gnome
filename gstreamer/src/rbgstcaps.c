
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
 *  Class: Gst::Caps
 *
 *  Capabilities of pads. 
 */

/*
 *  Method: name -> aString
 *
 *  Gets the name of the caps, as a String.
 */
static VALUE rb_gst_caps_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gst_caps_get_name(RGST_CAPS(self)));
}

/*
 *  Method: type_id -> aFixnum
 *
 *  Gets the type ID of the caps, as a Fixnum.
 */
static VALUE rb_gst_caps_get_type_id(self)
    VALUE self;
{
    return INT2FIX(gst_caps_get_type_id(RGST_CAPS(self)));
}

/*
 *  Method: == anOtherCaps -> aBoolean
 *
 *  Checks if two Gst::Caps objects are refering the same caps. 
 */
static VALUE rb_gst_caps_is_equal(self, other_caps)
    VALUE self, other_caps;
{
   return NIL_P(other_caps)
        ? Qfalse
        : rb_equal(rb_gst_caps_get_type_id(self),
                   rb_gst_caps_get_type_id(other_caps));
}

/*
 *  Method: fixed? -> aBoolean
 *
 *  Checks if the GstCaps has fixed properties, ie. 
 *  it has no ranges or lists.
 */
static VALUE rb_gst_caps_is_fixed(self)
    VALUE self;
{
    return CBOOL2RVAL(GST_CAPS_IS_FIXED(RGST_CAPS(self)));
}

/*
 *  Method: has_property?(aString) -> aBoolean
 *
 *  Checks if the Gst::Caps object has a property 
 *  with the given name.
 */
static VALUE rb_gst_caps_has_property(self, name)
    VALUE self, name;
{
    return CBOOL2RVAL(gst_caps_has_property(RGST_CAPS(self), 
                                         RVAL2CSTR(name)));
}

/*
 *  Method: has_fixed_property?(aString) -> aBoolean
 *
 *  Checks if the Gst::Caps object has a fixed property 
 *  with the given name.
 */
static VALUE rb_gst_caps_has_fixed_property(self, name)
    VALUE self, name;
{
    return CBOOL2RVAL(gst_caps_has_fixed_property(RGST_CAPS(self), 
                                               RVAL2CSTR(name)));
}

/*
 *  Converts accordingly a GstPropsEntry to a Ruby object.
 */
static VALUE rb_prop(prop)
    GstPropsEntry *prop;
{
    GstPropsType type;

    assert (prop != NULL);
    type = gst_props_entry_get_type(prop);
    switch (type) {
        case GST_PROPS_INT_TYPE:
        {
             gint val;
             gst_props_entry_get_int (prop, &val);
             return INT2FIX(val);
        }
        case GST_PROPS_INT_RANGE_TYPE:
        {
            gint min, max;
            gst_props_entry_get_int_range (prop, &min, &max);
            return rb_range_new(INT2FIX(min), INT2FIX(max), 0);
        }
        case GST_PROPS_FLOAT_TYPE:
        {
            gfloat val;
            gst_props_entry_get_float (prop, &val);
            return DBL2NUM(val);
        }
        case GST_PROPS_FLOAT_RANGE_TYPE:
        {
            gfloat min, max;
            gst_props_entry_get_float_range (prop, &min, &max);
            return rb_range_new(DBL2NUM(min), DBL2NUM(max), 0);
        }
        case GST_PROPS_BOOLEAN_TYPE:
        {
            gboolean val;
            gst_props_entry_get_boolean (prop, &val);
            return CBOOL2RVAL(val);
        }
        case GST_PROPS_STRING_TYPE:
        {
            const gchar *val;
            gst_props_entry_get_string (prop, &val);
            return CSTR2RVAL(val);
        }
        case GST_PROPS_FOURCC_TYPE:
        {
            guint32 val;
            gst_props_entry_get_fourcc_int (prop, &val);
            return ULL2NUM(val);
        }
        case GST_PROPS_LIST_TYPE:
        case GST_PROPS_GLIST_TYPE:
        {
            const GList *list;
            VALUE arr;      

            gst_props_entry_get_list(prop, &list);
            arr = rb_ary_new();
            while (list != NULL) {
                rb_ary_push(arr, rb_prop((GstPropsEntry*) list->data));
                list = g_list_next (list);
            }
            return arr;
        }
        /*
         *  These enumeration values are not used for conversion, but they 
         *  must anyway be handled here to avoid compiler warnings.
         */
        case GST_PROPS_END_TYPE:
        case GST_PROPS_INVALID_TYPE:
        case GST_PROPS_VAR_TYPE:
        case GST_PROPS_LAST_TYPE:
            break;
    }
    rb_raise(rb_eRuntimeError, "Invalid GstPropsEntry type (%d)", type);
    return Qnil;
}

/*
 *  Method: each_property { |name, value, isFixed| block } -> nil 
 *
 *  Calls the block for each property of the Gst::Caps, passing references
 *  of name (String), value (an Object), and fixed state (a Boolean)
 *  as parameters.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_caps_each_property(self)
    VALUE self;
{
    GstCaps *caps;
    GstProps *props;
    GList *list;

    if (rb_block_given_p() == Qfalse) {
        rb_raise(rb_eArgError, "No block given");
    }
    
    caps  = RGST_CAPS(self);
    props = gst_caps_get_props(caps);
    if (props != NULL) {
        for (list = props->properties; 
             list != NULL; 
             list = g_list_next(list)) 
        {
            GstPropsEntry *prop;
            VALUE params;

            prop   = (GstPropsEntry *) list->data;
            params = rb_ary_new();
            rb_ary_push(params, CSTR2RVAL(gst_props_entry_get_name(prop)));  
            rb_ary_push(params, rb_prop(prop));
            rb_ary_push(params, CBOOL2RVAL(gst_props_entry_is_fixed(prop)));
            rb_yield(params);
        }
    }
    return Qnil;
}

/*
 *  Method: mime -> aString
 *
 *  Get the mime type of the caps, as a String.
 */
static VALUE rb_gst_caps_get_mime(self)
    VALUE self;
{
    return CSTR2RVAL(gst_caps_get_mime(RGST_CAPS(self)));
}

/*
 *  Method: set_mime(aMimeString) -> self
 *
 *  Set the mime type of the caps, as a String.
 */
static VALUE rb_gst_caps_set_mime(self, mime)
    VALUE self, mime;
{
    gst_caps_set_mime(RGST_CAPS(self), RVAL2CSTR(mime));
    return self;
}

void Init_gst_caps(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_CAPS, "Caps", mGst);

    rb_define_method(c, "name", rb_gst_caps_get_name, 0);
    rb_define_method(c, "type_id", rb_gst_caps_get_type_id, 0);
    rb_define_method(c, "==", rb_gst_caps_is_equal, 1);
    rb_define_method(c, "fixed?", rb_gst_caps_is_fixed, 0);
    rb_define_method(c, "each_property",  rb_gst_caps_each_property, 0);
    rb_define_method(c, "has_property?",  rb_gst_caps_has_property, 1);
    rb_define_method(c, "has_fixed_property?",  rb_gst_caps_has_fixed_property, 1);
    rb_define_method(c, "mime",     rb_gst_caps_get_mime, 0);
    rb_define_method(c, "set_mime", rb_gst_caps_set_mime, 1);

    G_DEF_SETTERS(c);
}

