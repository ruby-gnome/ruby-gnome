/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkpapersize.c -

  $Author: mutoh $
  $Date: 2006/10/23 17:10:15 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (RVAL2BOXED(s, GTK_TYPE_PAPER_SIZE))
#define SIZE2RVAL(o) (BOXED2RVAL(o, GTK_TYPE_PAPER_SIZE))

#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))

static VALUE
ps_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkPaperSize *size;

    if (argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "01", &name);
        size = gtk_paper_size_new(RVAL2CSTR2(name));
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
ps_is_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(RTEST(rb_equal(rb_obj_class(self), rb_obj_class(other))) &&
                      gtk_paper_size_is_equal(_SELF(self), _SELF(other)));
}

/* The width is always the shortest side, measure in mm */
static VALUE
ps_get_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_name(_SELF(self)));
}

static VALUE
ps_get_display_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_display_name(_SELF(self)));
}

static VALUE
ps_get_ppd_name(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_ppd_name(_SELF(self)));
}

static VALUE
ps_get_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_width(_SELF(self), RVAL2UNIT(unit)));
}

static VALUE
ps_get_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_height(_SELF(self), RVAL2UNIT(unit)));
}

static VALUE
ps_is_custom(VALUE self)
{
    return CBOOL2RVAL(gtk_paper_size_is_custom(_SELF(self)));
}

/* Only for custom sizes: */
static VALUE
ps_set_size(VALUE self, VALUE width, VALUE height, VALUE unit)
{
    gtk_paper_size_set_size(_SELF(self), NUM2DBL(width), NUM2DBL(height),
                            RVAL2UNIT(unit));
    return self;
}


static VALUE
ps_get_default_top_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_top_margin(_SELF(self),
                                                              RVAL2UNIT(unit)));
}

static VALUE
ps_get_default_bottom_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_bottom_margin(_SELF(self),
                                                                 RVAL2UNIT(unit)));
}

static VALUE
ps_get_default_left_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_left_margin(_SELF(self),
                                                               RVAL2UNIT(unit)));
}

static VALUE
ps_get_default_right_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_paper_size_get_default_right_margin(_SELF(self),
                                                                RVAL2UNIT(unit)));
}

static VALUE
ps_s_get_default(VALUE self)
{
    return CSTR2RVAL(gtk_paper_size_get_default());
}
#endif

void
Init_gtk_paper_size()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE gPaperSize = G_DEF_CLASS(GTK_TYPE_PAPER_SIZE, "PaperSize", mGtk);

    rb_define_singleton_method(gPaperSize, "default", ps_s_get_default, 0);

    rb_define_const(gPaperSize, "A3", rb_str_new2(GTK_PAPER_NAME_A3));
    rb_define_const(gPaperSize, "A4", rb_str_new2(GTK_PAPER_NAME_A4));
    rb_define_const(gPaperSize, "A5", rb_str_new2(GTK_PAPER_NAME_A5));
    rb_define_const(gPaperSize, "B5", rb_str_new2(GTK_PAPER_NAME_B5));
    rb_define_const(gPaperSize, "LETTER", rb_str_new2(GTK_PAPER_NAME_LETTER));
    rb_define_const(gPaperSize, "EXECUTIVE",
                    rb_str_new2(GTK_PAPER_NAME_EXECUTIVE));
    rb_define_const(gPaperSize, "LEGAL", rb_str_new2(GTK_PAPER_NAME_LEGAL));

    rb_define_method(gPaperSize, "initialize", ps_initialize, -1);
    rb_define_method(gPaperSize, "==", ps_is_equal, 1);

    rb_define_method(gPaperSize, "name", ps_get_name, 0);
    rb_define_method(gPaperSize, "display_name", ps_get_display_name, 0);
    rb_define_method(gPaperSize, "ppd_name", ps_get_ppd_name, 0);

    rb_define_method(gPaperSize, "get_width", ps_get_width, 1);
    rb_define_method(gPaperSize, "get_height", ps_get_height, 1);

    rb_define_method(gPaperSize, "custom?", ps_is_custom, 0);

    rb_define_method(gPaperSize, "set_size", ps_set_size, 3);

    rb_define_method(gPaperSize, "get_default_top_margin",
                     ps_get_default_top_margin, 1);
    rb_define_method(gPaperSize, "get_default_bottom_margin",
                     ps_get_default_bottom_margin, 1);
    rb_define_method(gPaperSize, "get_default_left_margin",
                     ps_get_default_left_margin, 1);
    rb_define_method(gPaperSize, "get_default_right_margin",
                     ps_get_default_right_margin, 1);

    G_DEF_SETTERS(gPaperSize);
#endif
}
