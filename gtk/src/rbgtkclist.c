/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkclist.c -

  $Author: mutoh $
  $Date: 2002/05/28 15:19:24 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED
#include "global.h"

static void
clist_mark(clist)
    GtkCList *clist;
{
    if (clist) {
	GList *list;
	for (list = clist->row_list; list; list=list->next) {
	    GtkCListRow *row = GTK_CLIST_ROW(list);
	    if (row && row->data) {
		rb_gc_mark_maybe((VALUE)row->data);
	    }
	}
    }
}

static VALUE
clist_initialize(self, titles)
    VALUE self, titles;
{
    GtkWidget *widget;

    if (TYPE(titles) == T_ARRAY) {
	char **buf;
	int i, len;

	Check_Type(titles, T_ARRAY);
	len = RARRAY(titles)->len;
	buf = ALLOCA_N(char*, len);
	for (i=0; i<len; i++) {
	    buf[i] = STR2CSTR(RARRAY(titles)->ptr[i]);
	}
	widget = gtk_clist_new_with_titles(len, buf);
    }
    else {
	widget = gtk_clist_new(NUM2INT(titles));
    }
    set_widget(self, widget);

    return Qnil;
}

static VALUE
clist_set_sel_mode(self, mode)
    VALUE self, mode;
{
    gtk_clist_set_selection_mode(GTK_CLIST(get_widget(self)),
				 (GtkSelectionMode)NUM2INT(mode));
    return self;
}

static VALUE
clist_get_sel_mode(self)
    VALUE self;
{
    return INT2NUM(GTK_CLIST(get_widget(self))->selection_mode);
}

static VALUE
clist_freeze(self)
    VALUE self;
{
    gtk_clist_freeze(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_thaw(self)
    VALUE self;
{
    gtk_clist_thaw(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_col_titles_show(self)
    VALUE self;
{
    gtk_clist_column_titles_show(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_col_titles_hide(self)
    VALUE self;
{
    gtk_clist_column_titles_hide(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_col_title_active(self, column)
    VALUE self, column;
{
    gtk_clist_column_title_active(GTK_CLIST(get_widget(self)),
				  NUM2INT(column));
    return self;
}

static VALUE
clist_col_title_passive(self, column)
    VALUE self, column;
{
    gtk_clist_column_title_passive(GTK_CLIST(get_widget(self)),
				   NUM2INT(column));
    return self;
}

static VALUE
clist_col_titles_active(self)
    VALUE self;
{
    gtk_clist_column_titles_active(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_col_titles_passive(self)
    VALUE self;
{
    gtk_clist_column_titles_passive(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_set_col_title(self, col, title)
    VALUE self, col, title;
{
    gtk_clist_set_column_title(GTK_CLIST(get_widget(self)),
			       NUM2INT(col),
			       STR2CSTR(title));
    return self;
}

static VALUE
clist_set_col_widget(self, col, win)
    VALUE self, col, win;
{
    gtk_clist_set_column_widget(GTK_CLIST(get_widget(self)),
				NUM2INT(col),
				get_widget(win));
    return self;
}

static VALUE
clist_get_col_widget(self, col)
    VALUE self, col;
{
    GtkWidget *widget = NULL;

    widget = gtk_clist_get_column_widget(GTK_CLIST(get_widget(self)),
					 NUM2INT(col));
    return widget?get_value_from_gobject(GTK_OBJECT(widget)):Qnil;
}

static VALUE
clist_set_col_just(self, col, just)
    VALUE self, col, just;
{
    gtk_clist_set_column_justification(GTK_CLIST(get_widget(self)),
				       NUM2INT(col),
				       (GtkJustification)NUM2INT(just));
    return self;
}

static VALUE
clist_set_col_visibility(self, col, visible)
    VALUE self, col, visible;
{
    gtk_clist_set_column_visibility(GTK_CLIST(get_widget(self)),
				    NUM2INT(col),
				    RTEST(visible));
    return self;
}

static VALUE
clist_set_col_resizeable(self, col, resize)
    VALUE self, col, resize;
{
    gtk_clist_set_column_resizeable(GTK_CLIST(get_widget(self)),
				    NUM2INT(col),
				    RTEST(resize));
    return self;
}

static VALUE
clist_set_col_auto_resize(self, col, resize)
    VALUE self, col, resize;
{
    gtk_clist_set_column_auto_resize(GTK_CLIST(get_widget(self)),
				     NUM2INT(col),
				     RTEST(resize));
    return self;
}

static VALUE
clist_set_col_width(self, col, width)
    VALUE self, col, width;
{
    gtk_clist_set_column_width(GTK_CLIST(get_widget(self)),
			       NUM2INT(col), NUM2INT(width));
    return self;
}

static VALUE
clist_set_col_max_width(self, col, width)
    VALUE self, col, width;
{
    gtk_clist_set_column_max_width(GTK_CLIST(get_widget(self)),
				   NUM2INT(col), NUM2INT(width));
    return self;
}

static VALUE
clist_set_col_min_width(self, col, width)
    VALUE self, col, width;
{
    gtk_clist_set_column_min_width(GTK_CLIST(get_widget(self)),
				   NUM2INT(col), NUM2INT(width));
    return self;
}

static VALUE
clist_set_row_height(self, height)
    VALUE self, height;
{
    gtk_clist_set_row_height(GTK_CLIST(get_widget(self)), NUM2INT(height));
    return self;
}

static VALUE
clist_get_row_height(self)
    VALUE self;
{
    return INT2NUM( GTK_CLIST(get_widget(self))->row_height );
}

static VALUE
clist_get_selection_info(self, x, y)
    VALUE self, x, y;
{
    gint n, row, column;
    n = gtk_clist_get_selection_info(GTK_CLIST(get_widget(self)),
				     NUM2INT(x), NUM2INT(y),
				     &row, &column);
    return n ? rb_ary_new3(2, INT2FIX(row), INT2FIX(column)) : Qnil;
}

static VALUE
clist_moveto(self, row, col, row_align, col_align)
    VALUE self, row, col, row_align, col_align;
{
    gtk_clist_moveto(GTK_CLIST(get_widget(self)),
		     NUM2INT(row), NUM2INT(col),
		     (gfloat)NUM2DBL(row_align), (gfloat)NUM2DBL(col_align));
    return self;
}

static VALUE
clist_set_text(self, row, col, text)
    VALUE self, row, col, text;
{
    gtk_clist_set_text(GTK_CLIST(get_widget(self)),
		       NUM2INT(row), NUM2INT(col),
		       STR2CSTR(text));
    return self;
}

static VALUE
clist_set_pixmap(self, row, col, pixmap, mask)
    VALUE self, row, col, pixmap, mask;
{
    gtk_clist_set_pixmap(GTK_CLIST(get_widget(self)),
			 NUM2INT(row), NUM2INT(col),
			 get_gdkpixmap(pixmap),
			 (GdkBitmap*)get_gdkpixmap(mask));
    return self;
}

static VALUE
clist_set_pixtext(self, row, col, text, spacing, pixmap, mask)
    VALUE self, row, col, text, spacing, pixmap, mask;
{
    gtk_clist_set_pixtext(GTK_CLIST(get_widget(self)),
			  NUM2INT(row), NUM2INT(col),
			  STR2CSTR(text),
			  NUM2INT(spacing),
			  get_gdkpixmap(pixmap),
			  (GdkBitmap*)get_gdkpixmap(mask));
    return self;
}

static VALUE
clist_set_foreground(self, row, color)
    VALUE self, row, color;
{
    gtk_clist_set_foreground(GTK_CLIST(get_widget(self)),
			     NUM2INT(row), get_gdkcolor(color));
    return self;
}

static VALUE
clist_set_background(self, row, color)
    VALUE self, row, color;
{
    gtk_clist_set_background(GTK_CLIST(get_widget(self)),
			     NUM2INT(row), get_gdkcolor(color));
    return self;
}

static VALUE
clist_set_cell_style(self, row, column, style)
    VALUE self, row, column, style;
{
    gtk_clist_set_cell_style(GTK_CLIST(get_widget(self)),
			     NUM2INT(row), NUM2INT(column),
			     get_gstyle(style));
    return self;
}

static VALUE
clist_get_cell_style(self, row, column)
    VALUE self, row, column;
{
    GtkStyle* style;
    style = gtk_clist_get_cell_style(GTK_CLIST(get_widget(self)),
				     NUM2INT(row), NUM2INT(column));
    return make_gstyle(style);
}

static VALUE
clist_set_row_style(self, row, style)
    VALUE self, row, style;
{
    gtk_clist_set_row_style(GTK_CLIST(get_widget(self)),
			    NUM2INT(row), get_gstyle(style));
    return self;
}

static VALUE
clist_get_row_style(self, row)
    VALUE self, row;
{
    GtkStyle* style;
    style = gtk_clist_get_row_style(GTK_CLIST(get_widget(self)), NUM2INT(row));
    return make_gstyle(style);
}

static VALUE
clist_set_shift(self, row, col, verticle, horizontal)
    VALUE self, row, col, verticle, horizontal;
{
    gtk_clist_set_shift(GTK_CLIST(get_widget(self)),
			NUM2INT(row), NUM2INT(col),
			NUM2INT(verticle), NUM2INT(horizontal));
    return self;
}

static VALUE
clist_append(self, text)
    VALUE self, text;
{
    char **buf;
    int i, len;

    Check_Type(text, T_ARRAY);
    len = GTK_CLIST(get_widget(self))->columns;
    if (len > RARRAY(text)->len) {
	rb_raise(rb_eArgError, "text too short");
    }
    buf = ALLOCA_N(char*, len);
    for (i=0; i<len; i++) {
	buf[i] = (RARRAY(text)->ptr[i]==Qnil)?0:STR2CSTR(RARRAY(text)->ptr[i]);
    }
    i = gtk_clist_append(GTK_CLIST(get_widget(self)), buf);
    return INT2FIX(i);
}

static VALUE
clist_prepend(self, text)
    VALUE self, text;
{
    char **buf;
    int i, len;

    Check_Type(text, T_ARRAY);
    len = GTK_CLIST(get_widget(self))->columns;
    if (len > RARRAY(text)->len) {
	rb_raise(rb_eArgError, "text too short");
    }
    buf = ALLOCA_N(char*, len);
    for (i=0; i<len; i++) {
	buf[i] = (RARRAY(text)->ptr[i]==Qnil)?0:STR2CSTR(RARRAY(text)->ptr[i]);
    }
    i = gtk_clist_prepend(GTK_CLIST(get_widget(self)), buf);
    return INT2FIX(i);
}

static VALUE
clist_insert(self, row, text)
    VALUE self, row, text;
{
    char **buf;
    int i, len;

    Check_Type(text, T_ARRAY);
    len = GTK_CLIST(get_widget(self))->columns;
    if (len > RARRAY(text)->len) {
	rb_raise(rb_eArgError, "text too short");
    }
    buf = ALLOCA_N(char*, len);
    for (i=0; i<len; i++) {
	buf[i] = (RARRAY(text)->ptr[i]==Qnil)?0:STR2CSTR(RARRAY(text)->ptr[i]);
    }
    i = gtk_clist_insert(GTK_CLIST(get_widget(self)), NUM2INT(row), buf);
    return INT2FIX(i);
}

static VALUE
clist_remove(self, row)
    VALUE self, row;
{
    gtk_clist_remove(GTK_CLIST(get_widget(self)), NUM2INT(row));
    return self;
}

static VALUE
clist_set_row_data(self, row, data)
    VALUE self, row, data;
{
    gtk_clist_set_row_data(GTK_CLIST(get_widget(self)),
			   NUM2INT(row), (gpointer)data);
    return self;
}

static VALUE
clist_get_row_data(self, row)
    VALUE self, row;
{
    return (VALUE)gtk_clist_get_row_data(GTK_CLIST(get_widget(self)),
					 NUM2INT(row));
}

static VALUE
clist_get_text(self, row, col)
    VALUE self, row, col;
{
    char* text;

    gtk_clist_get_text(GTK_CLIST(get_widget(self)),
		       NUM2INT(row), NUM2INT(col), &text);
    return rb_str_new2(text);
}

static VALUE
clist_select_row(self, row, col)
    VALUE self, row, col;
{
    gtk_clist_select_row(GTK_CLIST(get_widget(self)),
			 NUM2INT(row), NUM2INT(col));
    return self;
}

static VALUE
clist_unselect_row(self, row, col)
    VALUE self, row, col;
{
    gtk_clist_unselect_row(GTK_CLIST(get_widget(self)),
			   NUM2INT(row), NUM2INT(col));
    return self;
}

static VALUE
clist_clear(self)
    VALUE self;
{
    gtk_clist_clear(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_set_sort_column(self, col)
    VALUE self, col;
{
    gtk_clist_set_sort_column(GTK_CLIST(get_widget(self)), NUM2INT(col));
    return self;
}

static VALUE
clist_get_sort_column(self)
    VALUE self;
{
    return INT2NUM(GTK_CLIST(get_widget(self))->sort_column);
}

static VALUE
clist_set_sort_type(self, stype)
    VALUE self, stype;
{
    gtk_clist_set_sort_type(GTK_CLIST(get_widget(self)), NUM2INT(stype));
    return self;
}

static VALUE
clist_get_sort_type(self)
    VALUE self;
{
    return INT2FIX(GTK_CLIST(get_widget(self))->sort_type);
}

static VALUE
clist_sort(self)
    VALUE self;
{
    gtk_clist_sort(GTK_CLIST(get_widget(self)));    
    return self;
}

static VALUE
clist_each_selection(self)
    VALUE self;
{
    GtkCList* clist;
    GList* sellist;

    clist = GTK_CLIST(get_widget(self));
    sellist = clist->selection;

    while (sellist) {
	gint row;
	row = GPOINTER_TO_INT(sellist->data);
	sellist = sellist->next;
	rb_yield(INT2NUM(row));
    }

    return Qnil;
}

static VALUE
clist_each(self)
    VALUE self;
{
    GtkCList* clist;
    GList* rowlist;

    clist = GTK_CLIST(get_widget(self));
    rowlist = clist->row_list;

    while (rowlist) {
	gint row;
	row = GPOINTER_TO_INT(rowlist->data);
	rowlist = rowlist->next;
	rb_yield(INT2NUM(row));
    }

    return Qnil;
}

static VALUE
clist_get_focus_row(self)
    VALUE self;
{
    return INT2NUM(GTK_CLIST(get_widget(self))->focus_row);
}

static VALUE
clist_set_reorderable(self, reorderable)
    VALUE self, reorderable;
{
    gtk_clist_set_reorderable(GTK_CLIST(get_widget(self)),
			      RTEST(reorderable));
    return self;
}

static VALUE
clist_undo_selection(self)
    VALUE self;
{
    gtk_clist_undo_selection(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_set_hadjustment(self, adjust)
    VALUE self, adjust;
{
    gtk_clist_set_hadjustment(GTK_CLIST(get_widget(self)),
                              NIL_P(adjust) ? NULL :
			        GTK_ADJUSTMENT(get_gobject(adjust)));
    return Qnil;
}

static VALUE
clist_get_hadjustment(self)
    VALUE self;
{
    GtkAdjustment *tmp;

    tmp = gtk_clist_get_hadjustment(GTK_CLIST(get_widget(self)));
    return make_gobject(gAdjustment, (GtkObject*)tmp);
}

static VALUE
clist_set_vadjustment(self, adjust)
    VALUE self, adjust;
{
    gtk_clist_set_vadjustment(GTK_CLIST(get_widget(self)),
                              NIL_P(adjust) ?  NULL :
			        GTK_ADJUSTMENT(get_gobject(adjust)));
    return Qnil;
}

static VALUE
clist_get_vadjustment(self)
    VALUE self;
{
    GtkAdjustment *tmp;

    tmp = gtk_clist_get_vadjustment(GTK_CLIST(get_widget(self)));
    return make_gobject(gAdjustment, (GtkObject*)tmp);
}

static VALUE
clist_get_rows(self)
    VALUE self;
{
    return INT2NUM(GTK_CLIST(get_widget(self))->rows);
}

static VALUE
clist_get_columns(self)
    VALUE self;
{
    return INT2NUM(GTK_CLIST(get_widget(self))->columns);
}

static VALUE
clist_row_is_visible(self, row)
	 VALUE self, row;
{
    GtkVisibility retval;
    retval = gtk_clist_row_is_visible(GTK_CLIST(get_widget(self)),
				                      NUM2INT(row));
    return (retval==GTK_VISIBILITY_NONE) ? Qnil : INT2NUM(retval);
}

static VALUE
clist_swap_rows(self, row1, row2)
	 VALUE self, row1, row2;
{
    gtk_clist_swap_rows(GTK_CLIST(get_widget(self)),
                        NUM2INT(row1), NUM2INT(row2));
    return self;
}

static VALUE
clist_row_move(self, source_row, dest_row)
	 VALUE self, source_row, dest_row;
{
    gtk_clist_row_move(GTK_CLIST(get_widget(self)),
		       NUM2INT(source_row), NUM2INT(dest_row));
    return self;
}

static VALUE
clist_set_auto_sort(self, auto_sort)
	 VALUE self, auto_sort;
{
    gtk_clist_set_auto_sort(GTK_CLIST(get_widget(self)), RTEST(auto_sort));
    return self;
}

static VALUE
clist_select_all(self)
	 VALUE self;
{
    gtk_clist_select_all(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_unselect_all(self)
	 VALUE self;
{
    gtk_clist_unselect_all(GTK_CLIST(get_widget(self)));
    return self;
}

static VALUE
clist_set_selectable(self, row, selectable)
	 VALUE self, row, selectable;
{
    gtk_clist_set_selectable(GTK_CLIST(get_widget(self)), NUM2INT(row), RTEST(selectable));
    return self;
}

static VALUE
clist_get_selectable(self, row)
	 VALUE self, row;
{
    return gtk_clist_get_selectable(GTK_CLIST(get_widget(self)), NUM2INT(row)) ? Qtrue : Qfalse;
}

static VALUE
clist_optimal_column_width(self, column)
	 VALUE self, column;
{
    return INT2NUM(gtk_clist_optimal_column_width(GTK_CLIST(get_widget(self)), NUM2INT(column)));
}

static VALUE
clist_set_use_drag_icons(self, use_icons)
	 VALUE self, use_icons;
{
    gtk_clist_set_use_drag_icons(GTK_CLIST(get_widget(self)), RTEST(use_icons));
    return self;
}

static VALUE
clist_get_column_title(self, column)
	 VALUE self, column;
{
    return CSTR2OBJ(gtk_clist_get_column_title(GTK_CLIST(get_widget(self)),
                                               NUM2INT(column)));
}

static VALUE
clist_set_shadow_type(self, type)
	 VALUE self, type;
{
    gtk_clist_set_shadow_type(GTK_CLIST(get_widget(self)), NUM2INT(type));
    return self;
}

static VALUE
clist_set_button_actions(self, button, button_actions)
	 VALUE self, button, button_actions;
{
    gtk_clist_set_button_actions(GTK_CLIST(get_widget(self)), NUM2INT(button), NUM2INT(button_actions));
    return self;
}

static VALUE
clist_get_cell_type(self, row, column)
	 VALUE self, row, column;
{
    return INT2FIX(gtk_clist_get_cell_type(GTK_CLIST(get_widget(self)), NUM2INT(row), NUM2INT(column)));
}

static VALUE
clist_find_row_from_data(self, data)
	 VALUE self, data;
{
    return INT2FIX(gtk_clist_find_row_from_data(GTK_CLIST(get_widget(self)), (gpointer)data));
}

void Init_gtk_clist()
{
    static rbgtk_class_info cinfo;

    gCList = rb_define_class_under(mGtk, "CList", gContainer);
    cinfo.klass = gCList;
    cinfo.gtype = gtk_clist_get_type();
    cinfo.mark = clist_mark;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);
    
    /* Signals */
    rb_define_const(gCList, "SIGNAL_SELECT_ROW", rb_str_new2("select_row"));
    rb_define_const(gCList, "SIGNAL_UNSELECT_ROW", rb_str_new2("unselect_row"));
    rb_define_const(gCList, "SIGNAL_ROW_MOVE", rb_str_new2("row_move"));
    rb_define_const(gCList, "SIGNAL_CLICK_COLUMN", rb_str_new2("click_column"));
    rb_define_const(gCList, "SIGNAL_RESIZE_COLUMN", rb_str_new2("resize_column"));
    rb_define_const(gCList, "SIGNAL_TOGGLE_FOCUS_ROW", rb_str_new2("toggle_focus_row"));
    rb_define_const(gCList, "SIGNAL_SELECT_ALL", rb_str_new2("select_all"));
    rb_define_const(gCList, "SIGNAL_UNSELECT_ALL", rb_str_new2("unselect_all"));
    rb_define_const(gCList, "SIGNAL_UNDO_SELECTION", rb_str_new2("undo_selection"));
    rb_define_const(gCList, "SIGNAL_END_SELECTION", rb_str_new2("end_selection"));
    rb_define_const(gCList, "SIGNAL_START_SELECTION", rb_str_new2("start_selection"));
    rb_define_const(gCList, "SIGNAL_TOGGLE_ADD_MODE", rb_str_new2("toggle_add_mode"));
    rb_define_const(gCList, "SIGNAL_SCROLL_VERTICAL", rb_str_new2("scroll_vertical"));
    rb_define_const(gCList, "SIGNAL_SCROLL_HORIZONTAL", rb_str_new2("scroll_horizontal"));
    rb_define_const(gCList, "CELL_EMPTY", INT2NUM(GTK_CELL_EMPTY));
    rb_define_const(gCList, "CELL_TEXT", INT2NUM(GTK_CELL_TEXT));
    rb_define_const(gCList, "CELL_PIXMAP", INT2NUM(GTK_CELL_PIXMAP));
    rb_define_const(gCList, "CELL_PIXTEXT", INT2NUM(GTK_CELL_PIXTEXT));
    rb_define_const(gCList, "CELL_WIDGET", INT2NUM(GTK_CELL_WIDGET));

    rb_define_method(gCList, "initialize", clist_initialize, 1);
    rb_define_method(gCList, "set_selection_mode", clist_set_sel_mode, 1);
    rb_define_method(gCList, "selection_mode=", clist_set_sel_mode, 1);
    rb_define_method(gCList, "selection_mode", clist_get_sel_mode, 0);
    rb_define_method(gCList, "freeze", clist_freeze, 0);
    rb_define_method(gCList, "thaw", clist_thaw, 0);
    rb_define_method(gCList, "column_titles_show", clist_col_titles_show, 0);
    rb_define_method(gCList, "column_titles_hide", clist_col_titles_hide, 0);
    rb_define_method(gCList, "column_title_active", clist_col_title_active, 1);
    rb_define_method(gCList, "column_title_passive", clist_col_title_passive, 1);
    rb_define_method(gCList, "column_titles_active", clist_col_titles_active, 0);
    rb_define_method(gCList, "column_titles_passive", clist_col_titles_passive, 0);
    rb_define_method(gCList, "set_column_title", clist_set_col_title, 2);
    rb_define_method(gCList, "set_column_widget", clist_set_col_widget, 2);
    rb_define_method(gCList, "get_column_widget", clist_get_col_widget, 1);
    rb_define_method(gCList, "set_column_justification", clist_set_col_just, 2);
    rb_define_method(gCList, "set_column_visibility", clist_set_col_visibility, 2);
    rb_define_method(gCList, "set_column_resizeable", clist_set_col_resizeable, 2);
    rb_define_method(gCList, "set_column_auto_resize", clist_set_col_auto_resize, 2);
    rb_define_method(gCList, "set_column_width", clist_set_col_width, 2);
    rb_define_method(gCList, "set_column_max_width", clist_set_col_max_width, 2);
    rb_define_method(gCList, "set_column_min_width", clist_set_col_min_width, 2);
    rb_define_method(gCList, "set_row_height", clist_set_row_height, 1);
    rb_define_method(gCList, "row_height=", clist_set_row_height, 1);
    rb_define_method(gCList, "row_height", clist_get_row_height, 0);
    rb_define_method(gCList, "get_selection_info", clist_get_selection_info, 2);
    rb_define_method(gCList, "moveto", clist_moveto, 4);
    rb_define_method(gCList, "set_text", clist_set_text, 3);
    rb_define_method(gCList, "set_pixmap", clist_set_pixmap, 4);
    rb_define_method(gCList, "set_pixtext", clist_set_pixtext, 6);
    rb_define_method(gCList, "set_foreground", clist_set_foreground, 2);
    rb_define_method(gCList, "set_background", clist_set_background, 2);
    rb_define_method(gCList, "set_cell_style", clist_set_cell_style, 3);
    rb_define_method(gCList, "get_cell_style", clist_get_cell_style, 2);
    rb_define_method(gCList, "set_row_style", clist_set_row_style, 2);
    rb_define_method(gCList, "get_row_style", clist_get_row_style, 1);
    rb_define_method(gCList, "set_shift", clist_set_shift, 4);
    rb_define_method(gCList, "append", clist_append, 1);
    rb_define_method(gCList, "prepend", clist_prepend, 1);
    rb_define_method(gCList, "insert", clist_insert, 2);
    rb_define_method(gCList, "remove_row", clist_remove, 1);
    rb_define_method(gCList, "set_row_data", clist_set_row_data, 2);
    rb_define_method(gCList, "get_row_data", clist_get_row_data, 1);
    rb_define_method(gCList, "get_text", clist_get_text, 2);
    rb_define_method(gCList, "select_row", clist_select_row, 2);
    rb_define_method(gCList, "unselect_row", clist_unselect_row, 2);
    rb_define_method(gCList, "clear", clist_clear, 0);
    rb_define_method(gCList, "set_sort_column", clist_set_sort_column, 1);
    rb_define_method(gCList, "sort_column=", clist_set_sort_column, 1);
    rb_define_method(gCList, "sort_column", clist_get_sort_column, 0);
    rb_define_method(gCList, "set_sort_type", clist_set_sort_type, 1);
    rb_define_method(gCList, "sort_type=", clist_set_sort_type, 1);
    rb_define_method(gCList, "sort_type", clist_get_sort_type, 0);
    rb_define_method(gCList, "sort", clist_sort, 0);
    rb_define_method(gCList, "each", clist_each, 0);
    rb_define_method(gCList, "each_selection", clist_each_selection, 0);
    rb_define_method(gCList, "focus_row", clist_get_focus_row, 0);
    rb_define_method(gCList, "set_reorderable", clist_set_reorderable, 1);
    rb_define_method(gCList, "undo_selection", clist_undo_selection, 0);
    rb_define_method(gCList, "rows", clist_get_rows, 0);
    rb_define_method(gCList, "columns", clist_get_columns, 0);
    rb_define_method(gCList, "hadjustment=", clist_set_hadjustment, 1);
    rb_define_method(gCList, "set_hadjustment", clist_set_hadjustment, 1);
    rb_define_method(gCList, "hadjustment", clist_get_hadjustment, 0);
    rb_define_method(gCList, "get_hadjustment", clist_get_hadjustment, 0);
    rb_define_method(gCList, "vadjustment=", clist_set_vadjustment, 1);
    rb_define_method(gCList, "set_vadjustment", clist_set_vadjustment, 1);
    rb_define_method(gCList, "vadjustment", clist_get_vadjustment, 0);
    rb_define_method(gCList, "get_vadjustment", clist_get_vadjustment, 0);
    rb_define_method(gCList, "row_is_visible", clist_row_is_visible, 1);

    rb_define_method(gCList, "select_all", clist_select_all, 0);
    rb_define_method(gCList, "unselect_all", clist_unselect_all, 0);
    rb_define_method(gCList, "swap_rows", clist_swap_rows, 2);
    rb_define_method(gCList, "row_move", clist_row_move, 2);
    rb_define_method(gCList, "set_auto_sort", clist_set_auto_sort, 1);

    rb_define_method(gCList, "set_selectable", clist_set_selectable, 2);
    rb_define_method(gCList, "get_selectable", clist_get_selectable, 1);
    rb_define_method(gCList, "optimal_column_width", clist_optimal_column_width, 1);
    rb_define_method(gCList, "set_use_drag_icons", clist_set_use_drag_icons, 1);
    rb_define_method(gCList, "get_column_title", clist_get_column_title, 1);
    rb_define_method(gCList, "set_shadow_type", clist_set_shadow_type, 1);
    rb_define_method(gCList, "set_button_actions", clist_set_button_actions, 2);
    rb_define_method(gCList, "get_cell_type", clist_get_cell_type, 2);
    rb_define_method(gCList, "find_row_from_data", clist_find_row_from_data, 1);
 
    /* child initialization */
    Init_gtk_ctree();
}
#endif
