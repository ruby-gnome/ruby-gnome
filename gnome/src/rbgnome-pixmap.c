/* $Id: rbgnome-pixmap.c,v 1.1 2002/05/19 12:39:27 mutoh Exp $ */

/* Gnome::Pixmap widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"
#include "rbgdkimlib.h"

VALUE gnoPixmap;

static VALUE
pixmap_initialize(argc, argv, self)
    int   argc;
    VALUE *argv;
    VALUE self;
{
    GtkWidget *pixmap = 0;

    switch (TYPE(argv[0])) {
        case T_STRING:
            switch (argc) {
                case 1:
                    pixmap = gnome_pixmap_new_from_file(STR2CSTR(argv[0]));
                    break;
                case 3:
                    pixmap = gnome_pixmap_new_from_file_at_size(STR2CSTR(argv[0]),
                                        NUM2INT(argv[1]), NUM2INT(argv[2]));
                    break;
            }
            break;
        case T_ARRAY:
        {
            int i;
            char **buf = ALLOCA_N(char*, RARRAY(argv[0])->len);
            for (i=0; i<RARRAY(argv[0])->len; ++i) {
                buf[i] = STR2CSTR(RARRAY(argv[0])->ptr[i]);
            }
            switch (argc) {
                case 1:
                    pixmap = gnome_pixmap_new_from_xpm_d(buf);
                    break;
                case 3:
                    pixmap = gnome_pixmap_new_from_xpm_d_at_size(buf,
                                        NUM2INT(argv[1]), NUM2INT(argv[2]));
                    break;
            }
			break;
        }
        case T_OBJECT:
            if (rb_obj_is_kind_of(argv[0], gnoPixmap)) {
                pixmap = gnome_pixmap_new_from_gnome_pixmap(
                            GNOME_PIXMAP(get_widget(argv[0])));
            } else if (rb_obj_is_kind_of(argv[0], cImlibImage)) {
                GdkImlibImage *im;
                Data_Get_Struct(argv[0], GdkImlibImage, im);
                    switch (argc) {
                        case 1:
                            pixmap = gnome_pixmap_new_from_imlib(im);
                            break;
                        case 3:
                            pixmap = gnome_pixmap_new_from_imlib_at_size(im,
                                        NUM2INT(argv[1]), NUM2INT(argv[2]));
                            break;
                    }
            }
			break;
        }

    set_widget(self, pixmap);
    return Qnil;
}

static VALUE
pixmap_load_file(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE filename, width, height;

    if (rb_scan_args(argc, argv, "12", &filename, &width, &height) == 1) {
        gnome_pixmap_load_file(GNOME_PIXMAP(get_widget(self)),
                               STR2CSTR(filename));
    } else {
        gnome_pixmap_load_file_at_size(GNOME_PIXMAP(get_widget(self)),
                                       STR2CSTR(filename),
                                       NUM2INT(width),
                                       NUM2INT(height));
    }
    return self;
}

static VALUE
pixmap_load_xpm_d(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE xpm_data, width, height;
    int i;
    char **buf;

    rb_scan_args(argc, argv, "12", &xpm_data, &width, &height);

    buf = ALLOCA_N(char*, RARRAY(xpm_data)->len);
    for (i=0; i<RARRAY(xpm_data)->len; ++i) {
        buf[i] = STR2CSTR(RARRAY(argv[0])->ptr[i]);
    }

    if (argc == 1) {
        gnome_pixmap_load_xpm_d(GNOME_PIXMAP(get_widget(self)), buf);
    } else {
        gnome_pixmap_load_xpm_d_at_size(GNOME_PIXMAP(get_widget(self)), buf,
                                        NUM2INT(width), NUM2INT(height));
    }
    return self;
}

static VALUE
pixmap_load_imlib(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE xpm_data, width, height;
    GdkImlibImage *im;

    rb_scan_args(argc, argv, "12", &xpm_data, &width, &height);

    Data_Get_Struct(argv[0], GdkImlibImage, im);

    if (argc == 1) {
        gnome_pixmap_load_imlib(GNOME_PIXMAP(get_widget(self)), im);
    } else {
        gnome_pixmap_load_imlib_at_size(GNOME_PIXMAP(get_widget(self)), im,
                                        NUM2INT(width), NUM2INT(height));
    }
    return self;
}

void
Init_gnome_pixmap()
{
    gnoPixmap = rb_define_class_under(mGnome, "Pixmap", gWidget);

    rb_define_method(gnoPixmap, "initialize", pixmap_initialize, -1);
    rb_define_method(gnoPixmap, "load_file", pixmap_load_file, -1);
    rb_define_method(gnoPixmap, "load_xpm_d", pixmap_load_xpm_d, -1);
    rb_define_method(gnoPixmap, "load_imlib", pixmap_load_imlib, -1);
}
