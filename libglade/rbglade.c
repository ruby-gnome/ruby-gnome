/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglade.c -

  $Author: mutoh $
  $Date: 2004/03/03 17:39:48 $


  Copyright (C) 2002-2004 Ruby-GNOME2 Project

  This program is free software.
  You can distribute/modify this program under the terms of
  the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

  Original author: Avi Bryant <avi@beta4.com>
************************************************/

#include "ruby.h"
#include "rbgtk.h"
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gmodule.h>

static VALUE cGladeXML;
static VALUE instances;

static void
xml_connect(const gchar *handler_name, GObject *_source,
            const gchar *signal_name, const gchar *signal_data,
            GObject *_target, gboolean _after, gpointer user_data)
{
    VALUE self = (VALUE)user_data;
    VALUE source, target, after, signal, handler, data;
    
    source = _source? GOBJ2RVAL(_source) : Qnil;
    target = _target? GOBJ2RVAL(_target) : Qnil;
    
    signal = signal_name ? rb_str_new2(signal_name) : Qnil;
    handler = handler_name ? rb_str_new2(handler_name) : Qnil;
    data = signal_data ? rb_str_new2(signal_data) : Qnil;
    after = CBOOL2RVAL(_after);
    
    rb_funcall(self, rb_intern("connect"), 6, source, target, signal, handler, data, after); 
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

    rb_scan_args(argc, argv, "13", &fileString, &rootString, &domainString, &handler_proc);

    fileName = NIL_P(fileString) ? 0 : STR2CSTR(fileString);
    root = NIL_P(rootString) ? 0 : STR2CSTR(rootString);
    domain = NIL_P(domainString) ? 0 : STR2CSTR(domainString);

    if (NIL_P(handler_proc) && rb_block_given_p())
        handler_proc = G_BLOCK_PROC();

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

static VALUE
rb_gladexml_filename(VALUE self)
{
    GladeXML *xml;
    char *filename;
    
    xml = GLADE_XML(RVAL2GOBJ(self));
    filename = xml->filename;
    
    return filename ? rb_str_new2(filename) : Qnil;
}

static GtkWidget*
custom_widget_handler(GladeXML *xml, gchar *func_name, gchar *name,
                      gchar *string1, gchar *string2, gint int1, gint int2,
                      gpointer user_data)
{
    VALUE widget;
    int state = 0;
    GtkWidget* ret;

    widget = rb_eval_string_protect(func_name, &state);
    if (state == 0){
        if (rb_obj_is_kind_of(widget, GTYPE2CLASS(GTK_TYPE_WIDGET))){
            ret = (GtkWidget*)RVAL2GOBJ(widget);
            gtk_widget_show_all(ret);
            return ret;
        } else {
            return NULL;
        }
    } else {
        typedef GtkWidget *(* create_func)(gchar *name,
                                           gchar *string1, gchar *string2,
                                           gint int1, gint int2);
        GModule *allsymbols;
        create_func func;
        if (!g_module_supported()) {
            rb_raise(rb_eRuntimeError, "%s() isn't found or gmodule doesn't supported.", func_name);
            return NULL;
        }
        allsymbols = g_module_open(NULL, 0);
        if (g_module_symbol(allsymbols, func_name, (gpointer)&func))
            return (* func)(name, string1, string2, int1, int2);
        rb_raise(rb_eRuntimeError, "%s() isn't found.", func_name);
        return NULL;
    }
}

static GtkWidget*
disable_custom_widget_handler(GladeXML *xml, gchar *func_name, gchar *name,
                              gchar *string1, gchar *string2, gint int1, gint int2,
                              gpointer user_data)
{
    return NULL;
}

static gboolean custom_widget_supported = FALSE;

static VALUE
rb_gladexml_set_custom_widget_handler(VALUE self, VALUE setting)
{
    if (! custom_widget_supported && RTEST(setting)){
        glade_set_custom_handler(custom_widget_handler, NULL);
        custom_widget_supported = TRUE;
    } else {
        glade_set_custom_handler(disable_custom_widget_handler, NULL);
    }
    return self;
}

static VALUE
rb_gladexml_require(VALUE self, VALUE library)
{
  glade_require(RVAL2CSTR(library));
  return self;
}

static VALUE
rb_gladexml_provide(VALUE self, VALUE library)
{
  glade_provide(RVAL2CSTR(library));
  return self;
}                                        

void 
Init_libglade2()
{
    instances = rb_ary_new();
    rb_global_variable(&instances);

    cGladeXML = G_DEF_CLASS(GLADE_TYPE_XML, "GladeXML", rb_cObject);
    rb_define_method(cGladeXML, "initialize", rb_gladexml_initialize, -1);
    rb_define_method(cGladeXML, "get_widget", rb_gladexml_get_widget, 1);
    rb_define_alias(cGladeXML, "[]", "get_widget");
    rb_define_method(cGladeXML, "filename"  , rb_gladexml_filename, 0);
    rb_define_singleton_method(cGladeXML, "set_custom_widget_handler", rb_gladexml_set_custom_widget_handler, 1);
    rb_define_singleton_method(cGladeXML, "require", rb_gladexml_require, 1);
    rb_define_singleton_method(cGladeXML, "provide", rb_gladexml_provide, 1);
}
