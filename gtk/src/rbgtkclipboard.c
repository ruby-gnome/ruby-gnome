/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkclipboard.c -
 
  $Author: sakai $
  $Date: 2003/03/14 03:12:21 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) RVAL2CLIPBOARD(s)

static VALUE
clipboard_initialize(self, selection)
    VALUE self, selection;
{
    GtkClipboard *clipboard;
    clipboard = gtk_clipboard_get(RVAL2ATOM(selection));

    G_INITIALIZE(self, clipboard);

    return Qnil;
}

#if 0
void        (*GtkClipboardReceivedFunc)     (GtkClipboard *clipboard,
                                             GtkSelectionData *selection_data,
                                             gpointer data);
void        (*GtkClipboardGetFunc)          (GtkClipboard *clipboard,
                                             GtkSelectionData *selection_data,
                                             guint info,
                                             gpointer user_data_or_owner);
void        (*GtkClipboardClearFunc)        (GtkClipboard *clipboard,
                                             gpointer user_data_or_owner);
GtkClipboard* gtk_clipboard_get             (GdkAtom selection);
GtkClipboard* gtk_clipboard_get_for_display (GdkDisplay *display,
                                             GdkAtom selection);
GdkDisplay* gtk_clipboard_get_display       (GtkClipboard *clipboard);
gboolean    gtk_clipboard_set_with_data     (GtkClipboard *clipboard,
                                             const GtkTargetEntry *targets,
                                             guint n_targets,
                                             GtkClipboardGetFunc get_func,
                                             GtkClipboardClearFunc clear_func,
                                             gpointer user_data);
gboolean    gtk_clipboard_set_with_owner    (GtkClipboard *clipboard,
                                             const GtkTargetEntry *targets,
                                             guint n_targets,
                                             GtkClipboardGetFunc get_func,
                                             GtkClipboardClearFunc clear_func,
                                             GObject *owner);
#endif

static VALUE
clipboard_get_owner(self)
    VALUE self;
{
    GObject *gobj;
    gobj = gtk_clipboard_get_owner(_SELF(self));
    return gobj ? GOBJ2RVAL(gobj) : Qnil;
}

static VALUE
clipboard_clear(self)
    VALUE self;
{
    gtk_clipboard_clear(_SELF(self));
    return self;
}

static VALUE
clipboard_set_text(self, text)
    VALUE self, text;
{
    gtk_clipboard_set_text(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

#if 0
void        gtk_clipboard_request_contents  (GtkClipboard *clipboard,
                                             GdkAtom target,
                                             GtkClipboardReceivedFunc callback,
                                             gpointer user_data);
GtkSelectionData* gtk_clipboard_wait_for_contents
                                            (GtkClipboard *clipboard,
                                             GdkAtom target);
#endif

static void
clipboard_text_received_func(clipboard, text, func)
    GtkClipboard *clipboard;
    const gchar *text;
    gpointer func;
{
    if(text) 
        rb_funcall((VALUE)func, id_call, 1, CSTR2RVAL(text));
}

static VALUE
clipboard_request_text(self)
    VALUE self;
{
    VALUE func = rb_f_lambda();
    G_RELATIVE(self, func);
    gtk_clipboard_request_text(_SELF(self),
                               (GtkClipboardTextReceivedFunc)clipboard_text_received_func,
                               (gpointer)func);
    return self;
}

static VALUE
clipboard_wait_for_text(self)
    VALUE self;
{
    gchar *str;
    str = gtk_clipboard_wait_for_text(_SELF(self));
    return str ? CSTR2RVAL(str) : Qnil;
}

static VALUE
clipboard_wait_is_text_available(self)
    VALUE self;
{
    return gtk_clipboard_wait_is_text_available(_SELF(self)) ? Qtrue : Qfalse;
}

void 
Init_gtk_clipboard()
{
  if (rbgtk_clipboard_get_type){
    VALUE gClipboard = G_DEF_CLASS(RBGTK_TYPE_CLIPBOARD, "Clipboard", mGtk);

    rb_define_method(gClipboard, "initialize", clipboard_initialize, 1);

    rb_define_method(gClipboard, "owner", clipboard_get_owner, 0);
    rb_define_method(gClipboard, "clear", clipboard_clear, 0);
    rb_define_method(gClipboard, "set_text", clipboard_set_text, 1);

    rb_define_method(gClipboard, "request_text", clipboard_request_text, 0);
    rb_define_method(gClipboard, "wait_for_text", clipboard_wait_for_text, 0);
    rb_define_method(gClipboard, "wait_is_text_available?", clipboard_wait_is_text_available, 0);
  }
}
