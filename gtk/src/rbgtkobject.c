/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkobject.c -

  $Author: mutoh $
  $Date: 2002/08/29 07:24:40 $

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
			GTK_VALUE_BOXED(*arg) = (GdkFont*)RVAL2BOXED(value);
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

    if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_WIDGET))) {
		if (signal_comp(signame, "draw", GTK_TYPE_WIDGET)) {
			rb_ary_push(args, BOXED2RVAL(GTK_VALUE_POINTER(params[0]), GDK_TYPE_RECTANGLE));
			return;
		}
		if (signal_comp(signame, "size_request", GTK_TYPE_WIDGET)) {
			rb_ary_push(args, make_grequisition(GTK_VALUE_POINTER(params[0])));
			return;
		}
		if (signal_comp(signame, "size_allocate", GTK_TYPE_WIDGET)) {
			rb_ary_push(args, BOXED2RVAL(GTK_VALUE_POINTER(params[0]), GDK_TYPE_RECTANGLE));
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
			rb_ary_push(args, BOXED2RVAL(GTK_VALUE_POINTER(params[1]), 
                                         GTK_TYPE_SELECTION_DATA));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[3])));
			return;
		}
		if (signal_comp(signame, "drag_data_received", GTK_TYPE_WIDGET)){
			rb_ary_push(args, GOBJ2RVAL(GTK_VALUE_POINTER(params[0])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[1])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[2])));
			rb_ary_push(args, BOXED2RVAL(GTK_VALUE_POINTER(params[3]), 
                                         GTK_TYPE_SELECTION_DATA));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[4])));
			rb_ary_push(args, INT2NUM(GTK_VALUE_INT(params[5])));
			return;
		}
    }
    if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_WINDOW))) {
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
    if (rb_obj_is_kind_of(obj, mEditable)) {
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
    if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_ENTRY))) {
		if (signal_comp(signame, "insert_position", GTK_TYPE_ENTRY)) {
			rb_ary_push(args, INT2NUM(*GTK_RETLOC_INT(params[0])));
			return;
		}
    }
    if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_CLIST))) {
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
	if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_NOTEBOOK))) {
/* FIXME
		if (signal_comp(signame, "switch_page", GTK_TYPE_NOTEBOOK)) {
			rb_ary_push(args, make_notepage((GtkNotebookPage*) GTK_VALUE_OBJECT(params[0])));
			rb_ary_push(args, INT2FIX(GTK_VALUE_INT(params[1])));
			return; 
		} 
*/
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

    if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_WIDGET))) {
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
    gtk_object_destroy(GTK_OBJECT(RVAL2GOBJ(self)));
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

void 
Init_gtk_object()
{
    VALUE gObject = G_DEF_CLASS(GTK_TYPE_OBJECT, "Object", mGtk);

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

    Init_gtk_accel_label();
    Init_gtk_adjustment();
    Init_gtk_alignment();
    Init_gtk_arrow();
    Init_gtk_aspect_frame();
    Init_gtk_bin();
    Init_gtk_box();
    Init_gtk_button();
    Init_gtk_button_box();
    Init_gtk_calendar();
    Init_gtk_check_button();
    Init_gtk_check_menu_item();
    Init_gtk_color_selection();
    Init_gtk_color_selection_dialog();
    Init_gtk_combo();
    Init_gtk_container();
    Init_gtk_curve();
    Init_gtk_dialog();
    Init_gtk_drawing_area();
    Init_gtk_entry();
    Init_gtk_eventbox();
    Init_gtk_file_selection();
    Init_gtk_fixed();
    Init_gtk_font_selection();
    Init_gtk_font_selection_dialog();
    Init_gtk_frame();
    Init_gtk_gamma_curve();
    Init_gtk_handle_box();
    Init_gtk_hbox();
    Init_gtk_hbutton_box();
    Init_gtk_hpaned();
    Init_gtk_hruler();
    Init_gtk_hscale();
    Init_gtk_hscrollbar();
    Init_gtk_hseparator();
    Init_gtk_image();
    Init_gtk_input_dialog();
    Init_gtk_item();
    Init_gtk_itemfactory();
    Init_gtk_label();
    Init_gtk_layout();
    Init_gtk_list_item();
    Init_gtk_menu();
    Init_gtk_menu_bar();
    Init_gtk_menu_item();
    Init_gtk_menu_shell();
    Init_gtk_misc();
    Init_gtk_notebook();
    Init_gtk_option_menu();
    Init_gtk_paned();
    Init_gtk_plug();
    Init_gtk_preview();
    Init_gtk_progress();
    Init_gtk_progress_bar();
    Init_gtk_radio_button();
    Init_gtk_radio_menu_item();
    Init_gtk_range();
    Init_gtk_ruler();
    Init_gtk_scale();
    Init_gtk_scrollbar();
    Init_gtk_scrolled_window();
    Init_gtk_separator();
    Init_gtk_socket();
    Init_gtk_spin_button();
    Init_gtk_statusbar();
    Init_gtk_table();
    Init_gtk_tearoff_menu_item();
    Init_gtk_textbuffer();
    Init_gtk_textchild();
    Init_gtk_textmark();
    Init_gtk_texttag();
    Init_gtk_toggle_button();
    Init_gtk_toolbar();
    Init_gtk_tooltips();
    Init_gtk_vbox();
    Init_gtk_vbutton_box();
    Init_gtk_viewport();
    Init_gtk_vpaned();
    Init_gtk_vruler();
    Init_gtk_vscale();
    Init_gtk_vscrollbar();
    Init_gtk_vseparator();
    Init_gtk_widget();
    Init_gtk_window();

#ifndef GTK_DISABLE_DEPRECATED
    Init_gtk_list();
    Init_gtk_clist();
    Init_gtk_ctree();
    Init_gtk_oldeditable();
    Init_gtk_pixmap();
    Init_gtk_tips_query();
#endif
#ifdef GTK_ENABLE_BROKEN
    Init_gtk_text();
    Init_gtk_tree();
    Init_gtk_tree_item();
#endif
}
