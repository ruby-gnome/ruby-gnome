/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf-format.c -

  $Author: mutoh $
  $Date: 2005/03/05 17:01:36 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "rbgdk-pixbuf.h"
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)

#ifdef HAVE_GDK_PIXBUF_GDK_PIXBUF_IO_H
#include <gdk-pixbuf/gdk-pixbuf-io.h>
#endif

#define _SELF(r) ((GdkPixbufFormat*)RVAL2BOXED(r, GDK_TYPE_PIXBUF_FORMAT))


/**********************************/
static GdkPixbufFormat*
format_copy(const GdkPixbufFormat* val)
{
    GdkPixbufFormat* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(GdkPixbufFormat, 1);
    *new_val = *val;
    return new_val;
}

GType
gdk_pixbuf_format_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GdkPixbufFormat",
                                                 (GBoxedCopyFunc)format_copy,
                                                 (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

/* Move to rbgdk-pixbuf.c
gboolean    gdk_pixbuf_set_option           (GdkPixbuf *pixbuf,
                                             const gchar *key,
                                             const gchar *value);
GSList*     gdk_pixbuf_get_formats          (void);
*/

static VALUE
get_name(self)
    VALUE self;
{
    return CSTR2RVAL2(gdk_pixbuf_format_get_name(_SELF(self)));
}

static VALUE
get_description(self)
    VALUE self;
{
    return CSTR2RVAL2(gdk_pixbuf_format_get_description(_SELF(self)));
}
    
static VALUE
get_mime_types(self)
    VALUE self;
{
    gint i = 0;
    gchar** mime_types = gdk_pixbuf_format_get_mime_types(_SELF(self));
    VALUE array = rb_ary_new();
    while(mime_types[i]){
        rb_ary_push(array, CSTR2RVAL(mime_types[i]));
        i++;
    }
    g_strfreev(mime_types);
    return array;
}

static VALUE
get_extensions(self)
    VALUE self;
{
    gint i = 0;
    gchar** extensions = gdk_pixbuf_format_get_extensions(_SELF(self));
    VALUE array = rb_ary_new();

    while(extensions[i]){
        rb_ary_push(array, CSTR2RVAL(extensions[i]));
        i++;
    }
    g_strfreev(extensions);
    return array;
}

static VALUE
is_writable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_pixbuf_format_is_writable(_SELF(self)));
}

/* Structure */
static VALUE
get_domain(self)
    VALUE self;
{
    return CSTR2RVAL(_SELF(self)->domain);
}

static VALUE
get_signature(self)
    VALUE self;
{
    GdkPixbufModulePattern* signature = _SELF(self)->signature;

    VALUE array = rb_ary_new();
    int i = 0;
    while(signature[i].prefix){
        rb_ary_push(array, rb_ary_new3(3, CSTR2RVAL(signature[i].prefix),
                                       CSTR2RVAL(signature[i].mask),
                                       INT2NUM(signature[i].relevance)));
        i++;
    }
    return array;
}
#endif

#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
static VALUE
is_scalable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_pixbuf_format_is_scalable(_SELF(self)));
}
static VALUE
is_disabled(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_pixbuf_format_is_disabled(_SELF(self)));
}
static VALUE
set_disabled(self, disabled)
    VALUE self, disabled;
{
    gdk_pixbuf_format_set_disabled(_SELF(self), RTEST(disabled));
    return self;
}
static VALUE
get_license(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_pixbuf_format_get_license(_SELF(self)));
}
#endif

void
Init_gdk_pixbuf_format(VALUE mGdk)
{
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    VALUE format = G_DEF_CLASS(GDK_TYPE_PIXBUF_FORMAT, "PixbufFormat", mGdk);

    rb_define_method(format, "name", get_name, 0);
    rb_define_method(format, "description", get_description, 0);
    rb_define_method(format, "mime_types", get_mime_types, 0);
    rb_define_method(format, "extensions", get_extensions, 0);
    rb_define_method(format, "writable?", is_writable, 0);
    rb_define_method(format, "domain", get_domain, 0);
    rb_define_method(format, "signature", get_signature, 0);
#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
    rb_define_method(format, "scalable?", is_scalable, 0);
    rb_define_method(format, "disabled?", is_disabled, 0);
    rb_define_method(format, "set_disabled", set_disabled, 1);
    rb_define_method(format, "license", get_license, 0);

#endif
    G_DEF_SETTERS(format);
#endif
}

