/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbindingset.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:09 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

/* 
   NOTE:
   - gtk_bindings_activate() is mapped to
     Gtk::Object#bindings_activate.

   - gtk_binding_set_by_class() is mapped to
     Gtk::Object.binding_set
 */

#include "global.h"

#define RVAL2MOD(mods) RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE)

/*****************************************/
static GtkBindingSet*
gtk_bindingset_copy(const GtkBindingSet* bin)
{
    /* GtkBindingSet should not be copied */
    return (GtkBindingSet*)bin;
}

static void
gtk_bindingset_free(GtkBindingSet* bin)
{
    /* GtkBindingSet should not be freed */
}

GType
gtk_bindingset_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkBindingSet",
                    (GBoxedCopyFunc)gtk_bindingset_copy,
                    (GBoxedFreeFunc)gtk_bindingset_free);
  return our_type;
}
/*****************************************/
                                                                                
#define _SELF(s) (GtkBindingSet*)(RVAL2BOXED(s, GTK_TYPE_BINDING_SET))

static VALUE
binding_initialize(VALUE self, VALUE set_name)
{
    G_INITIALIZE(self, gtk_binding_set_new(RVAL2CSTR(set_name)));
    return Qnil;
}

static VALUE
binding_s_find(VALUE self, VALUE set_name)
{
    return BOXED2RVAL(gtk_binding_set_find(RVAL2CSTR(set_name)), 
                      GTK_TYPE_BINDING_SET);
}

static VALUE
binding_activate(VALUE self, VALUE keyval, VALUE modifiers, VALUE object)
{
    return CBOOL2RVAL(gtk_binding_set_activate(_SELF(self), NUM2UINT(keyval),
                                               RVAL2MOD(modifiers),
                                               GTK_OBJECT(RVAL2GOBJ(object))));
}

static VALUE
binding_entry_clear(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_clear(_SELF(self), NUM2UINT(keyval),
                            RVAL2MOD(modifiers));
    return self;
}

static VALUE
binding_entry_add_signal(argc, argv, self)
     int argc; 
     VALUE* argv;
     VALUE self;
{
    VALUE keyval, modifiers, signame, rest;
    long i;
    VALUE param;
    GSList *slist, *free_slist;

    slist = NULL;

    rb_scan_args(argc, argv, "3*", &keyval, &modifiers, &signame, &rest);

    for (i = 0; i < RARRAY_LEN(rest); i++) {
        GtkBindingArg *arg;

        arg = g_new0 (GtkBindingArg, 1);
        slist = g_slist_prepend (slist, arg);

        param = RARRAY_PTR(rest)[i];
        if (TYPE(param) == T_FLOAT) {
            arg->arg_type = G_TYPE_DOUBLE;
            arg->d.double_data = NUM2DBL(param);
        } else if (rb_respond_to (param, rb_intern("to_int"))) {
            arg->arg_type = G_TYPE_LONG;
            arg->d.long_data = NUM2LONG(param);
        } else if (param == Qfalse) {
            arg->arg_type = G_TYPE_LONG;
            arg->d.long_data = 0;
        } else if (param == Qtrue) {
            arg->arg_type = G_TYPE_LONG;
            arg->d.long_data = 1;
        } else if (rb_respond_to (param, rb_intern("to_str"))) {
	    arg->arg_type = G_TYPE_STRING;
            arg->d.string_data = RVAL2CSTR(param);
        } else {
            rb_raise(rb_eTypeError, 
                     "can not convert %s into String, Numeric, "
                     "GLib::Enum/GLib::Flags or true/false",
                     rb_class2name(CLASS_OF(param)));

        }
    }
    slist = g_slist_reverse (slist);
    gtk_binding_entry_add_signall (_SELF(self), NUM2UINT(keyval), RVAL2MOD(modifiers),
                                   RVAL2CSTR(signame), slist);
    free_slist = slist;
    while (slist) {
        g_free (slist->data);
        slist = slist->next;
    }
    g_slist_free (free_slist);

    return self;
}

static VALUE
binding_add_path(VALUE self, VALUE path_type, VALUE path_pattern, VALUE priority)
{
    gtk_binding_set_add_path(_SELF(self), RVAL2GENUM(path_type, GTK_TYPE_PATH_TYPE),
                             RVAL2CSTR(path_pattern),
                             RVAL2GENUM(priority, GTK_TYPE_PATH_PRIORITY_TYPE));
    return self;
}

static VALUE
binding_entry_remove(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_remove(_SELF(self), NUM2UINT(keyval), RVAL2MOD(modifiers));
    return self;
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
binding_entry_skip(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_skip(_SELF(self), NUM2UINT(keyval), RVAL2MOD(modifiers));
    return self;
}
#endif

/* Deprecated.
guint       gtk_binding_parse_binding       (GScanner *scanner);
*/

void 
Init_gtk_bindings()
{
    VALUE gBinding = G_DEF_CLASS(GTK_TYPE_BINDING_SET, "BindingSet", mGtk);
    rb_define_method(gBinding, "initialize", binding_initialize, 1);
    rb_define_singleton_method(gBinding, "find", binding_s_find, 1);
    rb_define_method(gBinding, "activate", binding_activate, 3);
    rb_define_method(gBinding, "entry_clear", binding_entry_clear, 2);
    rb_define_method(gBinding, "add_signal", binding_entry_add_signal, -1);
    rb_define_method(gBinding, "add_path", binding_add_path, 3);
    rb_define_method(gBinding, "entry_add_signal", binding_entry_add_signal, -1);
    rb_define_method(gBinding, "entry_remove", binding_entry_remove, 2);
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gBinding, "entry_skip", binding_entry_skip, 2);
#endif
}
