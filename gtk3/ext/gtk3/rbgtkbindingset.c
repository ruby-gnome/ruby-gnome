/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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
gtk_bindingset_free(G_GNUC_UNUSED GtkBindingSet* bin)
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

#define RG_TARGET_NAMESPACE cBindingSet
#define _SELF(s) (GtkBindingSet*)(RVAL2BOXED(s, GTK_TYPE_BINDING_SET))

static VALUE
rg_initialize(VALUE self, VALUE set_name)
{
    G_INITIALIZE(self, gtk_binding_set_new(RVAL2CSTR(set_name)));
    return Qnil;
}

static VALUE
rg_s_find(G_GNUC_UNUSED VALUE self, VALUE set_name)
{
    return BOXED2RVAL(gtk_binding_set_find(RVAL2CSTR(set_name)), 
                      GTK_TYPE_BINDING_SET);
}

static VALUE
rg_activate(VALUE self, VALUE keyval, VALUE modifiers, VALUE object)
{
    return CBOOL2RVAL(gtk_binding_set_activate(_SELF(self), NUM2UINT(keyval),
                                               RVAL2MOD(modifiers),
                                               GTK_OBJECT(RVAL2GOBJ(object))));
}

static VALUE
rg_entry_clear(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_clear(_SELF(self), NUM2UINT(keyval),
                            RVAL2MOD(modifiers));
    return self;
}

static VALUE
rg_add_signal(int argc, VALUE *argv, VALUE self)
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
            arg->d.string_data = (gchar *)RVAL2CSTR(param);
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
rg_add_path(VALUE self, VALUE path_type, VALUE path_pattern, VALUE priority)
{
    gtk_binding_set_add_path(_SELF(self), RVAL2GENUM(path_type, GTK_TYPE_PATH_TYPE),
                             RVAL2CSTR(path_pattern),
                             RVAL2GENUM(priority, GTK_TYPE_PATH_PRIORITY_TYPE));
    return self;
}

static VALUE
rg_entry_remove(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_remove(_SELF(self), NUM2UINT(keyval), RVAL2MOD(modifiers));
    return self;
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
rg_entry_skip(VALUE self, VALUE keyval, VALUE modifiers)
{
    gtk_binding_entry_skip(_SELF(self), NUM2UINT(keyval), RVAL2MOD(modifiers));
    return self;
}
#endif

void 
Init_gtk_bindings(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BINDING_SET, "BindingSet", mGtk);
    RG_DEF_METHOD(initialize, 1);
    RG_DEF_SMETHOD(find, 1);
    RG_DEF_METHOD(activate, 3);
    RG_DEF_METHOD(entry_clear, 2);
    RG_DEF_METHOD(add_signal, -1);
    RG_DEF_ALIAS("entry_add_signal", "add_signal");
    RG_DEF_METHOD(add_path, 3);
    RG_DEF_METHOD(entry_remove, 2);
#if GTK_CHECK_VERSION(2,12,0)
    RG_DEF_METHOD(entry_skip, 2);
#endif
}
