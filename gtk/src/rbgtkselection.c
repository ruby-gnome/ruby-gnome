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
gtkdrag_selection_owner_set(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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
gtkdrag_selection_add_target(self, widget, selection, target, info)
    VALUE self, widget, selection, target, info;
{
    gtk_selection_add_target(RVAL2WIDGET(widget), RVAL2ATOM(selection),
                             RVAL2ATOM(target), NUM2INT(info));
    return self;
}

static VALUE
gtkdrag_selection_add_targets(self, widget, selection, targets)
    VALUE self, widget, selection, targets;
{
    gtk_selection_add_targets(RVAL2WIDGET(widget), 
                              RVAL2ATOM(selection),
                              rbgtk_get_target_entry(targets), RARRAY(targets)->len);
    return self;
}

static VALUE
gtkdrag_selection_clear_targets(self, widget, selection)
    VALUE self, widget, selection;
{
    gtk_selection_clear_targets(RVAL2WIDGET(widget), RVAL2ATOM(selection));
    return self;
}

static VALUE
gtkdrag_selection_convert(self, widget, selection, target, time)
    VALUE self, widget, selection, target, time;
{
    gboolean ret = gtk_selection_convert(RVAL2WIDGET(widget), 
                                         RVAL2ATOM(selection), RVAL2ATOM(target),
                                         NUM2INT(time));
    return CBOOL2RVAL(ret);
}

static VALUE
gtkdrag_selection_remove_all(self, widget)
    VALUE self, widget;
{
    gtk_selection_remove_all(RVAL2WIDGET(widget));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
targets_include_image(self, targets, writable)
     VALUE self, targets, writable;
{
  gint i;
  gint len = RARRAY(targets)->len;
  GdkAtom* gtargets = ALLOCA_N(GdkAtom, len);

  for (i = 0; i < len; i++){
    gtargets[i] = RVAL2ATOM(RARRAY(targets)->ptr[i]);
  }

  return CBOOL2RVAL(gtk_targets_include_image(gtargets, len, RVAL2CBOOL(writable)));
}

static VALUE
targets_include_text(self, targets)
     VALUE self, targets;
{
  gint i;
  gint len = RARRAY(targets)->len;
  GdkAtom* gtargets = ALLOCA_N(GdkAtom, len);
  for (i = 0; i < len; i++){
    gtargets[i] = RVAL2ATOM(RARRAY(targets)->ptr[i]);
  }
  return CBOOL2RVAL(gtk_targets_include_text(gtargets, len));
}

static VALUE
targets_include_uri(self, targets)
     VALUE self, targets;
{
  gint i;
  gint len = RARRAY(targets)->len;
  GdkAtom* gtargets = ALLOCA_N(GdkAtom, len);

  for (i = 0; i < len; i++){
    gtargets[i] = RVAL2ATOM(RARRAY(targets)->ptr[i]);
  }

  return CBOOL2RVAL(gtk_targets_include_uri(gtargets, len));
}

static VALUE
targets_include_rich_text(self, targets, buffer)
     VALUE self, targets, buffer;
{
  gint i;
  gint len = RARRAY(targets)->len;
  GdkAtom* gtargets = ALLOCA_N(GdkAtom, len);

  for (i = 0; i < len; i++){
    gtargets[i] = RVAL2ATOM(RARRAY(targets)->ptr[i]);
  }

  return CBOOL2RVAL(gtk_targets_include_rich_text(gtargets, len, RVAL2GOBJ(buffer)));
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
