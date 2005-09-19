/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglade.c -

  $Author: mutoh $
  $Date: 2005/09/19 14:05:21 $


  Copyright (C) 2002-2005 Ruby-GNOME2 Project

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

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif

static const int RB_GLADE_XML_FILE = 1;
static const int RB_GLADE_XML_BUFFER = 2;

static VALUE cGladeXML;

static VALUE
rb_gladexml_get_tooltips(VALUE self, VALUE toplevel)
{
    return GOBJ2RVAL(g_object_get_qdata(RVAL2GOBJ(toplevel), 
                                        g_quark_from_static_string("GladeXML::tooltips")));
}

static VALUE
rb_gladexml_get_widget(VALUE self, VALUE nameString)
{
    GtkWidget *widget = glade_xml_get_widget(GLADE_XML(RVAL2GOBJ(self)), RVAL2CSTR(nameString));
    return widget ? GOBJ2RVAL(widget) : Qnil;
}

static VALUE
rb_gladexml_get_widget_prefix(VALUE self, VALUE name)
{
    GList* list = glade_xml_get_widget_prefix(GLADE_XML(RVAL2GOBJ(self)), RVAL2CSTR(name));
    return GLIST2ARY(list);
}

static VALUE
rb_gladexml_s_get_widget_name(VALUE self, VALUE widget)
{
    return CSTR2RVAL(glade_get_widget_name(RVAL2GOBJ(widget)));
}

static VALUE
rb_gladexml_s_get_widget_tree(VALUE self, VALUE widget)
{
    return GOBJ2RVAL(glade_get_widget_tree(RVAL2GOBJ(widget)));
}

static void
xml_autoconnect(const gchar *handler_name, GObject *_source,
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
    
    rb_funcall(rb_iv_get(self, "@autoconnect_proc"), rb_intern("call"), 
               6, source, target, signal, handler, data, after); 
}

static VALUE
rb_gladexml_signal_autoconnect_full(VALUE self)
{
    rb_iv_set(self, "@autoconnect_proc", G_BLOCK_PROC());
    glade_xml_signal_autoconnect_full(GLADE_XML(RVAL2GOBJ(self)), 
                                      xml_autoconnect, (gpointer)self);
    return self;
}

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
rb_gladexml_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE text, rootString, domainString, localedir_or_flag, flag;
    GladeXML *xml;
    char* root;
    char* domain;
    char* localedir = NULL;
    int dflag = 0;

    rb_scan_args(argc, argv, "14", &text, &rootString, &domainString, &localedir_or_flag, &flag);

    root = NIL_P(rootString) ? NULL : RVAL2CSTR(rootString);
    domain = NIL_P(domainString) ? NULL : RVAL2CSTR(domainString);
    if (TYPE(localedir_or_flag) == T_STRING){
        localedir = RVAL2CSTR(localedir_or_flag);
        dflag = NIL_P(flag) ? RB_GLADE_XML_FILE : NUM2INT(flag); 
    } else if (NIL_P(localedir_or_flag)){
        dflag = NIL_P(flag) ? RB_GLADE_XML_FILE : NUM2INT(flag); 
    } else {
        dflag = NUM2INT(localedir_or_flag);
    }

    glade_init();

#ifdef HAVE_BINDTEXTDOMAIN
    if (localedir){
        bindtextdomain(domain, localedir);
    } else {
        const gchar* env = g_getenv("GETTEXT_PATH");
        if (env){
            bindtextdomain(domain, env);
        }
    }
    if (domain){
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
        bind_textdomain_codeset(domain, "UTF-8");
#endif
        textdomain(domain);
    }
#endif
    
    if (dflag == RB_GLADE_XML_FILE){
        xml = glade_xml_new(RVAL2CSTR(text), root, domain);
    } else if (dflag == RB_GLADE_XML_BUFFER) {
        StringValue(text);
        xml = glade_xml_new_from_buffer(RVAL2CSTR(text), RSTRING(text)->len, root, domain);
    } else {
         rb_raise(rb_eArgError, "flag is wrong valiable %d", dflag);
    }

    if (xml) {
        G_INITIALIZE(self, xml);
        if (rb_block_given_p()){
            rb_iv_set(self, "@handler_proc", G_BLOCK_PROC());
            glade_xml_signal_autoconnect_full(xml, xml_connect, (gpointer)self);
        }
    } else {
        rb_raise(rb_eIOError, "could not load glade file %s", RVAL2CSTR(text));
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
    if (RTEST(setting)){
        glade_set_custom_handler(custom_widget_handler, NULL);
        custom_widget_supported = TRUE;
    } else {
        glade_set_custom_handler(disable_custom_widget_handler, NULL);
        custom_widget_supported = FALSE;
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
    cGladeXML = G_DEF_CLASS(GLADE_TYPE_XML, "GladeXML", rb_cObject);
    rb_define_method(cGladeXML, "signal_autoconnect_full", rb_gladexml_signal_autoconnect_full, 0);
    rb_define_method(cGladeXML, "initialize", rb_gladexml_initialize, -1);
    rb_define_method(cGladeXML, "get_tooltips", rb_gladexml_get_tooltips, 1);
    rb_define_method(cGladeXML, "get_widget", rb_gladexml_get_widget, 1);
    rb_define_alias(cGladeXML, "[]", "get_widget");
    rb_define_method(cGladeXML, "get_widget_prefix", rb_gladexml_get_widget_prefix, 1);
    rb_define_singleton_method(cGladeXML, "get_widget_name", rb_gladexml_s_get_widget_name, 1);
    rb_define_singleton_method(cGladeXML, "get_widget_tree", rb_gladexml_s_get_widget_tree, 1);
    rb_define_method(cGladeXML, "filename"  , rb_gladexml_filename, 0);
    rb_define_singleton_method(cGladeXML, "set_custom_widget_handler", rb_gladexml_set_custom_widget_handler, 1);
    rb_define_singleton_method(cGladeXML, "require", rb_gladexml_require, 1);
    rb_define_singleton_method(cGladeXML, "provide", rb_gladexml_provide, 1);
    rb_define_const(cGladeXML, "FILE", INT2FIX(RB_GLADE_XML_FILE));
    rb_define_const(cGladeXML, "BUFFER", INT2FIX(RB_GLADE_XML_BUFFER));

}
