/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimage.c -

  $Author: sakai $
  $Date: 2003/08/20 17:07:03 $

  Copyright (C) 2002,2003 Masao Mutoh
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
    } else if (TYPE(arg1) == T_SYMBOL){
        widget = gtk_image_new_from_stock(rb_id2name(SYM2ID(arg1)), FIX2INT(arg2));
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
        } else if (gtype == GTK_TYPE_ICON_SET){
            widget = gtk_image_new_from_icon_set((GtkIconSet*)RVAL2BOXED(arg1, GTK_TYPE_ICON_SET), NUM2INT(arg2));
        } else if (gtype == GDK_TYPE_PIXBUF_ANIMATION){
            widget = gtk_image_new_from_animation(GDK_PIXBUF_ANIMATION(RVAL2GOBJ(arg1)));
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

    if (TYPE(arg1) == T_STRING){
        gtk_image_set_from_file(_SELF(self), RVAL2CSTR(arg1));
    } else if (TYPE(arg1) == T_SYMBOL){
        gtk_image_set_from_stock(_SELF(self), rb_id2name(SYM2ID(arg1)), FIX2INT(arg2));
    } else {
        gtype = RVAL2GTYPE(arg1);
        if (gtype == GDK_TYPE_IMAGE){
            gtk_image_set_from_image(_SELF(self), GDK_IMAGE(RVAL2GOBJ(arg1)),
                                     GDK_BITMAP(RVAL2GOBJ(arg2)));
        } else if (gtype == GDK_TYPE_PIXBUF){
            gtk_image_set_from_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(arg1)));
            
        } else if (gtype == GDK_TYPE_PIXMAP){
            gtk_image_set_from_pixmap(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(arg1)),
                                      GDK_BITMAP(RVAL2GOBJ(arg2)));
        } else if (gtype == GTK_TYPE_ICON_SET){
            gtk_image_set_from_icon_set(_SELF(self), 
                                        (GtkIconSet*)RVAL2BOXED(arg1, GTK_TYPE_ICON_SET), NUM2INT(arg2));
        } else if (gtype == GDK_TYPE_PIXBUF_ANIMATION){
            gtk_image_set_from_animation(_SELF(self), GDK_PIXBUF_ANIMATION(RVAL2GOBJ(arg1)));
        }
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
    G_DEF_CLASS(GTK_TYPE_IMAGE_TYPE, "Type", gImage);
    G_DEF_CONSTANTS(gImage, GTK_TYPE_IMAGE_TYPE, "GTK_IMAGE_");

    G_SET_SYMBOL_PROPERTY(GTK_TYPE_IMAGE, "stock");

}
