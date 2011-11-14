/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cPaperSize
#define _SELF(s) (RVAL2BOXED(s, GTK_TYPE_PAPER_SIZE))
#define SIZE2RVAL(o) (BOXED2RVAL(o, GTK_TYPE_PAPER_SIZE))

#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkPaperSize *size;

    if (argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "01", &name);
        size = gtk_paper_size_new(RVAL2CSTR_ACCEPT_NIL(name));
    } else if (argc == 4) {
        size = gtk_paper_size_new_from_ppd(RVAL2CSTR(argv[0]),
                                           RVAL2CSTR(argv[1]),
                                           NUM2DBL(argv[2]),
                                           NUM2DBL(argv[3]));
    } else if (argc == 5) {
        size = gtk_paper_size_new_custom(RVAL2CSTR(argv[0]),
                                         RVAL2CSTR(argv[1]),
                                         NUM2DBL(argv[2]),
                                         NUM2DBL(argv[3]),
                                         RVAL2UNIT(argv[4]));
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 0, 1, 4 or 5)", argc);
    }

    G_INITIALIZE(self, size);
    return Qnil;
}

static VALUE
rg_operator_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(RVAL2CBOOL(rb_equal(rb_obj_class(self), rb_obj_class(other))) &&
                      gtk_paper_size_is_equal(_SELF(self), _SELF(other)));
}

/* The width is always the shortest side, measure in mm */
static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_name(_SELF(self)));
}

static VALUE
rg_display_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_display_name(_SELF(self)));
}

static VALUE
rg_ppd_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_ppd_name(_SELF(self)));
}

static VALUE
rg_get_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_width(_SELF(self), RVAL2UNIT(unit)));
}

static VALUE
rg_get_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_height(_SELF(self), RVAL2UNIT(unit)));
}

static VALUE
rg_custom_p(VALUE self)
{
    return CBOOL2RVAL(gtk_paper_size_is_custom(_SELF(self)));
}

/* Only for custom sizes: */
static VALUE
rg_set_size(VALUE self, VALUE width, VALUE height, VALUE unit)
{
    gtk_paper_size_set_size(_SELF(self), NUM2DBL(width), NUM2DBL(height),
                            RVAL2UNIT(unit));
    return self;
}

static VALUE
rg_get_default_top_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_top_margin(_SELF(self),
                                                              RVAL2UNIT(unit)));
}

static VALUE
rg_get_default_bottom_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_bottom_margin(_SELF(self),
                                                                 RVAL2UNIT(unit)));
}

static VALUE
rg_get_default_left_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_left_margin(_SELF(self),
                                                               RVAL2UNIT(unit)));
}

static VALUE
rg_get_default_right_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_right_margin(_SELF(self),
                                                                RVAL2UNIT(unit)));
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_default());
}
#endif

void
Init_gtk_paper_size(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PAPER_SIZE, "PaperSize", mGtk);

    RG_DEF_SMETHOD(default, 0);

    rb_define_const(RG_TARGET_NAMESPACE, "A3", CSTR2RVAL(GTK_PAPER_NAME_A3));
    rb_define_const(RG_TARGET_NAMESPACE, "A4", CSTR2RVAL(GTK_PAPER_NAME_A4));
    rb_define_const(RG_TARGET_NAMESPACE, "A5", CSTR2RVAL(GTK_PAPER_NAME_A5));
    rb_define_const(RG_TARGET_NAMESPACE, "B5", CSTR2RVAL(GTK_PAPER_NAME_B5));
    rb_define_const(RG_TARGET_NAMESPACE, "LETTER", CSTR2RVAL(GTK_PAPER_NAME_LETTER));
    rb_define_const(RG_TARGET_NAMESPACE, "EXECUTIVE",
                    CSTR2RVAL(GTK_PAPER_NAME_EXECUTIVE));
    rb_define_const(RG_TARGET_NAMESPACE, "LEGAL", CSTR2RVAL(GTK_PAPER_NAME_LEGAL));

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD_OPERATOR("==", equal, 1);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(display_name, 0);
    RG_DEF_METHOD(ppd_name, 0);

    RG_DEF_METHOD(get_width, 1);
    RG_DEF_METHOD(get_height, 1);

    RG_DEF_METHOD_P(custom, 0);

    RG_DEF_METHOD(set_size, 3);

    RG_DEF_METHOD(get_default_top_margin, 1);
    RG_DEF_METHOD(get_default_bottom_margin, 1);
    RG_DEF_METHOD(get_default_left_margin, 1);
    RG_DEF_METHOD(get_default_right_margin, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkUnit */
    G_DEF_CLASS(GTK_TYPE_UNIT, "Unit", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_UNIT, "GTK_");

#endif
}
