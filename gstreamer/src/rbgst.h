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

#ifndef __RBGST_H_
#define __RBGST_H_

#include <ruby.h>
#include <gst/gst.h>
#include <gst/gsttypefind.h>

#include <rbglib.h>
#include <rbgobject.h>

/* Gst module */
extern VALUE mGst;

/* Gst::EventSeek class (needed for inheritance) */
extern VALUE cGstEventSeek;

#define RBGST_INITIALIZE(obj,gstobj) \
    (rbgst_initialize_gstobject(obj, GST_OBJECT(gstobj)))

#define GST_EVENT_MASK(o)   ((GstEventMask *) o)
#define GST_FORMAT2(o)      ((GstFormat *) o)
#define GST_INDEX_ENTRY(o)  ((GstIndexEntry *) o)
#define GST_QUERY_TYPE2(o)  ((GstQueryType *) o)
#define GST_TYPE(o)         ((GstType *) o)

#define GST_TYPE_CLOCK_ENTRY    (gst_clock_entry_get_type())
#define GST_TYPE_EVENT_MASK     (gst_event_mask_get_type())
#define GST_TYPE_FORMAT2        (gst_format_get_type2())
#define GST_TYPE_PLUGIN         (gst_plugin_get_type())
#define GST_TYPE_QUERY_TYPE2    (gst_query_type_get_type2())
#define GST_TYPE_TYPE           (gst_type_get_type())

#define RGST_BIN(o)                 (GST_BIN(RVAL2GOBJ(o)))
#define RGST_CAPS(o)                (GST_CAPS(RVAL2BOXED(o, GST_TYPE_CAPS)))
#define RGST_CLOCK(o)               (GST_CLOCK(RVAL2GOBJ(o)))
#define RGST_CLOCK_ENTRY(o)         (GST_CLOCK_ENTRY(RVAL2BOXED(o, GST_TYPE_CLOCK_ENTRY)))
#define RGST_ELEMENT(o)             (GST_ELEMENT(RVAL2GOBJ(o)))
#define RGST_ELEMENT_FACTORY(o)     (GST_ELEMENT_FACTORY(RVAL2GOBJ(o)))
#define RGST_EVENT(o)               (GST_EVENT(RVAL2BOXED(o, GST_TYPE_EVENT)))
#define RGST_EVENT_MASK(o)          (GST_EVENT_MASK(RVAL2BOXED(o, GST_TYPE_EVENT_MASK)))
#define RGST_FORMAT(o)              (GST_FORMAT2(RVAL2BOXED(o, GST_TYPE_FORMAT2)))
#define RGST_INDEX(o)               (GST_INDEX(RVAL2GOBJ(o)))
#define RGST_INDEX_ENTRY(o)         (GST_INDEX_ENTRY(RVAL2BOXED(o, GST_TYPE_INDEX_ENTRY)))
#define RGST_INDEX_FACTORY(o)       (GST_INDEX_FACTORY(RVAL2GOBJ(o)))
#define RGST_OBJECT(o)              (GST_OBJECT(RVAL2GOBJ(o)))
#define RGST_PAD(o)                 (GST_PAD(RVAL2GOBJ(o)))
#define RGST_PAD_TEMPLATE(o)        (GST_PAD_TEMPLATE(RVAL2GOBJ(o)))
#define RGST_PIPELINE(o)            (GST_PIPELINE(RVAL2GOBJ(o)))
#define RGST_PLUGIN(o)              (GST_PLUGIN(RVAL2BOXED(o, GST_TYPE_PLUGIN)))
#define RGST_PLUGIN_FEATURE(o)      (GST_PLUGIN_FEATURE(RVAL2GOBJ(o)))
#define RGST_QUERY_TYPE(o)          (GST_QUERY_TYPE2(RVAL2BOXED(o, GST_TYPE_QUERY_TYPE2)))
#define RGST_REGISTRY(o)            (GST_REGISTRY(RVAL2GOBJ(o)))
#define RGST_SCHEDULER_FACTORY(o)   (GST_SCHEDULER_FACTORY(RVAL2GOBJ(o)))
#define RGST_STRUCTURE(o)           (GST_STRUCTURE(RVAL2BOXED(o, GST_TYPE_STRUCTURE)))
#define RGST_THREAD(o)              (GST_THREAD(RVAL2GOBJ(o)))
#define RGST_XML(o)                 (GST_XML(RVAL2GOBJ(o)))

#define RGST_GOBJ_NEW(o)    (rbgst_new_gstobject(o))

#define RGST_BIN_NEW(o)                 (RGST_GOBJ_NEW(GST_BIN(o)))
#define RGST_CAPS_NEW(o)                (BOXED2RVAL(GST_CAPS(o), GST_TYPE_CAPS))
#define RGST_CLOCK_NEW(o)               (RGST_GOBJ_NEW(GST_CLOCK(o)))
#define RGST_CLOCK_ENTRY_NEW(o)         (BOXED2RVAL(GST_CLOCK_ENTRY(o), GST_TYPE_CLOCK_ENTRY))
#define RGST_ELEMENT_FACTORY_NEW(o)     (RGST_GOBJ_NEW(GST_ELEMENT_FACTORY(o)))
#define RGST_ELEMENT_NEW(o)             (RGST_GOBJ_NEW(GST_ELEMENT(o)))
#define RGST_EVENT_NEW(o)               (BOXED2RVAL(GST_EVENT(o), GST_TYPE_EVENT))
#define RGST_EVENT_MASK_NEW(o)          (BOXED2RVAL(GST_EVENT_MASK(o), GST_TYPE_EVENT_MASK))
#define RGST_FORMAT_NEW(o)              (BOXED2RVAL(GST_FORMAT2(o), GST_TYPE_FORMAT2))
#define RGST_INDEX_NEW(o)               (RGST_GOBJ_NEW(GST_INDEX(o)))
#define RGST_INDEX_ENTRY_NEW(o)         (BOXED2RVAL(GST_INDEX_ENTRY(o), GST_TYPE_INDEX_ENTRY))
#define RGST_INDEX_FACTORY_NEW(o)       (RGST_GOBJ_NEW(GST_INDEX_FACTORY(o)))
#define RGST_OBJECT_NEW(o)              (RGST_GOBJ_NEW(GST_OBJECT(o)))
#define RGST_PAD_NEW(o)                 (RGST_GOBJ_NEW(GST_PAD(o)))
#define RGST_PAD_TEMPLATE_NEW(o)        (RGST_GOBJ_NEW(GST_PAD_TEMPLATE(o)))
#define RGST_PIPELINE_NEW(o)            (RGST_GOBJ_NEW(GST_PIPELINE(o)))
#define RGST_PLUGIN_NEW(o)              (BOXED2RVAL(GST_PLUGIN(o), GST_TYPE_PLUGIN))
#define RGST_QUERY_TYPE_NEW(o)          (BOXED2RVAL(GST_QUERY_TYPE2(o), GST_TYPE_QUERY_TYPE2))
#define RGST_REGISTRY_NEW(o)            (RGST_GOBJ_NEW(GST_REGISTRY(o)))
#define RGST_SCHEDULER_FACTORY_NEW(o)   (RGST_GOBJ_NEW(GST_SCHEDULER_FACTORY(o)))
#define RGST_STRUCTURE_NEW(o)           (BOXED2RVAL(GST_STRUCTURE(o), GST_TYPE_STRUCTURE))
#define RGST_SYSTEM_CLOCK_NEW(o)        (RGST_GOBJ_NEW(GST_SYSTEM_CLOCK(o)))
#define RGST_THREAD_NEW(o)              (RGST_GOBJ_NEW(GST_THREAD(o)))

GType gst_clock_entry_get_type ();
GType gst_event_mask_get_type ();
GType gst_format_get_type2 ();
GType gst_plugin_get_type ();
GType gst_query_type_get_type2 ();
GType gst_type_get_type ();

/* misc.c interface */
void rbgst_initialize_gstobject (VALUE obj, GstObject * gstobj);
VALUE rbgst_new_gstobject (void *);
VALUE rb_str_format (char *fmt, ...);
VALUE rb_ary_yield (VALUE arr);

/* implemented in rbgstpluginfeature.c */
gboolean is_valid_pluginfeature_type (const GType type);
VALUE instanciate_pluginfeature (GstPluginFeature * feature);

#define DBL2NUM(v)      (rb_float_new(v))

/* 
 *  Ruby 1.6.8 does not support "unsigned long long" type.
 *  In this case, issue a compilation warning, and redirect the ULL2NUM
 *  macro to ULONG2NUM (unsigned long).
 */
#if !defined(ULL2NUM)
#define ULL2NUM ULONG2NUM
#define NUM2ULL NUM2ULONG
#warning \
    This Ruby has no ``unsigned long long''! \
    Using ``unsigned long instead''.  \
    Some data precision may be lost.  \
    Upgrade your Ruby to a newer version to fix this issue.
#else
#define NUM2ULL(v)      (rb_num2ull(v))
#endif

#endif                          /* __RBGST_H_ */
