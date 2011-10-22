/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define _SELF(self) GTK_NOTEBOOK(RVAL2GOBJ(self))
#define RVAL2WIDGET(w) GTK_WIDGET(RVAL2GOBJ(w))
#define GTK_TYPE_NOTEBOOK_PAGE (gtk_notebookpage_get_type())

static VALUE
note_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_notebook_new());
    return Qnil;
}

static VALUE
note_append_page(int argc, VALUE *argv, VALUE self)
{
    VALUE child, label;

    rb_scan_args(argc, argv, "11", &child, &label);
    gtk_notebook_append_page(_SELF(self),
                             RVAL2WIDGET(child),
                             NIL_P(label) ? NULL : RVAL2WIDGET(label));
    G_CHILD_ADD(self, child);

    return self;
}

static VALUE
note_append_page_menu(int argc, VALUE *argv, VALUE self)
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
note_prepend_page(int argc, VALUE *argv, VALUE self)
{
    VALUE child, label;

    rb_scan_args(argc, argv, "11", &child, &label);
    gtk_notebook_prepend_page(_SELF(self),
                              RVAL2WIDGET(child),
                              NIL_P(label) ? NULL : RVAL2WIDGET(label));
    return self;
}

static VALUE
note_prepend_page_menu(int argc, VALUE *argv, VALUE self)
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
note_insert_page(int argc, VALUE *argv, VALUE self)
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
note_insert_page_menu(int argc, VALUE *argv, VALUE self)
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
note_remove_page(VALUE self, VALUE pos)
{
    gtk_notebook_remove_page(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
note_page_num(VALUE self, VALUE child)
{
    return INT2FIX(gtk_notebook_page_num(_SELF(self),
                                         RVAL2WIDGET(child)));
}

static VALUE
note_next_page(VALUE self)
{
    gtk_notebook_next_page(_SELF(self));
    return self;
}

static VALUE
note_prev_page(VALUE self)
{
    gtk_notebook_prev_page(_SELF(self));
    return self;
}

static VALUE
note_reorder_child(VALUE self, VALUE child, VALUE pos)
{
    gtk_notebook_reorder_child(_SELF(self), RVAL2WIDGET(child),
                               NUM2INT(pos));
    return self;
}

static VALUE
note_get_menu_label(VALUE self, VALUE child)
{
    return GOBJ2RVAL(gtk_notebook_get_menu_label(_SELF(self),
                                                 RVAL2WIDGET(child)));
}

static VALUE
note_get_nth_page(VALUE self, VALUE page_num)
{
    GtkWidget *page = gtk_notebook_get_nth_page(_SELF(self),
                                                NUM2INT(page_num));
    return page ? GOBJ2RVAL(page) : Qnil;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
note_get_n_pages(VALUE self)
{
    return INT2NUM(gtk_notebook_get_n_pages(_SELF(self)));
}
#endif

static VALUE
note_get_tab_label(VALUE self, VALUE child)
{
    return GOBJ2RVAL(gtk_notebook_get_tab_label(_SELF(self),
                                                RVAL2WIDGET(child)));
}

static VALUE
note_query_tab_label_packing(VALUE self, VALUE child)
{
    gboolean expand, fill;
    GtkPackType pack_type;
    VALUE ary;

    gtk_notebook_query_tab_label_packing(_SELF(self),
                                         RVAL2WIDGET(child),
                                         &expand, &fill, &pack_type);
    ary = rb_ary_new2(3);
    rb_ary_push(ary, CBOOL2RVAL(expand));
    rb_ary_push(ary, CBOOL2RVAL(fill));
    rb_ary_push(ary, GENUM2RVAL(pack_type, GTK_TYPE_PACK_TYPE));
    return ary;
}

static VALUE
note_set_menu_label(VALUE self, VALUE child, VALUE label)
{
    gtk_notebook_set_menu_label(_SELF(self),
                                RVAL2WIDGET(child),
                                NIL_P(label)?NULL:RVAL2WIDGET(label));
    return self;
}

static VALUE
note_set_menu_label_text(VALUE self, VALUE child, VALUE text)
{
    gtk_notebook_set_menu_label_text(_SELF(self),
                                     RVAL2WIDGET(child),
                                     RVAL2CSTR(text));
    return self;
}

static VALUE
note_set_tab_label(VALUE self, VALUE child, VALUE label)
{
    gtk_notebook_set_tab_label(_SELF(self),
                               RVAL2WIDGET(child),
                               NIL_P(label)?NULL:RVAL2WIDGET(label));
    return self;
}

static VALUE
note_set_tab_label_packing(VALUE self, VALUE child, VALUE expand, VALUE fill, VALUE pack_type)
{
    gtk_notebook_set_tab_label_packing(_SELF(self),
                                       RVAL2WIDGET(child),
                                       RVAL2CBOOL(expand), RVAL2CBOOL(fill),
                                       RVAL2GENUM(pack_type, GTK_TYPE_PACK_TYPE));
    return self;
}

static VALUE
note_set_tab_label_text(VALUE self, VALUE child, VALUE text)
{
    gtk_notebook_set_tab_label_text(_SELF(self),
                                    RVAL2WIDGET(child),
                                    RVAL2CSTR(text));
    return self;
}

static VALUE
note_get_menu_label_text(VALUE self, VALUE child)
{
    return CSTR2RVAL(gtk_notebook_get_menu_label_text(_SELF(self), 
                                                      RVAL2WIDGET(child)));
}

static VALUE
note_get_tab_label_text(VALUE self, VALUE child)
{
    return CSTR2RVAL(gtk_notebook_get_tab_label_text(_SELF(self), 
                                                     RVAL2WIDGET(child)));
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
note_get_tab_reorderable(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_notebook_get_tab_reorderable(_SELF(self), 
                                                       GTK_WIDGET(RVAL2GOBJ(child))));
}
static VALUE
note_set_tab_reorderable(VALUE self, VALUE child, VALUE reorderable)
{
    gtk_notebook_set_tab_reorderable(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                                     RVAL2CBOOL(reorderable));
    return self;
}

static VALUE
note_get_tab_detachable(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_notebook_get_tab_detachable(_SELF(self), 
                                                      GTK_WIDGET(RVAL2GOBJ(child))));
}
static VALUE
note_set_tab_detachable(VALUE self, VALUE child, VALUE detachable)
{
    gtk_notebook_set_tab_detachable(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                                     RVAL2CBOOL(detachable));
    return self;
}

static GtkNotebook*
creation_func(GtkNotebook *source, GtkWidget *page, gint x, gint y, gpointer func)
{
    VALUE ret;
    ret = rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(source), GOBJ2RVAL(page),
                     INT2NUM(x), INT2NUM(y));
    return NIL_P(ret) ? (GtkNotebook*)NULL : GTK_NOTEBOOK(RVAL2GOBJ(ret));
}

static VALUE
note_set_window_creation_hook(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_notebook_set_window_creation_hook((GtkNotebookWindowCreationFunc)creation_func, 
                                          (gpointer)func, (GDestroyNotify)NULL);
    return self;
}    
#endif

#if GTK_CHECK_VERSION(2,20,0)
static VALUE
note_set_action_widget(VALUE self, VALUE widget, VALUE pack_type)
{
    gtk_notebook_set_action_widget(_SELF(self),
                                   RVAL2WIDGET(widget),
                                   RVAL2GENUM(pack_type, GTK_TYPE_PACK_TYPE));
    return self;
}

static VALUE
note_get_action_widget(VALUE self, VALUE pack_type)
{
    return GOBJ2RVAL(gtk_notebook_get_action_widget(_SELF(self),
                                                    RVAL2GENUM(pack_type, GTK_TYPE_PACK_TYPE)));
}
#endif

/***********************************************/
/*
 * Gtk::NotebookPage
 */
typedef struct {
    VALUE parent;
    GtkNotebookPage *page;
} GtkNotebookPageData;

static GtkNotebookPageData *
notebookpage_copy(const GtkNotebookPage *page)
{
  GtkNotebookPageData *data;
  g_return_val_if_fail (page != NULL, NULL);
  data = g_new(GtkNotebookPageData, 1);
  data->page = (GtkNotebookPage *)page;
  return data;
}

static GType
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
signal_g2r_func(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkNotebookPageData npp;
    npp.parent = GVAL2RVAL(&values[0]);
    npp.page = (GtkNotebookPage*)g_value_get_pointer(&values[1]);
    return rb_ary_new3(3, GVAL2RVAL(&values[0]), BOXED2RVAL(&npp, GTK_TYPE_NOTEBOOK_PAGE),
                       GVAL2RVAL(&values[2]));
}

/* Defined as the "page" property.
*/

/* Defined as properties
// "enable-popup" property
void        gtk_notebook_popup_enable       (GtkNotebook *notebook);
void        gtk_notebook_popup_disable      (GtkNotebook *notebook);

void        gtk_notebook_set_group_id       (GtkNotebook *notebook,
                                             gint group_id);
gint        gtk_notebook_get_group_id       (GtkNotebook *notebook);

// "homogeneous" property
void        gtk_notebook_set_homogeneous_tabs
                                            (GtkNotebook *notebook,
                                             gboolean homogeneous);
// "page" property.
#define     gtk_notebook_current_page
#define     gtk_notebook_set_page
gint        gtk_notebook_get_current_page   (GtkNotebook *notebook);
void        gtk_notebook_set_current_page   (GtkNotebook *notebook,
                                             gint page_num);

gboolean    gtk_notebook_get_scrollable     (GtkNotebook *notebook);
void        gtk_notebook_set_scrollable     (GtkNotebook *notebook,
                                             gboolean scrollable);

void        gtk_notebook_set_show_border    (GtkNotebook *notebook,
                                             gboolean show_border);
gboolean    gtk_notebook_get_show_border    (GtkNotebook *notebook);

gboolean    gtk_notebook_get_show_tabs      (GtkNotebook *notebook);
void        gtk_notebook_set_show_tabs      (GtkNotebook *notebook,
                                             gboolean show_tabs);

GtkPositionType gtk_notebook_get_tab_pos    (GtkNotebook *notebook);
void        gtk_notebook_set_tab_pos        (GtkNotebook *notebook,
                                             GtkPositionType pos);

void        gtk_notebook_set_tab_hborder    (GtkNotebook *notebook,
                                             guint tab_hborder);
void        gtk_notebook_set_tab_border     (GtkNotebook *notebook,
                                             guint border_width);
void        gtk_notebook_set_tab_vborder    (GtkNotebook *notebook,
                                             guint tab_vborder);
 */

/*****************************************/

void 
Init_gtk_notebook(void)
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
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gNotebook, "n_pages", note_get_n_pages, 0);
#endif
    rb_define_method(gNotebook, "get_tab_label", note_get_tab_label, 1);
    rb_define_method(gNotebook, "query_tab_label_packing", note_query_tab_label_packing, 1);
    rb_define_method(gNotebook, "set_menu_label", note_set_menu_label, 2);
    rb_define_method(gNotebook, "set_menu_label_text", note_set_menu_label_text, 2);
    rb_define_method(gNotebook, "set_tab_label", note_set_tab_label, 2);
    rb_define_method(gNotebook, "set_tab_label_packing", note_set_tab_label_packing, 4);
    rb_define_method(gNotebook, "set_tab_label_text", note_set_tab_label_text, 2);
    rb_define_method(gNotebook, "get_menu_label_text", note_get_menu_label_text, 1);
    rb_define_method(gNotebook, "get_tab_label_text", note_get_tab_label_text, 1);

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gNotebook, "get_tab_reorderable", note_get_tab_reorderable, 1);
    rb_define_method(gNotebook, "set_tab_reorderable", note_set_tab_reorderable, 2);
    rb_define_method(gNotebook, "get_tab_detachable", note_get_tab_detachable, 1);
    rb_define_method(gNotebook, "set_tab_detachable", note_set_tab_detachable, 2);
    rb_define_singleton_method(gNotebook, "set_window_creation_hook", note_set_window_creation_hook, 0);
#endif
#if GTK_CHECK_VERSION(2,20,0)
    rb_define_method(gNotebook, "set_action_widget", note_set_action_widget, 2);
    rb_define_method(gNotebook, "get_action_widget", note_get_action_widget, 1);
#endif
    /* GtkNotebookTab */
    rb_define_const(gNotebook, "TAB_FIRST", GTK_NOTEBOOK_TAB_FIRST);
    rb_define_const(gNotebook, "TAB_LAST", GTK_NOTEBOOK_TAB_LAST);

    G_DEF_SIGNAL_FUNC(gNotebook, "switch_page", (GValToRValSignalFunc)signal_g2r_func);
}
