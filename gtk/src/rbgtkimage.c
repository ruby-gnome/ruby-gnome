/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimage.c -

  $Author: mutoh $
  $Date: 2002/11/09 19:45:36 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_IMAGE(RVAL2GOBJ(s)))
/*
void        gtk_image_get_icon_set          (GtkImage *image,
                                             GtkIconSet **icon_set,
                                             GtkIconSize *size);
void        gtk_image_get_image             (GtkImage *image,
                                             GdkImage **gdk_image,
                                             GdkBitmap **mask);
GtkImageType gtk_image_get_storage_type     (GtkImage *image);
*/

static VALUE
image_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GType gtype;
    GtkWidget* widget = NULL;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);

    if (NIL_P(arg1)){
        widget = gtk_image_new();
    } else if (TYPE(arg1) == T_STRING){
        widget = gtk_image_new_from_file(RVAL2CSTR(arg1));
    } else {
        gtype = RVAL2GTYPE(arg1);
        if (gtype == GDK_TYPE_IMAGE){
            widget = gtk_image_new_from_image(GDK_IMAGE(RVAL2GOBJ(arg1)),
                                              GDK_BITMAP(RVAL2GOBJ(arg2)));
        } else if (gtype == GDK_TYPE_PIXBUF){
            widget = gtk_image_new_from_pixbuf(GDK_PIXBUF(RVAL2GOBJ(arg1)));
        } else if (gtype == GDK_TYPE_PIXMAP){
            widget = gtk_image_new_from_pixmap(GDK_PIXMAP(RVAL2GOBJ(arg1)),
                                               GDK_BITMAP(RVAL2GOBJ(arg2)));
        } 
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
image_set(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GType gtype;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);

    gtype = RVAL2GTYPE(arg1);
    if (gtype == GDK_TYPE_IMAGE){
        gtk_image_set_from_image(_SELF(self), GDK_IMAGE(RVAL2GOBJ(arg1)),
                                 GDK_BITMAP(RVAL2GOBJ(arg2)));

    } else if (gtype == GDK_TYPE_PIXBUF){
        gtk_image_set_from_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(arg1)));

    } else if (gtype == GDK_TYPE_PIXMAP){
        gtk_image_set_from_pixmap(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(arg1)),
                                  GDK_BITMAP(RVAL2GOBJ(arg2)));
    }

    return self;
}

void 
Init_gtk_image()
{
    VALUE gImage = G_DEF_CLASS(GTK_TYPE_IMAGE, "Image", mGtk);
    rb_define_method(gImage, "initialize", image_initialize, -1);
    rb_define_method(gImage, "set", image_set, -1);

    /* GtkImageType */
    rb_define_const(gImage, "EMPTY", INT2FIX(GTK_IMAGE_EMPTY));
    rb_define_const(gImage, "PIXMAP", INT2FIX(GTK_IMAGE_PIXMAP));
    rb_define_const(gImage, "IMAGE", INT2FIX(GTK_IMAGE_IMAGE));
    rb_define_const(gImage, "PIXBUF", INT2FIX(GTK_IMAGE_PIXBUF));
    rb_define_const(gImage, "STOCK", INT2FIX(GTK_IMAGE_STOCK));
    rb_define_const(gImage, "ICON_SET", INT2FIX(GTK_IMAGE_ICON_SET));
    rb_define_const(gImage, "ANIMATION", INT2FIX(GTK_IMAGE_ANIMATION));

}
