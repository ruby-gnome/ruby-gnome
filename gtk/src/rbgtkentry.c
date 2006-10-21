/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkentry.c -

  $Author: mutoh $
  $Date: 2006/10/21 16:58:00 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_ENTRY(RVAL2GOBJ(self)))

static VALUE
entry_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_entry_new());
    return Qnil;
}

static VALUE
entry_get_layout(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_entry_get_layout(_SELF(self)));
}

static VALUE
entry_get_layout_offsets(self)
    VALUE self;
{
    int x, y;
    gtk_entry_get_layout_offsets(_SELF(self), &x, &y);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
entry_set_completion(self, completion)
    VALUE self, completion;
{
    gtk_entry_set_completion(_SELF(self), GTK_ENTRY_COMPLETION(RVAL2GOBJ(completion)));
    return self;
}

static VALUE
entry_get_completion(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_entry_get_completion(_SELF(self)));
}
#endif

static VALUE
entry_layout_index_to_text_index(self, layout_index)
    VALUE self, layout_index;
{
    return INT2NUM(gtk_entry_layout_index_to_text_index(_SELF(self), NUM2INT(layout_index)));
}

static VALUE
entry_text_index_to_layout_index(self, text_index)
    VALUE self, text_index;
{
    return INT2NUM(gtk_entry_text_index_to_layout_index(_SELF(self), NUM2INT(text_index)));
}

/* Deprecated.
GtkWidget*  gtk_entry_new_with_max_length   (gint max);
void        gtk_entry_append_text           (GtkEntry *entry,
                                             const gchar *text);
void        gtk_entry_prepend_text          (GtkEntry *entry,
                                             const gchar *text);
void        gtk_entry_set_position          (GtkEntry *entry,
                                             gint position);
void        gtk_entry_select_region         (GtkEntry *entry,
                                             gint start,
                                             gint end);
void        gtk_entry_set_editable          (GtkEntry *entry,
                                             gboolean editable);
*/

/* Defined as properties
const gchar* gtk_entry_get_text             (GtkEntry *entry);
void        gtk_entry_set_text              (GtkEntry *entry,
                                             const gchar *text);

void        gtk_entry_set_visibility        (GtkEntry *entry,
                                             gboolean visible);
void        gtk_entry_set_invisible_char    (GtkEntry *entry,
                                             gunichar ch);

void        gtk_entry_set_max_length        (GtkEntry *entry,
                                             gint max);
gboolean    gtk_entry_get_activates_default (GtkEntry *entry);
gboolean    gtk_entry_get_has_frame         (GtkEntry *entry);
const GtkBorder* gtk_entry_get_inner_border (GtkEntry *entry);
gint        gtk_entry_get_width_chars       (GtkEntry *entry);
void        gtk_entry_set_activates_default (GtkEntry *entry,
                                             gboolean setting);
void        gtk_entry_set_has_frame         (GtkEntry *entry,
                                             gboolean setting);
void        gtk_entry_set_inner_border      (GtkEntry *entry,
                                             const GtkBorder *border);
void        gtk_entry_set_width_chars       (GtkEntry *entry,
                                             gint n_chars);
gunichar    gtk_entry_get_invisible_char    (GtkEntry *entry);
// Use xalign/xalign=.
void        gtk_entry_set_alignment         (GtkEntry *entry,
                                             gfloat xalign); 
gfloat      gtk_entry_get_alignment         (GtkEntry *entry);

gint        gtk_entry_get_max_length        (GtkEntry *entry);
gboolean    gtk_entry_get_visibility        (GtkEntry *entry);
*/

void 
Init_gtk_entry()
{
    VALUE gEntry = G_DEF_CLASS(GTK_TYPE_ENTRY, "Entry", mGtk);

    rb_define_method(gEntry, "initialize", entry_initialize, 0);
    rb_define_method(gEntry, "layout", entry_get_layout, 0);
    rb_define_method(gEntry, "layout_offsets", entry_get_layout_offsets, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gEntry, "set_completion", entry_set_completion, 1);
    rb_define_method(gEntry, "completion", entry_get_completion, 0);
#endif
    rb_define_method(gEntry, "layout_index_to_text_index", entry_layout_index_to_text_index, 1);
    rb_define_method(gEntry, "text_index_to_layout_index", entry_text_index_to_layout_index, 1);

    G_DEF_SETTERS(gEntry);
}
