/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Sjoerd Simons <sjoerd@luon.net>
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cPipeline
#define SELF(obj) (RVAL2GST_PIPELINE(obj))

/* Class: Gst::Pipeline
 * In almost all cases, you'll want to use a Gst::Pipeline when creating a 
 * filter graph. The Gst::Pipeline will manage all the scheduling issues, 
 * including threading, as well as provide simple interfaces to common 
 * functions, like 'Play'.
 */

/*
 * Class method: new(name=nil)
 * name: a name for the pipeline.
 *
 * Constructs a new Gst::Pipeline object.
 *
 * If element name is ommited (or nil), then the pipeline will receive a guaranteed
 * unique name, consisting of the "pipeline" string and a number.
 * If name is given, it will be given the name supplied.
 *
 * Returns: a newly constructed Gst::Pipeline.
 */
static VALUE
rg_initialize (int argc, VALUE *argv, VALUE self)
{
    GstElement *bin;
    VALUE name;

    rb_scan_args (argc, argv, "01", &name);

    bin = gst_pipeline_new (RVAL2CSTR_ACCEPT_NIL(name));
    if (bin != NULL)
        RBGST_INITIALIZE (self, bin);
    return Qnil;
}

static VALUE
rg_bus(VALUE self)
{
    GstBus *bus;
    VALUE ret;

    bus = gst_pipeline_get_bus(SELF(self));
    if (bus == NULL)
      return Qnil;

    ret = GST_BUS2RVAL_UNREF(bus);

    /* Make the bus a child of the pipeline as their (minimum) livetimes are
     * bound together.. A pipeline can potentially change it's bus though, but
     * we have no way to catch that case */
    G_CHILD_ADD(self, ret);

    return ret;
}

void
Init_gst_pipeline(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_PIPELINE, "Pipeline", mGst);

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(bus, 0);
}
