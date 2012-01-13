/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004-2006 Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

static ID id_action_procs;
static ID id_toggle_action_procs;

#define RG_TARGET_NAMESPACE cActionGroup
#define _SELF(self) (GTK_ACTION_GROUP(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
rg_initialize(VALUE self, VALUE name)
{
    G_INITIALIZE(self, gtk_action_group_new(RVAL2CSTR(name)));
    return Qnil;
}

static VALUE
rg_get_action(VALUE self, VALUE action_name)
{
    return GOBJ2RVAL(gtk_action_group_get_action(_SELF(self), RVAL2CSTR(action_name)));
}

static VALUE
rg_actions(VALUE self)
{
    return GLIST2ARYF(gtk_action_group_list_actions(_SELF(self)));
}

static VALUE
rg_add_action(int argc, VALUE *argv, VALUE self)
{
    VALUE action, accelerator;

    rb_scan_args(argc, argv, "11", &action, &accelerator);

    gtk_action_group_add_action_with_accel(_SELF(self),
                                           GTK_ACTION(RVAL2GOBJ(action)),
                                           NIL_P(accelerator) ? NULL : RVAL2CSTR(accelerator));
    G_CHILD_ADD(self, action);

    return self;
}

static VALUE
rg_remove_action(VALUE self, VALUE action)
{
    gtk_action_group_remove_action(_SELF(self), GTK_ACTION(RVAL2GOBJ(action)));
    G_CHILD_REMOVE(self, action);
    return self;
}

static void
activate_action(GtkAction *action, VALUE self)
{
    VALUE proc = rb_hash_aref(rb_ivar_get(self, id_action_procs),
                              CSTR2RVAL(gtk_action_get_name(action)));

    if (!NIL_P(proc))
        rb_funcall(proc, id_call, 2, self, GOBJ2RVAL(action));
}

struct rbg_rval2gtkactionentries_args {
    VALUE ary;
    long n;
    GtkActionEntry *result;
    VALUE procs;
};

static VALUE
rbg_rval2gtkactionentries_body(VALUE value)
{
    long i;
    struct rbg_rval2gtkactionentries_args *args = (struct rbg_rval2gtkactionentries_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE entry;
        long n;

        RARRAY_PTR(args->ary)[i] = rb_ary_dup(rb_ary_to_ary(RARRAY_PTR(args->ary)[i]));
        entry = RARRAY_PTR(args->ary)[i];
        n = RARRAY_LEN(entry);

        args->result[i].callback = G_CALLBACK(activate_action);

        if (n < 1 || n > 6)
            rb_raise(rb_eArgError, "entry does not contain right number of entries %ld (1..6)", n);

        args->result[i].name = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[0]);

        switch (n) {
        case 6:
            /* TODO: Call #to_proc on entry 5? */
            rb_hash_aset(args->procs, RARRAY_PTR(entry)[0], RARRAY_PTR(entry)[5]);
        case 5:
            args->result[i].tooltip = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[4]);
        case 4:
            args->result[i].accelerator = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[3]);
        case 3:
            args->result[i].label = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[2]);
        case 2:
            args->result[i].stock_id = RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(RARRAY_PTR(entry)[1]);
            break;
        default:
            break;
        }
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gtkactionentries_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gtkactionentries_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GtkActionEntry *
rbg_rval2gtkactionentries(volatile VALUE *value, long *n, VALUE procs)
{
    struct rbg_rval2gtkactionentries_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new0(GtkActionEntry, args.n + 1);
    args.procs = procs;

    rb_rescue(rbg_rval2gtkactionentries_body, (VALUE)&args,
              rbg_rval2gtkactionentries_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

#define RVAL2GTKACTIONENTRIES(value, n, procs) rbg_rval2gtkactionentries(&(value), &(n), procs)

static VALUE
rg_add_actions(VALUE self, VALUE rbentries)
{
    GtkActionGroup *group = _SELF(self);
    VALUE action_procs = rb_ivar_defined(self, id_action_procs) == Qtrue ?
        rb_ivar_get(self, id_action_procs) :
        rb_hash_new();
    long n;
    GtkActionEntry *entries = RVAL2GTKACTIONENTRIES(rbentries, n, action_procs);

    rb_ivar_set(self, id_action_procs, action_procs);

    gtk_action_group_add_actions(group, entries, n, (gpointer)self);

    g_free(entries);

    return self;
}

static void
activate_toggle_action(GtkAction *action, VALUE self)
{
    VALUE proc = rb_hash_aref(rb_ivar_get(self, id_toggle_action_procs),
                              CSTR2RVAL(gtk_action_get_name(action)));

    if (!NIL_P(proc))
        rb_funcall(proc, id_call, 2, self, GOBJ2RVAL(action));
}

struct rbg_rval2gtktoggleactionentries_args {
    VALUE ary;
    long n;
    GtkToggleActionEntry *result;
    VALUE procs;
};

static VALUE
rbg_rval2gtktoggleactionentries_body(VALUE value)
{
    long i;
    struct rbg_rval2gtktoggleactionentries_args *args = (struct rbg_rval2gtktoggleactionentries_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE entry;
        long n;

        RARRAY_PTR(args->ary)[i] = rb_ary_dup(rb_ary_to_ary(RARRAY_PTR(args->ary)[i]));
        entry = RARRAY_PTR(args->ary)[i];
        n = RARRAY_LEN(entry);

        args->result[i].callback = G_CALLBACK(activate_toggle_action);

        if (n < 1 || n > 7)
            rb_raise(rb_eArgError, "entry does not contain right number of entries %ld (1..7)", n);

        args->result[i].name = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[0]);

        switch (n) {
        case 7:
            args->result[i].is_active = RVAL2CBOOL(RARRAY_PTR(entry)[6]);
        case 6:
            /* TODO: Call #to_proc on entry 5? */
            rb_hash_aset(args->procs, RARRAY_PTR(entry)[0], RARRAY_PTR(entry)[5]);
        case 5:
            args->result[i].tooltip = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[4]);
        case 4:
            args->result[i].accelerator = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[3]);
        case 3:
            args->result[i].label = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[2]);
        case 2:
            args->result[i].stock_id = RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(RARRAY_PTR(entry)[1]);
            break;
        default:
            break;
        }
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gtktoggleactionentries_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gtktoggleactionentries_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GtkToggleActionEntry *
rbg_rval2gtktoggleactionentries(volatile VALUE *value, long *n, VALUE procs)
{
    struct rbg_rval2gtktoggleactionentries_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new0(GtkToggleActionEntry, args.n + 1);
    args.procs = procs;

    rb_rescue(rbg_rval2gtktoggleactionentries_body, (VALUE)&args,
              rbg_rval2gtktoggleactionentries_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

#define RVAL2GTKTOGGLEACTIONENTRIES(value, n, procs) rbg_rval2gtktoggleactionentries(&(value), &(n), procs)

static VALUE
rg_add_toggle_actions(VALUE self, VALUE rbentries)
{
    GtkActionGroup *group = _SELF(self);
    VALUE toggle_action_procs = rb_ivar_defined(self, id_toggle_action_procs) == Qtrue ?
        rb_ivar_get(self, id_toggle_action_procs) :
        rb_hash_new();
    long n;
    GtkToggleActionEntry *entries = RVAL2GTKTOGGLEACTIONENTRIES(rbentries, n, toggle_action_procs);

    rb_ivar_set(self, id_toggle_action_procs, toggle_action_procs);

    gtk_action_group_add_toggle_actions(group, entries, n, (gpointer)self);

    g_free(entries);

    return self;
}

static void
activate_radio_action(GtkAction *action, GtkRadioAction *current, VALUE proc)
{
    if (!NIL_P(proc))
        rb_funcall(proc, id_call, 2, GOBJ2RVAL(action), GOBJ2RVAL(current));
}

struct rbg_rval2gtkradioactionentries_args {
    VALUE ary;
    long n;
    GtkRadioActionEntry *result;
};

static VALUE
rbg_rval2gtkradioactionentries_body(VALUE value)
{
    long i;
    struct rbg_rval2gtkradioactionentries_args *args = (struct rbg_rval2gtkradioactionentries_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE entry;
        long n;

        RARRAY_PTR(args->ary)[i] = rb_ary_dup(rb_ary_to_ary(RARRAY_PTR(args->ary)[i]));
        entry = RARRAY_PTR(args->ary)[i];
        n = RARRAY_LEN(entry);

        switch (n) {
        case 6:
            args->result[i].value = NUM2INT(RARRAY_PTR(entry)[5]);
        case 5:
            args->result[i].tooltip = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[4]);
        case 4:
            args->result[i].accelerator = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[3]);
        case 3:
            args->result[i].label = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[2]);
        case 2:
            args->result[i].stock_id = RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(RARRAY_PTR(entry)[1]);
        case 1:
            args->result[i].name = RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[0]);
            break;
        default:
            rb_raise(rb_eArgError, "entry does not contain right number of entries %ld (1..6)", n);
            break;
        }
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gtkradioactionentries_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gtkradioactionentries_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GtkRadioActionEntry *
rbg_rval2gtkradioactionentries(volatile VALUE *value, long *n)
{
    struct rbg_rval2gtkradioactionentries_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new0(GtkRadioActionEntry, args.n + 1);

    rb_rescue(rbg_rval2gtkradioactionentries_body, (VALUE)&args,
              rbg_rval2gtkradioactionentries_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

#define RVAL2GTKRADIOACTIONENTRIES(value, n) rbg_rval2gtkradioactionentries(&(value), &(n))

static VALUE
rg_add_radio_actions(int argc, VALUE *argv, VALUE self)
{
    GtkActionGroup *group = _SELF(self);
    VALUE rbentries, rbvalue, proc;
    long n;
    GtkRadioActionEntry *entries;
    gint value;

    rb_scan_args(argc, argv, "12", &rbentries, &rbvalue, &proc);

    value = NIL_P(rbvalue) ? -1 : NUM2INT(rbvalue);

    if (NIL_P(proc) && rb_block_given_p()) {
        proc = rb_block_proc();
        /* TODO: How do we remove this proc when all the radio actions passed
         * have been removed? */
        G_RELATIVE(self, proc);
    }

    entries = RVAL2GTKRADIOACTIONENTRIES(rbentries, n);

    gtk_action_group_add_radio_actions(group,
                                       entries,
                                       n,
                                       value,
                                       G_CALLBACK(activate_radio_action),
                                       (gpointer)proc);

    g_free(entries);

    return self;
}

static gchar *
translate_func(const gchar *path, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 1, CSTR2RVAL(path));
    return (gchar *)RVAL2CSTR(ret);
}

static VALUE
rg_set_translate_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_action_group_set_translate_func(_SELF(self), 
                                        (GtkTranslateFunc)translate_func, 
                                        (gpointer)func, NULL);
    return self;
}

static VALUE
rg_set_translation_domain(VALUE self, VALUE domain)
{
    gtk_action_group_set_translation_domain(_SELF(self), 
                                            NIL_P(domain) ? (const gchar*)NULL : RVAL2CSTR(domain));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_translate_string(VALUE self, VALUE str)
{
    return CSTR2RVAL(gtk_action_group_translate_string(_SELF(self), RVAL2CSTR(str)));
}
#endif

static void
action_group_mark(void *p)
{
    GtkActionGroup *group;
    GList *actions, *node;

    group = GTK_ACTION_GROUP(p);
    actions = gtk_action_group_list_actions(group);
    for (node = actions; node; node = g_list_next(node)) {
        GtkWidget *action = node->data;
        rbgobj_gc_mark_instance(action);
    }
    g_list_free(actions);
}
#endif

void 
Init_gtk_actiongroup(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_ACTION_GROUP, "ActionGroup",
                                                    mGtk, action_group_mark, NULL);

    id_action_procs = rb_intern("@action_procs");
    id_toggle_action_procs = rb_intern("@toggle_action_procs");

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(get_action, 1);
    RG_DEF_METHOD(actions, 0);
    RG_DEF_METHOD(add_action, -1);
    RG_DEF_METHOD(remove_action, 1);
    RG_DEF_METHOD(add_actions, 1);
    RG_DEF_METHOD(add_toggle_actions, 1);
    RG_DEF_METHOD(add_radio_actions, -1);
    RG_DEF_METHOD(set_translate_func, 0);
    RG_DEF_METHOD(set_translation_domain, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "translation_domain");
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(translate_string, 1);
#endif
#endif
}
