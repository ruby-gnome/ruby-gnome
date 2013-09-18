/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgst.h"

#define RG_TARGET_NAMESPACE cElementFactory

#define SELF(object) (GST_ELEMENT_FACTORY(RVAL2GOBJ(object)))

static VALUE cStaticPadTemplate;

static VALUE
rg_static_pad_templates(VALUE self)
{
    GstElementFactory *factory = SELF(self);
    const GList *templates;
    VALUE rb_templates;

    rb_templates = rb_ary_new();
    templates = gst_element_factory_get_static_pad_templates(factory);
    for (; templates; templates = g_list_next(templates)) {
        GstStaticPadTemplate *template = templates->data;
        VALUE rb_template;
        rb_template = rb_funcall(cStaticPadTemplate, rb_intern("new"), 0);
        memcpy(DATA_PTR(rb_template), template, sizeof(GstStaticPadTemplate));
        rb_ary_push(rb_templates, rb_template);
    }

    return rb_templates;
}

void
rb_gst_init_element_factory(void)
{
    VALUE mGst;
    VALUE RG_TARGET_NAMESPACE;

    mGst = rb_const_get(rb_cObject, rb_intern("Gst"));
    RG_TARGET_NAMESPACE = rb_const_get(mGst, rb_intern("ElementFactory"));
    cStaticPadTemplate = rb_const_get(mGst, rb_intern("StaticPadTemplate"));

    rb_remove_method(RG_TARGET_NAMESPACE, "static_pad_templates");
    RG_DEF_METHOD(static_pad_templates, 0);
}
