/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk20.c -

  $Author: mutoh $
  $Date: 2005/01/29 11:44:15 $

  Copyright (C) 2003 KUBO Takehiro
                but many codes are copyed from src/rbgtkclipboard.c
                and src/rbgtktreerowreference.c.

************************************************/
#include "global.h"
#include "rbgtk.h"
#include <gtk/gtk.h>

static GtkClipboard*
clipboard_copy(const GtkClipboard *clipboard)
{
    /* I don't know how to copy this object ... */
    return (GtkClipboard*)clipboard;
}

static GType
clipboard_get_type()
{
    static GType our_type = 0;
    if(our_type == 0)
        our_type = g_boxed_type_register_static("GtkClipboard",
                                                (GBoxedCopyFunc)clipboard_copy,
                                                (GBoxedFreeFunc)g_free);
    return our_type;
}

static GtkClipboard* 
get_clipboard(obj)
    VALUE obj;
{
    return (GtkClipboard*)RVAL2BOXED(obj, RBGTK_TYPE_CLIPBOARD);
}

static VALUE
make_clipboard(gobj)
    GtkClipboard* gobj;
{
    return BOXED2RVAL(gobj, RBGTK_TYPE_CLIPBOARD);
}

/*****************************************/
static GtkTreeRowReference*
treerowref_copy(ref)
    const GtkTreeRowReference* ref;
{ 
/* XXXX This code is broken, Is GtkTreeRowReference not available ?
  GtkTreeRowReference* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(GtkTreeRowReference, 1);
  *new_ref = *ref;
  return new_ref;
*/
  return (GtkTreeRowReference*)ref;
}

static GType
tree_row_reference_get_type(void)
{ 
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkTreeRowReference",
                    (GBoxedCopyFunc)treerowref_copy,
                    (GBoxedFreeFunc)gtk_tree_row_reference_free);
  return our_type;
}

static GtkTreeRowReference *
get_tree_row_reference(obj)
    VALUE obj;
{
    return (GtkTreeRowReference*)RVAL2BOXED(obj, RBGTK_TYPE_TREE_ROW_REFERENCE);
}

void
Init_gtk20()
{
    rbgtk_clipboard_get_type = clipboard_get_type;
    rbgtk_get_clipboard = (RBGtkClipboardGetFunc)get_clipboard;
    rbgtk_make_clipboard = (RBGtkClipboardMakeFunc)make_clipboard;
    rbgtk_tree_row_reference_get_type = tree_row_reference_get_type;
    rbgtk_get_tree_row_reference = (RBGtkTreeRowReferenceGetFunc)get_tree_row_reference;
}
