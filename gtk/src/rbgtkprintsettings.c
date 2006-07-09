/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintsettings.c -

  $Author: ktou $
  $Date: 2006/07/09 14:08:33 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (GTK_PRINT_SETTINGS(RVAL2GOBJ(s)))

#define RVAL2SIZE(o) (GTK_PAPER_SIZE(RVAL2GOBJ(o)))

#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))
#define UNIT2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_UNIT))

#define RVAL2PO(o) (RVAL2GENUM(o, GTK_TYPE_PAGE_ORIENTATION))
#define PO2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PAGE_ORIENTATION))
#define RVAL2PD(o) (RVAL2GENUM(o, GTK_TYPE_PRINT_DUPLEX))
#define PD2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PRINT_DUPLEX))
#define RVAL2PQ(o) (RVAL2GENUM(o, GTK_TYPE_PRINT_QUALITY))
#define PQ2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PRINT_QUALITY))
#define RVAL2PP(o) (RVAL2GENUM(o, GTK_TYPE_PRINT_PAGES))
#define PP2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PRINT_PAGES))
#define RVAL2PS(o) (RVAL2GENUM(o, GTK_TYPE_PAGE_SET))
#define PS2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PAGE_SET))

static VALUE s_string, s_bool, s_double, s_length, s_int;

static VALUE
ps_initialize(VALUE self)
{
    GtkPrintSettings *settings;

    settings = gtk_print_settings_new();

    G_INITIALIZE(self, settings);
    return Qnil;
}

static VALUE
ps_copy(VALUE self)
{
    return GOBJ2RVALU(gtk_print_settings_copy(_SELF(self)));
}

static VALUE
ps_has_key(VALUE self, VALUE key)
{
    return CBOOL2RVAL(gtk_print_settings_has_key(_SELF(self), RVAL2CSTR(key)));
}

static VALUE
ps_get(VALUE self, VALUE key)
{
    return CSTR2RVAL(gtk_print_settings_get(_SELF(self), RVAL2CSTR(key)));
}

static VALUE
ps_set(VALUE self, VALUE key, VALUE value)
{
    gtk_print_settings_set(_SELF(self), RVAL2CSTR(key), RVAL2CSTR2(value));
    return Qnil;
}

static VALUE
ps_unset(VALUE self, VALUE key)
{
    gtk_print_settings_unset(_SELF(self), RVAL2CSTR(key));
    return Qnil;
}

static void
ps_foreach_cb(const gchar *key, const gchar *value, gpointer data)
{
    VALUE callback = (VALUE)data;
    rb_funcall(callback, id_call, 2, CSTR2RVAL(key), CSTR2RVAL(value));
}

static VALUE
ps_foreach(VALUE self)
{
    gtk_print_settings_foreach(_SELF(self), ps_foreach_cb,
                               (gpointer)G_BLOCK_PROC());
    return Qnil;
}

static VALUE
ps_get_bool(VALUE self, VALUE key)
{
    return CBOOL2RVAL(gtk_print_settings_get_bool(_SELF(self), RVAL2CSTR(key)));
}

static VALUE
ps_set_bool(VALUE self, VALUE key, VALUE value)
{
    gtk_print_settings_set_bool(_SELF(self), RVAL2CSTR(key), RVAL2CBOOL(value));
    return self;
}

static VALUE
ps_get_double(int argc, VALUE *argv, VALUE self)
{
    VALUE key, default_value;
    gdouble value;

    rb_scan_args(argc, argv, "11", &key, &default_value);
    if (NIL_P(default_value))
        value = gtk_print_settings_get_double(_SELF(self), RVAL2CSTR(key));
    else
        value = gtk_print_settings_get_double_with_default(_SELF(self),
                                                           RVAL2CSTR(key),
                                                           NUM2DBL(default_value));

    return rb_float_new(value);
}

static VALUE
ps_set_double(VALUE self, VALUE key, VALUE value)
{
    gtk_print_settings_set_double(_SELF(self), RVAL2CSTR(key), NUM2DBL(value));
    return self;
}

static VALUE
ps_get_length(VALUE self, VALUE key, VALUE unit)
{
    return rb_float_new(gtk_print_settings_get_length(_SELF(self),
                                                      RVAL2CSTR(key),
                                                      RVAL2UNIT(unit)));
}

static VALUE
ps_set_length(VALUE self, VALUE key, VALUE value, VALUE unit)
{
    gtk_print_settings_set_length(_SELF(self), RVAL2CSTR(key),
                                  NUM2DBL(value), RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_int(int argc, VALUE *argv, VALUE self)
{
    VALUE key, default_value;
    gint value;

    rb_scan_args(argc, argv, "11", &key, &default_value);
    if (NIL_P(default_value))
        value = gtk_print_settings_get_int(_SELF(self), RVAL2CSTR(key));
    else
        value = gtk_print_settings_get_int_with_default(_SELF(self),
                                                        RVAL2CSTR(key),
                                                        NUM2INT(default_value));

    return INT2NUM(value);
}

static VALUE
ps_set_int(VALUE self, VALUE key, VALUE value)
{
    gtk_print_settings_set_int(_SELF(self), RVAL2CSTR(key), NUM2INT(value));
    return self;
}

static VALUE
ps_get_generic(int argc, VALUE *argv, VALUE self)
{
    VALUE key, type, unit_or_default, result;

    rb_scan_args(argc, argv, "12", &key, &type, &unit_or_default);

    if (NIL_P(type) || (RTEST(rb_equal(type, s_string)))) {
        result = ps_get(self, key);
    } else if (RTEST(rb_equal(type, s_bool))) {
        result = ps_get_bool(self, key);
    } else if (RTEST(rb_equal(type, s_double))) {
        VALUE double_argv[2];
        double_argv[0] = key;
        double_argv[1] = unit_or_default;
        result = ps_get_double(2, double_argv, self);
    } else if (RTEST(rb_equal(type, s_length))) {
        result = ps_get_length(self, key, unit_or_default);
    } else if (RTEST(rb_equal(type, s_int))) {
        VALUE int_argv[2];
        int_argv[0] = key;
        int_argv[1] = unit_or_default;
        result = ps_get_int(2, int_argv, self);
    } else {
        VALUE inspected_type;
        inspected_type = rb_inspect(type);
        rb_raise(rb_eArgError,
                 "%s must be nil, :string, :bool, :double, :length or :int",
                 RVAL2CSTR(inspected_type));
    }

    return result;
}

static VALUE
ps_set_generic(int argc, VALUE *argv, VALUE self)
{
    VALUE key, value, unit;
    VALUE result = Qnil;

    rb_scan_args(argc, argv, "21", &key, &value, &unit);

    switch (TYPE(value)) {
      case T_STRING:
        result = ps_set(self, key, value);
        break;
      case T_TRUE:
      case T_FALSE:
        result = ps_set_bool(self, key, value);
        break;
      case T_FIXNUM:
        result = ps_set_int(self, key, value);
        break;
      case T_FLOAT:
        if (NIL_P(unit)) {
            result = ps_set_double(self, key, value);
        } else {
            result = ps_set_length(self, key, value, unit);
        }
        break;
      case T_NIL:
        result = ps_unset(self, key);
        break;
      default:
      {
          VALUE inspected_value;
          inspected_value = rb_inspect(value);
          rb_raise(rb_eArgError,
                   "%s is invalid value", RVAL2CSTR(inspected_value));
          break;
      }
    }

    return result;
}

static VALUE
ps_set_generic_indexer(int argc, VALUE *argv, VALUE self)
{
    VALUE result = Qnil;

    if (argc == 3) {
        VALUE swapped_argv[3];
        swapped_argv[0] = argv[0];
        swapped_argv[1] = argv[2];
        swapped_argv[2] = argv[1];
        result = ps_set_generic(argc, swapped_argv, self);
    } else if (argc == 2) {
        result = ps_set_generic(argc, argv, self);
    } else {
	rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 2 or 3)", argc);
    }

    return result;
}


/* Helpers: */
static VALUE
ps_get_printer(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_printer(_SELF(self)));
}

static VALUE
ps_set_printer(VALUE self, VALUE printer)
{
    gtk_print_settings_set_printer(_SELF(self), RVAL2CSTR2(printer));
    return self;
}

static VALUE
ps_get_orientation(VALUE self)
{
    return PO2RVAL(gtk_print_settings_get_orientation(_SELF(self)));
}

static VALUE
ps_set_orientation(VALUE self, VALUE orientation)
{
    gtk_print_settings_set_orientation(_SELF(self), RVAL2PO(orientation));
    return self;
}

static VALUE
ps_get_paper_size(VALUE self)
{
    return GOBJ2RVAL(gtk_print_settings_get_paper_size(_SELF(self)));
}

static VALUE
ps_set_paper_size(VALUE self, VALUE paper_size)
{
    gtk_print_settings_set_paper_size(_SELF(self), RVAL2SIZE(paper_size));
    return self;
}

static VALUE
ps_get_paper_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_print_settings_get_paper_width(_SELF(self),
                                                           RVAL2UNIT(unit)));
}

static VALUE
ps_set_paper_width(VALUE self, VALUE paper_width, VALUE unit)
{
    gtk_print_settings_set_paper_width(_SELF(self), NUM2DBL(paper_width),
                                       RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_paper_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_print_settings_get_paper_height(_SELF(self),
                                                            RVAL2UNIT(unit)));
}

static VALUE
ps_set_paper_height(VALUE self, VALUE paper_height, VALUE unit)
{
    gtk_print_settings_set_paper_height(_SELF(self), NUM2DBL(paper_height),
                                        RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_use_color(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_use_color(_SELF(self)));
}

static VALUE
ps_set_use_color(VALUE self, VALUE use_color)
{
    gtk_print_settings_set_use_color(_SELF(self), RVAL2CBOOL(use_color));
    return self;
}

static VALUE
ps_get_collate(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_collate(_SELF(self)));
}

static VALUE
ps_set_collate(VALUE self, VALUE collate)
{
    gtk_print_settings_set_collate(_SELF(self), RVAL2CBOOL(collate));
    return self;
}

static VALUE
ps_get_reverse(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_reverse(_SELF(self)));
}

static VALUE
ps_set_reverse(VALUE self, VALUE reverse)
{
    gtk_print_settings_set_reverse(_SELF(self), RVAL2CBOOL(reverse));
    return self;
}

static VALUE
ps_get_duplex(VALUE self)
{
    return PD2RVAL(gtk_print_settings_get_duplex(_SELF(self)));
}

static VALUE
ps_set_duplex(VALUE self, VALUE duplex)
{
    gtk_print_settings_set_duplex(_SELF(self), RVAL2PD(duplex));
    return self;
}

static VALUE
ps_get_quality(VALUE self)
{
    return PQ2RVAL(gtk_print_settings_get_quality(_SELF(self)));
}

static VALUE
ps_set_quality(VALUE self, VALUE quality)
{
    gtk_print_settings_set_quality(_SELF(self), RVAL2PQ(quality));
    return self;
}

static VALUE
ps_get_n_copies(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_n_copies(_SELF(self)));
}

static VALUE
ps_set_n_copies(VALUE self, VALUE n_copies)
{
    gtk_print_settings_set_n_copies(_SELF(self), NUM2INT(n_copies));
    return self;
}

static VALUE
ps_get_number_up(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_number_up(_SELF(self)));
}

static VALUE
ps_set_number_up(VALUE self, VALUE number_up)
{
    gtk_print_settings_set_number_up(_SELF(self), NUM2INT(number_up));
    return self;
}

static VALUE
ps_get_resolution(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_resolution(_SELF(self)));
}

static VALUE
ps_set_resolution(VALUE self, VALUE resolution)
{
    gtk_print_settings_set_resolution(_SELF(self), NUM2INT(resolution));
    return self;
}

static VALUE
ps_get_scale(VALUE self)
{
    return rb_float_new(gtk_print_settings_get_scale(_SELF(self)));
}

static VALUE
ps_set_scale(VALUE self, VALUE scale)
{
    gtk_print_settings_set_scale(_SELF(self), NUM2DBL(scale));
    return self;
}

static VALUE
ps_get_print_pages(VALUE self)
{
    return PP2RVAL(gtk_print_settings_get_print_pages(_SELF(self)));
}

static VALUE
ps_set_print_pages(VALUE self, VALUE print_pages)
{
    gtk_print_settings_set_print_pages(_SELF(self), RVAL2PP(print_pages));
    return self;
}

static VALUE
ps_get_page_ranges(VALUE self)
{
    VALUE rb_ranges;
    GtkPageRange *ranges;
    int i, num_ranges;

    ranges = gtk_print_settings_get_page_ranges(_SELF(self), &num_ranges);

    rb_ranges = rb_ary_new2(num_ranges);
    for (i = 0; i < num_ranges; i++) {
        rb_ary_push(rb_ranges,
                    rb_ary_new3(2,
                                INT2NUM(ranges[i].start),
                                INT2NUM(ranges[i].end)));
    }
    g_free(ranges);
    return rb_ranges;
}

static VALUE
ps_set_page_ranges(VALUE self, VALUE rb_page_ranges)
{
    GtkPageRange *page_ranges;
    gint i, num_ranges;

    num_ranges = RARRAY(rb_page_ranges)->len;
    page_ranges = ALLOCA_N(GtkPageRange, num_ranges);
    for (i = 0; i < num_ranges; i++) {
        VALUE page_range;
        page_range = RARRAY(rb_page_ranges)->ptr[i];
        page_ranges[i].start = NUM2INT(RARRAY(page_range)->ptr[0]);
        page_ranges[i].end = NUM2INT(RARRAY(page_range)->ptr[1]);
    }
    gtk_print_settings_set_page_ranges(_SELF(self), page_ranges, num_ranges);
    return self;
}

static VALUE
ps_get_page_set(VALUE self)
{
    return PS2RVAL(gtk_print_settings_get_page_set(_SELF(self)));
}

static VALUE
ps_set_page_set(VALUE self, VALUE page_set)
{
    gtk_print_settings_set_page_set(_SELF(self), RVAL2PS(page_set));
    return self;
}

static VALUE
ps_get_default_source(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_default_source(_SELF(self)));
}

static VALUE
ps_set_default_source(VALUE self, VALUE default_source)
{
    gtk_print_settings_set_default_source(_SELF(self),
                                          RVAL2CSTR2(default_source));
    return self;
}

static VALUE
ps_get_media_type(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_media_type(_SELF(self)));
}

static VALUE
ps_set_media_type(VALUE self, VALUE media_type)
{
    gtk_print_settings_set_media_type(_SELF(self), RVAL2CSTR2(media_type));
    return self;
}

static VALUE
ps_get_dither(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_dither(_SELF(self)));
}

static VALUE
ps_set_dither(VALUE self, VALUE dither)
{
    gtk_print_settings_set_dither(_SELF(self), RVAL2CSTR2(dither));
    return self;
}

static VALUE
ps_get_finishings(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_finishings(_SELF(self)));
}

static VALUE
ps_set_finishings(VALUE self, VALUE finishings)
{
    gtk_print_settings_set_finishings(_SELF(self), RVAL2CSTR2(finishings));
    return self;
}

static VALUE
ps_get_output_bin(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_output_bin(_SELF(self)));
}

static VALUE
ps_set_output_bin(VALUE self, VALUE output_bin)
{
    gtk_print_settings_set_output_bin(_SELF(self), RVAL2CSTR2(output_bin));
    return self;
}
#endif

void
Init_gtk_print_settings()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE gPrintSettings;

    s_string = ID2SYM(rb_intern("string"));
    s_bool = ID2SYM(rb_intern("bool"));
    s_double = ID2SYM(rb_intern("double"));
    s_length = ID2SYM(rb_intern("length"));
    s_int = ID2SYM(rb_intern("int"));

    G_DEF_CLASS(GTK_TYPE_PAGE_ORIENTATION, "PageOrientation", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_DUPLEX, "PrintDuplex", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_QUALITY, "PrintQuality", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_PAGES, "PrintPages", mGtk);
    G_DEF_CLASS(GTK_TYPE_PAGE_SET, "PageSet", mGtk);

    gPrintSettings = G_DEF_CLASS(GTK_TYPE_PRINT_SETTINGS,
                                 "PrintSettings", mGtk);

    rb_include_module(gPrintSettings, rb_mEnumerable);

    rb_define_method(gPrintSettings, "initialize", ps_initialize, 0);
    rb_define_method(gPrintSettings, "dup", ps_copy, 0);
    rb_define_method(gPrintSettings, "has_key?", ps_has_key, 1);

    rb_define_method(gPrintSettings, "get", ps_get_generic, -1);
    rb_define_alias(gPrintSettings, "[]", "get");
    rb_define_method(gPrintSettings, "get_bool", ps_get_bool, 1);
    rb_define_method(gPrintSettings, "get_double", ps_get_double, -1);
    rb_define_method(gPrintSettings, "get_length", ps_get_length, 2);
    rb_define_method(gPrintSettings, "get_int", ps_get_int, -1);

    rb_define_method(gPrintSettings, "set", ps_set_generic, -1);
    rb_define_method(gPrintSettings, "[]=", ps_set_generic_indexer, -1);

    rb_define_method(gPrintSettings, "unset", ps_unset, -1);
    rb_define_alias(gPrintSettings, "delete", "unset");

    rb_define_method(gPrintSettings, "each", ps_foreach, 0);

    rb_define_const(gPrintSettings, "PRINTER",
                    rb_str_new2(GTK_PRINT_SETTINGS_PRINTER));
    rb_define_const(gPrintSettings, "ORIENTATION",
                    rb_str_new2(GTK_PRINT_SETTINGS_ORIENTATION));
    rb_define_const(gPrintSettings, "PAPER_FORMAT",
                    rb_str_new2(GTK_PRINT_SETTINGS_PAPER_FORMAT));
    rb_define_const(gPrintSettings, "PAPER_WIDTH",
                    rb_str_new2(GTK_PRINT_SETTINGS_PAPER_WIDTH));
    rb_define_const(gPrintSettings, "PAPER_HEIGHT",
                    rb_str_new2(GTK_PRINT_SETTINGS_PAPER_HEIGHT));
    rb_define_const(gPrintSettings, "N_COPIES",
                    rb_str_new2(GTK_PRINT_SETTINGS_N_COPIES));
    rb_define_const(gPrintSettings, "DEFAULT_SOURCE",
                    rb_str_new2(GTK_PRINT_SETTINGS_DEFAULT_SOURCE));
    rb_define_const(gPrintSettings, "QUALITY",
                    rb_str_new2(GTK_PRINT_SETTINGS_QUALITY));
    rb_define_const(gPrintSettings, "RESOLUTION",
                    rb_str_new2(GTK_PRINT_SETTINGS_RESOLUTION));
    rb_define_const(gPrintSettings, "USE_COLOR",
                    rb_str_new2(GTK_PRINT_SETTINGS_USE_COLOR));
    rb_define_const(gPrintSettings, "DUPLEX",
                    rb_str_new2(GTK_PRINT_SETTINGS_DUPLEX));
    rb_define_const(gPrintSettings, "COLLATE",
                    rb_str_new2(GTK_PRINT_SETTINGS_COLLATE));
    rb_define_const(gPrintSettings, "REVERSE",
                    rb_str_new2(GTK_PRINT_SETTINGS_REVERSE));
    rb_define_const(gPrintSettings, "MEDIA_TYPE",
                    rb_str_new2(GTK_PRINT_SETTINGS_MEDIA_TYPE));
    rb_define_const(gPrintSettings, "DITHER",
                    rb_str_new2(GTK_PRINT_SETTINGS_DITHER));
    rb_define_const(gPrintSettings, "SCALE",
                    rb_str_new2(GTK_PRINT_SETTINGS_SCALE));
    rb_define_const(gPrintSettings, "PRINT_PAGES",
                    rb_str_new2(GTK_PRINT_SETTINGS_PRINT_PAGES));
    rb_define_const(gPrintSettings, "PAGE_RANGES",
                    rb_str_new2(GTK_PRINT_SETTINGS_PAGE_RANGES));
    rb_define_const(gPrintSettings, "PAGE_SET",
                    rb_str_new2(GTK_PRINT_SETTINGS_PAGE_SET));
    rb_define_const(gPrintSettings, "FINISHINGS",
                    rb_str_new2(GTK_PRINT_SETTINGS_FINISHINGS));
    rb_define_const(gPrintSettings, "NUMBER_UP",
                    rb_str_new2(GTK_PRINT_SETTINGS_NUMBER_UP));
    rb_define_const(gPrintSettings, "OUTPUT_BIN",
                    rb_str_new2(GTK_PRINT_SETTINGS_OUTPUT_BIN));

    rb_define_const(gPrintSettings, "OUTPUT_FILE_FORMAT",
                    rb_str_new2(GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT));
    rb_define_const(gPrintSettings, "OUTPUT_URI",
                    rb_str_new2(GTK_PRINT_SETTINGS_OUTPUT_URI));

    rb_define_const(gPrintSettings, "WIN32_DRIVER_VERSION",
                    rb_str_new2(GTK_PRINT_SETTINGS_WIN32_DRIVER_VERSION));
    rb_define_const(gPrintSettings, "WIN32_DRIVER_EXTRA",
                    rb_str_new2(GTK_PRINT_SETTINGS_WIN32_DRIVER_EXTRA));

    /* Helpers: */
    rb_define_method(gPrintSettings, "printer", ps_get_printer, 0);
    rb_define_method(gPrintSettings, "set_printer", ps_set_printer, 1);
    rb_define_method(gPrintSettings, "orientation", ps_get_orientation, 0);
    rb_define_method(gPrintSettings, "set_orientation", ps_set_orientation, 1);
    rb_define_method(gPrintSettings, "paper_size", ps_get_paper_size, 0);
    rb_define_method(gPrintSettings, "set_paper_size", ps_set_paper_size, 1);
    rb_define_method(gPrintSettings, "paper_width", ps_get_paper_width, 1);
    rb_define_method(gPrintSettings, "set_paper_width", ps_set_paper_width, 2);
    rb_define_method(gPrintSettings, "paper_height", ps_get_paper_height, 1);
    rb_define_method(gPrintSettings, "set_paper_height",
                     ps_set_paper_height, 2);
    rb_define_method(gPrintSettings, "use_color?", ps_get_use_color, 0);
    rb_define_method(gPrintSettings, "set_use_color", ps_set_use_color, 1);
    rb_define_method(gPrintSettings, "collate?", ps_get_collate, 0);
    rb_define_method(gPrintSettings, "set_collate", ps_set_collate, 1);
    rb_define_method(gPrintSettings, "reverse?", ps_get_reverse, 0);
    rb_define_method(gPrintSettings, "set_reverse", ps_set_reverse, 1);
    rb_define_method(gPrintSettings, "duplex", ps_get_duplex, 0);
    rb_define_method(gPrintSettings, "set_duplex", ps_set_duplex, 1);
    rb_define_method(gPrintSettings, "quality", ps_get_quality, 0);
    rb_define_method(gPrintSettings, "set_quality", ps_set_quality, 1);
    rb_define_method(gPrintSettings, "n_copies", ps_get_n_copies, 0);
    rb_define_method(gPrintSettings, "set_n_copies", ps_set_n_copies, 1);
    rb_define_method(gPrintSettings, "number_up", ps_get_number_up, 0);
    rb_define_method(gPrintSettings, "set_number_up", ps_set_number_up, 1);
    rb_define_method(gPrintSettings, "resolution", ps_get_resolution, 0);
    rb_define_method(gPrintSettings, "set_resolution", ps_set_resolution, 1);
    rb_define_method(gPrintSettings, "scale", ps_get_scale, 0);
    rb_define_method(gPrintSettings, "set_scale", ps_set_scale, 1);
    rb_define_method(gPrintSettings, "print_pages", ps_get_print_pages, 0);
    rb_define_method(gPrintSettings, "set_print_pages", ps_set_print_pages, 1);
    rb_define_method(gPrintSettings, "page_ranges", ps_get_page_ranges, 0);
    rb_define_method(gPrintSettings, "set_page_ranges", ps_set_page_ranges, 1);
    rb_define_method(gPrintSettings, "page_set", ps_get_page_set, 0);
    rb_define_method(gPrintSettings, "set_page_set", ps_set_page_set, 1);
    rb_define_method(gPrintSettings, "default_source",
                     ps_get_default_source, 0);
    rb_define_method(gPrintSettings, "set_default_source",
                     ps_set_default_source, 1);
    rb_define_method(gPrintSettings, "media_type", ps_get_media_type, 0);
    rb_define_method(gPrintSettings, "set_media_type", ps_set_media_type, 1);
    rb_define_method(gPrintSettings, "dither", ps_get_dither, 0);
    rb_define_method(gPrintSettings, "set_dither", ps_set_dither, 1);
    rb_define_method(gPrintSettings, "finishings", ps_get_finishings, 0);
    rb_define_method(gPrintSettings, "set_finishings", ps_set_finishings, 1);
    rb_define_method(gPrintSettings, "output_bin", ps_get_output_bin, 0);
    rb_define_method(gPrintSettings, "set_output_bin", ps_set_output_bin, 1);

    G_DEF_SETTERS(gPrintSettings);
#endif
}
