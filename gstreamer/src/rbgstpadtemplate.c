
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
 *  Class: Gst::PadTemplate < Gst::Object
 *
 *  Describe the media type of a Gst::Pad.  
 */

/*
 *  Method: name -> aString
 *
 *  Gets the name of the pad template.
 *  This overrides Gst::Object#get_name.
 */
static VALUE rb_gst_padtemplate_get_name(self)
    VALUE self;
{
    /* 
     *  We can't call Gst::Object#get_name since the real name
     *  of the pad template is in the "name_template" field of 
     *  GstPadTemplate.
     */
    GstPadTemplate *pad = RGST_PAD_TEMPLATE(self);
    return CSTR2RVAL(GST_PAD_TEMPLATE_NAME_TEMPLATE(pad));
}

/*
 *  Method: presence -> aFixnum
 *
 *  Gets presence:
 *
 *    - Gst::Pad::PRESENCE_ALWAYS;
 *    - Gst::Pad::PRESENCE_SOMETIMES;
 *    - Gst::Pad::PRESENCE_REQUEST.
 */
static VALUE rb_gst_padtemplate_get_presence(self)
    VALUE self;
{
    GstPadTemplate *pad = RGST_PAD_TEMPLATE(self);
    return INT2FIX(pad->presence);
}

/*
 *  Method: direction -> aFixnum
 *
 *  Gets direction:
 *
 *    - Gst::Pad::DIRECTION_SRC;
 *    - Gst::Pad::DIRECTION_SINK;
 *    - Gst::Pad::DIRECTION_UNKNOWN.
 */
static VALUE rb_gst_padtemplate_get_direction(self)
    VALUE self;
{
    GstPadTemplate *pad = RGST_PAD_TEMPLATE(self);
    return INT2FIX(pad->direction);
}

/*
 *  Method: caps -> anArray
 *
 *  Gets the capabilities of the pad template element, 
 *  in an array of Gst::Caps object.
 */
static VALUE rb_gst_padtemplate_get_caps(self)
    VALUE self;
{
    GstPadTemplate *pad;
    GstCaps *list;
    VALUE arr;

    pad = RGST_PAD_TEMPLATE(self);
    arr = rb_ary_new();

    for (list = pad->caps; list != NULL; list = list->next) {
        /*
         *  Increment the ref count of the Caps, since it was not
         *  created with gst_caps_new(), and it will be unref() from
         *  the GC.  
         */
        //GstCaps *caps = gst_caps_ref(list);
        rb_ary_push(arr, RGST_CAPS_NEW(list));
    }
    return arr;
}

/*
 *  Method: each_caps { |aCapsObject| block } -> nil
 *
 *  Calls the block for each capability of the pad template, 
 *  passing a reference to the Gst::Caps object as parameter.
 */
static VALUE rb_gst_padtemplate_each_caps(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_padtemplate_get_caps(self));
}

/*
 *  Method: has_caps? -> aBoolean
 *
 *  Checks if the pad template has capabilities. 
 */
static VALUE rb_gst_padtemplate_has_caps(self)
    VALUE self;
{
    GstPadTemplate *pad = RGST_PAD_TEMPLATE(self);
    return CBOOL2RVAL(pad->caps != NULL);
}

void Init_gst_padtemplate(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_PAD_TEMPLATE, "PadTemplate", mGst);

    rb_define_method(c, "name",      rb_gst_padtemplate_get_name, 0);
    rb_define_method(c, "presence",  rb_gst_padtemplate_get_presence, 0);
    rb_define_method(c, "direction", rb_gst_padtemplate_get_direction, 0);

    rb_define_method(c, "caps", rb_gst_padtemplate_get_caps, 0);
    rb_define_method(c, "each_caps", rb_gst_padtemplate_each_caps, 0);
    rb_define_method(c, "has_caps?", rb_gst_padtemplate_has_caps, 0);
}

