/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project
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

#ifndef __RBGST_PRIVATE_H_
#define __RBGST_PRIVATE_H_

#include "rbgst.h"

/* TODO: move to rbgutil.h */
#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rbg_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rbg_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT  { 0, { { 0 } } }
#endif

char *_rbgst_inspect(VALUE object);
void _rbgst_define_class_if_need(VALUE klass, GType type, const gchar *prefix);
VALUE _rbgst_collect_elements(GstIterator *iter);

void _rbgst_mini_object_free(void *ptr);

/* implemented in rbgst-object.c */
VALUE rbgst_object_instance2robj(gpointer instance);


G_GNUC_INTERNAL void Init_gst_bin (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_buffer(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_bus(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_caps (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_child_proxy(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_clock (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_clock_entry (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_element (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_elementfactory (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_event(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_format (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_seek(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_structure(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_index (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_indexentry (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_indexfactory (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_message(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_mini_object(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_object (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_pad (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_pad_template(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_ghost_pad(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_parse (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_pipeline (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_plugin (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_plugin_feature (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_query(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_querytype (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_registry (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_registry_pool (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_scheduler (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_schedulerfactory (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_static_caps(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_static_pad_template(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_systemclock (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_tag (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_tag_setter (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_thread (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_typefindfactory (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_value(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_xml (VALUE mGst);

#if defined(HAVE_GST_OVERLAY)
G_GNUC_INTERNAL void Init_gst_x_overlay (VALUE mGst);
#endif

#if defined(HAVE_MEDIA_INFO)
G_GNUC_INTERNAL void Init_gst_mediatype (VALUE mGst);
#endif

#ifdef HAVE_GST_PBUTILS
G_GNUC_INTERNAL void Init_gst_install_plugins_context (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_install_plugins (VALUE mGst);
G_GNUC_INTERNAL void Init_gst_install_plugins_return (VALUE mGst);
#endif

#endif /* __RBGST_PRIVATE_H_ */
