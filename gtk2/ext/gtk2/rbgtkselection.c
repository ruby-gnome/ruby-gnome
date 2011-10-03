/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkselection.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:10 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is devided from rbgtkwidget.c.
  rbgtkwidget.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
gtkdrag_selection_owner_set(int argc, VALUE *argv, VALUE self)
{
    gboolean ret;

    if (argc == 3){
        VALUE widget, selection, time;
        rb_scan_args(argc, argv, "30", &widget, &selection, &time);
        ret = gtk_selection_owner_set(RVAL2WIDGET(widget), 
                                      RVAL2ATOM(selection), NUM2INT(time));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display, widget, selection, time;
        rb_scan_args(argc, argv, "40", &display, &widget, &selection, &time);
        ret = gtk_selection_owner_set_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                  RVAL2WIDGET(widget), 
                                                  RVAL2ATOM(selection), NUM2INT(time));
#else
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    }
    return CBOOL2RVAL(ret);
}

static VALUE
gtkdrag_selection_add_target(VALUE self, VALUE widget, VALUE selection, VALUE target, VALUE info)
{
    gtk_selection_add_target(RVAL2WIDGET(widget), RVAL2ATOM(selection),
                             RVAL2ATOM(target), NUM2INT(info));
    return self;
}

static VALUE
gtkdrag_selection_add_targets(VALUE self, VALUE rbwidget, VALUE rbselection, VALUE rbtargets)
{
    GtkWidget *widget = RVAL2WIDGET(rbwidget);
    GdkAtom selection = RVAL2ATOM(rbselection);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_selection_add_targets(widget, selection, targets, n);

    g_free(targets);

    return self;
}

static VALUE
gtkdrag_selection_clear_targets(VALUE self, VALUE widget, VALUE selection)
{
    gtk_selection_clear_targets(RVAL2WIDGET(widget), RVAL2ATOM(selection));
    return self;
}

static VALUE
gtkdrag_selection_convert(VALUE self, VALUE widget, VALUE selection, VALUE target, VALUE time)
{
    gboolean ret = gtk_selection_convert(RVAL2WIDGET(widget), 
                                         RVAL2ATOM(selection), RVAL2ATOM(target),
                                         NUM2INT(time));
    return CBOOL2RVAL(ret);
}

static VALUE
gtkdrag_selection_remove_all(VALUE self, VALUE widget)
{
    gtk_selection_remove_all(RVAL2WIDGET(widget));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
targets_include_image(VALUE self, VALUE rbtargets, VALUE rbwritable)
{
    gboolean writable = RVAL2CBOOL(rbwritable);
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_image(targets, n, writable);

    g_free(targets);

    return result;
}

static VALUE
targets_include_text(VALUE self, VALUE rbtargets)
{
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_text(targets, n);

    g_free(targets);

    return result;
}

static VALUE
targets_include_uri(VALUE self, VALUE rbtargets)
{
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_uri(targets, n);

    g_free(targets);

    return result;
}

static VALUE
targets_include_rich_text(VALUE self, VALUE rbtargets, VALUE rbbuffer)
{
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(RVAL2GOBJ(rbbuffer));
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);
    gboolean result;

    result = gtk_targets_include_rich_text(targets, n, buffer);

    g_free(targets);

    return result;
}
#endif

void
Init_gtk_selection()
{
    VALUE mSelection =  rb_define_module_under(mGtk, "Selection");

    rb_define_module_function(mSelection, "owner_set", gtkdrag_selection_owner_set, 3);
    rb_define_module_function(mSelection, "add_target", gtkdrag_selection_add_target, 4);
    rb_define_module_function(mSelection, "add_targets", gtkdrag_selection_add_targets, 3);
    rb_define_module_function(mSelection, "clear_targets", gtkdrag_selection_clear_targets, 2);
    rb_define_module_function(mSelection, "convert", gtkdrag_selection_convert, 4);
    rb_define_module_function(mSelection, "remove_all", gtkdrag_selection_remove_all, 1);

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_module_function(mSelection, "include_image?", targets_include_image, 2);
    rb_define_module_function(mSelection, "include_text?", targets_include_text, 1);
    rb_define_module_function(mSelection, "include_uri?", targets_include_uri, 1);
    rb_define_module_function(mSelection, "include_rich_text?", targets_include_rich_text, 2);
#endif
}
