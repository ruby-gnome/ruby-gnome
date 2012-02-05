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

#define RG_TARGET_NAMESPACE cPrintSettings
#define _SELF(s) (GTK_PRINT_SETTINGS(RVAL2GOBJ(s)))

#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))

static VALUE s_string, s_bool, s_double, s_length, s_int;

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2;
    GtkPrintSettings* settings = NULL;
    GError *error = NULL;
    rb_scan_args(argc, argv, "02", &arg1, &arg2);
    if (NIL_P(arg1)){
        settings = gtk_print_settings_new();
    } else if (NIL_P(arg2)){
        settings = gtk_print_settings_new_from_file(RVAL2CSTR(arg1), &error);
        if (!settings) {
            RAISE_GERROR(error);
        }
    } else {
        settings = gtk_print_settings_new_from_key_file((GKeyFile*)RVAL2BOXED(arg1, G_TYPE_KEY_FILE),
                                                        RVAL2CSTR(arg2),
                                                        &error);
        if (!settings) {
            RAISE_GERROR(error);
        }
    }
    G_INITIALIZE(self, settings);
    return Qnil;
}
#else
static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_print_settings_new());
    return Qnil;
}
#endif

static VALUE
rg_dup(VALUE self)
{
    return GOBJ2RVALU(gtk_print_settings_copy(_SELF(self)));
}

static VALUE
rg_has_key_p(VALUE self, VALUE key)
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
    gtk_print_settings_set(_SELF(self), RVAL2CSTR(key), RVAL2CSTR_ACCEPT_NIL(value));
    return self;
}

static VALUE
rg_unset(VALUE self, VALUE key)
{
    gtk_print_settings_unset(_SELF(self), RVAL2CSTR(key));
    return self;
}

static void
ps_foreach_cb(const gchar *key, const gchar *value, gpointer data)
{
    rb_funcall((VALUE)data, id_call, 2, CSTR2RVAL(key), CSTR2RVAL(value));
}

static VALUE
rg_each(VALUE self)
{
    gtk_print_settings_foreach(_SELF(self), ps_foreach_cb,
                               (gpointer)rb_block_proc());
    return self;
}

static VALUE
rg_get_bool(VALUE self, VALUE key)
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
rg_get_double(int argc, VALUE *argv, VALUE self)
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
rg_get_length(VALUE self, VALUE key, VALUE unit)
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
rg_get_int(int argc, VALUE *argv, VALUE self)
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
rg_get(int argc, VALUE *argv, VALUE self)
{
    VALUE key, type, unit_or_default, result;

    rb_scan_args(argc, argv, "12", &key, &type, &unit_or_default);

    if (NIL_P(type) || (RVAL2CBOOL(rb_equal(type, s_string)))) {
        result = ps_get(self, key);
    } else if (RVAL2CBOOL(rb_equal(type, s_bool))) {
        result = rg_get_bool(self, key);
    } else if (RVAL2CBOOL(rb_equal(type, s_double))) {
        VALUE double_argv[2];
        double_argv[0] = key;
        double_argv[1] = unit_or_default;
        result = rg_get_double(2, double_argv, self);
    } else if (RVAL2CBOOL(rb_equal(type, s_length))) {
        result = rg_get_length(self, key, unit_or_default);
    } else if (RVAL2CBOOL(rb_equal(type, s_int))) {
        VALUE int_argv[2];
        int_argv[0] = key;
        int_argv[1] = unit_or_default;
        result = rg_get_int(2, int_argv, self);
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
rg_set(int argc, VALUE *argv, VALUE self)
{
    VALUE key, value, unit;

    rb_scan_args(argc, argv, "21", &key, &value, &unit);

    switch (TYPE(value)) {
      case T_STRING:
        ps_set(self, key, value);
        break;
      case T_TRUE:
      case T_FALSE:
        ps_set_bool(self, key, value);
        break;
      case T_FIXNUM:
        ps_set_int(self, key, value);
        break;
      case T_FLOAT:
        if (NIL_P(unit)) {
            ps_set_double(self, key, value);
        } else {
            ps_set_length(self, key, value, unit);
        }
        break;
      case T_NIL:
        rg_unset(self, key);
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

    return self;
}

static VALUE
rg_operator_ps_set_generic_indexer(int argc, VALUE *argv, VALUE self)
{
    if (argc == 3) {
        VALUE swapped_argv[3];
        swapped_argv[0] = argv[0];
        swapped_argv[1] = argv[2];
        swapped_argv[2] = argv[1];
        rg_set(argc, swapped_argv, self);
    } else if (argc == 2) {
        rg_set(argc, argv, self);
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 2 or 3)", argc);
    }

    return argv[2];
}

/* Helpers: */
static VALUE
rg_printer(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_printer(_SELF(self)));
}

static VALUE
rg_set_printer(VALUE self, VALUE printer)
{
    gtk_print_settings_set_printer(_SELF(self), RVAL2CSTR_ACCEPT_NIL(printer));
    return self;
}

static VALUE
rg_orientation(VALUE self)
{
    return GENUM2RVAL(gtk_print_settings_get_orientation(_SELF(self)), 
                      GTK_TYPE_PAGE_ORIENTATION);
}

static VALUE
rg_set_orientation(VALUE self, VALUE orientation)
{
    gtk_print_settings_set_orientation(_SELF(self),
                                       RVAL2GENUM(orientation, GTK_TYPE_PAGE_ORIENTATION));
    return self;
}

static VALUE
rg_paper_size(VALUE self)
{
    return GOBJ2RVAL(gtk_print_settings_get_paper_size(_SELF(self)));
}

static VALUE
rg_set_paper_size(VALUE self, VALUE paper_size)
{
    gtk_print_settings_set_paper_size(_SELF(self), 
                                      RVAL2BOXED(paper_size, GTK_TYPE_PAPER_SIZE));
    return self;
}

static VALUE
rg_paper_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_print_settings_get_paper_width(_SELF(self),
                                                           RVAL2UNIT(unit)));
}

static VALUE
rg_set_paper_width(VALUE self, VALUE paper_width, VALUE unit)
{
    gtk_print_settings_set_paper_width(_SELF(self), NUM2DBL(paper_width),
                                       RVAL2UNIT(unit));
    return self;
}

static VALUE
rg_paper_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_print_settings_get_paper_height(_SELF(self),
                                                            RVAL2UNIT(unit)));
}

static VALUE
rg_set_paper_height(VALUE self, VALUE paper_height, VALUE unit)
{
    gtk_print_settings_set_paper_height(_SELF(self), NUM2DBL(paper_height),
                                        RVAL2UNIT(unit));
    return self;
}

static VALUE
rg_use_color_p(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_use_color(_SELF(self)));
}

static VALUE
rg_set_use_color(VALUE self, VALUE use_color)
{
    gtk_print_settings_set_use_color(_SELF(self), RVAL2CBOOL(use_color));
    return self;
}

static VALUE
rg_collate_p(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_collate(_SELF(self)));
}

static VALUE
rg_set_collate(VALUE self, VALUE collate)
{
    gtk_print_settings_set_collate(_SELF(self), RVAL2CBOOL(collate));
    return self;
}

static VALUE
rg_reverse_p(VALUE self)
{
    return CBOOL2RVAL(gtk_print_settings_get_reverse(_SELF(self)));
}

static VALUE
rg_set_reverse(VALUE self, VALUE reverse)
{
    gtk_print_settings_set_reverse(_SELF(self), RVAL2CBOOL(reverse));
    return self;
}

static VALUE
rg_duplex(VALUE self)
{
    return GENUM2RVAL(gtk_print_settings_get_duplex(_SELF(self)), 
                      GTK_TYPE_PRINT_DUPLEX);
}

static VALUE
rg_set_duplex(VALUE self, VALUE duplex)
{
    gtk_print_settings_set_duplex(_SELF(self), 
                                  RVAL2GENUM(duplex, GTK_TYPE_PRINT_DUPLEX));
    return self;
}

static VALUE
rg_quality(VALUE self)
{
    return GENUM2RVAL(gtk_print_settings_get_quality(_SELF(self)), 
                      GTK_TYPE_PRINT_QUALITY);
}

static VALUE
rg_set_quality(VALUE self, VALUE quality)
{
    gtk_print_settings_set_quality(_SELF(self), 
                                   RVAL2GENUM(quality, GTK_TYPE_PRINT_QUALITY));
    return self;
}

static VALUE
rg_n_copies(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_n_copies(_SELF(self)));
}

static VALUE
rg_set_n_copies(VALUE self, VALUE n_copies)
{
    gtk_print_settings_set_n_copies(_SELF(self), NUM2INT(n_copies));
    return self;
}

static VALUE
rg_number_up(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_number_up(_SELF(self)));
}

static VALUE
rg_set_number_up(VALUE self, VALUE number_up)
{
    gtk_print_settings_set_number_up(_SELF(self), NUM2INT(number_up));
    return self;
}

static VALUE
rg_resolution(VALUE self)
{
    return INT2NUM(gtk_print_settings_get_resolution(_SELF(self)));
}

static VALUE
rg_set_resolution(VALUE self, VALUE resolution)
{
    gtk_print_settings_set_resolution(_SELF(self), NUM2INT(resolution));
    return self;
}

static VALUE
rg_scale(VALUE self)
{
    return rb_float_new(gtk_print_settings_get_scale(_SELF(self)));
}

static VALUE
rg_set_scale(VALUE self, VALUE scale)
{
    gtk_print_settings_set_scale(_SELF(self), NUM2DBL(scale));
    return self;
}

static VALUE
rg_print_pages(VALUE self)
{
    return GENUM2RVAL(gtk_print_settings_get_print_pages(_SELF(self)), 
                      GTK_TYPE_PRINT_PAGES);
}

static VALUE
rg_set_print_pages(VALUE self, VALUE print_pages)
{
    gtk_print_settings_set_print_pages(_SELF(self), RVAL2GENUM(print_pages, GTK_TYPE_PRINT_PAGES));
    return self;
}

static VALUE
rg_page_ranges(VALUE self)
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

struct rbgtk_rval2gtkpageranges_args {
    VALUE ary;
    long n;
    GtkPageRange *result;
};

static VALUE
rbgtk_rval2gtkpageranges_body(VALUE value)
{
    long i;
    struct rbgtk_rval2gtkpageranges_args *args = (struct rbgtk_rval2gtkpageranges_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE ary = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);

        if (RARRAY_LEN(ary) != 2)
            rb_raise(rb_eArgError, "range %ld should be array of size 2", i);

        args->result[i].start = NUM2INT(RARRAY_PTR(ary)[0]);
        args->result[i].end = NUM2INT(RARRAY_PTR(ary)[1]);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgtk_rval2gtkpageranges_rescue(VALUE value)
{
    g_free(((struct rbgtk_rval2gtkpageranges_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GtkPageRange *
rbgtk_rval2gtkpageranges(VALUE value, long *n)
{
    struct rbgtk_rval2gtkpageranges_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GtkPageRange, args.n + 1);

    rb_rescue(rbgtk_rval2gtkpageranges_body, (VALUE)&args,
              rbgtk_rval2gtkpageranges_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2GTKPAGERANGES(value, n) rbgtk_rval2gtkpageranges(value, n)

static VALUE
rg_set_page_ranges(VALUE self, VALUE rbpage_ranges)
{
    GtkPrintSettings *settings = _SELF(self);
    long n;
    GtkPageRange *page_ranges = RVAL2GTKPAGERANGES(rbpage_ranges, &n);

    gtk_print_settings_set_page_ranges(settings, page_ranges, n);

    g_free(page_ranges);

    return self;
}

static VALUE
rg_page_set(VALUE self)
{
    return GENUM2RVAL(gtk_print_settings_get_page_set(_SELF(self)), 
                      GTK_TYPE_PAGE_SET);
}

static VALUE
rg_set_page_set(VALUE self, VALUE page_set)
{
    gtk_print_settings_set_page_set(_SELF(self), 
                                    RVAL2GENUM(page_set, GTK_TYPE_PAGE_SET));
    return self;
}

static VALUE
rg_default_source(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_default_source(_SELF(self)));
}

static VALUE
rg_set_default_source(VALUE self, VALUE default_source)
{
    gtk_print_settings_set_default_source(_SELF(self),
                                          RVAL2CSTR_ACCEPT_NIL(default_source));
    return self;
}

static VALUE
rg_media_type(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_media_type(_SELF(self)));
}

static VALUE
rg_set_media_type(VALUE self, VALUE media_type)
{
    gtk_print_settings_set_media_type(_SELF(self), RVAL2CSTR_ACCEPT_NIL(media_type));
    return self;
}

static VALUE
rg_dither(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_dither(_SELF(self)));
}

static VALUE
rg_set_dither(VALUE self, VALUE dither)
{
    gtk_print_settings_set_dither(_SELF(self), RVAL2CSTR_ACCEPT_NIL(dither));
    return self;
}

static VALUE
rg_finishings(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_finishings(_SELF(self)));
}

static VALUE
rg_set_finishings(VALUE self, VALUE finishings)
{
    gtk_print_settings_set_finishings(_SELF(self), RVAL2CSTR_ACCEPT_NIL(finishings));
    return self;
}

static VALUE
rg_output_bin(VALUE self)
{
    return CSTR2RVAL(gtk_print_settings_get_output_bin(_SELF(self)));
}

static VALUE
rg_set_output_bin(VALUE self, VALUE output_bin)
{
    gtk_print_settings_set_output_bin(_SELF(self), RVAL2CSTR_ACCEPT_NIL(output_bin));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
rg_to_file(VALUE self, VALUE file_name)
{
    GError *error = NULL;
    if (!gtk_print_settings_to_file(_SELF(self), RVAL2CSTR(file_name), &error)) {
        RAISE_GERROR(error);
    }
    return self;
}
static VALUE
rg_to_key_file(int argc, VALUE *argv, VALUE self)
{
    VALUE key_file, group_name;
    rb_scan_args(argc, argv, "11", &key_file, &group_name);
    gtk_print_settings_to_key_file(_SELF(self),
                                   (GKeyFile*)RVAL2BOXED(key_file, G_TYPE_KEY_FILE),
                                   RVAL2CSTR_ACCEPT_NIL(group_name));
    return self;
}
#endif

void
Init_gtk_print_settings(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE;

    s_string = ID2SYM(rb_intern("string"));
    s_bool = ID2SYM(rb_intern("bool"));
    s_double = ID2SYM(rb_intern("double"));
    s_length = ID2SYM(rb_intern("length"));
    s_int = ID2SYM(rb_intern("int"));

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_SETTINGS,
                                 "PrintSettings", mGtk);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

#if GTK_CHECK_VERSION(2,12,0)
    RG_DEF_METHOD(initialize, -1);
#else
    RG_DEF_METHOD(initialize, 0);
#endif
    RG_DEF_METHOD(dup, 0);
    RG_DEF_METHOD_P(has_key, 1);

    RG_DEF_METHOD(get, -1);
    RG_DEF_ALIAS("[]", "get");
    RG_DEF_METHOD(get_bool, 1);
    RG_DEF_METHOD(get_double, -1);
    RG_DEF_METHOD(get_length, 2);
    RG_DEF_METHOD(get_int, -1);

    RG_DEF_METHOD(set, -1);
    RG_DEF_METHOD_OPERATOR("[]=", ps_set_generic_indexer, -1);

    RG_DEF_METHOD(unset, -1);
    RG_DEF_ALIAS("delete", "unset");

    RG_DEF_METHOD(each, 0);

    rb_define_const(RG_TARGET_NAMESPACE, "PRINTER",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PRINTER));
    rb_define_const(RG_TARGET_NAMESPACE, "ORIENTATION",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_ORIENTATION));
    rb_define_const(RG_TARGET_NAMESPACE, "PAPER_FORMAT",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PAPER_FORMAT));
    rb_define_const(RG_TARGET_NAMESPACE, "PAPER_WIDTH",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PAPER_WIDTH));
    rb_define_const(RG_TARGET_NAMESPACE, "PAPER_HEIGHT",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PAPER_HEIGHT));
    rb_define_const(RG_TARGET_NAMESPACE, "N_COPIES",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_N_COPIES));
    rb_define_const(RG_TARGET_NAMESPACE, "DEFAULT_SOURCE",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_DEFAULT_SOURCE));
    rb_define_const(RG_TARGET_NAMESPACE, "QUALITY",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_QUALITY));
    rb_define_const(RG_TARGET_NAMESPACE, "RESOLUTION",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_RESOLUTION));
    rb_define_const(RG_TARGET_NAMESPACE, "USE_COLOR",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_USE_COLOR));
    rb_define_const(RG_TARGET_NAMESPACE, "DUPLEX",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_DUPLEX));
    rb_define_const(RG_TARGET_NAMESPACE, "COLLATE",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_COLLATE));
    rb_define_const(RG_TARGET_NAMESPACE, "REVERSE",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_REVERSE));
    rb_define_const(RG_TARGET_NAMESPACE, "MEDIA_TYPE",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_MEDIA_TYPE));
    rb_define_const(RG_TARGET_NAMESPACE, "DITHER",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_DITHER));
    rb_define_const(RG_TARGET_NAMESPACE, "SCALE",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_SCALE));
    rb_define_const(RG_TARGET_NAMESPACE, "PRINT_PAGES",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PRINT_PAGES));
    rb_define_const(RG_TARGET_NAMESPACE, "PAGE_RANGES",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PAGE_RANGES));
    rb_define_const(RG_TARGET_NAMESPACE, "PAGE_SET",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_PAGE_SET));
    rb_define_const(RG_TARGET_NAMESPACE, "FINISHINGS",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_FINISHINGS));
    rb_define_const(RG_TARGET_NAMESPACE, "NUMBER_UP",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_NUMBER_UP));
    rb_define_const(RG_TARGET_NAMESPACE, "OUTPUT_BIN",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_OUTPUT_BIN));

    rb_define_const(RG_TARGET_NAMESPACE, "OUTPUT_FILE_FORMAT",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT));
    rb_define_const(RG_TARGET_NAMESPACE, "OUTPUT_URI",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_OUTPUT_URI));

    rb_define_const(RG_TARGET_NAMESPACE, "WIN32_DRIVER_VERSION",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_WIN32_DRIVER_VERSION));
    rb_define_const(RG_TARGET_NAMESPACE, "WIN32_DRIVER_EXTRA",
                    CSTR2RVAL(GTK_PRINT_SETTINGS_WIN32_DRIVER_EXTRA));

    /* Helpers: */
    RG_DEF_METHOD(printer, 0);
    RG_DEF_METHOD(set_printer, 1);
    RG_DEF_METHOD(orientation, 0);
    RG_DEF_METHOD(set_orientation, 1);
    RG_DEF_METHOD(paper_size, 0);
    RG_DEF_METHOD(set_paper_size, 1);
    RG_DEF_METHOD(paper_width, 1);
    RG_DEF_METHOD(set_paper_width, 2);
    RG_DEF_METHOD(paper_height, 1);
    RG_DEF_METHOD(set_paper_height, 2);
    RG_DEF_METHOD_P(use_color, 0);
    RG_DEF_METHOD(set_use_color, 1);
    RG_DEF_METHOD_P(collate, 0);
    RG_DEF_METHOD(set_collate, 1);
    RG_DEF_METHOD_P(reverse, 0);
    RG_DEF_METHOD(set_reverse, 1);
    RG_DEF_METHOD(duplex, 0);
    RG_DEF_METHOD(set_duplex, 1);
    RG_DEF_METHOD(quality, 0);
    RG_DEF_METHOD(set_quality, 1);
    RG_DEF_METHOD(n_copies, 0);
    RG_DEF_METHOD(set_n_copies, 1);
    RG_DEF_METHOD(number_up, 0);
    RG_DEF_METHOD(set_number_up, 1);
    RG_DEF_METHOD(resolution, 0);
    RG_DEF_METHOD(set_resolution, 1);
    RG_DEF_METHOD(scale, 0);
    RG_DEF_METHOD(set_scale, 1);
    RG_DEF_METHOD(print_pages, 0);
    RG_DEF_METHOD(set_print_pages, 1);
    RG_DEF_METHOD(page_ranges, 0);
    RG_DEF_METHOD(set_page_ranges, 1);
    RG_DEF_METHOD(page_set, 0);
    RG_DEF_METHOD(set_page_set, 1);
    RG_DEF_METHOD(default_source, 0);
    RG_DEF_METHOD(set_default_source, 1);
    RG_DEF_METHOD(media_type, 0);
    RG_DEF_METHOD(set_media_type, 1);
    RG_DEF_METHOD(dither, 0);
    RG_DEF_METHOD(set_dither, 1);
    RG_DEF_METHOD(finishings, 0);
    RG_DEF_METHOD(set_finishings, 1);
    RG_DEF_METHOD(output_bin, 0);
    RG_DEF_METHOD(set_output_bin, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkPageOrientation */
    G_DEF_CLASS(GTK_TYPE_PAGE_ORIENTATION, "PageOrientation", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PAGE_ORIENTATION, "GTK_PAGE_");
    /* GtkPrintDuplex */
    G_DEF_CLASS(GTK_TYPE_PRINT_DUPLEX, "PrintDuplex", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_DUPLEX, "GTK_PRINT_");
    /* GtkPrintQuality */
    G_DEF_CLASS(GTK_TYPE_PRINT_QUALITY, "PrintQuality", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_QUALITY, "GTK_PRINT_");
    /* GtkPrintPages */
    G_DEF_CLASS(GTK_TYPE_PRINT_PAGES, "PrintPages", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_PAGES, "GTK_PRINT_");
    /* GtkPageSet */
    G_DEF_CLASS(GTK_TYPE_PAGE_SET, "PageSet", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PAGE_SET, "GTK_");
#endif
#if GTK_CHECK_VERSION(2,12,0)
    RG_DEF_METHOD(to_file, 1);
    RG_DEF_METHOD(to_key_file, -1);
#endif
}
