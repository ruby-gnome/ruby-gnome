/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtknotebook.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
note_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_notebook_new());
    return Qnil;
}

static VALUE
note_append_page(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, label;

    rb_scan_args(argc, argv, "11", &child, &label);
    gtk_notebook_append_page(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                             GTK_WIDGET(RVAL2GOBJ(child)),
                             NIL_P(label)?NULL:GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
note_append_page_menu(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, tab_label, menu_label;

    rb_scan_args(argc, argv, "21", &child, &tab_label, &menu_label);
    gtk_notebook_append_page_menu(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                  GTK_WIDGET(RVAL2GOBJ(child)),
                                  GTK_WIDGET(RVAL2GOBJ(tab_label)),
                                  NIL_P(menu_label) ? NULL :
								  GTK_WIDGET(RVAL2GOBJ(menu_label)));
    return self;
}

static VALUE
note_prepend_page(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, label;

    rb_scan_args(argc, argv, "11", &child, &label);
    gtk_notebook_prepend_page(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                              GTK_WIDGET(RVAL2GOBJ(child)),
                              NIL_P(label)?NULL:GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
note_prepend_page_menu(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, tab_label, menu_label;

    rb_scan_args(argc, argv, "21", &child, &tab_label, &menu_label);
    gtk_notebook_prepend_page_menu(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                   GTK_WIDGET(RVAL2GOBJ(child)),
                                   GTK_WIDGET(RVAL2GOBJ(tab_label)),
								   NIL_P(menu_label) ? NULL :
								   GTK_WIDGET(RVAL2GOBJ(menu_label)));

    return self;
}

static VALUE
note_insert_page(self, child, label, pos)
    VALUE self, child, label, pos;
{
    gtk_notebook_insert_page(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                             GTK_WIDGET(RVAL2GOBJ(child)),
							 NIL_P(label)?NULL:GTK_WIDGET(RVAL2GOBJ(label)),
                             NUM2INT(pos));
    return self;
}

static VALUE
note_insert_page_menu(self, child, tab_label, menu_label, pos)
    VALUE self, child, tab_label, menu_label, pos;
{
    gtk_notebook_insert_page_menu(GTK_NOTEBOOK(RVAL2GOBJ(self)),
								  GTK_WIDGET(RVAL2GOBJ(child)),
								  GTK_WIDGET(RVAL2GOBJ(tab_label)),
								  NIL_P(menu_label) ? NULL :
								  GTK_WIDGET(RVAL2GOBJ(menu_label)),
								  NUM2INT(pos));
    return self;
}

static VALUE
note_remove_page(self, pos)
    VALUE self, pos;
{
    gtk_notebook_remove_page(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(pos));
    return self;
}

static VALUE
note_set_page(self, pos)
    VALUE self, pos;
{
    gtk_notebook_set_page(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(pos));
    return self;
}

static VALUE
note_get_current_page(self)
    VALUE self;
{
    return INT2FIX(gtk_notebook_get_current_page(GTK_NOTEBOOK(RVAL2GOBJ(self))));
}

static VALUE
note_next_page(self)
    VALUE self;
{
    gtk_notebook_next_page(GTK_NOTEBOOK(RVAL2GOBJ(self)));
    return self;
}

static VALUE
note_prev_page(self)
    VALUE self;
{
    gtk_notebook_prev_page(GTK_NOTEBOOK(RVAL2GOBJ(self)));
    return self;
}

static VALUE
note_page_num(self, child)
    VALUE self, child;
{
    return INT2FIX(gtk_notebook_page_num(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                         GTK_WIDGET(RVAL2GOBJ(child))));
}

static VALUE
note_get_nth_page(self, page_num)
    VALUE self, page_num;
{
    GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                                NUM2INT(page_num));
    return page ? GOBJ2RVAL(page) : Qnil;
}

static VALUE
note_reorder_child(self, child, pos)
    VALUE self, child, pos;
{
    gtk_notebook_reorder_child(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                               GTK_WIDGET(RVAL2GOBJ(child)),
                               NUM2INT(pos));
    return self;
}

static VALUE
note_set_tab_pos(self, pos)
    VALUE self, pos;
{
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(pos));
    return self;
}

static VALUE
note_tab_pos(self)
    VALUE self;
{
    return INT2FIX(GTK_NOTEBOOK(RVAL2GOBJ(self))->tab_pos);
}

static VALUE
note_set_show_tabs(self, show_tabs)
    VALUE self, show_tabs;
{
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(RVAL2GOBJ(self)), RTEST(show_tabs));
    return self;
}

static VALUE
note_show_tabs(self)
    VALUE self;
{
    return GTK_NOTEBOOK(RVAL2GOBJ(self))->show_tabs?Qtrue:Qfalse;
}

static VALUE
note_set_show_border(self, show_border)
    VALUE self, show_border;
{
    gtk_notebook_set_show_border(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                 RTEST(show_border));
    return self;
}

static VALUE
note_show_border(self)
    VALUE self;
{
    return GTK_NOTEBOOK(RVAL2GOBJ(self))->show_border?Qtrue:Qfalse;
}

static VALUE
note_set_scrollable(self, scrollable)
    VALUE self, scrollable;
{
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                RTEST(scrollable));
    return self;
}

static VALUE
note_scrollable(self)
    VALUE self;
{
    return GTK_NOTEBOOK(RVAL2GOBJ(self))->scrollable?Qtrue:Qfalse;
}

static VALUE
note_set_homogeneous_tabs(self, homo)
    VALUE self, homo;
{
    gtk_notebook_set_homogeneous_tabs(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                      RTEST(homo));
    return self;
}

static VALUE
note_homogeneous(self)
    VALUE self;
{
    return GTK_NOTEBOOK(RVAL2GOBJ(self))->homogeneous?Qtrue:Qfalse;
}

static VALUE
note_popup_enable(self)
    VALUE self;
{
    gtk_notebook_popup_enable(GTK_NOTEBOOK(RVAL2GOBJ(self)));
    return self;
}

static VALUE
note_popup_disable(self)
    VALUE self;
{
    gtk_notebook_popup_disable(GTK_NOTEBOOK(RVAL2GOBJ(self)));
    return self;
}

static VALUE
note_set_tab_border(self, width)
    VALUE self, width;
{
    gtk_notebook_set_tab_border(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(width));
    return self;
}

static VALUE
note_set_tab_hborder(self, width)
    VALUE self, width;
{
    gtk_notebook_set_tab_hborder(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(width));
    return self;
}

static VALUE
note_set_tab_vborder(self, width)
    VALUE self, width;
{
    gtk_notebook_set_tab_vborder(GTK_NOTEBOOK(RVAL2GOBJ(self)), NUM2INT(width));
    return self;
}

static VALUE
note_query_tab_label_packing(self, child)
    VALUE self, child;
{
    gboolean expand, fill;
    GtkPackType pack_type;
    VALUE ary;

    gtk_notebook_query_tab_label_packing(GTK_NOTEBOOK(RVAL2GOBJ(self)),
										 GTK_WIDGET(RVAL2GOBJ(child)),
										 &expand, &fill, &pack_type);
    ary = rb_ary_new2(3);
    rb_ary_push(ary, expand ? Qtrue : Qfalse);
    rb_ary_push(ary, fill ? Qtrue : Qfalse);
    rb_ary_push(ary, INT2FIX(pack_type));
    return ary;
}

static VALUE
note_set_tab_label_packing(self, child, expand, fill, pack_type)
    VALUE self, child, expand, fill, pack_type;
{
    gtk_notebook_set_tab_label_packing(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                       GTK_WIDGET(RVAL2GOBJ(child)),
                                       RTEST(expand),
                                       RTEST(fill),
                                       NUM2INT(pack_type));
    return self;
}

static VALUE
note_get_menu_label(self, child)
    VALUE self, child;
{
    return GOBJ2RVAL(gtk_notebook_get_menu_label(GTK_NOTEBOOK(RVAL2GOBJ(self)),
												 GTK_WIDGET(RVAL2GOBJ(child))));
}

static VALUE
note_set_menu_label(self, child, label)
    VALUE self, child, label;
{
    gtk_notebook_set_menu_label(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                GTK_WIDGET(RVAL2GOBJ(child)),
                                NIL_P(label)?NULL:GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
note_set_menu_label_text(self, child, text)
    VALUE self, child, text;
{
    gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                     GTK_WIDGET(RVAL2GOBJ(child)),
                                     STR2CSTR(text));
    return self;
}

static VALUE
note_get_tab_label(self, child)
    VALUE self, child;
{
    return GOBJ2RVAL(gtk_notebook_get_tab_label(GTK_NOTEBOOK(RVAL2GOBJ(self)),
												GTK_WIDGET(RVAL2GOBJ(child))));
}

static VALUE
note_set_tab_label(self, child, label)
    VALUE self, child, label;
{
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                               GTK_WIDGET(RVAL2GOBJ(child)),
                               NIL_P(label)?NULL:GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
note_set_tab_label_text(self, child, text)
    VALUE self, child, text;
{
    gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(RVAL2GOBJ(self)),
                                    GTK_WIDGET(RVAL2GOBJ(child)),
                                    STR2CSTR(text));
    return self;
}

void 
Init_gtk_notebook()
{
    VALUE gNotebook = G_DEF_CLASS(GTK_TYPE_NOTEBOOK, "Notebook", mGtk);

    rb_define_const(gNotebook, "SIGNAL_SWITCH_PAGE", rb_str_new2("switch_page"));

    rb_define_method(gNotebook, "initialize", note_initialize, 0);
    rb_define_method(gNotebook, "append_page", note_append_page, -1);
    rb_define_method(gNotebook, "append_page_menu", note_append_page_menu, -1);
    rb_define_method(gNotebook, "prepend_page", note_prepend_page, -1);
    rb_define_method(gNotebook, "prepend_page_menu", note_prepend_page_menu, -1);
    rb_define_method(gNotebook, "insert_page", note_insert_page, 3);
    rb_define_method(gNotebook, "insert_page_menu", note_insert_page_menu, 4);
    rb_define_method(gNotebook, "remove_page", note_remove_page, 1);
    rb_define_method(gNotebook, "set_page", note_set_page, 1);
    rb_define_method(gNotebook, "get_current_page", note_get_current_page, 0);
    rb_define_method(gNotebook, "next_page", note_next_page, 0);
    rb_define_method(gNotebook, "prev_page", note_prev_page, 0);
    rb_define_method(gNotebook, "page_num", note_page_num, 1);
    rb_define_method(gNotebook, "get_nth_page", note_get_nth_page, 1);
    rb_define_method(gNotebook, "reorder_child", note_reorder_child, 2);
    rb_define_method(gNotebook, "set_tab_pos", note_set_tab_pos, 1);
    rb_define_method(gNotebook, "tab_pos", note_tab_pos, 0);
    rb_define_method(gNotebook, "set_show_tabs", note_set_show_tabs, 1);
    rb_define_method(gNotebook, "show_tabs", note_show_tabs, 0);
    rb_define_method(gNotebook, "set_show_border", note_set_show_border, 1);
    rb_define_method(gNotebook, "show_border", note_show_border, 0);
    rb_define_method(gNotebook, "set_scrollable", note_set_scrollable, 1);
    rb_define_method(gNotebook, "scrollable", note_scrollable, 0);
    rb_define_method(gNotebook, "set_homogeneous_tabs", note_set_homogeneous_tabs, 1);
    rb_define_method(gNotebook, "homogeneous", note_homogeneous, 0);
    rb_define_method(gNotebook, "popup_enable", note_popup_enable, 0);
    rb_define_method(gNotebook, "popup_disable", note_popup_disable, 0);
    rb_define_method(gNotebook, "set_tab_border", note_set_tab_border, 1);
    rb_define_method(gNotebook, "set_tab_hborder", note_set_tab_hborder, 1);
    rb_define_method(gNotebook, "set_tab_vborder", note_set_tab_vborder, 1);
    rb_define_method(gNotebook, "query_tab_label_packing", note_query_tab_label_packing, 1);
    rb_define_method(gNotebook, "set_tab_label_packing", note_set_tab_label_packing, 4);
    rb_define_method(gNotebook, "get_menu_label", note_get_menu_label, 1);
    rb_define_method(gNotebook, "set_menu_label", note_set_menu_label, 2);
    rb_define_method(gNotebook, "set_menu_label_text", note_set_menu_label_text, 2);
    rb_define_method(gNotebook, "get_tab_label", note_get_tab_label, 1);
    rb_define_method(gNotebook, "set_tab_label", note_set_tab_label, 2);
    rb_define_method(gNotebook, "set_tab_label_text", note_set_tab_label_text, 2);
}
