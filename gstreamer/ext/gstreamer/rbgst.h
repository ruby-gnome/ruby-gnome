/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Ruby-GNOME2 Project
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

#ifndef __RBGST_H_
#define __RBGST_H_

#include <ruby.h>
#include <gst/gst.h>
#include <gst/gsttypefind.h>

#include <rbglib.h>
#include <rbgobject.h>

/* Gst module */
extern VALUE mGst;

extern VALUE rb_cGstMiniObject;

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
#define GST_TYPE_QUERY_TYPE2    (gst_query_type_get_type2())
#define GST_TYPE_TYPE           (gst_type_get_type())

#define RVAL2GST_OBJ(obj)       (GST_OBJECT(RVAL2GOBJ(obj)))
#define GST_OBJ2RVAL(obj)       (GOBJ2RVAL(obj))
#define RVAL2GST_MINI_OBJ(obj)  (GST_MINI_OBJECT(RVAL2GOBJ(obj)))
#define RVAL2GST_BUF(obj)       (GST_BUFFER(RVAL2GOBJ(obj)))
#define RVAL2GST_MSG(obj)       (GST_MESSAGE(RVAL2GOBJ(obj)))
#define RVAL2GST_ELEMENT(obj)   (GST_ELEMENT(RVAL2GOBJ(obj)))
#define RVAL2GST_EVENT(obj)     (GST_EVENT(RVAL2GOBJ(obj)))
#define GST_ELEMENT2RVAL(obj)   (GOBJ2RVAL(obj))
#define RVAL2GST_ELEMENT_FACTORY(obj) \
                                (GST_ELEMENT_FACTORY(RVAL2GOBJ(obj)))
#define RVAL2GST_INDEX_FACTORY(obj) \
                                (GST_INDEX_FACTORY(RVAL2GOBJ(obj)))
#define RVAL2GST_TYPE_FIND_FACTORY(obj) \
                                (GST_TYPE_FIND_FACTORY(RVAL2GOBJ(obj)))
#define RVAL2GST_PLUGIN_FEATURE(obj) \
                                (GST_PLUGIN_FEATURE(RVAL2GOBJ(obj)))
#define RVAL2GST_PLUGIN(obj)    (GST_PLUGIN(RVAL2GOBJ(obj)))

#define RVAL2GST_CAPS(obj)      (GST_CAPS(RVAL2BOXED(obj, GST_TYPE_CAPS)))
#define GST_CAPS2RVAL(obj)      (BOXED2RVAL(obj, GST_TYPE_CAPS))
#define GST_CAPS2RVAL_UNREF(obj) \
    (rbgst_ruby_object_from_gst_caps_with_unref((obj)))
#define RVAL2GST_STATIC_CAPS(obj) ((GstStaticCaps *)RVAL2GPTR(obj))
#define GST_STATIC_CAPS2RVAL(obj) (GPTR2RVAL(obj, GST_TYPE_STATIC_CAPS))

#define RVAL2GST_STATE(obj)     (RVAL2GENUM(obj, GST_TYPE_STATE))
#define GST_STATE2RVAL(obj)     (GENUM2RVAL(obj, GST_TYPE_STATE))
#define RVAL2GST_FORMAT(obj)    (RVAL2GENUM(obj, GST_TYPE_FORMAT))
#define GST_FORMAT2RVAL(obj)    (GENUM2RVAL(obj, GST_TYPE_FORMAT))

#define RVAL2GST_STRUCT(obj)    (GST_STRUCTURE(RVAL2BOXED(obj, GST_TYPE_STRUCTURE)))
#define GST_STRUCT2RVAL(obj)    (BOXED2RVAL(obj, GST_TYPE_STRUCTURE))

#define RVAL2GST_PAD(obj)       (GST_PAD(RVAL2GOBJ(obj)))
#define GST_PAD2RVAL(obj)       (GOBJ2RVAL(obj))
#define GST_PAD2RVAL_UNREF(obj) (GOBJ2RVAL_UNREF(obj))

#define RVAL2GST_PAD_TEMPLATE(obj) (GST_PAD_TEMPLATE(RVAL2GOBJ(obj)))
#define GST_PAD_TEMPLATE2RVAL(obj) (GOBJ2RVAL(obj))
#define RVAL2GST_STATIC_PAD_TEMPLATE(obj) ((GstStaticPadTemplate *)(RVAL2GPTR(obj)))
#define GST_STATIC_PAD_TEMPLATE2RVAL(obj) (GPTR2RVAL(obj, GST_TYPE_STATIC_PAD_TEMPLATE))

#define RVAL2GST_BUS(obj)           (GST_BUS(RVAL2GOBJ(obj)))
#define GST_BUS2RVAL(obj)           (GOBJ2RVAL(obj))
#define GST_BUS2RVAL_UNREF(obj)     (GOBJ2RVAL_UNREF(obj))
#define RVAL2GST_PIPELINE(obj)      (GST_PIPELINE(RVAL2GOBJ(obj)))
#define RVAL2GST_CLOCK(obj)         (GST_CLOCK(RVAL2GOBJ(obj)))
#define GST_CLOCK2RVAL(obj)         (GOBJ2RVAL(obj))

#define RVAL2GST_BIN(obj)           (GST_BIN(RVAL2GOBJ(obj)))
#define GST_BIN2RVAL(obj)           (GOBJ2RVAL(obj))

#define RVAL2GST_QUERY(obj)         (GST_QUERY(RVAL2GOBJ(obj)))
#define GST_QUERY2RVAL(obj)         (GOBJ2RVAL(obj))



#define RGST_BIN(o)                 (GST_BIN(RVAL2GOBJ(o)))
#define RGST_CAPS(o)                (GST_CAPS(RVAL2BOXED(o, GST_TYPE_CAPS)))
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
#define RGST_PIPELINE(o)            (RVAL2GST_PIPELINE(o))
#define RGST_PLUGIN(o)              (GST_PLUGIN(RVAL2GOBJ(o)))
#define RGST_PLUGIN_FEATURE(o)      (GST_PLUGIN_FEATURE(RVAL2GOBJ(o)))
#define RGST_QUERY_TYPE(o)          (GST_QUERY_TYPE2(RVAL2BOXED(o, GST_TYPE_QUERY_TYPE2)))
#define RGST_REGISTRY(o)            (GST_REGISTRY(RVAL2GOBJ(o)))
#define RGST_SCHEDULER(o)           (GST_SCHEDULER(RVAL2GOBJ(o)))
#define RGST_SCHEDULER_FACTORY(o)   (GST_SCHEDULER_FACTORY(RVAL2GOBJ(o)))
#define RGST_TAG_SETTER(o)          (GST_TAG_SETTER(RVAL2GOBJ(o)))
#define RGST_THREAD(o)              (GST_THREAD(RVAL2GOBJ(o)))
#define RGST_TYPE_FIND_FACTORY(o)   (GST_TYPE_FIND_FACTORY(RVAL2GOBJ(o)))
#define RGST_XML(o)                 (GST_XML(RVAL2GOBJ(o)))

#define RGST_GOBJ_NEW(o)    (rbgst_new_gstobject(o))
#define GOBJ2RGST(obj)        (RGST_GOBJ_NEW(obj))

#define GST_MINI_OBJ2RVAL(obj) (rbgst_mini_object_to_ruby_object(obj))

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
#define RGST_PLUGIN_NEW(o)              (RGST_GOBJ_NEW(GST_PLUGIN(o)))
#define RGST_PLUGIN_FEATURE_NEW(o)      (RGST_GOBJ_NEW(GST_PLUGIN_FEATURE(o)))
#define RGST_QUERY_TYPE_NEW(o)          (BOXED2RVAL(GST_QUERY_TYPE2(o), GST_TYPE_QUERY_TYPE2))
#define RGST_REGISTRY_NEW(o)            (RGST_GOBJ_NEW(GST_REGISTRY(o)))
#define RGST_SCHEDULER_NEW(o)           (RGST_GOBJ_NEW(GST_SCHEDULER(o)))
#define RGST_SCHEDULER_FACTORY_NEW(o)   (RGST_GOBJ_NEW(GST_SCHEDULER_FACTORY(o)))
#define RGST_SYSTEM_CLOCK_NEW(o)        (RGST_GOBJ_NEW(GST_SYSTEM_CLOCK(o)))
#define RGST_THREAD_NEW(o)              (RGST_GOBJ_NEW(GST_THREAD(o)))
#define RGST_TYPE_FIND_FACTORY_NEW(o)   (RGST_GOBJ_NEW(GST_TYPE_FIND_FACTORY(o)))

#define RGST_BUS2RVAL(obj) (GOBJ2RGST(obj))
#define GST_MSG2RVAL(obj) (GOBJ2RGST(obj))

extern VALUE rbgst_ruby_object_from_gst_caps_with_unref(GstCaps *caps);

GType gst_clock_entry_get_type ();
GType gst_event_mask_get_type ();
GType gst_format_get_type2 ();
GType gst_query_type_get_type2 ();
GType gst_type_get_type ();


/* misc.c interface */
void rbgst_initialize_gstobject (VALUE obj, GstObject * gstobj);
VALUE rbgst_new_gstobject (void *);
VALUE rb_str_new_with_format (char *fmt, ...);
VALUE rb_ary_yield (VALUE arr);

/* implemented in rbgstpluginfeature.c */
gboolean is_valid_pluginfeature_type (const GType type);
VALUE instanciate_pluginfeature (GstPluginFeature * feature);

/* implemented in rbgst-structure.c */
VALUE rbgst_structure_to_hash(GstStructure *structure);
GstStructure *rbgst_hash_to_structure(VALUE hash);
GstStructure *rbgst_hash_to_structure_with_name(VALUE hash, const char *name);


#define GST_TAG_LIST_STRUCTURE_NAME   "taglist"

#endif                          /* __RBGST_H_ */
