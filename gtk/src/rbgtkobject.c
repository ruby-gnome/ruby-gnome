/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkobject.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:15 $

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

	  case G_TYPE_OBJECT:
		*GTK_RETLOC_OBJECT(*arg) = GTK_OBJECT(RVAL2GOBJ(value));
		break;
	    
	  case GTK_TYPE_POINTER:
		*GTK_RETLOC_POINTER(*arg) = (gpointer)value;
		break;

	  case GTK_TYPE_BOXED:
		if (arg->type == GDK_TYPE_EVENT)
			GTK_VALUE_BOXED(*arg) = get_gdkevent(value);
#ifdef GTK_TYPE_GDK_COLORMAP
		else if (arg->type == GTK_TYPE_GDK_COLORMAP)
			GTK_VALUE_BOXED(*arg) = GDK_COLORMAP(RVAL2GOBJ(value));
#endif
#ifdef GTK_TYPE_GDK_FONT
#ifndef GTK_DISABLE_DEPRECATED
		else if (arg->type == GTK_TYPE_GDK_FONT)
			GTK_VALUE_BOXED(*arg) = get_gdkfont(value);
#endif
#endif
#ifdef GTK_TYPE_GDK_PIXMAP
		else if (arg->type == GTK_TYPE_GDK_PIXMAP)
			GTK_VALUE_BOXED(*arg) = GDK_PIXMAP(RVAL2GOBJ(value));
#endif
#ifdef GTK_TYPE_GDK_VISUAL
		else if (arg->type == GTK_TYPE_GDK_VISUAL)
			GTK_VALUE_BOXED(*arg) = GDK_VISUAL(RVAL2GOBJ(value));
#endif
#ifdef GTK_TYPE_ACCEL_GROUP
        else if (arg->type == GTK_TYPE_ACCEL_GROUP)
			GTK_VALUE_BOXED(*arg) = RVAL2GOBJ(value);
#endif
#ifdef GTK_TYPE_ACCELERATOR_TABLE
		else if (arg->type == GTK_TYPE_ACCELERATOR_TABLE)
			GTK_VALUE_BOXED(*arg) = get_gtkacceltbl(value);
#endif
#ifdef GTK_TYPE_STYLE
		else if (arg->type == GTK_TYPE_STYLE)
			GTK_VALUE_BOXED(*arg) = RVAL2GOBJ(value);
#endif
#ifdef GTK_TYPE_TOOLTIPS
		else if (arg->type == GTK_TYPE_TOOLTIPS)
			GTK_VALUE_BOXED(*arg) = RVAL2GOBJ(value);
#endif
		else
			goto unsupported;

	  unsupported:
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
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			return;
		}
		if (signal_comp(signame, "drag_leave", GTK_TYPE_WIDGET)){
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
			return;
		}
		if (signal_comp(signame, "drag_motion", GTK_TYPE_WIDGET) ||
			signal_comp(signame, "drag_drop", GTK_TYPE_WIDGET)){
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
			return;
		}
		if (signal_comp(signame, "drag_data_get", GTK_TYPE_WIDGET)){
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			rb_ary_push(args, make_gtkselectiondata(GTK_VALUE_POINTER(params[1])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
			return;
		}
		if (signal_comp(signame, "drag_data_received", GTK_TYPE_WIDGET)){
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
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
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			return;
		}
    }
    if (rb_obj_is_kind_of(obj, gEditable)) {
		if (signal_comp(signame, "insert_text", GTK_TYPE_EDITABLE)) {
			rb_ary_push(args, rb_str_new(GTK_VALUE_STRING(params[0]), 
										 GTK_VALUE_INT(params[1])));
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
	if (rb_obj_is_kind_of(obj, gNotebook)) {
		if (signal_comp(signame, "switch_page", GTK_TYPE_NOTEBOOK)) {
			rb_ary_push(args, make_notepage((GtkNotebookPage*) GTK_VALUE_OBJECT(params[0])));
			rb_ary_push(args, INT2FIX(GTK_VALUE_INT(params[1])));
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
    VALUE self = GOBJ2RVAL(widget);
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
gobj_destroy(self)
    VALUE self;
{
    VALUE iv = rb_ivar_get(self, id_gobject_data);

    if (NIL_P(iv) || RDATA(iv)->data == 0) {
		/* destroyed object */
		return Qnil;
    }
    gtk_object_destroy(GTK_OBJECT(RVAL2GOBJ(self)));
    rb_ivar_set(self, id_relatives, Qnil);
    return Qnil;
}

static VALUE
gobj_get_flags(self)
    VALUE self;
{
    /* _GtkObject.flags is int32 */
    return(INT2FIX(GTK_OBJECT_FLAGS(RVAL2GOBJ(self))));
}

static VALUE
gobj_set_flags(self, flags)
    VALUE self, flags;
{
    GTK_OBJECT_SET_FLAGS(RVAL2GOBJ(self), NUM2INT(flags));
    return self;
}

static VALUE
gobj_unset_flags(self, flags)
    VALUE self, flags;
{
    GTK_OBJECT_UNSET_FLAGS(RVAL2GOBJ(self), NUM2INT(flags));
    return self;
}

static VALUE
gobj_get_gtk_type(self)
    VALUE self;
{
    return INT2NUM(GTK_OBJECT_TYPE(RVAL2GOBJ(self)));
}

void Init_gtk_object()
{
    static RGObjClassInfo cinfo;

    gObject = rb_define_class_under(mGtk, "Object", rbgobj_cGObject);
    cinfo.klass = gObject;
    cinfo.gtype = GTK_TYPE_OBJECT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* GtkArgFlags */
    rb_define_const(gObject, "ARG_READABLE", INT2NUM(GTK_ARG_READABLE));
    rb_define_const(gObject, "ARG_WRITABLE", INT2NUM(GTK_ARG_WRITABLE));
    rb_define_const(gObject, "ARG_CONSTRUCT", INT2NUM(GTK_ARG_CONSTRUCT));
    rb_define_const(gObject, "ARG_CONSTRUCT_ONLY", INT2NUM(GTK_ARG_CONSTRUCT_ONLY));
    rb_define_const(gObject, "ARG_CHILD_ARG", INT2NUM(GTK_ARG_CHILD_ARG));
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
    rb_define_method(gObject, "flags", gobj_get_flags, 0);
    rb_define_method(gObject, "flags=", gobj_set_flags, 1);
    rb_define_method(gObject, "unset_flags", gobj_unset_flags, 1);
    rb_define_alias(gObject, "get_flags", "flags");
    rb_define_alias(gObject, "set_flags", "flags=");
    rb_define_method(gObject, "gtk_type", gobj_get_gtk_type, 0);

    rb_define_method(gObject, "destroy", gobj_destroy, 0);

    /* child initialize */
    Init_gtk_data();
    Init_gtk_widget();
    Init_gtk_itemfactory();
}
