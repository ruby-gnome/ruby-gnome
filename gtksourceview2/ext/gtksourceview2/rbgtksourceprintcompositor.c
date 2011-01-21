/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourceprintcompositor.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2005  Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

#ifdef HAVE_GTK_SOURCE_PRINT_COMPOSITOR_GET_TYPE
/* Module: Gtk::SourcePrintCompositor
 */
#define _SELF(self) (GTK_SOURCE_PRINT_COMPOSITOR(RVAL2GOBJ(self)))
#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))

/*
 * Class method: new(buffer)
 * buffer: a Gtk::SourceBuffer or Gtk::SourceView object.
 *
 * Creates a new print compositor to print buffer.
 *
 * Returns: the new print compositor object.
 */
static VALUE
sprintcompositor_initialize (self, val)
    VALUE self, val;
{
    if (rb_obj_is_kind_of (val, GTYPE2CLASS (GTK_TYPE_SOURCE_BUFFER))) {
       G_INITIALIZE(self,
           gtk_source_print_compositor_new (GTK_SOURCE_BUFFER (RVAL2GOBJ (val))));
    } else
        if (rb_obj_is_kind_of (val, GTYPE2CLASS (GTK_TYPE_SOURCE_VIEW))) {
            G_INITIALIZE(self,
                gtk_source_print_compositor_new_from_view (GTK_SOURCE_VIEW (RVAL2GOBJ (val))));
    } else {
        rb_raise (rb_eArgError,
            "invalid argument %s (expect Gtk::SourceBuffer or Gtk::SourceView)",
            rb_class2name (CLASS_OF (val)));
    }
    return Qnil;
}

/* Defined as properties.
GtkSourceBuffer*    gtk_source_print_compositor_get_buffer
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_tab_width
                                                        (GtkSourcePrintCompositor *compositor,
                                                         guint width);
guint               gtk_source_print_compositor_get_tab_width
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_wrap_mode
                                                        (GtkSourcePrintCompositor *compositor,
                                                         GtkWrapMode wrap_mode);
GtkWrapMode         gtk_source_print_compositor_get_wrap_mode
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_highlight_syntax
                                                        (GtkSourcePrintCompositor *compositor,
                                                         gboolean highlight);
gboolean            gtk_source_print_compositor_get_highlight_syntax
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_print_line_numbers
                                                        (GtkSourcePrintCompositor *compositor,
                                                         guint interval);
guint               gtk_source_print_compositor_get_print_line_numbers
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_body_font_name
                                                        (GtkSourcePrintCompositor *compositor,
                                                         const gchar *font_name);
gchar*              gtk_source_print_compositor_get_body_font_name
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_line_numbers_font_name
                                                        (GtkSourcePrintCompositor *compositor,
                                                         const gchar *font_name);
gchar*              gtk_source_print_compositor_get_line_numbers_font_name
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_header_font_name
                                                        (GtkSourcePrintCompositor *compositor,
                                                         const gchar *font_name);
gchar*              gtk_source_print_compositor_get_header_font_name
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_footer_font_name
                                                        (GtkSourcePrintCompositor *compositor,
                                                         const gchar *font_name);
gchar*              gtk_source_print_compositor_get_footer_font_name
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_print_header
                                                        (GtkSourcePrintCompositor *compositor,
                                                         gboolean print);
gboolean            gtk_source_print_compositor_get_print_header
                                                        (GtkSourcePrintCompositor *compositor);
void                gtk_source_print_compositor_set_print_footer
                                                        (GtkSourcePrintCompositor *compositor,
                                                         gboolean print);
gboolean            gtk_source_print_compositor_get_print_footer
                                                        (GtkSourcePrintCompositor *compositor);
gint                gtk_source_print_compositor_get_n_pages
                                                        (GtkSourcePrintCompositor *compositor);
*/

static VALUE
sprintcompositor_get_top_margin(self, unit)
    VALUE self, unit;
{
    return DBL2NUM(gtk_source_print_compositor_get_top_margin(_SELF(self), RVAL2UNIT (unit)));
}

static VALUE
sprintcompositor_set_top_margin(self, top, unit)
    VALUE self, top, unit;
{
    gtk_source_print_compositor_set_top_margin(_SELF(self), NUM2DBL(top), RVAL2UNIT (unit));
    return self;
}

static VALUE
sprintcompositor_get_bottom_margin(self, unit)
    VALUE self, unit;
{
    return DBL2NUM(gtk_source_print_compositor_get_bottom_margin(_SELF(self), RVAL2UNIT (unit)));
}

static VALUE
sprintcompositor_set_bottom_margin(self, bottom, unit)
    VALUE self, bottom, unit;
{
    gtk_source_print_compositor_set_bottom_margin(_SELF(self), NUM2DBL(bottom), RVAL2UNIT (unit));
    return self;
}

static VALUE
sprintcompositor_get_left_margin(self, unit)
    VALUE self, unit;
{
    return DBL2NUM(gtk_source_print_compositor_get_left_margin(_SELF(self), RVAL2UNIT (unit)));
}

static VALUE
sprintcompositor_set_left_margin(self, left, unit)
    VALUE self, left, unit;
{
    gtk_source_print_compositor_set_left_margin(_SELF(self), NUM2DBL(left), RVAL2UNIT (unit));
    return self;
}

static VALUE
sprintcompositor_get_right_margin(self, unit)
    VALUE self, unit;
{
    return DBL2NUM(gtk_source_print_compositor_get_right_margin(_SELF(self), RVAL2UNIT (unit)));
}

static VALUE
sprintcompositor_set_right_margin(self, right, unit)
    VALUE self, right, unit;
{
    gtk_source_print_compositor_set_right_margin(_SELF(self), NUM2DBL(right), RVAL2UNIT (unit));
    return self;
}

static VALUE
sprintcompositor_set_header_format(self, separator, left, center, right)
    VALUE self, separator, left, center, right;
{
    gtk_source_print_compositor_set_header_format(_SELF(self),
                                           RVAL2CBOOL(separator),
                                           RVAL2CSTR(left),
                                           RVAL2CSTR(center),
                                           RVAL2CSTR(right));
    return self;
}

static VALUE
sprintcompositor_set_footer_format(self, separator, left, center, right)
    VALUE self, separator, left, center, right;
{
    gtk_source_print_compositor_set_footer_format(_SELF(self),
                                           RVAL2CBOOL(separator),
                                           RVAL2CSTR(left),
                                           RVAL2CSTR(center),
                                           RVAL2CSTR(right));
    return self;
}

static VALUE
sprintcompositor_paginate (self, context)
    VALUE self, context;
{
    return CBOOL2RVAL (gtk_source_print_compositor_paginate (_SELF (self),
                                        GTK_PRINT_CONTEXT (RVAL2GOBJ (context))));
}

static VALUE
sprintcompositor_get_pagination_progress (self)
    VALUE self;
{
    return DBL2NUM (gtk_source_print_compositor_get_pagination_progress (_SELF (self)));
}

static VALUE
sprintcompositor_draw_page (self, context, page_nr)
    VALUE self, context, page_nr;
{
    gtk_source_print_compositor_draw_page (_SELF (self),
                                           GTK_PRINT_CONTEXT (RVAL2GOBJ (context)),
                                           NUM2INT (page_nr));
    return self;
}
#endif /* HAVE_GTK_SOURCE_PRINT_COMPOSITOR_GET_TYPE */

void
Init_gtk_sourceprintcompositor()
{
#ifdef HAVE_GTK_SOURCE_PRINT_COMPOSITOR_GET_TYPE
    VALUE pc = G_DEF_CLASS(GTK_TYPE_SOURCE_PRINT_COMPOSITOR, "SourcePrintCompositor", mGtk);

    rb_define_method(pc, "initialize", sprintcompositor_initialize, 1);
    rb_define_method(pc, "get_top_margin",
                     sprintcompositor_get_top_margin, 1);
    rb_define_method(pc, "set_top_margin",
                     sprintcompositor_set_top_margin, 2);
    rb_define_method(pc, "get_bottom_margin",
                     sprintcompositor_get_bottom_margin, 1);
    rb_define_method(pc, "set_bottom_margin",
                     sprintcompositor_set_bottom_margin, 2);
    rb_define_method(pc, "get_left_margin",
                     sprintcompositor_get_left_margin, 1);
    rb_define_method(pc, "set_left_margin",
                     sprintcompositor_set_left_margin, 2);
    rb_define_method(pc, "get_right_margin",
                     sprintcompositor_get_right_margin, 1);
    rb_define_method(pc, "set_right_margin",
                     sprintcompositor_set_right_margin, 2);
    rb_define_method(pc, "set_header_format",
                     sprintcompositor_set_header_format, 4);
    rb_define_method(pc, "set_footer_format",
                     sprintcompositor_set_footer_format, 4);
    rb_define_method(pc, "paginate",
                     sprintcompositor_paginate, 1);
    rb_define_method(pc, "pagination_progress",
                     sprintcompositor_get_pagination_progress, 0);
    rb_define_method(pc, "draw_page",
                     sprintcompositor_draw_page, 2);

    G_DEF_SETTERS(pc);
#endif /* HAVE_GTK_SOURCE_PRINT_COMPOSITOR_GET_TYPE */
}
