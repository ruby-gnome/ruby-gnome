/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtknotebook.c -

  $Author: mutoh $
  $Date: 2003/06/21 18:19:00 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) GTK_NOTEBOOK(RVAL2GOBJ(self))
#define RVAL2WIDGET(w) GTK_WIDGET(RVAL2GOBJ(w))
#define GTK_TYPE_NOTEBOOK_PAGE (gtk_notebookpage_get_type())

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
    gtk_notebook_append_page(_SELF(self),
                             RVAL2WIDGET(child),
                             NIL_P(label) ? NULL : RVAL2WIDGET(label));
    return self;
}

static VALUE
note_append_page_menu(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, tab_label, menu_label;
    
    rb_scan_args(argc, argv, "12", &child, &tab_label, &menu_label);
    gtk_notebook_append_page_menu(_SELF(self),
                                  RVAL2WIDGET(child),
                                  RVAL2WIDGET(tab_label),
                                  NIL_P(menu_label) ? NULL :
                                  RVAL2WIDGET(menu_label));
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
    gtk_notebook_prepend_page(_SELF(self),
                              RVAL2WIDGET(child),
                              NIL_P(label) ? NULL : RVAL2WIDGET(label));
    return self;
}

static VALUE
note_prepend_page_menu(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, tab_label, menu_label;

    rb_scan_args(argc, argv, "12", &child, &tab_label, &menu_label);
    gtk_notebook_prepend_page_menu(_SELF(self),
                                   RVAL2WIDGET(child),
                                   RVAL2WIDGET(tab_label),
                                   NIL_P(menu_label) ? NULL :
                                   RVAL2WIDGET(menu_label));
    return self;
}

static VALUE
note_insert_page(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE pos, child, label;

    rb_scan_args(argc, argv, "21", &pos, &child, &label);
    gtk_notebook_insert_page(_SELF(self),
                             RVAL2WIDGET(child),
                             NIL_P(label) ? NULL : RVAL2WIDGET(label),
                             NUM2INT(pos));
    return self;
}

static VALUE
note_insert_page_menu(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE pos, child, tab_label, menu_label;

    rb_scan_args(argc, argv, "22", &pos, &child, &tab_label, &menu_label);
    gtk_notebook_insert_page_menu(_SELF(self),
                                  RVAL2WIDGET(child),
                                  RVAL2WIDGET(tab_label),
                                  RVAL2WIDGET(menu_label),
                                  NUM2INT(pos));
    return self;
}

static VALUE
note_remove_page(self, pos)
    VALUE self, pos;
{
    gtk_notebook_remove_page(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
note_page_num(self, child)
    VALUE self, child;
{
    return INT2FIX(gtk_notebook_page_num(_SELF(self),
                                         RVAL2WIDGET(child)));
}

static VALUE
note_next_page(self)
    VALUE self;
{
    gtk_notebook_next_page(_SELF(self));
    return self;
}

static VALUE
note_prev_page(self)
    VALUE self;
{
    gtk_notebook_prev_page(_SELF(self));
    return self;
}

static VALUE
note_reorder_child(self, child, pos)
    VALUE self, child, pos;
{
    gtk_notebook_reorder_child(_SELF(self), RVAL2WIDGET(child),
                               NUM2INT(pos));
    return self;
}

static VALUE
note_get_menu_label(self, child)
    VALUE self, child;
{
    return GOBJ2RVAL(gtk_notebook_get_menu_label(_SELF(self),
                                                 RVAL2WIDGET(child)));
}

static VALUE
note_get_nth_page(self, page_num)
    VALUE self, page_num;
{
    GtkWidget *page = gtk_notebook_get_nth_page(_SELF(self),
                                                NUM2INT(page_num));
    return page ? GOBJ2RVAL(page) : Qnil;
}

/* Is this existed?
static VALUE
note_get_n_pages(self)
    VALUE self;
{
    return INT2NUM(gtk_notebook_get_n_pages(_SELF(self)));
}
*/

static VALUE
note_get_tab_label(self, child)
    VALUE self, child;
{
    return GOBJ2RVAL(gtk_notebook_get_tab_label(_SELF(self),
                                                RVAL2WIDGET(child)));
}

static VALUE
note_query_tab_label_packing(self, child)
    VALUE self, child;
{
    gboolean expand, fill;
    GtkPackType pack_type;
    VALUE ary;

    gtk_notebook_query_tab_label_packing(_SELF(self),
                                         RVAL2WIDGET(child),
                                         &expand, &fill, &pack_type);
    ary = rb_ary_new2(3);
    rb_ary_push(ary, expand ? Qtrue : Qfalse);
    rb_ary_push(ary, fill ? Qtrue : Qfalse);
    rb_ary_push(ary, INT2FIX(pack_type));
    return ary;
}

static VALUE
note_set_menu_label(self, child, label)
    VALUE self, child, label;
{
    gtk_notebook_set_menu_label(_SELF(self),
                                RVAL2WIDGET(child),
                                NIL_P(label)?NULL:RVAL2WIDGET(label));
    return self;
}

static VALUE
note_set_menu_label_text(self, child, text)
    VALUE self, child, text;
{
    gtk_notebook_set_menu_label_text(_SELF(self),
                                     RVAL2WIDGET(child),
                                     RVAL2CSTR(text));
    return self;
}

static VALUE
note_set_tab_label(self, child, label)
    VALUE self, child, label;
{
    gtk_notebook_set_tab_label(_SELF(self),
                               RVAL2WIDGET(child),
                               NIL_P(label)?NULL:RVAL2WIDGET(label));
    return self;
}

static VALUE
note_set_tab_label_packing(self, child, expand, fill, pack_type)
    VALUE self, child, expand, fill, pack_type;
{
    gtk_notebook_set_tab_label_packing(_SELF(self),
                                       RVAL2WIDGET(child),
                                       RTEST(expand), RTEST(fill),
                                       NUM2INT(pack_type));
    return self;
}

static VALUE
note_set_tab_label_text(self, child, text)
    VALUE self, child, text;
{
    gtk_notebook_set_tab_label_text(_SELF(self),
                                    RVAL2WIDGET(child),
                                    RVAL2CSTR(text));
    return self;
}

static VALUE
note_get_menu_label_text(self, child)
    VALUE self, child;
{
    return CSTR2RVAL(gtk_notebook_get_menu_label_text(_SELF(self), 
                                                      RVAL2WIDGET(child)));
}

static VALUE
note_get_tab_label_text(self, child)
    VALUE self, child;
{
    return CSTR2RVAL(gtk_notebook_get_tab_label_text(_SELF(self), 
                                                     RVAL2WIDGET(child)));
}

/***********************************************/
/*
 * Gtk::NotebookPage
 */
typedef struct {
    VALUE parent;
    GtkNotebookPage* page;
} GtkNotebookPageData;

GtkNotebookPageData*
notebookpage_copy (const GtkNotebookPage* page)
{
  GtkNotebookPageData* data;
  g_return_val_if_fail (page != NULL, NULL);
  data = g_new(GtkNotebookPageData, 1);
  data->page = (GtkNotebookPage*)page;
  return data;
}

GType
gtk_notebookpage_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkNotebookPageData",
                    (GBoxedCopyFunc)notebookpage_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}

static VALUE
signal_g2r_func(num, values)
    guint num;
    const GValue* values;
{
    GtkNotebookPageData npp;
    npp.parent = GVAL2RVAL(&values[0]);
    npp.page = (GtkNotebookPage*)g_value_get_pointer(&values[1]);
    return rb_ary_new3(3, GVAL2RVAL(&values[0]), BOXED2RVAL(&npp, GTK_TYPE_NOTEBOOK_PAGE),
                       GVAL2RVAL(&values[2]));
}

/*****************************************/

void 
Init_gtk_notebook()
{
    VALUE gNotebook = G_DEF_CLASS(GTK_TYPE_NOTEBOOK, "Notebook", mGtk);
    G_DEF_CLASS(GTK_TYPE_NOTEBOOK_PAGE, "NotebookPage", mGtk);

    rb_define_method(gNotebook, "initialize", note_initialize, 0);
    rb_define_method(gNotebook, "append_page", note_append_page, -1);
    rb_define_method(gNotebook, "append_page_menu", note_append_page_menu, -1);
    rb_define_method(gNotebook, "prepend_page", note_prepend_page, -1);
    rb_define_method(gNotebook, "prepend_page_menu", note_prepend_page_menu, -1);
    rb_define_method(gNotebook, "insert_page", note_insert_page, -1);
    rb_define_method(gNotebook, "insert_page_menu", note_insert_page_menu, -1);
    rb_define_method(gNotebook, "remove_page", note_remove_page, 1);
    rb_define_method(gNotebook, "page_num", note_page_num, 1);
    rb_define_method(gNotebook, "next_page", note_next_page, 0);
    rb_define_method(gNotebook, "prev_page", note_prev_page, 0);
    rb_define_method(gNotebook, "reorder_child", note_reorder_child, 2);
    rb_define_method(gNotebook, "get_menu_label", note_get_menu_label, 1);
    rb_define_method(gNotebook, "get_nth_page", note_get_nth_page, 1);
/*
    rb_define_method(gNotebook, "n_pages", note_get_n_pages, 0);
*/
    rb_define_method(gNotebook, "get_tab_label", note_get_tab_label, 1);
    rb_define_method(gNotebook, "query_tab_label_packing", note_query_tab_label_packing, 1);
    rb_define_method(gNotebook, "set_menu_label", note_set_menu_label, 2);
    rb_define_method(gNotebook, "set_menu_label_text", note_set_menu_label_text, 2);
    rb_define_method(gNotebook, "set_tab_label", note_set_tab_label, 2);
    rb_define_method(gNotebook, "set_tab_label_packing", note_set_tab_label_packing, 4);
    rb_define_method(gNotebook, "set_tab_label_text", note_set_tab_label_text, 2);
    rb_define_method(gNotebook, "get_menu_label_text", note_get_menu_label_text, 1);
    rb_define_method(gNotebook, "get_tab_label_text", note_get_tab_label_text, 1);
    /* GtkNotebookTab */
    rb_define_const(gNotebook, "TAB_FIRST", GTK_NOTEBOOK_TAB_FIRST);
    rb_define_const(gNotebook, "TAB_LAST", GTK_NOTEBOOK_TAB_LAST);

    G_DEF_SETTERS(gNotebook);
    G_DEF_SIGNAL_FUNC(gNotebook, "switch_page", signal_g2r_func);
}
