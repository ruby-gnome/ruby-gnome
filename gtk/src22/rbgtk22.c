/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk22.c -

  $Author: mutoh $
  $Date: 2005/01/29 11:44:15 $

  Copyright (C) 2003 KUBO Takehiro

************************************************/
#include "global.h"
#include "rbgtk.h"
#include <gtk/gtk.h>

#ifndef GTK_TYPE_CLIPBOARD
#define GTK_TYPE_CLIPBOARD            (gtk_clipboard_get_type ())
extern GtkType gtk_clipboard_get_type(void);
#endif

#ifndef GTK_CLIPBOARD
#define GTK_CLIPBOARD(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CLIPBOARD, GtkClipboard))
#endif

#ifndef GTK_TYPE_TREE_ROW_REFERENCE
#define GTK_TYPE_TREE_ROW_REFERENCE    (gtk_tree_row_reference_get_type ())
extern GtkType gtk_tree_row_reference_get_type(void);
#endif

static GType
clipboard_get_type()
{
    static GType our_type = 0;
    if(our_type == 0)
        our_type = GTK_TYPE_CLIPBOARD;
    return our_type;
}

static GtkClipboard* 
get_clipboard(obj)
    VALUE obj;
{
    return GTK_CLIPBOARD(RVAL2GOBJ(obj));
}

static VALUE
make_clipboard(gobj)
    GtkClipboard* gobj;
{
    return GOBJ2RVAL(gobj);
}

static GType
tree_row_reference_get_type(void)
{ 
  static GType our_type = 0;
  if (our_type == 0)
      our_type = GTK_TYPE_TREE_ROW_REFERENCE;
  return our_type;
}

static GtkTreeRowReference *
get_tree_row_reference(obj)
    VALUE obj;
{
    return (GtkTreeRowReference*)RVAL2BOXED(obj, GTK_TYPE_TREE_ROW_REFERENCE);
}

void
Init_gtk22()
{
    rbgtk_clipboard_get_type = clipboard_get_type;
    rbgtk_get_clipboard = (RBGtkClipboardGetFunc)get_clipboard;
    rbgtk_make_clipboard = (RBGtkClipboardMakeFunc)make_clipboard;
    rbgtk_tree_row_reference_get_type = tree_row_reference_get_type;
    rbgtk_get_tree_row_reference = (RBGtkTreeRowReferenceGetFunc)get_tree_row_reference;
}
