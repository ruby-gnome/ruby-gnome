/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconsize.c -

  $Author: ogs $
  $Date: 2002/12/02 14:19:09 $

  Copyright (C) 2002 OGASAWARA, Takeshi
************************************************/
#include "global.h"

static VALUE
icon_size_lookup(self, size)
    VALUE self, size;
{
    gint width, height;

    if(gtk_icon_size_lookup(FIX2INT(size), &width, &height))
        return rb_ary_new3(2, INT2FIX(width), INT2FIX(height));
    else
        return Qnil;
}

#if 0
/* gtk_icon_size_lookup_for_settings is not defined in GTK+. */

/*
gboolean    gtk_icon_size_lookup_for_settings
                                            (GtkSettings *settings,
                                             GtkIconSize size,
                                             gint *width,
                                             gint *height);
*/
static VALUE
icon_size_lookup_for_settings(self, settings, size)
    VALUE self, settings, size;
{
    gint width, height;

    if(gtk_icon_size_lookup_for_settings(GTK_SETTINGS(RVAL2GOBJ(settings)),
                                         FIX2INT(size), &width, &height))
        return rb_ary_new3(2, INT2FIX(width), INT2FIX(height));
    else
        return Qnil;
}
#endif

static VALUE
icon_size_register(self, name, width, height)
    VALUE self, name, width, height;
{
    return INT2FIX(gtk_icon_size_register(RVAL2CSTR(name),
                                          NUM2INT(width),
                                          NUM2INT(height)));
}

static VALUE
icon_size_register_alias(self, alias, target)
    VALUE self, alias, target;
{
    gtk_icon_size_register_alias(RVAL2CSTR(alias), FIX2INT(target));
    return Qnil;
}

static VALUE
icon_size_from_name(self, name)
    VALUE self, name;
{
    return INT2FIX(gtk_icon_size_from_name(RVAL2CSTR(name)));
}

static VALUE
icon_size_get_name(self, size)
    VALUE self, size;
{
    return CSTR2RVAL(gtk_icon_size_get_name(FIX2INT(size)));
}

void
Init_icon_size()
{
    VALUE mIconSize = rb_define_module_under(mGtk, "IconSize");

    rb_define_module_function(mIconSize, "lookup", icon_size_lookup, 1);
    rb_define_module_function(mIconSize, "register", icon_size_register, 3);
    rb_define_module_function(mIconSize, "register_alias", icon_size_register_alias, 2);
    rb_define_module_function(mIconSize, "from_name", icon_size_from_name, 1);
    rb_define_module_function(mIconSize, "get_name", icon_size_get_name, 1);
    
    /* GtkIconSize */
    rb_define_const(mIconSize, "INVALID", INT2FIX(GTK_ICON_SIZE_INVALID));
    rb_define_const(mIconSize, "MENU", INT2FIX(GTK_ICON_SIZE_MENU));
    rb_define_const(mIconSize, "SMALL_TOOLBAR", INT2FIX(GTK_ICON_SIZE_SMALL_TOOLBAR));
    rb_define_const(mIconSize, "LARGE_TOOLBAR", INT2FIX(GTK_ICON_SIZE_LARGE_TOOLBAR));
    rb_define_const(mIconSize, "BUTTON", INT2FIX(GTK_ICON_SIZE_BUTTON));
    rb_define_const(mIconSize, "DND", INT2FIX(GTK_ICON_SIZE_DND));
    rb_define_const(mIconSize, "DIALOG", INT2FIX(GTK_ICON_SIZE_DIALOG));
}
