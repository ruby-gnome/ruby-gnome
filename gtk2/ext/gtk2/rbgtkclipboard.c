/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2006 Ruby-GNOME2 Project
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#define _SELF(s) RVAL2CLIPBOARD(s)

#ifndef GTK_TYPE_CLIPBOARD
static GtkClipboard*
clipboard_copy(const GtkClipboard *clipboard)
{
    /* I don't know how to copy this object ... */
    return (GtkClipboard*)clipboard;
}

GType
rbgtk_clipboard_get_type()
{
    static GType our_type = 0;
    if(our_type == 0)
        our_type = g_boxed_type_register_static("GtkClipboard",
                                                (GBoxedCopyFunc)clipboard_copy,
                                                (GBoxedFreeFunc)g_free);
    return our_type;
}

GtkClipboard*
rbgtk_get_clipboard(VALUE obj)
{
    return GTK_CLIPBOARD(RVAL2GOBJ(obj));
}

VALUE
rbgtk_make_clipboard(GtkClipboard *gobj)
{
    return GOBJ2RVAL(gobj);
}
#endif

static VALUE
clipboard_get(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
clipboard_get_display(VALUE self)
{
#if GTK_CHECK_VERSION(2,2,0)
    return GOBJ2RVAL(gtk_clipboard_get_display(_SELF(self)));
#else
    return Qnil;
#endif
}

static void
clipboard_get_func(GtkClipboard *clipboard, GtkSelectionData *selection_data, G_GNUC_UNUSED guint info, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

struct clipboard_set_args {
    VALUE self;
    VALUE targets;
    VALUE func;
};

static VALUE
clipboard_set_body(VALUE value)
{
    struct clipboard_set_args *args = (struct clipboard_set_args *)value;
    GtkClipboard *clipboard = _SELF(args->self);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(args->targets, &n);
    gboolean result = gtk_clipboard_set_with_data(clipboard,
                                                  targets,
                                                  n,
                                                  (GtkClipboardGetFunc)clipboard_get_func,
                                                  (GtkClipboardClearFunc)NULL,
                                                  (gpointer)args->func);

    g_free(targets);

    return CBOOL2RVAL(result);
}

static G_GNUC_NORETURN VALUE
clipboard_set_rescue(VALUE value)
{
    struct clipboard_set_args *args = (struct clipboard_set_args *)value;

    G_CHILD_REMOVE(args->self, args->func);

    rb_exc_raise(rb_errinfo());
}

static VALUE
clipboard_set(VALUE self, VALUE targets)
{
    struct clipboard_set_args args = { self, targets, rb_block_proc() };

    G_CHILD_ADD(self, args.func);

    return rb_rescue(clipboard_set_body, (VALUE)&args,
                     clipboard_set_rescue, (VALUE)&args);
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
clipboard_get_owner(VALUE self)
{
    GObject *gobj;
    gobj = gtk_clipboard_get_owner(_SELF(self));
    return gobj ? GOBJ2RVAL(gobj) : Qnil;
}
*/

static VALUE
clipboard_clear(VALUE self)
{
    gtk_clipboard_clear(_SELF(self));
    return self;
}

static VALUE
clipboard_set_text(VALUE self, VALUE text)
{
    StringValue(text);
    gtk_clipboard_set_text(_SELF(self), RVAL2CSTR(text), RSTRING_LEN(text));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
clipboard_set_image(VALUE self, VALUE pixbuf)
{
    gtk_clipboard_set_image(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}
#endif

static void
clipboard_received_func(GtkClipboard *clipboard, GtkSelectionData *selection_data, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

static VALUE
clipboard_request_contents(VALUE self, VALUE target)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_clipboard_request_contents(_SELF(self), RVAL2ATOM(target),
                                   (GtkClipboardReceivedFunc)clipboard_received_func,
                                   (gpointer)func);
    return self;
}

static void
clipboard_text_received_func(GtkClipboard *clipboard, const gchar *text,
                             gpointer func)
{
    VALUE vtext = Qnil;
    if (text) {
        vtext = CSTR2RVAL(text);
    }

    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard), vtext);
}

static VALUE
clipboard_request_text(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_clipboard_request_text(_SELF(self),
                               (GtkClipboardTextReceivedFunc)clipboard_text_received_func,
                               (gpointer)func);
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static void
clipboard_image_received_func(GtkClipboard *clipboard, GdkPixbuf *pixbuf, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard),
               GOBJ2RVAL(pixbuf));
}

static VALUE
clipboard_request_image(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_clipboard_request_image(_SELF(self),
                               (GtkClipboardImageReceivedFunc)clipboard_image_received_func,
                               (gpointer)func);
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,4,0)
static void
clipboard_target_received_func(GtkClipboard *clipboard, GdkAtom *atoms, gint n_atoms, gpointer func)
{
    gint i;
    VALUE ary = rb_ary_new();
    for (i = 0; i < n_atoms; i++){
        rb_ary_push(ary, BOXED2RVAL(atoms[i], GDK_TYPE_ATOM));
    }
        
    rb_funcall((VALUE)func, id_call, 2, CLIPBOARD2RVAL(clipboard), ary);
}

static VALUE
clipboard_request_targets(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);

    gtk_clipboard_request_targets(_SELF(self),
                                  (GtkClipboardTargetsReceivedFunc)clipboard_target_received_func, 
                                  (gpointer)func);
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static void
clipboard_rich_text_received_func(GtkClipboard *clipboard, GdkAtom format, const guint8 *text, gsize length, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 3, CLIPBOARD2RVAL(clipboard), 
               BOXED2RVAL(format, GDK_TYPE_ATOM), rb_str_new((char*)text, length));
}

static VALUE
clipboard_request_rich_text(VALUE self, VALUE buffer)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);

    gtk_clipboard_request_rich_text(_SELF(self), GTK_TEXT_BUFFER(RVAL2GOBJ(buffer)),
                                    (GtkClipboardRichTextReceivedFunc)clipboard_rich_text_received_func,
                                    (gpointer)func);
    return self;
}
#endif

static VALUE
clipboard_wait_for_contents(VALUE self, VALUE target)
{
    return BOXED2RVAL(gtk_clipboard_wait_for_contents(
                          _SELF(self), 
                          RVAL2ATOM(target)), 
                      GTK_TYPE_SELECTION_DATA);
}

static VALUE
clipboard_wait_for_text(VALUE self)
{
    gchar *str;
    str = gtk_clipboard_wait_for_text(_SELF(self));
    return str ? CSTR2RVAL(str) : Qnil;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
clipboard_wait_for_image(VALUE self)
{
    GdkPixbuf* pixbuf = gtk_clipboard_wait_for_image(_SELF(self));
    return GOBJ2RVAL(pixbuf);
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
clipboard_wait_for_rich_text(VALUE self, VALUE buffer)
{
    GdkAtom format;
    gsize length;
    guint8* data = gtk_clipboard_wait_for_rich_text(_SELF(self), 
                                                    GTK_TEXT_BUFFER(RVAL2GOBJ(buffer)),
                                                                    &format, &length);
    if (data){
        VALUE str = rb_str_new((char*)data, length);
        g_free(data);
        return rb_assoc_new(str, BOXED2RVAL(format, GDK_TYPE_ATOM));
    } else {
        return rb_assoc_new(Qnil, BOXED2RVAL(format, GDK_TYPE_ATOM));
    }
    
}
#endif

static VALUE
clipboard_wait_is_text_available(VALUE self)
{
    return CBOOL2RVAL(gtk_clipboard_wait_is_text_available(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
clipboard_wait_is_image_available(VALUE self)
{
    return CBOOL2RVAL(gtk_clipboard_wait_is_image_available(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
clipboard_wait_is_rich_text_available(VALUE self, VALUE buffer)
{
    return CBOOL2RVAL(gtk_clipboard_wait_is_rich_text_available(_SELF(self), GTK_TEXT_BUFFER(RVAL2GOBJ(buffer))));
}
#endif

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
clipboard_wait_for_targets(VALUE self)
{
    gint i;
    VALUE ary = Qnil;
    GdkAtom* targets;
    gint n_targets = 0;
    gboolean ret = gtk_clipboard_wait_for_targets(_SELF(self), &targets, &n_targets);

    if (ret){
        ary = rb_ary_new();
        for (i = 0; i < n_targets; i++){
            if ((GdkAtom)targets == (GdkAtom)GDK_NONE){
                rb_ary_push(ary, rb_eval_string("Gdk::Atom::NONE"));
            }
            else
                rb_ary_push(ary, BOXED2RVAL(targets, GDK_TYPE_ATOM));
            targets++;
        } 
        /* How can I this ?
        g_free(&o_targets);
        */
    }
    return ary;
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
clipboard_wait_is_target_available(VALUE self, VALUE target)
{
    return CBOOL2RVAL(gtk_clipboard_wait_is_target_available(_SELF(self), RVAL2ATOM(target)));
}

static VALUE
clipboard_set_can_store(VALUE self, VALUE rbtargets)
{
    GtkClipboard *clipboard = _SELF(self);
    long n = 0;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES_ACCEPT_NIL(rbtargets, &n);

    gtk_clipboard_set_can_store(clipboard, targets, n);

    g_free(targets);

    return self;
}

static VALUE
clipboard_store(VALUE self)
{
    gtk_clipboard_store(_SELF(self));

    return self;
}
#endif

void 
Init_gtk_clipboard(void)
{
    VALUE gClipboard = G_DEF_CLASS(GTK_TYPE_CLIPBOARD, "Clipboard", mGtk);

    rb_define_singleton_method(gClipboard, "get", clipboard_get, -1);
    rb_define_method(gClipboard, "display", clipboard_get_display, 0);
    rb_define_method(gClipboard, "set", clipboard_set, 1);
/*
  rb_define_method(gClipboard, "owner", clipboard_get_owner, 0);
*/
    rb_define_method(gClipboard, "clear", clipboard_clear, 0);
    rb_define_method(gClipboard, "set_text", clipboard_set_text, 1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gClipboard, "set_image", clipboard_set_image, 1);
#endif
    rb_define_method(gClipboard, "request_contents", clipboard_request_contents, 1);
    rb_define_method(gClipboard, "request_text", clipboard_request_text, 0);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gClipboard, "request_image", clipboard_request_image, 0);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gClipboard, "request_targets", clipboard_request_targets, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gClipboard, "request_rich_text", clipboard_request_rich_text, 1);
#endif
    rb_define_method(gClipboard, "wait_for_contents", clipboard_wait_for_contents, 1);
    rb_define_method(gClipboard, "wait_for_text", clipboard_wait_for_text, 0);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gClipboard, "wait_for_image", clipboard_wait_for_image, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gClipboard, "wait_for_rich_text", clipboard_wait_for_rich_text, 1);
#endif
    rb_define_method(gClipboard, "wait_is_text_available?", clipboard_wait_is_text_available, 0);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gClipboard, "wait_is_image_available?", clipboard_wait_is_image_available, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gClipboard, "wait_is_rich_text_available?", clipboard_wait_is_rich_text_available, 1);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gClipboard, "wait_for_targets", clipboard_wait_for_targets, 0);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gClipboard, "wait_is_target_available?", clipboard_wait_is_target_available, 1);
    rb_define_method(gClipboard, "set_can_store", clipboard_set_can_store, 1);
    rb_define_method(gClipboard, "store", clipboard_store, 0);
#endif

    G_DEF_SETTERS(gClipboard);
}
