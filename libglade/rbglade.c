/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglade.c -

  $Author: mutoh $
  $Date: 2003/05/27 14:49:14 $


  Copyright (C) 2002,2003 Ruby-GNOME2 Project

  This program is free software.
  You can distribute/modify this program under the terms of
  the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

  Original author: Avi Bryant <avi@beta4.com>
************************************************/

#include "ruby.h"
#include "rbgtk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>

static VALUE cGladeXML;
static VALUE instances;

static void
xml_connect(const gchar *handler_name, GObject *_source,
            const gchar *signal_name, const gchar *signal_data,
            GObject *_target, gboolean after, gpointer user_data)
{
    VALUE self = (VALUE)user_data;
    VALUE source, target, signal, handler, data;
    
    source = _source? GOBJ2RVAL(_source) : Qnil;
    target = _target? GOBJ2RVAL(_target) : Qnil;
    
    signal = signal_name ? rb_str_new2(signal_name) : Qnil;
    handler = handler_name ? rb_str_new2(handler_name) : Qnil;
    data = signal_data ? rb_str_new2(signal_data) : Qnil;
    
    rb_funcall(self, rb_intern("connect"), 5, source, target, signal, handler, data); 
}

static VALUE
rb_gladexml_get_widget(VALUE self, VALUE nameString)
{
    GtkWidget *widget;
    widget = glade_xml_get_widget(GLADE_XML(RVAL2GOBJ(self)), STR2CSTR(nameString));
    return widget ? GOBJ2RVAL(widget) : Qnil;
}

static VALUE
rb_gladexml_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE fileString, rootString, domainString, handler_proc;
    GladeXML *xml;
    char *fileName;
    char *root;
    char *domain;

    rb_scan_args(argc, argv, "12&", &fileString, &rootString, &domainString, &handler_proc);

    fileName = NIL_P(fileString) ? 0 : STR2CSTR(fileString);
    root = NIL_P(rootString) ? 0 : STR2CSTR(rootString);
    domain = NIL_P(domainString) ? 0 : STR2CSTR(domainString);

    glade_init();

    xml = glade_xml_new(fileName, root, domain);

    if(xml)
    {
        G_INITIALIZE(self, xml);
        /* Once constructed, this means a GladeXML object can never be freed. */
        rb_ary_push(instances, self);
        rb_iv_set(self, "@handler_proc", handler_proc);
        glade_xml_signal_autoconnect_full(xml, xml_connect, (gpointer)self);
    }
    else
    {
        /* why does that raise not work properly?? */
        rb_raise(rb_eIOError, "could not load glade file %s", fileName);
    }

    return self;
}

void Init_libglade2()
{
    instances = rb_ary_new();
    rb_global_variable(&instances);

    cGladeXML = G_DEF_CLASS(GLADE_TYPE_XML, "GladeXML", rb_cObject);
    rb_define_method(cGladeXML, "initialize", rb_gladexml_initialize, -1);
    rb_define_method(cGladeXML, "get_widget", rb_gladexml_get_widget, 1);
}
