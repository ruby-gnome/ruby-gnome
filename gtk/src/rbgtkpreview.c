/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkpreview.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:15 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
preview_initialize(self, type)
    VALUE self, type;
{
    RBGTK_INITIALIZE(self, gtk_preview_new((GtkPreviewType)NUM2INT(type)));
    return Qnil;
}

static VALUE
preview_size(self, w, h)
    VALUE self, w, h;
{
    gtk_preview_size(GTK_PREVIEW(RVAL2GOBJ(self)), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
preview_put(self, win, gc, srcx, srcy, dstx, dsty, w, h)
    VALUE self, win, gc, srcx, srcy, dstx, dsty, w, h;
{
    gtk_preview_put(GTK_PREVIEW(RVAL2GOBJ(self)), GDK_WINDOW(RVAL2GOBJ(win)),
					GDK_GC(RVAL2GOBJ(gc)),
					NUM2INT(srcx), NUM2INT(srcy),
					NUM2INT(dstx), NUM2INT(dsty),
					NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
preview_draw_row(self, data, x, y, w)
    VALUE self, data, x, y, w;
{
    int width = NUM2INT(w);
    int dlen = width;

    if (GTK_PREVIEW(RVAL2GOBJ(self))->type == GTK_PREVIEW_COLOR) {
		dlen *= 3;
    }
    Check_Type(data, T_STRING);
    if (RSTRING(data)->len < dlen) {
		rb_raise(rb_eArgError, "data too short");
    }

    gtk_preview_draw_row(GTK_PREVIEW(RVAL2GOBJ(self)), RSTRING(data)->ptr,
						 NUM2INT(x), NUM2INT(y), width);
    return self;
}

static VALUE
preview_set_expand(self, expand)
    VALUE self, expand;
{
    gtk_preview_set_expand(GTK_PREVIEW(RVAL2GOBJ(self)), NUM2INT(expand));
    return self;
}

static VALUE
preview_set_gamma(self, gamma)
    VALUE self, gamma;
{
    gtk_preview_set_gamma(NUM2DBL(gamma));
    return Qnil;
}

static VALUE
preview_set_color_cube(self, nred, ngreen, nblue, ngray)
    VALUE self, nred, ngreen, nblue, ngray;
{
    gtk_preview_set_color_cube(NUM2INT(nred),
							   NUM2INT(ngreen),
							   NUM2INT(nblue),
							   NUM2INT(ngray));
    return Qnil;
}

static VALUE
preview_set_install_cmap(self, cmap)
    VALUE self, cmap;
{
    gtk_preview_set_install_cmap(RTEST(cmap));
    return Qnil;
}

static VALUE
preview_set_reserved(self, nreserved)
    VALUE self, nreserved;
{
    gtk_preview_set_reserved(NUM2INT(nreserved));
    return Qnil;
}

static VALUE
preview_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_preview_get_visual());
}

static VALUE
preview_get_cmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_preview_get_cmap());
}

static VALUE
preview_get_info(self)
    VALUE self;
{
    GtkPreviewInfo *i = gtk_preview_get_info();
    return make_gtkprevinfo(i);
}

void Init_gtk_preview()
{
    static RGObjClassInfo cinfo;

    gPreview = rb_define_class_under(mGtk, "Preview", gWidget);
    cinfo.klass = gPreview;
    cinfo.gtype = GTK_TYPE_PREVIEW;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /*
     * instance methods
     */
    rb_define_method(gPreview, "initialize", preview_initialize, 1);
    rb_define_method(gPreview, "size", preview_size, 2);
    rb_define_method(gPreview, "put", preview_put, 8);
    rb_define_method(gPreview, "draw_row", preview_draw_row, 4);
    rb_define_method(gPreview, "set_expand", preview_set_expand, 1);

    /*
     * singleton methods
     */
    rb_define_singleton_method(gPreview, "set_gamma", preview_set_gamma, 1);
    rb_define_singleton_method(gPreview, "set_color_cube",
							   preview_set_color_cube, 4);
    rb_define_singleton_method(gPreview, "set_install_cmap",
							   preview_set_install_cmap, 1);
    rb_define_singleton_method(gPreview, "set_reserved",
							   preview_set_reserved, 1);
    rb_define_singleton_method(gPreview, "get_visual", preview_get_visual, 0);
    rb_define_singleton_method(gPreview, "get_cmap", preview_get_cmap, 0);
    rb_define_singleton_method(gPreview, "get_info", preview_get_info, 0);
}
