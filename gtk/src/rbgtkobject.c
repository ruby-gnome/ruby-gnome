/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkobject.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

gboolean
signal_comp(sig1, sig2, type)
    char *sig1;
    char *sig2;
    guint type;
{
    if (gtk_signal_lookup(sig1, type) == gtk_signal_lookup(sig2, type))
	return TRUE;
    else
	return FALSE;
}

static void
arg_set_value(arg, value)
    GtkArg *arg;
    VALUE value;
{
    switch (GTK_FUNDAMENTAL_TYPE(arg->type)) {
    case GTK_TYPE_NONE:
	break;

    case GTK_TYPE_CHAR:
	*GTK_RETLOC_CHAR(*arg) = NUM2INT(value);
	break;
    case GTK_TYPE_BOOL:
	*GTK_RETLOC_BOOL(*arg) = (TYPE(value) == T_TRUE)? TRUE: FALSE;
	break;
    case GTK_TYPE_INT:
    case GTK_TYPE_ENUM:
    case GTK_TYPE_FLAGS:
	*GTK_RETLOC_INT(*arg) = NUM2INT(value);
	break;
    case GTK_TYPE_UINT:
	*GTK_RETLOC_UINT(*arg) = NUM2INT(value);
	break;
    case GTK_TYPE_LONG:
	*GTK_RETLOC_LONG(*arg) = NUM2INT(value);
	break;
    case GTK_TYPE_ULONG:
	*GTK_RETLOC_ULONG(*arg) = NUM2INT(value);
	break;

    case GTK_TYPE_FLOAT:
	value = rb_Float(value);
	*GTK_RETLOC_FLOAT(*arg) = (float)RFLOAT(value)->value;
	break;

    case GTK_TYPE_STRING:
	*GTK_RETLOC_STRING(*arg) = STR2CSTR(value);
	break;

#if GTK_MAJOR_VERSION < 2
    case GTK_TYPE_OBJECT:
#else
    case G_TYPE_OBJECT:
#endif
	*GTK_RETLOC_OBJECT(*arg) = get_gobject(value);
	break;
	    
    case GTK_TYPE_POINTER:
	*GTK_RETLOC_POINTER(*arg) = (gpointer)value;
	break;

    case GTK_TYPE_BOXED:
#if GTK_MAJOR_VERSION < 2
	if (arg->type == GTK_TYPE_GDK_EVENT)
#else
	if (arg->type == GDK_TYPE_EVENT)
#endif
	    GTK_VALUE_BOXED(*arg) = get_gdkevent(value);
#ifdef GTK_TYPE_GDK_COLORMAP
	else if (arg->type == GTK_TYPE_GDK_COLORMAP)
	    GTK_VALUE_BOXED(*arg) = get_gdkcmap(value);
#endif
#ifdef GTK_TYPE_GDK_FONT
	else if (arg->type == GTK_TYPE_GDK_FONT)
	    GTK_VALUE_BOXED(*arg) = get_gdkfont(value);
#endif
#ifdef GTK_TYPE_GDK_PIXMAP
	else if (arg->type == GTK_TYPE_GDK_PIXMAP)
	    GTK_VALUE_BOXED(*arg) = get_gdkpixmap(value);
#endif
#ifdef GTK_TYPE_GDK_VISUAL
	else if (arg->type == GTK_TYPE_GDK_VISUAL)
	    GTK_VALUE_BOXED(*arg) = get_gdkvisual(value);
#endif
#ifdef GTK_TYPE_ACCEL_GROUP
        else if (arg->type == GTK_TYPE_ACCEL_GROUP)
	    GTK_VALUE_BOXED(*arg) = get_gtkaccelgrp(value);
#endif
#ifdef GTK_TYPE_ACCELERATOR_TABLE
	else if (arg->type == GTK_TYPE_ACCELERATOR_TABLE)
	    GTK_VALUE_BOXED(*arg) = get_gtkacceltbl(value);
#endif
#ifdef GTK_TYPE_STYLE
	else if (arg->type == GTK_TYPE_STYLE)
	    GTK_VALUE_BOXED(*arg) = get_gstyle(value);
#endif
#ifdef GTK_TYPE_TOOLTIPS
	else if (arg->type == GTK_TYPE_TOOLTIPS)
	    GTK_VALUE_BOXED(*arg) = get_widget(value);
#endif
	else
	    goto unsupported;

    unsupported:
#if GTK_MAJOR_VERSION < 0
    case GTK_TYPE_INVALID:
    case GTK_TYPE_FOREIGN:
    case GTK_TYPE_CALLBACK:
    case GTK_TYPE_ARGS:
    case GTK_TYPE_SIGNAL:
    case GTK_TYPE_C_CALLBACK:
#endif
    default:
	rb_raise(rb_eTypeError, "unsupported return type %s (fundamental type %s)",
		 gtk_type_name(arg->type),
		 gtk_type_name(GTK_FUNDAMENTAL_TYPE(arg->type)));
	break;
    }
}

static void
signal_setup_args(obj, sig, argc, params, args)
    VALUE obj;
    ID sig;
    int argc;
    GtkArg *params;
    VALUE args;
{
    GtkArg *params1;
    int i;
    char *signame = rb_id2name(sig);

    if (rb_obj_is_kind_of(obj, gWidget)) {
	if (signal_comp(signame, "draw", GTK_TYPE_WIDGET)) {
	    rb_ary_push(args, make_gdkrectangle(GTK_VALUE_POINTER(params[0])));
	    return;
	}
	if (signal_comp(signame, "size_request", GTK_TYPE_WIDGET)) {
	    rb_ary_push(args, make_grequisition(GTK_VALUE_POINTER(params[0])));
	    return;
	}
	if (signal_comp(signame, "size_allocate", GTK_TYPE_WIDGET)) {
	    rb_ary_push(args, make_gallocation(GTK_VALUE_POINTER(params[0])));
	    return;
	}
	if (signal_comp(signame, "drag_begin", GTK_TYPE_WIDGET) ||
	    signal_comp(signame, "drag_end", GTK_TYPE_WIDGET) ||
        signal_comp(signame, "drag_data_delete", GTK_TYPE_WIDGET)) {
	    rb_ary_push(args, make_gdkdragcontext(GTK_VALUE_POINTER(params[0])));
	    return;
	}
	if (signal_comp(signame, "drag_leave", GTK_TYPE_WIDGET)){
	    rb_ary_push(args, make_gdkdragcontext(GTK_VALUE_POINTER(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "drag_motion", GTK_TYPE_WIDGET) ||
        signal_comp(signame, "drag_drop", GTK_TYPE_WIDGET)){
	    rb_ary_push(args, make_gdkdragcontext(GTK_VALUE_POINTER(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
	    return;
	}
	if (signal_comp(signame, "drag_data_get", GTK_TYPE_WIDGET)){
	    rb_ary_push(args, make_gdkdragcontext(GTK_VALUE_POINTER(params[0])));
	    rb_ary_push(args, make_gtkselectiondata(GTK_VALUE_POINTER(params[1])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
	    return;
	}
	if (signal_comp(signame, "drag_data_received", GTK_TYPE_WIDGET)){
	    rb_ary_push(args, make_gdkdragcontext(GTK_VALUE_POINTER(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
	    rb_ary_push(args, make_gtkselectiondata(GTK_VALUE_POINTER(params[3])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[4])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[5])));
	    return;
	}
    }
    if (rb_obj_is_kind_of(obj, gWindow)) {
	if (signal_comp(signame, "move_resize", GTK_TYPE_WINDOW)) {
	    rb_ary_push(args, INT2NUM(*GTK_RETLOC_INT(params[0])));
	    rb_ary_push(args, INT2NUM(*GTK_RETLOC_INT(params[1])));	
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[4])));
	    return;
	}
	if (signal_comp(signame, "set_focus", GTK_TYPE_WINDOW)) {
	    rb_ary_push(args, get_value_from_gobject(GTK_VALUE_POINTER(params[0])));
	    return;
	}
    }
    if (rb_obj_is_kind_of(obj, gEditable)) {
	if (signal_comp(signame, "insert_text", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, rb_str_new(GTK_VALUE_STRING(params[0]), GTK_VALUE_INT(params[1])));
	    rb_ary_push(args, INT2NUM(*GTK_RETLOC_INT(params[2])));
	    return;
	}
	if (signal_comp(signame, "delete_text", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "set_editable", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, GTK_VALUE_BOOL(params[0])? Qtrue: Qfalse);
	    return;
	}
	if (signal_comp(signame, "move_cursor", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "move_word", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[0])));
	    return;
	}
	if (signal_comp(signame, "move_page", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "move_to_row", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "move_to_column", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "kill_char", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "kill_word", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
	if (signal_comp(signame, "kill_line", GTK_TYPE_EDITABLE)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    return;
	}
    }
    if (rb_obj_is_kind_of(obj, gEntry)) {
	if (signal_comp(signame, "insert_position", GTK_TYPE_ENTRY)) {
	    rb_ary_push(args, INT2NUM(*GTK_RETLOC_INT(params[0])));
	    return;
	}
    }
    if (rb_obj_is_kind_of(obj, gCList)) {
	if (signal_comp(signame, "select_row", GTK_TYPE_CLIST) ||
	    signal_comp(signame, "unselect_row", GTK_TYPE_CLIST)) {
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[0])));
	    rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
	    if (GTK_VALUE_POINTER(params[2]))
		rb_ary_push(args, make_gdkevent(GTK_VALUE_POINTER(params[2])));
	    else
		rb_ary_push(args, Qnil);
	    return;
	}
    }
  
    params1 = params;
    for (i=0; i<argc; i++) {
	rb_ary_push(args, arg_to_value(params1));
	params1++;
    }
}

static void
signal_sync_args(obj, sig, argc, params, args)
    VALUE obj;
    ID sig;
    int argc;
    GtkArg *params;
    VALUE args;
{
    char *signame = rb_id2name(sig);

    if (rb_obj_is_kind_of(obj, gWidget)) {
	if (signal_comp(signame, "size_request", GTK_TYPE_WIDGET)) {
	    memcpy(GTK_VALUE_POINTER(params[0]), get_grequisition(rb_ary_pop(args)),
		   sizeof(GtkRequisition));
	    return;
	}
    }
}

void
signal_callback(widget, data, nparams, params)
    GtkWidget *widget;
    VALUE data;
    int nparams;
    GtkArg *params;
{
    VALUE self = get_value_from_gobject(GTK_OBJECT(widget));
    VALUE proc = RARRAY(data)->ptr[0];
    VALUE a = RARRAY(data)->ptr[2];
    ID id = NUM2INT(RARRAY(data)->ptr[1]);
    VALUE result = Qnil;
    VALUE args = rb_ary_new2(nparams+1+RARRAY(a)->len);
    int i;

    signal_setup_args(self, id, nparams, params, args);
    for (i=0; i<RARRAY(a)->len; i++) {
	rb_ary_push(args, RARRAY(a)->ptr[i]);
    }
    if (NIL_P(proc)) {
	if (rb_respond_to(self, id)) {
	    result = rb_apply(self, id, args);
	}
    }
    else {
	rb_ary_unshift(args, self);
	result = rb_apply(proc, id_call, args);
	rb_ary_shift(args);
    }
    if (params) {
	arg_set_value(params+nparams, result); /* I cannot understand it...?_? */
    }

    for (i=0; i<RARRAY(a)->len; i++) {
	rb_ary_pop(args);
    }
    signal_sync_args(self, id, nparams, params, args);
}

static VALUE
gobj_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_raise(rb_eRuntimeError, "can't instantiate class %s", rb_class2name(self));
}

static VALUE
gobj_smethod_added(self, id)
    VALUE self, id;
{
    GtkObject *obj = get_gobject(self);
    char *name = rb_id2name(NUM2INT(id));
    
    if (gtk_signal_lookup(name, GTK_OBJECT_TYPE(obj))) {
	VALUE data = rb_ary_new3(3, Qnil, id, rb_ary_new2(0));

	add_relative(self, data);
	gtk_signal_connect_full(obj, name, NULL,
				signal_callback, (gpointer)data,
				NULL, FALSE, 0);
    }
    return Qnil;
}

static VALUE
null()
{
    return (VALUE)NULL;
}

GtkObject*
force_get_gobject(self)
    VALUE self;
{
    return (GtkObject*)rb_rescue((VALUE(*)())get_gobject, self, null, 0);
}

static VALUE
gobj_equal(self, other)
    VALUE self, other;
{
    if (self == other) return Qtrue;
    if (get_gobject(self) == force_get_gobject(other)) return Qtrue;
    return Qfalse;
}

static VALUE
gobj_inspect(self)
    VALUE self;
{
    VALUE iv = rb_ivar_get(self, id_gtkdata);
    char *cname = rb_class2name(CLASS_OF(self));
    char *s;

    if (NIL_P(iv) || RDATA(iv)->data == 0) {
	s = ALLOCA_N(char, 2+strlen(cname)+2+9+1+1);
	sprintf(s, "#<%s: destroyed>", cname);
    }
    else {
	s = ALLOCA_N(char, 2+strlen(cname)+1+18+1+4+18+1+1);
	sprintf(s, "#<%s:%p ptr=%p>", cname, (void *)self, get_gobject(self));
    }
    return rb_str_new2(s);
}

#if GTK_MAJOR_VERSION < 2

static VALUE
gobj_sig_n_emissions(self, sig_id)
    VALUE self, sig_id;
{
    return INT2NUM(gtk_signal_n_emissions(get_gobject(self),
                                          NUM2INT(sig_id)));
}

static VALUE
gobj_sig_n_emissions_by_name(self, sig_name)
    VALUE self, sig_name;
{
    return INT2NUM(gtk_signal_n_emissions_by_name(get_gobject(self),
                                                  STR2CSTR(sig_name)));
}

#endif

/* TODO */
static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_notimplement();
    return self;
}

/* TODO */
static VALUE
gobj_sig_emit_by_name(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_notimplement();
    return self;
}

static VALUE
gobj_sig_emit_stop(self, sig_id)
    VALUE self, sig_id;
{
    gtk_signal_emit_stop(get_gobject(self), NUM2INT(sig_id));
    return self;
}

static VALUE
gobj_sig_emit_stop_by_name(self, sig_name)
    VALUE self, sig_name;
{
    gtk_signal_emit_stop_by_name(get_gobject(self), STR2CSTR(sig_name));
    return self;
}

static VALUE
gobj_sig_handler_block(self, id)
     VALUE self, id;
{
    gtk_signal_handler_block(GTK_OBJECT(get_gobject(self)), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_unblock(self, id)
     VALUE self, id;
{
    gtk_signal_handler_unblock(GTK_OBJECT(get_gobject(self)), NUM2INT(id));
    return self;
}

static void
clear_gobject(obj)
    VALUE obj;
{
    rb_ivar_set(obj, id_relatives, Qnil);
}

static VALUE
gobj_destroy(self)
    VALUE self;
{
    VALUE iv = rb_ivar_get(self, id_gtkdata);

    if (NIL_P(iv) || RDATA(iv)->data == 0) {
	/* destroyed object */
	return Qnil;
    }
    gtk_object_destroy(get_gobject(self));
    clear_gobject(self);
    return Qnil;
}

static VALUE
gobj_get_flags(self)
    VALUE self;
{
    /* _GtkObject.flags is int32 */
    return(INT2FIX( GTK_OBJECT_FLAGS(get_gobject(self)) ));
}

static VALUE
gobj_set_flags(self, flags)
    VALUE self, flags;
{
    GtkObject *object = get_gobject(self);
    GTK_OBJECT_SET_FLAGS(object, NUM2INT(flags));
    return self;
}

static VALUE
gobj_unset_flags(self, flags)
    VALUE self, flags;
{
    GtkObject *object = get_gobject(self);
    GTK_OBJECT_UNSET_FLAGS(object, NUM2INT(flags));
    return self;
}

static VALUE
gobj_get_gtk_type(self)
    VALUE self;
{
    return INT2NUM(GTK_OBJECT_TYPE(get_gobject(self)));
}

static VALUE
gobj_sig_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, data, args;
    ID id = 0;
    int i;

    rb_scan_args(argc, argv, "1*", &sig, &args);
    id = rb_intern(STR2CSTR(sig));
    data = rb_ary_new3(3, rb_f_lambda(), INT2NUM(id), args);
    add_relative(self, data);
    i = gtk_signal_connect_full(get_gobject(self),
				STR2CSTR(sig), NULL,
				signal_callback, (gpointer)data,
				NULL, FALSE, 0);

    return INT2FIX(i);
}

static VALUE
gobj_sig_connect_after(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, data, args;
    ID id = 0;
    int i;

    rb_scan_args(argc, argv, "1*", &sig, &args);
    id = rb_intern(STR2CSTR(sig));
    data = rb_ary_new3(3, rb_f_lambda(), INT2NUM(id), args);
    add_relative(self, data);
    i = gtk_signal_connect_full(get_gobject(self),
				STR2CSTR(sig), NULL,
				signal_callback, (gpointer)data,
				NULL, FALSE, 1);

    return INT2FIX(i);
}

static VALUE
gobj_sig_disconnect(self, handler_id)
    VALUE self, handler_id;
{
    gtk_signal_disconnect(get_gobject(self), NUM2INT(handler_id));
    return Qnil;
}

static VALUE
gobj_clone(self)
    VALUE self;
{
    rb_raise(rb_eTypeError, "can't clone %s", rb_class2name(CLASS_OF(self)));
}

void Init_gtk_object()
{
    gObject = rb_define_class_under(mGtk, "Object", rb_cObject);

#if GTK_MAJOR_VERSION < 2
    /* GtkObjectFlags */
    rb_define_const(gObject, "DESTROYED", INT2NUM(GTK_DESTROYED));
    rb_define_const(gObject, "FLOATING", INT2NUM(GTK_FLOATING));
    rb_define_const(gObject, "CONNECTED", INT2NUM(GTK_CONNECTED));
    rb_define_const(gObject, "CONSTRUCTED", INT2NUM(GTK_CONSTRUCTED));
#endif
    /* GtkArgFlags */
    rb_define_const(gObject, "ARG_READABLE", INT2NUM(GTK_ARG_READABLE));
    rb_define_const(gObject, "ARG_WRITABLE", INT2NUM(GTK_ARG_WRITABLE));
    rb_define_const(gObject, "ARG_CONSTRUCT", INT2NUM(GTK_ARG_CONSTRUCT));
    rb_define_const(gObject, "ARG_CONSTRUCT_ONLY", INT2NUM(GTK_ARG_CONSTRUCT_ONLY));
    rb_define_const(gObject, "ARG_CHILD_ARG", INT2NUM(GTK_ARG_CHILD_ARG));
#if GTK_MAJOR_VERSION < 2
    rb_define_const(gObject, "ARG_MASK", INT2NUM(GTK_ARG_MASK));
#endif
    rb_define_const(gObject, "ARG_READWRITE", INT2NUM(GTK_ARG_READWRITE));
    /* GtkSignalRunType */
    rb_define_const(gObject, "RUN_FIRST", INT2FIX(GTK_RUN_FIRST));
    rb_define_const(gObject, "RUN_LAST", INT2FIX(GTK_RUN_LAST));
    rb_define_const(gObject, "RUN_BOTH", INT2FIX(GTK_RUN_BOTH));
    rb_define_const(gObject, "RUN_NO_RECURSE", INT2FIX(GTK_RUN_NO_RECURSE));
    rb_define_const(gObject, "RUN_ACTION", INT2FIX(GTK_RUN_ACTION));
    rb_define_const(gObject, "RUN_NO_HOOKS", INT2FIX(GTK_RUN_NO_HOOKS));

    /*
     * signals
     */
    rb_define_const(gObject, "SIGNAL_DESTROY", rb_str_new2("destroy"));

    /*
     * instance methods
     */
    rb_define_method(gObject, "initialize", gobj_initialize, -1);
    rb_define_method(gObject, "flags", gobj_get_flags, 0);
    rb_define_method(gObject, "flags=", gobj_set_flags, 1);
    rb_define_method(gObject, "unset_flags", gobj_unset_flags, 1);
    rb_define_alias(gObject, "get_flags", "flags");
    rb_define_alias(gObject, "set_flags", "flags=");
    rb_define_method(gObject, "gtk_type", gobj_get_gtk_type, 0);

    rb_define_method(gObject, "destroy", gobj_destroy, 0);
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gObject, "signal_n_emissions", gobj_sig_n_emissions, 1);
    rb_define_method(gObject, "signal_n_emissions_by_name",
            gobj_sig_n_emissions_by_name, 1);
#endif
    rb_define_method(gObject, "signal_emit",
            gobj_sig_emit, -1);
    rb_define_method(gObject, "signal_emit_by_name",
            gobj_sig_emit_by_name, -1);
    rb_define_method(gObject, "signal_emit_stop",
            gobj_sig_emit_stop, 1);
    rb_define_method(gObject, "signal_emit_stop_by_name",
            gobj_sig_emit_stop_by_name, 1);
    rb_define_method(gObject, "signal_handler_block",
            gobj_sig_handler_block, 1);
    rb_define_method(gObject, "signal_handler_unblock",
            gobj_sig_handler_unblock, 1);
    rb_define_method(gObject, "signal_connect",
            gobj_sig_connect, -1);
    rb_define_method(gObject, "signal_connect_after",
            gobj_sig_connect_after, -1);
    rb_define_method(gObject, "signal_disconnect",
            gobj_sig_disconnect, 1);
    rb_define_method(gObject, "singleton_method_added", gobj_smethod_added, 1);
    rb_define_method(gObject, "==", gobj_equal, 1);
    rb_define_method(gObject, "inspect", gobj_inspect, 0);
    rb_define_method(gObject, "clone", gobj_clone, 0);

    /* child initialize */
    Init_gtk_data();
    Init_gtk_widget();
    Init_gtk_itemfactory();
}
