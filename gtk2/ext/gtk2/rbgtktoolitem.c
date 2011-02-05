/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktoolitem.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2004,2005 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)
#define _SELF(self) (GTK_TOOL_ITEM(RVAL2GOBJ(self)))
static VALUE
toolitem_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_tool_item_new());
    return Qnil;
}

static VALUE
toolitem_set_homogeneous(VALUE self, VALUE homogeneous)
{
    gtk_tool_item_set_homogeneous(_SELF(self), RVAL2CBOOL(homogeneous));
    return self;
}
static VALUE
toolitem_get_homogeneous(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_homogeneous(_SELF(self)));
}

static VALUE
toolitem_set_expand(VALUE self, VALUE expand)
{
    gtk_tool_item_set_expand(_SELF(self), RVAL2CBOOL(expand));
    return self;
}
static VALUE
toolitem_get_expand(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_expand(_SELF(self)));
}

static VALUE
toolitem_set_tooltip(int argc, VALUE *argv, VALUE self)
{
    VALUE tooltip, tip_text, tip_private;

    rb_scan_args(argc, argv, "21", &tooltip, &tip_text, &tip_private);

    gtk_tool_item_set_tooltip(_SELF(self), GTK_TOOLTIPS(RVAL2GOBJ(tooltip)),
                              RVAL2CSTR(tip_text), 
                              NIL_P(tip_private) ? NULL : RVAL2CSTR(tip_private));
    return self;
}

static VALUE
toolitem_set_use_drag_window(VALUE self, VALUE use_drag_window)
{
    gtk_tool_item_set_use_drag_window(_SELF(self), RVAL2CBOOL(use_drag_window));
    return self;
}
static VALUE
toolitem_get_use_drag_window(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_use_drag_window(_SELF(self)));
}

static VALUE
toolitem_get_icon_size(VALUE self)
{
    return GENUM2RVAL(gtk_tool_item_get_icon_size(_SELF(self)), GTK_TYPE_ICON_SIZE);
}

static VALUE
toolitem_get_orientation(VALUE self)
{
    return GENUM2RVAL(gtk_tool_item_get_orientation(_SELF(self)), GTK_TYPE_ORIENTATION);
}

static VALUE
toolitem_get_toolbar_style(VALUE self)
{
    return GENUM2RVAL(gtk_tool_item_get_toolbar_style(_SELF(self)), GTK_TYPE_TOOLBAR_STYLE);
}

static VALUE
toolitem_get_relief_style(VALUE self)
{
    return GENUM2RVAL(gtk_tool_item_get_relief_style(_SELF(self)), GTK_TYPE_RELIEF_STYLE);
}

static VALUE
toolitem_retrieve_proxy_menu_item(VALUE self)
{
    return GOBJ2RVAL(gtk_tool_item_retrieve_proxy_menu_item(_SELF(self)));
}

static VALUE
toolitem_get_proxy_menu_item(VALUE self, VALUE menu_item_id)
{
    return GOBJ2RVAL(gtk_tool_item_get_proxy_menu_item(_SELF(self), RVAL2CSTR(menu_item_id)));
}

static VALUE
toolitem_set_proxy_menu_item(VALUE self, VALUE menu_item_id, VALUE menu_item)
{
    gtk_tool_item_set_proxy_menu_item(_SELF(self), RVAL2CSTR(menu_item_id),
                                      GTK_WIDGET(RVAL2GOBJ(menu_item)));
    return self;
}

/* Defined as Properties
void        gtk_tool_item_set_visible_horizontal
                                            (GtkToolItem *toolitem,
                                             gboolean visible_horizontal);
gboolean    gtk_tool_item_get_visible_horizontal
                                            (GtkToolItem *toolitem);
void        gtk_tool_item_set_visible_vertical
                                            (GtkToolItem *toolitem,
                                             gboolean visible_vertical);
gboolean    gtk_tool_item_get_visible_vertical
                                            (GtkToolItem *toolitem);
void        gtk_tool_item_set_is_important  (GtkToolItem *tool_item,
                                             gboolean is_important);
gboolean    gtk_tool_item_get_is_important  (GtkToolItem *tool_item);
*/
#if GTK_CHECK_VERSION(2,6,0)
static VALUE
toolitem_rebuild_menu(VALUE self)
{
    gtk_tool_item_rebuild_menu(_SELF(self));
    return self;
}
#endif
#endif

void 
Init_gtk_toolitem()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gToolItem = G_DEF_CLASS(GTK_TYPE_TOOL_ITEM, "ToolItem", mGtk);
    rb_define_method(gToolItem, "initialize", toolitem_initialize, 0);
    rb_define_method(gToolItem, "set_homogeneous", toolitem_set_homogeneous, 1);
    rb_define_method(gToolItem, "homogeneous?", toolitem_get_homogeneous, 0);
    rb_define_method(gToolItem, "set_expand", toolitem_set_expand, 1);
    rb_define_method(gToolItem, "expand?", toolitem_get_expand, 0);
    rb_define_method(gToolItem, "set_tooltip", toolitem_set_tooltip, -1);
    rb_define_method(gToolItem, "set_use_drag_window", toolitem_set_use_drag_window, 1);
    rb_define_method(gToolItem, "use_drag_window?", toolitem_get_use_drag_window, 0);
    rb_define_method(gToolItem, "icon_size", toolitem_get_icon_size, 0);
    rb_define_method(gToolItem, "orientation", toolitem_get_orientation, 0);
    rb_define_method(gToolItem, "toolbar_style", toolitem_get_toolbar_style, 0);
    rb_define_method(gToolItem, "relief_style", toolitem_get_relief_style, 0);
    rb_define_method(gToolItem, "retrieve_proxy_menu_item", toolitem_retrieve_proxy_menu_item, 0);
    rb_define_method(gToolItem, "get_proxy_menu_item", toolitem_get_proxy_menu_item, 1);
    rb_define_method(gToolItem, "set_proxy_menu_item", toolitem_set_proxy_menu_item, 2);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gToolItem, "rebuild_menu", toolitem_rebuild_menu, 0);
#endif
    G_DEF_SETTERS(gToolItem);
#endif
}
