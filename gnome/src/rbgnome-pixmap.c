/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-pixmap.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

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

#define _SELF(self) GNOME_PIXMAP(RVAL2GOBJ(self))

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
            pixmap = gnome_pixmap_new_from_file(RVAL2CSTR(argv[0]));
            break;
          case 3:
            pixmap = gnome_pixmap_new_from_file_at_size(RVAL2CSTR(argv[0]),
                                                        NUM2INT(argv[1]), NUM2INT(argv[2]));
            break;
        }
        break;
      case T_ARRAY:
      {
          int i;
          const gchar **buf = ALLOCA_N(const gchar*, RARRAY(argv[0])->len);
          for (i=0; i<RARRAY(argv[0])->len; ++i) {
              buf[i] = RVAL2CSTR(RARRAY(argv[0])->ptr[i]);
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
        pixmap = gnome_pixmap_new_from_gnome_pixmap(GNOME_PIXMAP(RVAL2GOBJ(argv[0])));
        break;
    }
    
    RBGTK_INITIALIZE(self, pixmap);
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
        gnome_pixmap_load_file(_SELF(self), RVAL2CSTR(filename));
    } else {
        gnome_pixmap_load_file_at_size(_SELF(self),
                                       RVAL2CSTR(filename),
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
    const gchar **buf;

    rb_scan_args(argc, argv, "12", &xpm_data, &width, &height);

    buf = ALLOCA_N(const gchar*, RARRAY(xpm_data)->len);
    for (i=0; i<RARRAY(xpm_data)->len; ++i) {
        buf[i] = RVAL2CSTR(RARRAY(argv[0])->ptr[i]);
    }

    if (argc == 1) {
        gnome_pixmap_load_xpm_d(_SELF(self), buf);
    } else {
        gnome_pixmap_load_xpm_d_at_size(_SELF(self), buf,
                                        NUM2INT(width), NUM2INT(height));
    }
    return self;
}

void
Init_gnome_pixmap(mGnome)
    VALUE mGnome;
{
    VALUE gnoPixmap = G_DEF_CLASS(GNOME_TYPE_PIXMAP, "Pixmap", mGnome);

    rb_define_method(gnoPixmap, "initialize", pixmap_initialize, -1);
    rb_define_method(gnoPixmap, "load_file", pixmap_load_file, -1);
    rb_define_method(gnoPixmap, "load_xpm_d", pixmap_load_xpm_d, -1);
}
