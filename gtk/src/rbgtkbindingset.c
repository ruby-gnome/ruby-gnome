/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbindingset.c -

  $Author: mutoh $
  $Date: 2003/06/21 18:19:00 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

/*****************************************/
static GtkBindingSet*
gtk_bindingset_copy(const GtkBindingSet* bin)
{
  GtkBindingSet* new_bin;
  g_return_val_if_fail (bin != NULL, NULL);
  new_bin = g_new(GtkBindingSet, 1);
  *new_bin = *bin;
  return new_bin;
}
                                                                                
GType
gtk_bindingset_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkBindingSet",
                    (GBoxedCopyFunc)gtk_bindingset_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/
                                                                                
#define _SELF(s) (GtkBindingSet*)(RVAL2BOXED(s, GTK_TYPE_BINDING_SET))

static VALUE
binding_initialize(self, set_name)
    VALUE self, set_name;
{
    RBGTK_INITIALIZE(self, gtk_binding_set_new(RVAL2CSTR(set_name)));
    return Qnil;
}
/*
GtkBindingSet* gtk_binding_set_by_class     (gpointer object_class);
*/

static VALUE
binding_s_find(self, set_name)
    VALUE self, set_name;
{
    return BOXED2RVAL(gtk_binding_set_find(RVAL2CSTR(set_name)), 
                      GTK_TYPE_BINDING_SET);
}

/* Move to Gtk::Object
gboolean    gtk_bindings_activate           (GtkObject *object,
                                             guint keyval,
                                             GdkModifierType modifiers);
*/

static VALUE
binding_activate(self, keyval, modifiers, object)
    VALUE self, keyval, modifiers, object;
{
    return gtk_binding_set_activate(_SELF(self), NUM2UINT(keyval),
                                    NUM2UINT(modifiers),
                                    GTK_OBJECT(RVAL2GOBJ(object))) ? Qtrue : Qfalse;
}

static VALUE
binding_entry_clear(self, keyval, modifiers)
    VALUE self, keyval, modifiers;
{
    gtk_binding_entry_clear(_SELF(self), NUM2UINT(keyval),
                            NUM2UINT(modifiers));
    return self;
}

/*
void        gtk_binding_entry_add_signal    (GtkBindingSet *binding_set,
                                             guint keyval,
                                             GdkModifierType modifiers,
                                             const gchar *signal_name,
                                             guint n_args,
                                             ...);
*/

static VALUE
binding_add_path(self, path_type, path_pattern, priority)
    VALUE self, path_type, path_pattern, priority;
{
    gtk_binding_set_add_path(_SELF(self), NUM2INT(path_type),
                             RVAL2CSTR(path_pattern),
                             NUM2INT(priority));
    return self;
}

static VALUE
binding_entry_remove(self, keyval, modifiers)
    VALUE self, keyval, modifiers;
{
    gtk_binding_entry_remove(_SELF(self), NUM2UINT(keyval),
                             NUM2UINT(modifiers));

    return self;
}

/*
static VALUE
binding_entry_add_signall(self, keyval, modifiers, signal_name, binding_args)
    VALUE self, keyval, modifiers, signal_name, binding_args;
{
    gtk_binding_entry_add_signall(_SELF(self), NUM2UINT(keyval),
                                  NUM2INT(modifiers), RVAL2CSTR(signal_name),
                                  
                                             guint keyval,
                                             GdkModifierType modifiers,
                                             const gchar *signal_name,
                                             GSList *binding_args);
guint       gtk_binding_parse_binding       (GScanner *scanner);
*/

void 
Init_gtk_bindings()
{
    VALUE gBinding = G_DEF_CLASS(GTK_TYPE_BINDING_SET, "BindingSet", mGtk);
    rb_define_method(gBinding, "initialize", binding_initialize, 1);
    rb_define_method(gBinding, "find", binding_s_find, 1);
    rb_define_method(gBinding, "activate", binding_activate, 3);
    rb_define_method(gBinding, "entry_clear", binding_entry_clear, 2);
    rb_define_method(gBinding, "add_path", binding_add_path, 3);
    rb_define_method(gBinding, "entry_remove", binding_entry_remove, 2);

}
