/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconsize.c -

  $Author: sakai $
  $Date: 2003/11/20 18:27:54 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

static VALUE
icon_size_lookup(VALUE self, VALUE size)
{
    gint width, height;

    if(gtk_icon_size_lookup(RVAL2GENUM(size, GTK_TYPE_ICON_SIZE), &width, &height))
        return rb_ary_new3(2, INT2FIX(width), INT2FIX(height));
    else
        return Qnil;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
icon_size_lookup_for_settings(VALUE self, VALUE settings, VALUE size)
{
    gint width, height;

    if(gtk_icon_size_lookup_for_settings(GTK_SETTINGS(RVAL2GOBJ(settings)),
                                         RVAL2GENUM(size, GTK_TYPE_ICON_SIZE), 
                                         &width, &height))
        return rb_ary_new3(2, INT2FIX(width), INT2FIX(height));
    else
        return Qnil;
}
#endif

static VALUE
icon_size_register(VALUE self, VALUE name, VALUE width, VALUE height)
{
    /* XXXX FIXME This should be GENUM2RVAL */
    return INT2FIX(gtk_icon_size_register(RVAL2CSTR(name),
                                          NUM2INT(width),
                                          NUM2INT(height)));
}

static VALUE
icon_size_register_alias(VALUE self, VALUE alias, VALUE target)
{
    gtk_icon_size_register_alias(RVAL2CSTR(alias), RVAL2GENUM(target, GTK_TYPE_ICON_SIZE));
    return Qnil;
}

static VALUE
icon_size_from_name(VALUE self, VALUE name)
{
    return INT2FIX(gtk_icon_size_from_name(RVAL2CSTR(name)));
}

static VALUE
icon_size_get_name(VALUE self, VALUE size)
{
    return CSTR2RVAL(gtk_icon_size_get_name(RVAL2GENUM(size, GTK_TYPE_ICON_SIZE)));
}

void
Init_icon_size()
{
    VALUE mIconSize = rb_define_module_under(mGtk, "IconSize");

    rb_define_module_function(mIconSize, "lookup", icon_size_lookup, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_module_function(mIconSize, "lookup_for_settings", icon_size_lookup_for_settings, 2);
#endif
    rb_define_module_function(mIconSize, "register", icon_size_register, 3);
    rb_define_module_function(mIconSize, "register_alias", icon_size_register_alias, 2);
    rb_define_module_function(mIconSize, "from_name", icon_size_from_name, 1);
    rb_define_module_function(mIconSize, "get_name", icon_size_get_name, 1);
    
    /* GtkIconSize */
    G_DEF_CLASS(GTK_TYPE_ICON_SIZE, "IconSize", mIconSize);
    G_DEF_CONSTANTS(mIconSize, GTK_TYPE_ICON_SIZE, "GTK_ICON_SIZE_");
}
