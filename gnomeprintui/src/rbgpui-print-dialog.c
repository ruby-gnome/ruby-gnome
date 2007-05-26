/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-dialog.c -

  $Author: ktou $
  $Date: 2007/05/26 02:23:08 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#include <libgnomeprintui/gnome-print-dialog.h>

#define _SELF(self) (RVAL2GOBJ(self))
#define RVAL2GPDF(obj) (RVAL2GFLAGS(obj, GNOME_TYPE_PRINT_PRINT_DIALOG_FLAGS))
#define RVAL2GPDRF(obj) (RVAL2GFLAGS(obj, GNOME_TYPE_PRINT_PRINT_DIALOG_RANGE_FLAGS))
#define GPRT2RVAL(obj) (GENUM2RVAL(obj, GNOME_TYPE_PRINT_PRINT_RANGE_TYPE))
#define GPRF2RVAL(obj) (GENUM2RVAL(obj, GNOME_TYPE_PRINT_PRINT_DIALOG_RANGE_FLAGS))
#define GPDR2RVAL(obj) (GENUM2RVAL(obj, GNOME_TYPE_PRINT_PRINT_BUTTONS))

static VALUE
gpui_dialog_new(int argc, VALUE *argv, VALUE self)
{
    VALUE job, title, flags;
    int n_arg;
    gint g_flags = 0;

    n_arg = rb_scan_args(argc, argv, "03", &job, &title, &flags);

    if (n_arg == 3) {
        g_flags = RVAL2GPDF(flags);
    } else if (n_arg < 3) {
        g_flags = GNOME_PRINT_DIALOG_RANGE | GNOME_PRINT_DIALOG_COPIES;
        if (n_arg < 2) {
            title = rb_str_new2("Gnome::PrintDialog");
        }
    }
  
    RBGTK_INITIALIZE(self,
                     gnome_print_dialog_new(RVAL2GOBJ(job),
                                            (const guchar*)RVAL2CSTR(title),
                                            g_flags));
    return Qnil;
}

static VALUE
gpui_dialog_run(VALUE self)
{
    return GPDR2RVAL(gtk_dialog_run(_SELF(self)));
}

static VALUE
gpui_dialog_get_copies(VALUE self)
{
    gint copies;
    gboolean collate;
    gnome_print_dialog_get_copies(_SELF(self), &copies, &collate);
    return rb_ary_new3(2, INT2NUM(copies), CBOOL2RVAL(collate));
}

static VALUE
gpui_dialog_set_copies(VALUE self, VALUE copies, VALUE collate)
{
    gnome_print_dialog_set_copies(_SELF(self),
                                  NUM2INT(copies),
                                  RTEST(collate));
    return Qnil;
}

static VALUE
gpui_dialog_get_config(VALUE self)
{
    return GOBJ2RVAL(gnome_print_dialog_get_config(_SELF(self)));
}

static VALUE
gpui_dialog_get_range(VALUE self)
{
    return GPRF2RVAL(gnome_print_dialog_get_range(_SELF(self)));
}

static VALUE
gpui_dialog_get_range_page(VALUE self)
{
    gint start, end;
    int bit_mask;
  
    bit_mask = gnome_print_dialog_get_range_page(_SELF(self), &start, &end);
  
    return rb_ary_new3(3, INT2NUM(start), INT2NUM(end), GPRF2RVAL(bit_mask));
}

static VALUE
gpui_dialog_construct_range_custom(VALUE self, VALUE widget)
{
    gnome_print_dialog_construct_range_custom(_SELF(self), RVAL2GOBJ(widget));
    return Qnil;
}

static VALUE
gpui_dialog_construct_range_any(VALUE self, VALUE flags, VALUE widget,
                                VALUE current_label, VALUE range_label)
{
    gnome_print_dialog_construct_range_any(_SELF(self),
                                           RVAL2GPDRF(flags),
                                           RVAL2GOBJ(widget),
                                           (const guchar*)RVAL2CSTR(current_label),
                                           (const guchar*)RVAL2CSTR(range_label));
    return Qnil;
}

static VALUE
gpui_dialog_construct_range_page(VALUE self, VALUE flags,
                                 VALUE start, VALUE end,
                                 VALUE current_label, VALUE range_label)
{
    gnome_print_dialog_construct_range_page(_SELF(self),
                                            RVAL2GPDRF(flags),
                                            NUM2INT(start),
                                            NUM2INT(end),
                                            (const guchar*)RVAL2CSTR(current_label),
                                            (const guchar*)RVAL2CSTR(range_label));
    return Qnil;
}


void
Init_gnome_print_dialog(VALUE mGnome)
{
    VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_DIALOG, "PrintDialog", mGnome);

    /* GnomePrintRangeType */
    G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_RANGE_TYPE, "RangeType", c);
    G_DEF_CONSTANTS(c, GNOME_TYPE_PRINT_PRINT_RANGE_TYPE, "GNOME_PRINT_");
    /* GnomePrintDialogRangeFlags */
    G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_DIALOG_RANGE_FLAGS, "RangeFlags", c);
    G_DEF_CONSTANTS(c, GNOME_TYPE_PRINT_PRINT_DIALOG_RANGE_FLAGS,
                    "GNOME_PRINT_");
    /* GnomePrintDialogFlags */
    G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_DIALOG_FLAGS, "Flags", c);
    G_DEF_CONSTANTS(c, GNOME_TYPE_PRINT_PRINT_DIALOG_FLAGS,
                    "GNOME_PRINT_DIALOG_");
    /* GnomePrintButtons */
    G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_BUTTONS, "Response", c);
    G_DEF_CONSTANTS(c, GNOME_TYPE_PRINT_PRINT_BUTTONS, "GNOME_PRINT_DIALOG_");
  
              
    rb_define_method(c, "initialize", gpui_dialog_new, -1);

    rb_define_method(c, "run", gpui_dialog_run, 0);
    rb_define_method(c, "copies", gpui_dialog_get_copies, 0);
    rb_define_method(c, "set_copies", gpui_dialog_set_copies, 2);
    rb_define_method(c, "config", gpui_dialog_get_config, 0);
    rb_define_method(c, "range", gpui_dialog_get_range, 0);
    rb_define_method(c, "range_page", gpui_dialog_get_range_page, 0);
    rb_define_method(c, "construct_range_custom",
                     gpui_dialog_construct_range_custom, 1);
    rb_define_method(c, "construct_range_any",
                     gpui_dialog_construct_range_any, 4);
    rb_define_method(c, "construct_range_page",
                     gpui_dialog_construct_range_page, 5);

    G_DEF_SETTERS(c);
}
