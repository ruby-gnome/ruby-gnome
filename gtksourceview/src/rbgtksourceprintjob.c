/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourceprintjob.c -

  $Author: mutoh $
  $Date: 2006/12/27 16:27:37 $

  Copyright (C) 2005  Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Module: Gtk::SourcePrintJob
 */
#include <gtksourceview/gtksourceprintjob.h>
#define _SELF(self) (GTK_SOURCE_PRINT_JOB(RVAL2GOBJ(self)))

/*
 * Class method: new(config, buffer = nil)
 * config: an optional Gnome::PrintConfig
 * buffer: a Gtk::SourceBuffer object.
 *
 * Creates a new print job to print buffer.
 * 
 * Returns: the new print job object.
 */
static VALUE
sprintjob_initialize (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE config, buffer;
    GtkSourcePrintJob* ret;

    rb_scan_args (argc, argv, "11", &config, &buffer);

    if (NIL_P (buffer))
        ret = gtk_source_print_job_new(GNOME_PRINT_CONFIG(RVAL2GOBJ(config)));
    else
        ret = gtk_source_print_job_new_with_buffer(GNOME_PRINT_CONFIG(RVAL2GOBJ(config)),
                                                   GTK_SOURCE_BUFFER(RVAL2GOBJ(buffer)));
    
    G_INITIALIZE(self, ret);
    return self;
}

/* Defined as Properties.
void        gtk_source_print_job_set_config (GtkSourcePrintJob *job,
                                             GnomePrintConfig *config);
GnomePrintConfig* gtk_source_print_job_get_config
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_buffer (GtkSourcePrintJob *job,
                                             GtkSourceBuffer *buffer);
GtkSourceBuffer* gtk_source_print_job_get_buffer
                                            (GtkSourcePrintJob *job);
*/

static VALUE
sprintjob_setup(self, view)
    VALUE self, view;
{
    gtk_source_print_job_setup_from_view(_SELF(self), GTK_SOURCE_VIEW(RVAL2GOBJ(view)));
    return self;
}

/* Defined as Properties
void        gtk_source_print_job_set_tabs_width
                                            (GtkSourcePrintJob *job,
                                             guint tabs_width);
guint       gtk_source_print_job_get_tabs_width
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_wrap_mode
                                            (GtkSourcePrintJob *job,
                                             GtkWrapMode wrap);
GtkWrapMode gtk_source_print_job_get_wrap_mode
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_highlight
                                            (GtkSourcePrintJob *job,
                                             gboolean highlight);
gboolean    gtk_source_print_job_get_highlight
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_font_desc
                                            (GtkSourcePrintJob *job,
                                             PangoFontDescription *desc);
void        gtk_source_print_job_set_font   (GtkSourcePrintJob *job,
                                             const gchar *font_name);
PangoFontDescription* gtk_source_print_job_get_font_desc
                                            (GtkSourcePrintJob *job);
gchar*      gtk_source_print_job_get_font   (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_numbers_font_desc
                                            (GtkSourcePrintJob *job,
                                             PangoFontDescription *desc);
void        gtk_source_print_job_set_numbers_font
                                            (GtkSourcePrintJob *job,
                                             const gchar *font_name);
PangoFontDescription* gtk_source_print_job_get_numbers_font_desc
                                            (GtkSourcePrintJob *job);
gchar*      gtk_source_print_job_get_numbers_font
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_print_numbers
                                            (GtkSourcePrintJob *job,
                                             guint interval);
guint       gtk_source_print_job_get_print_numbers
                                            (GtkSourcePrintJob *job);
*/

static VALUE
sprintjob_set_text_margins(self, top, bottom, left, right)
    VALUE self, top, bottom, left, right;
{
    gtk_source_print_job_set_text_margins(_SELF(self), NUM2DBL(top),
                                          NUM2DBL(bottom),
                                          NUM2DBL(left), NUM2DBL(right));
    return self;
}

static VALUE
sprintjob_get_text_margins(self)
    VALUE self;
{
    gdouble top, bottom, left, right;
    
    gtk_source_print_job_get_text_margins(_SELF(self),
                                          &top, &bottom, &left, &right);
    return rb_ary_new3(4, rb_float_new(top), rb_float_new(bottom),
                       rb_float_new(left), rb_float_new(right));
}

static VALUE
sprintjob_print(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_source_print_job_print(_SELF(self)));
}

static VALUE
sprintjob_print_range(self, start, end)
    VALUE self, start, end;
{
    return GOBJ2RVAL(gtk_source_print_job_print_range(_SELF(self),
                                                      (const GtkTextIter*)RVAL2BOXED(start, GTK_TYPE_TEXT_ITER),
                                                      (const GtkTextIter*)RVAL2BOXED(end, GTK_TYPE_TEXT_ITER)));
}

static VALUE
sprintjob_print_range_async(self, start, end)
    VALUE self, start, end;
{
    return CBOOL2RVAL(gtk_source_print_job_print_range_async(_SELF(self),
                                                             (const GtkTextIter*)RVAL2BOXED(start, GTK_TYPE_TEXT_ITER),
                                                             (const GtkTextIter*)RVAL2BOXED(end, GTK_TYPE_TEXT_ITER)));
}

static VALUE
sprintjob_cancel(self)
    VALUE self;
{
    gtk_source_print_job_cancel(_SELF(self));
    return self;
}

static VALUE
sprintjob_get_printjob(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_source_print_job_get_print_job(_SELF(self)));
}

static VALUE
sprintjob_get_page(self)
    VALUE self;
{
    return UINT2NUM(gtk_source_print_job_get_page(_SELF(self)));
}

static VALUE
sprintjob_get_page_count(self)
    VALUE self;
{
    return UINT2NUM(gtk_source_print_job_get_page_count(_SELF(self)));
}

static VALUE
sprintjob_get_print_context(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_source_print_job_get_print_context(_SELF(self)));
}

/* Defined as properties
void        gtk_source_print_job_set_print_header
                                            (GtkSourcePrintJob *job,
                                             gboolean setting);
gboolean    gtk_source_print_job_get_print_header
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_print_footer
                                            (GtkSourcePrintJob *job,
                                             gboolean setting);
gboolean    gtk_source_print_job_get_print_footer
                                            (GtkSourcePrintJob *job);
void        gtk_source_print_job_set_header_footer_font_desc
                                            (GtkSourcePrintJob *job,
                                             PangoFontDescription *desc);
void        gtk_source_print_job_set_header_footer_font
                                            (GtkSourcePrintJob *job,
                                             const gchar *font_name);
PangoFontDescription* gtk_source_print_job_get_header_footer_font_desc
                                            (GtkSourcePrintJob *job);
gchar*      gtk_source_print_job_get_header_footer_font
                                            (GtkSourcePrintJob *job);
*/

static VALUE
sprintjob_set_header_format(self, left, center, right, separator)
    VALUE self, left, center, right, separator;
{
    gtk_source_print_job_set_header_format(_SELF(self),
                                           (const gchar*)RVAL2CSTR(left),
                                           (const gchar*)RVAL2CSTR(center),
                                           (const gchar*)RVAL2CSTR(right),
                                           RTEST(separator));
    return self;
}

static VALUE
sprintjob_set_footer_format(self, left, center, right, separator)
    VALUE self, left, center, right, separator;
{
    gtk_source_print_job_set_footer_format(_SELF(self),
                                           (const gchar*)RVAL2CSTR(left),
                                           (const gchar*)RVAL2CSTR(center),
                                           (const gchar*)RVAL2CSTR(right),
                                           RTEST(separator));
    return self;
}

void
Init_gtk_sourceprintjob()
{
    VALUE pj = G_DEF_CLASS(GTK_TYPE_SOURCE_PRINT_JOB, "SourcePrintJob", mGtk);
    
    rb_define_method(pj, "initialize", sprintjob_initialize, -1);
    rb_define_method(pj, "setup", sprintjob_setup, 1);
    rb_define_method(pj, "set_text_margins", sprintjob_set_text_margins, 4);
    rb_define_method(pj, "text_margins", sprintjob_get_text_margins, 0);
    rb_define_method(pj, "print", sprintjob_print, 0);
    rb_define_method(pj, "print_range", sprintjob_print_range, 2);
    rb_define_method(pj, "print_range_async", sprintjob_print_range_async, 2);
    rb_define_method(pj, "cancel", sprintjob_cancel, 0);
    rb_define_method(pj, "printjob", sprintjob_get_printjob, 0);
    rb_define_method(pj, "page", sprintjob_get_page, 0);
    rb_define_method(pj, "page_count", sprintjob_get_page_count, 0);
    rb_define_method(pj, "print_context", sprintjob_get_print_context, 0);
    rb_define_method(pj, "set_header_format", sprintjob_set_header_format, 4);
    rb_define_method(pj, "set_footer_format", sprintjob_set_footer_format, 4);

    G_DEF_SETTERS(pj);
}
