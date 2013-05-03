/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cImage
#define _SELF(s) (RVAL2GTKIMAGE(s))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE image;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &image);
    if (NIL_P(image)) {
        widget = gtk_image_new();
    } else if (TYPE(image) == T_HASH) {
        VALUE stock, icon_name, icon_set, gicon, file, pixbuf, animation, size, buffer;
        rbg_scan_options(image,
                         "stock", &stock,
                         "icon_name", &icon_name,
                         "icon_set", &icon_set,
                         "gicon", &gicon,
                         "file", &file,
                         "pixbuf", &pixbuf,
                         "animation", &animation,
                         "size", &size,
                         NULL);

        if (!NIL_P(stock))
            widget = gtk_image_new_from_stock(RVAL2GLIBID(stock, buffer), RVAL2GTKICONSIZE(size));
        else if (!NIL_P(icon_name))
            widget = gtk_image_new_from_icon_name(RVAL2CSTR(icon_name), RVAL2GTKICONSIZE(size));
        else if (!NIL_P(icon_set))
            widget = gtk_image_new_from_icon_set(RVAL2GTKICONSET(icon_set), RVAL2GTKICONSIZE(size));
        else if (!NIL_P(gicon))
            widget = gtk_image_new_from_gicon(RVAL2GICON(gicon), RVAL2GTKICONSIZE(size));
        else if (!NIL_P(file))
            widget = gtk_image_new_from_file(RVAL2CSTR(file));
        else if (!NIL_P(pixbuf))
            widget = gtk_image_new_from_pixbuf(RVAL2GDKPIXBUF(pixbuf));
        else if (!NIL_P(animation))
            widget = gtk_image_new_from_animation(RVAL2GDKPIXBUFANIMATION(animation));
    } else {
        GType gtype = RVAL2GTYPE(image);

        if (gtype == GDK_TYPE_PIXBUF)
            widget = gtk_image_new_from_pixbuf(RVAL2GDKPIXBUF(image));
        else if (g_type_is_a(gtype, GDK_TYPE_PIXBUF_ANIMATION))
            widget = gtk_image_new_from_animation(RVAL2GDKPIXBUFANIMATION(image));
    }
    if (!widget)
        rb_raise(rb_eArgError, "Invalid arguments.");

    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rg_clear(VALUE self)
{
    gtk_image_clear(_SELF(self));
    return self;
}

static VALUE
rg_set_from_icon_set(VALUE self, VALUE icon_set, VALUE size)
{
    gtk_image_set_from_icon_set(RVAL2GTKIMAGE(self),
                                RVAL2GTKICONSET(icon_set),
                                RVAL2GTKICONSIZE(size));
    return self;
}

static VALUE
rg_set_from_stock(VALUE self, VALUE stock, VALUE size)
{
    VALUE buffer;

    gtk_image_set_from_stock(RVAL2GTKIMAGE(self),
                             RVAL2GLIBID(stock, buffer),
                             RVAL2GTKICONSIZE(size));
    return self;
}

static VALUE
rg_set_from_gicon(VALUE self, VALUE gicon, VALUE size)
{
    gtk_image_set_from_gicon(RVAL2GTKIMAGE(self),
                             RVAL2GICON(gicon),
                             RVAL2GTKICONSIZE(size));
    return self;
}

void
Init_gtk_image(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IMAGE, "Image", mGtk);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(clear, 0);
    RG_DEF_METHOD(set_from_icon_set, 2);
    RG_DEF_METHOD(set_from_stock, 2);
    RG_DEF_METHOD(set_from_gicon, 2);

    G_DEF_CLASS(GTK_TYPE_IMAGE_TYPE, "Type", RG_TARGET_NAMESPACE);

    RG_REG_GLIBID_SETTER("stock");
}
