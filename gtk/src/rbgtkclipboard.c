/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkclipboard.c -
 
  $Author: sakai $
  $Date: 2003/11/20 18:27:54 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) RVAL2CLIPBOARD(s)

static VALUE
clipboard_get(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    GtkClipboard *clipboard;

    if (argc == 1){
        VALUE selection;
        rb_scan_args(argc, argv, "10", &selection);
        clipboard = gtk_clipboard_get(RVAL2ATOM(selection));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display, selection;
        rb_scan_args(argc, argv, "20", &display, &selection);
        clipboard = gtk_clipboard_get_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                  RVAL2ATOM(selection));
#else
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    } 
    return CLIPBOARD2RVAL(clipboard);
}

static VALUE
clipboard_get_display(self)
    VALUE self;
{
#if GTK_CHECK_VERSION(2,2,0)
    return GOBJ2RVAL(gtk_clipboard_get_display(_SELF(self)));
#else
	 return Qnil;
#endif
}

static void
clipboard_get_func(clipboard, selection_data, info, func)
    GtkClipboard *clipboard;
    GtkSelectionData* selection_data;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

static VALUE
clipboard_set(self, targets)
    VALUE self, targets;
{
    const GtkTargetEntry* gtargets = (const GtkTargetEntry*)rbgtk_get_target_entry(targets);
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    return gtk_clipboard_set_with_data(_SELF(self), 
                                       gtargets,
                                       RARRAY(targets)->len,
                                       (GtkClipboardGetFunc)clipboard_get_func,
                                       (GtkClipboardClearFunc)NULL,
                                       (gpointer)func) ? Qtrue : Qfalse;
}
/*
Do not implement this. Use Gtk::Clipboard#set_with_data instead.
gboolean    gtk_clipboard_set_with_owner    (GtkClipboard *clipboard,
                                             const GtkTargetEntry *targets,
                                             guint n_targets,
                                             GtkClipboardGetFunc get_func,
                                             GtkClipboardClearFunc clear_func,
                                             GObject *owner);

static VALUE
clipboard_get_owner(self)
    VALUE self;
{
    GObject *gobj;
    gobj = gtk_clipboard_get_owner(_SELF(self));
    return gobj ? GOBJ2RVAL(gobj) : Qnil;
}
*/

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
    StringValue(text);
    gtk_clipboard_set_text(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

static void
clipboard_received_func(clipboard, selection_data, func)
    GtkClipboard *clipboard;
    GtkSelectionData* selection_data;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

static VALUE
clipboard_request_contents(self, target)
    VALUE self, target;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_clipboard_request_contents(_SELF(self), RVAL2ATOM(target),
                                   (GtkClipboardReceivedFunc)clipboard_received_func,
                                   (gpointer)func);
    return self;
}

static void
clipboard_text_received_func(clipboard, text, func)
    GtkClipboard *clipboard;
    const gchar *text;
    gpointer func;
{
    if(text) 
        rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
                   CSTR2RVAL(text));
}

static VALUE
clipboard_request_text(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_clipboard_request_text(_SELF(self),
                               (GtkClipboardTextReceivedFunc)clipboard_text_received_func,
                               (gpointer)func);
    return self;
}

static VALUE
clipboard_wait_for_contents(self, target)
    VALUE self, target;
{
    return BOXED2RVAL(gtk_clipboard_wait_for_contents(
                          _SELF(self), 
                          RVAL2ATOM(target)), 
                      GTK_TYPE_SELECTION_DATA);
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

    rb_define_singleton_method(gClipboard, "get", clipboard_get, -1);
    rb_define_method(gClipboard, "display", clipboard_get_display, 0);
    rb_define_method(gClipboard, "set", clipboard_set, 1);
/*
    rb_define_method(gClipboard, "owner", clipboard_get_owner, 0);
*/
    rb_define_method(gClipboard, "clear", clipboard_clear, 0);
    rb_define_method(gClipboard, "set_text", clipboard_set_text, 1);

    rb_define_method(gClipboard, "request_contents", clipboard_request_contents, 1);
    rb_define_method(gClipboard, "request_text", clipboard_request_text, 0);
    rb_define_method(gClipboard, "wait_for_contents", clipboard_wait_for_contents, 1);
    rb_define_method(gClipboard, "wait_for_text", clipboard_wait_for_text, 0);
    rb_define_method(gClipboard, "wait_is_text_available?", clipboard_wait_is_text_available, 0);

    G_DEF_SETTERS(gClipboard);
  }
}
