/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-app-helper.c,v 1.8 2002/10/19 16:36:25 tkubo Exp $ */
/* based on libgnomeui/gnome-app-helper.h */

/* Gnome::UIInfo module for Ruby/GNOME2
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *               2002 KUBO Takehiro <kubo@jiubao.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_APP(RVAL2GOBJ(self))

static VALUE
uiinfo_separator(self)
    VALUE self;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SEPARATOR));  /* type */
    rb_ary_push(ary, Qnil);             /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, Qnil);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));           /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_item(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_item_stock(self, label, tooltip, callback, stock_id)
    VALUE self, label, tooltip, callback, stock_id;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_STOCK));  /* pixmap_type */
    rb_ary_push(ary, stock_id);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_item_none(self, label, tooltip, callback)
    VALUE self, label, tooltip, callback;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_NONE));   /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_item_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, user_data);                /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_toggleitem(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_TOGGLEITEM)); /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_toggleitem_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_TOGGLEITEM)); /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, user_data);            /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_help(self, app_name)
    VALUE self, app_name;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_HELP));   /* type */
    rb_ary_push(ary, Qnil);             /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, app_name);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));           /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_subtree(self, label, tree)
    VALUE self, label, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));    /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, tree);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));           /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_subtree_hint(self, label, hint, tree)
    VALUE self, label, hint, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));    /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, hint);             /* hint */
    rb_ary_push(ary, tree);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));           /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_subtree_stock(self, label, tree, stock_id)
    VALUE self, label, tree, stock_id;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));    /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, tree);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_STOCK));  /* pixmap_type */
    rb_ary_push(ary, stock_id);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_include(self, tree)
    VALUE self, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_INCLUDE));    /* type */
    rb_ary_push(ary, Qnil);                /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, tree);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));       /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_radiolist(self, list)
    VALUE self, list;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_RADIOITEMS)); /* type */
    rb_ary_push(ary, Qnil);             /* label */
    rb_ary_push(ary, Qnil);             /* hint */
    rb_ary_push(ary, list);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(0));           /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_radioitem(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, Qnil);             /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_radioitem_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tooltip);              /* hint */
    rb_ary_push(ary, callback);             /* moreinfo */
    rb_ary_push(ary, user_data);                /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));   /* pixmap_type */
    rb_ary_push(ary, xpm_data);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_menu_new_item(self, label, tip, cb, data)
    VALUE self, label, tip, cb, data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM_CONFIGURABLE));   /* type */
    rb_ary_push(ary, label);                /* label */
    rb_ary_push(ary, tip);              /* hint */
    rb_ary_push(ary, cb);             /* moreinfo */
    rb_ary_push(ary, data);                /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_NONE));   /* pixmap_type */
    rb_ary_push(ary, Qnil);             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

static VALUE
uiinfo_menu_new_subtree(self, tree)
    VALUE self, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE_STOCK));   /* type */
    rb_ary_push(ary, rb_str_new2(N_("_New")));                /* label */
    rb_ary_push(ary, Qnil);              /* hint */
    rb_ary_push(ary, tree);             /* moreinfo */
    rb_ary_push(ary, Qnil);                /* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_STOCK));   /* pixmap_type */
    rb_ary_push(ary, rb_str_new2(GTK_STOCK_NEW));             /* pixmap_info */
    rb_ary_push(ary, INT2FIX(GNOME_KEY_NAME_NEW));           /* accelerator_key */
    rb_ary_push(ary, INT2FIX(GNOME_KEY_MOD_NEW));           /* ac_mods */
    rb_ary_push(ary, Qnil);             /* widget */
    return ary;
}

#define DEF_UIINFO_CONF(func_name, type) \
static VALUE \
func_name(self, cb, data) \
    VALUE self, cb, data; \
{ \
    return rb_ary_new3(10, \
                       INT2FIX(GNOME_APP_UI_ITEM_CONFIGURABLE), /* type */ \
                       Qnil, /* label */ \
                       Qnil, /* hint */ \
                       (cb), /* moreinfo */ \
                       (data), /* user_data */ \
                       INT2FIX(GNOME_APP_PIXMAP_NONE), /* pixmap_type */ \
                       Qnil, /* pixmap_info */ \
                       INT2FIX(type), /* accelerator_key */ \
                       INT2FIX(0), /* ac_mods */ \
                       Qnil /* widget */ ); \
}

DEF_UIINFO_CONF(uiinfo_menu_open_item, GNOME_APP_CONFIGURABLE_ITEM_OPEN);
DEF_UIINFO_CONF(uiinfo_menu_save_item, GNOME_APP_CONFIGURABLE_ITEM_SAVE);
DEF_UIINFO_CONF(uiinfo_menu_save_as_item, GNOME_APP_CONFIGURABLE_ITEM_SAVE_AS);
DEF_UIINFO_CONF(uiinfo_menu_revert_item, GNOME_APP_CONFIGURABLE_ITEM_REVERT);
DEF_UIINFO_CONF(uiinfo_menu_print_item, GNOME_APP_CONFIGURABLE_ITEM_PRINT);
DEF_UIINFO_CONF(uiinfo_menu_print_setup_item, GNOME_APP_CONFIGURABLE_ITEM_PRINT_SETUP);
DEF_UIINFO_CONF(uiinfo_menu_close_item, GNOME_APP_CONFIGURABLE_ITEM_CLOSE);
DEF_UIINFO_CONF(uiinfo_menu_quit_item, GNOME_APP_CONFIGURABLE_ITEM_QUIT);
DEF_UIINFO_CONF(uiinfo_menu_cut_item, GNOME_APP_CONFIGURABLE_ITEM_CUT);
DEF_UIINFO_CONF(uiinfo_menu_copy_item, GNOME_APP_CONFIGURABLE_ITEM_COPY);
DEF_UIINFO_CONF(uiinfo_menu_paste_item, GNOME_APP_CONFIGURABLE_ITEM_PASTE);
DEF_UIINFO_CONF(uiinfo_menu_select_all_item, GNOME_APP_CONFIGURABLE_ITEM_SELECT_ALL);
DEF_UIINFO_CONF(uiinfo_menu_clear_item, GNOME_APP_CONFIGURABLE_ITEM_CLEAR);
DEF_UIINFO_CONF(uiinfo_menu_undo_item, GNOME_APP_CONFIGURABLE_ITEM_UNDO);
DEF_UIINFO_CONF(uiinfo_menu_redo_item, GNOME_APP_CONFIGURABLE_ITEM_REDO);
DEF_UIINFO_CONF(uiinfo_menu_find_item, GNOME_APP_CONFIGURABLE_ITEM_FIND);
DEF_UIINFO_CONF(uiinfo_menu_find_again_item, GNOME_APP_CONFIGURABLE_ITEM_FIND_AGAIN);
DEF_UIINFO_CONF(uiinfo_menu_replace_item, GNOME_APP_CONFIGURABLE_ITEM_REPLACE);
DEF_UIINFO_CONF(uiinfo_menu_properties_item, GNOME_APP_CONFIGURABLE_ITEM_PROPERTIES);
DEF_UIINFO_CONF(uiinfo_menu_preferences_item, GNOME_APP_CONFIGURABLE_ITEM_PREFERENCES);
DEF_UIINFO_CONF(uiinfo_menu_new_window_item, GNOME_APP_CONFIGURABLE_ITEM_NEW_WINDOW);
DEF_UIINFO_CONF(uiinfo_menu_close_window_item, GNOME_APP_CONFIGURABLE_ITEM_CLOSE_WINDOW);
DEF_UIINFO_CONF(uiinfo_menu_about_item, GNOME_APP_CONFIGURABLE_ITEM_ABOUT);
DEF_UIINFO_CONF(uiinfo_menu_new_game_item, GNOME_APP_CONFIGURABLE_ITEM_NEW_GAME);
DEF_UIINFO_CONF(uiinfo_menu_pause_game_item, GNOME_APP_CONFIGURABLE_ITEM_PAUSE_GAME);
DEF_UIINFO_CONF(uiinfo_menu_restart_game_item, GNOME_APP_CONFIGURABLE_ITEM_RESTART_GAME);
DEF_UIINFO_CONF(uiinfo_menu_undo_move_item, GNOME_APP_CONFIGURABLE_ITEM_UNDO_MOVE);
DEF_UIINFO_CONF(uiinfo_menu_redo_move_item, GNOME_APP_CONFIGURABLE_ITEM_REDO_MOVE);
DEF_UIINFO_CONF(uiinfo_menu_hint_item, GNOME_APP_CONFIGURABLE_ITEM_HINT);
DEF_UIINFO_CONF(uiinfo_menu_scores_item, GNOME_APP_CONFIGURABLE_ITEM_SCORES);
DEF_UIINFO_CONF(uiinfo_menu_end_game_item, GNOME_APP_CONFIGURABLE_ITEM_END_GAME);

#define DEF_UIINFO_TREE(func_name, label) \
static VALUE \
func_name(self, tree) \
    VALUE self, tree; \
{ \
    return rb_ary_new3(10, \
                       INT2FIX(GNOME_APP_UI_SUBTREE_STOCK), /* type */ \
                       rb_str_new2(N_(label)), /* label */ \
                       Qnil, /* hint */ \
                       (tree), /* moreinfo */ \
                       Qnil, /* user_data */ \
                       INT2FIX(0), /* pixmap_type */ \
                       Qnil, /* pixmap_info */ \
                       INT2FIX(0), /* accelerator_key */ \
                       INT2FIX(0), /* ac_mods */ \
                       Qnil /* widget */ ); \
}

DEF_UIINFO_TREE(uiinfo_menu_file_tree, "_File");
DEF_UIINFO_TREE(uiinfo_menu_edit_tree, "_Edit");
DEF_UIINFO_TREE(uiinfo_menu_view_tree, "_View");
DEF_UIINFO_TREE(uiinfo_menu_settings_tree, "_Settings");
DEF_UIINFO_TREE(uiinfo_menu_files_tree, "Fi_les");
DEF_UIINFO_TREE(uiinfo_menu_windows_tree, "_Windows");
DEF_UIINFO_TREE(uiinfo_menu_help_tree, "_Help");
DEF_UIINFO_TREE(uiinfo_menu_game_tree, "_Game");

static void
do_ui_signal_connect(uiinfo, signal_name, uibdata)
    GnomeUIInfo *uiinfo;
    gchar *signal_name;
    GnomeUIBuilderData *uibdata;
{
    VALUE proc = (VALUE)uiinfo->moreinfo;
    VALUE args;
    GClosure* rclosure;

    if (NIL_P(proc))
        return;
    args = rb_ary_new3(1, (VALUE)uiinfo->user_data);
    rclosure = g_rclosure_new(proc, args);
    g_signal_connect_closure(G_OBJECT(uiinfo->widget),
                             signal_name, rclosure, FALSE);
}

static GnomeUIBuilderData RbGnome_UIBuilder = {
    do_ui_signal_connect,
    0,
    0,
    0,
    0
};

enum UIInfoType {
    RBGNO_APP_MENUS,
    RBGNO_APP_TOOLBAR,
    RBGNO_APP_MENU_HINTS,
};

static void
fill_ui_info(uiinfo, ary, uitype)
    GnomeUIInfo *uiinfo;
    VALUE ary;
    enum UIInfoType uitype;
{
    GnomeUIInfoType type;
    GnomeUIInfo *sub;
    VALUE item, moreinfo, pixmap_info;
    char **xpm_data;
    int i, j;

    /* ary is T_ARRAY. */
    for (i = 0; i < RARRAY(ary)->len; i++) {
        item = RARRAY(ary)->ptr[i];

        /* set temporal terminator for free_ui_info. */
        uiinfo[i].type = GNOME_APP_UI_ENDOFINFO;
        uiinfo[i].moreinfo = NULL;
        uiinfo[i].pixmap_type = GNOME_APP_PIXMAP_NONE;
        uiinfo[i].pixmap_info = NULL;

        if (TYPE(item) != T_ARRAY)
            rb_raise(rb_eArgError,
                     "invalid data type for UIInfo entry (%s for Array)",
                     rb_class2name(CLASS_OF(item)));
        if (RARRAY(item)->len != 10)
            rb_raise(rb_eArgError,
                     "wrong array size for UIInfo entry (%d for 10)",
                     RARRAY(item)->len);;

        type = NUM2INT(RARRAY(item)->ptr[0]);
        uiinfo[i].label = NIL_P(RARRAY(item)->ptr[1])?0:RVAL2CSTR(RARRAY(item)->ptr[1]);
        uiinfo[i].hint = NIL_P(RARRAY(item)->ptr[2])?0:RVAL2CSTR(RARRAY(item)->ptr[2]);
        moreinfo = RARRAY(item)->ptr[3];
        uiinfo[i].user_data = (gpointer)RARRAY(item)->ptr[4];
        uiinfo[i].unused_data = NULL;
        uiinfo[i].pixmap_type = NUM2INT(RARRAY(item)->ptr[5]);
        pixmap_info = RARRAY(item)->ptr[6];
        uiinfo[i].accelerator_key = NUM2INT(RARRAY(item)->ptr[7]);
        uiinfo[i].ac_mods = NUM2INT(RARRAY(item)->ptr[8]);
        uiinfo[i].widget = NIL_P(RARRAY(item)->ptr[9])?0:GTK_WIDGET(RVAL2GOBJ((RARRAY(item)->ptr[9])));

        switch (uitype) {
          case RBGNO_APP_MENUS:
            break;
          case RBGNO_APP_TOOLBAR:
            switch (type) {
              case GNOME_APP_UI_INCLUDE:
              case GNOME_APP_UI_RADIOITEMS:
              case GNOME_APP_UI_SEPARATOR:
              case GNOME_APP_UI_ITEM:
              case GNOME_APP_UI_TOGGLEITEM:
                break;
              default:
                rb_raise(rb_eArgError, "Invalid type for toolbar");
            }
            break;
          case RBGNO_APP_MENU_HINTS:
            if (!uiinfo[i].widget)
                rb_raise(rb_eArgError, "empty widget found. Use the return value of Gnome::App#create_menus()");
            break;
        }


        if (NIL_P(uiinfo[i].user_data))
            uiinfo[i].user_data = NULL;


#define RAISE_TYPE_MISMATCH(pos, obj, expect) \
    rb_raise(rb_eArgError, \
             "wrong data type for %s (%s for %s)", \
             pos, rb_class2name(CLASS_OF(obj)), expect)

        /* set moreinfo */
        switch (type) {
          case GNOME_APP_UI_ENDOFINFO:
          case GNOME_APP_UI_SEPARATOR:
            /* expect nil */
            if (!NIL_P(moreinfo))
                RAISE_TYPE_MISMATCH("moreinfo", moreinfo, "nil");
            uiinfo[i].moreinfo = NULL;
            uiinfo[i].type = type;
            break;
          case GNOME_APP_UI_ITEM:
          case GNOME_APP_UI_TOGGLEITEM:
          case GNOME_APP_UI_ITEM_CONFIGURABLE:
            /* expect Proc */
            if (!rb_obj_is_kind_of(moreinfo, rb_cProc))
                RAISE_TYPE_MISMATCH("moreinfo", moreinfo, "Proc");
            uiinfo[i].moreinfo = (gpointer)moreinfo;
            uiinfo[i].type = type;
            break;
          case GNOME_APP_UI_RADIOITEMS:
          case GNOME_APP_UI_SUBTREE:
          case GNOME_APP_UI_SUBTREE_STOCK:
          case GNOME_APP_UI_INCLUDE:
            /* expect subarray */
            if (TYPE(moreinfo) != T_ARRAY)
                RAISE_TYPE_MISMATCH("moreinfo", moreinfo, "Array");
            sub = g_new(GnomeUIInfo, RARRAY(moreinfo)->len + 1);
            sub[0].type = GNOME_APP_UI_ENDOFINFO;
            uiinfo[i].moreinfo = sub;
            uiinfo[i].type = type;
            fill_ui_info(sub, moreinfo, uitype);
            break;
          case GNOME_APP_UI_HELP:
            /* expect string */
            if (TYPE(moreinfo) != T_STRING)
                RAISE_TYPE_MISMATCH("moreinfo", moreinfo, "String");
            uiinfo[i].moreinfo = RVAL2CSTR(moreinfo);
            uiinfo[i].type = type;
            break;
          case GNOME_APP_UI_BUILDER_DATA:
          default:
            rb_raise(rb_eArgError, "wrong type (%d for Gnome::App::UI_*)", type);
        }

        /* set pixmap_info */
        switch (uiinfo[i].pixmap_type) {
          case GNOME_APP_PIXMAP_NONE:
            if (!NIL_P(pixmap_info))
                rb_raise(rb_eArgError,
                         "wrong pixmap data for Gnome::App::PIXMAP_NONE (%s for nil)",
                         rb_class2name(CLASS_OF(pixmap_info)));
            break;
          case GNOME_APP_PIXMAP_STOCK:
            if (NIL_P(pixmap_info))
                break;
            if (TYPE(pixmap_info) != T_STRING)
                rb_raise(rb_eArgError,
                         "wrong pixmap data for Gnome::App::PIXMAP_STOCK (%s for String)",
                         rb_class2name(CLASS_OF(pixmap_info)));
            uiinfo[i].pixmap_info = RVAL2CSTR(pixmap_info);
            break;
          case GNOME_APP_PIXMAP_DATA:
            if (NIL_P(pixmap_info))
                break;
            if (TYPE(pixmap_info) != T_ARRAY)
                rb_raise(rb_eArgError,
                         "wrong pixmap data for Gnome::App::PIXMAP_DATA (%s for Array)",
                         rb_class2name(CLASS_OF(pixmap_info)));
            xpm_data = g_new(char *, RARRAY(pixmap_info)->len + 1);
            uiinfo[i].pixmap_info = xpm_data;
            for (j = 0; j < RARRAY(pixmap_info)->len; j++) {
                xpm_data[j] = RVAL2CSTR(RARRAY(pixmap_info)->ptr[j]);
            }
            xpm_data[j] = NULL;
            break;
          case GNOME_APP_PIXMAP_FILENAME:
            if (NIL_P(pixmap_info))
                break;
            if (TYPE(pixmap_info) != T_STRING)
                rb_raise(rb_eArgError,
                         "wrong pixmap data for Gnome::App::PIXMAP_FILENAME (%s for String)",
                         rb_class2name(CLASS_OF(pixmap_info)));
            uiinfo[i].pixmap_info = RVAL2CSTR(pixmap_info);
            break;
          default:
            rb_raise(rb_eArgError, "wrong pixmap type (%d for Gnome::App::PIXMAP_*)",
                     uiinfo[i].pixmap_type);
        }
    }
    uiinfo[i].type = GNOME_APP_UI_ENDOFINFO;
}

static void
free_ui_info(uiinfo)
    GnomeUIInfo *uiinfo;
{
    GnomeUIInfo *inf;
    for (inf = uiinfo; inf->type != GNOME_APP_UI_ENDOFINFO; inf++) {
        /* free moreinfo */
        switch (inf->type) {
          case GNOME_APP_UI_RADIOITEMS:
          case GNOME_APP_UI_SUBTREE:
          case GNOME_APP_UI_SUBTREE_STOCK:
          case GNOME_APP_UI_INCLUDE:
            free_ui_info(inf->moreinfo);
            break;
          default:
        }
        /* free pixmap_info */
        switch (inf->pixmap_type) {
          case GNOME_APP_PIXMAP_DATA:
            if (inf->pixmap_info)
                g_free((gpointer)inf->pixmap_info);
            break;
          default:
        }
    }
    g_free(uiinfo);
}

/*
 * Don't use returned value after 'ary' is GCed.
 * This is only for short time lifetime.
 */
static VALUE
ary_to_ui_info(ary, uitype)
    VALUE ary;
    enum UIInfoType uitype;
{
    GnomeUIInfo *uiinfo;
    VALUE obj;

    Check_Type(ary, T_ARRAY);
    uiinfo = g_new(GnomeUIInfo, RARRAY(ary)->len + 1);
    /* set terminator for free_ui_info. */
    uiinfo[0].type = GNOME_APP_UI_ENDOFINFO;

    obj = Data_Wrap_Struct(rb_cData, NULL, free_ui_info, uiinfo);
    /* uiinfo will be freed, even if any exception occurs in
       the following function.*/
    fill_ui_info(uiinfo, ary, uitype);
    return obj;
}

static VALUE
ui_info_to_ary(uiinfo)
    GnomeUIInfo *uiinfo;
{
    VALUE ary, item, xpm_data;
    char **xpm;

    ary = rb_ary_new();
	for (; uiinfo->type != GNOME_APP_UI_ENDOFINFO; uiinfo++) {
        item = rb_ary_new();
        rb_ary_push(item, INT2FIX(uiinfo->type)); /* 0 */
        rb_ary_push(item, uiinfo->label ? rb_str_new2(uiinfo->label) : Qnil); /* 1 */
        rb_ary_push(item, uiinfo->hint ? rb_str_new2(uiinfo->hint) : Qnil); /* 2 */
        switch (uiinfo->type) { /* 3 */
          case GNOME_APP_UI_ENDOFINFO:
          case GNOME_APP_UI_SEPARATOR:
            rb_ary_push(item, Qnil);
            break;
          case GNOME_APP_UI_ITEM:
          case GNOME_APP_UI_TOGGLEITEM:
          case GNOME_APP_UI_ITEM_CONFIGURABLE:
            rb_ary_push(item, (VALUE)uiinfo->moreinfo);
            break;
          case GNOME_APP_UI_RADIOITEMS:
          case GNOME_APP_UI_SUBTREE:
          case GNOME_APP_UI_SUBTREE_STOCK:
          case GNOME_APP_UI_INCLUDE:
            rb_ary_push(item, ui_info_to_ary(uiinfo->moreinfo));
            break;
          case GNOME_APP_UI_HELP:
            rb_ary_push(item, rb_str_new2(uiinfo->moreinfo));
            break;
          case GNOME_APP_UI_BUILDER_DATA:
            /* ignore this entry */
            continue;
        }
        rb_ary_push(item, (VALUE)uiinfo->user_data); /* 4 */
        rb_ary_push(item, INT2FIX(uiinfo->pixmap_type)); /* 5 */
        switch (uiinfo->pixmap_type) { /* 6 */
          case GNOME_APP_PIXMAP_NONE:
            rb_ary_push(item, Qnil);
            break;
          case GNOME_APP_PIXMAP_STOCK:
          case GNOME_APP_PIXMAP_FILENAME:
            rb_ary_push(item, rb_str_new2(uiinfo->pixmap_info));
            break;
          case GNOME_APP_PIXMAP_DATA:
            if (uiinfo->pixmap_info == NULL) {
                xpm_data = Qnil;
            } else {
                xpm_data = rb_ary_new();
                for (xpm = (char **)uiinfo->pixmap_info; *xpm != NULL; xpm++) {
                    rb_ary_push(xpm_data, rb_str_new2(*xpm));
                }
                rb_ary_push(item, xpm_data);
            }
            break;
        }
        rb_ary_push(item, INT2FIX(uiinfo->accelerator_key)); /* 7 */
        rb_ary_push(item, INT2FIX(uiinfo->ac_mods)); /* 8 */
        rb_ary_push(item, uiinfo->widget ? GOBJ2RVAL(uiinfo->widget) : Qnil); /* 9 */
        rb_ary_push(ary, item);
    }
    return ary;
}

static VALUE
app_create_menus(self, menuinfo)
    VALUE self, menuinfo;
{
    VALUE uiinfo = ary_to_ui_info(menuinfo, RBGNO_APP_MENUS);

    gnome_app_create_menus_custom(GNOME_APP(RVAL2GOBJ(self)),
                                  DATA_PTR(uiinfo),
                                  &RbGnome_UIBuilder);
    return ui_info_to_ary(DATA_PTR(uiinfo));
}

static VALUE
app_create_toolbar(self, menuinfo)
    VALUE self, menuinfo;
{
    VALUE uiinfo = ary_to_ui_info(menuinfo, RBGNO_APP_TOOLBAR);

    gnome_app_create_toolbar_custom(_SELF(self),
                                    DATA_PTR(uiinfo),
                                    &RbGnome_UIBuilder);
    return ui_info_to_ary(DATA_PTR(uiinfo));
}

static VALUE
app_remove_menus(self, path, items)
    VALUE self, path, items;
{
    gnome_app_remove_menus(_SELF(self),
                           RVAL2CSTR(path),
                           NUM2INT(items));
    return self;
}

static VALUE
app_remove_menu_range(self, path, start, items)
    VALUE self, path, start, items;
{
    gnome_app_remove_menu_range(_SELF(self),
                                RVAL2CSTR(path),
                                NUM2INT(start),
                                NUM2INT(items));
    return self;
}

static VALUE
app_insert_menus(self, path, menuinfo)
    VALUE self, path, menuinfo;
{
    VALUE uiinfo = ary_to_ui_info(menuinfo, RBGNO_APP_MENUS);

    gnome_app_insert_menus_custom(_SELF(self),
                                  RVAL2CSTR(path),
                                  DATA_PTR(uiinfo),
                                  &RbGnome_UIBuilder);
    return ui_info_to_ary(DATA_PTR(uiinfo));
}

static VALUE
app_install_appbar_menu_hints(self, menuinfo)
    VALUE self, menuinfo;
{
    VALUE uiinfo = ary_to_ui_info(menuinfo, RBGNO_APP_MENU_HINTS);

    gnome_app_install_appbar_menu_hints(GNOME_APPBAR(RVAL2GOBJ(self)), DATA_PTR(uiinfo));
    return self;
}

static VALUE
app_install_statusbar_menu_hints(self, menuinfo)
    VALUE self, menuinfo;
{
    VALUE uiinfo = ary_to_ui_info(menuinfo, RBGNO_APP_MENU_HINTS);

    gnome_app_install_statusbar_menu_hints(GTK_STATUSBAR(RVAL2GOBJ(self)), DATA_PTR(uiinfo));
    return self;
}

void
Init_gnome_app_helper(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeUIInfo = rb_define_module_under(mGnome, "UIInfo");
    VALUE gnoApp = GTYPE2CLASS(GNOME_TYPE_APP);
    VALUE gnoAppBar = GTYPE2CLASS(GNOME_TYPE_APPBAR);
    VALUE gtkStatusBar = GTYPE2CLASS(GTK_TYPE_STATUSBAR);

    /* GnomeUIInfoType */
    rb_define_const(gnoApp, "UI_ENDOFINFO", INT2FIX(GNOME_APP_UI_ENDOFINFO));
    rb_define_const(gnoApp, "UI_ITEM", INT2FIX(GNOME_APP_UI_ITEM));
    rb_define_const(gnoApp, "UI_TOGGLEITEM", INT2FIX(GNOME_APP_UI_TOGGLEITEM));
    rb_define_const(gnoApp, "UI_RADIOITEMS", INT2FIX(GNOME_APP_UI_RADIOITEMS));
    rb_define_const(gnoApp, "UI_SUBTREE", INT2FIX(GNOME_APP_UI_SUBTREE));
    rb_define_const(gnoApp, "UI_SEPARATOR", INT2FIX(GNOME_APP_UI_SEPARATOR));
    rb_define_const(gnoApp, "UI_HELP", INT2FIX(GNOME_APP_UI_HELP));
#if 0 /* Don't use from ruby */
    rb_define_const(gnoApp, "UI_BUILDER_DATA", INT2FIX(GNOME_APP_UI_BUILDER_DATA));
#endif
    rb_define_const(gnoApp, "UI_ITEM_CONFIGURABLE", INT2FIX(GNOME_APP_UI_ITEM_CONFIGURABLE));
    rb_define_const(gnoApp, "UI_SUBTREE_STOCK", INT2FIX(GNOME_APP_UI_SUBTREE_STOCK));
    rb_define_const(gnoApp, "UI_INCLUDE", INT2FIX(GNOME_APP_UI_INCLUDE));

    /* GnomeUIInfoConfigurableTypes */
    /* 0 */
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_OPEN", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_OPEN));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SAVE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SAVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SAVE_AS", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SAVE_AS));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REVERT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REVERT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PRINT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PRINT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PRINT_SETUP", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PRINT_SETUP));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLOSE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLOSE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_QUIT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_QUIT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CUT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CUT));
    /* 10 */
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_COPY", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_COPY));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PASTE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PASTE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLEAR", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLEAR));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_UNDO", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_UNDO));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REDO", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REDO));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_FIND", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_FIND));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_FIND_AGAIN", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_FIND_AGAIN));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REPLACE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REPLACE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PROPERTIES", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PROPERTIES));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PREFERENCES", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PREFERENCES));
    /* 20 */
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_ABOUT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_ABOUT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SELECT_ALL", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SELECT_ALL));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW_WINDOW", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW_WINDOW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLOSE_WINDOW", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLOSE_WINDOW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW_GAME", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PAUSE_GAME", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PAUSE_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_RESTART_GAME", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_RESTART_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_UNDO_MOVE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_UNDO_MOVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REDO_MOVE", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REDO_MOVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_HINT", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_HINT));
    /* 30 */
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SCORES", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SCORES));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_END_GAME", INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_END_GAME));

    /* GnomeUIPixmapType */
    rb_define_const(gnoApp, "PIXMAP_NONE", INT2FIX(GNOME_APP_PIXMAP_NONE));
    rb_define_const(gnoApp, "PIXMAP_STOCK", INT2FIX(GNOME_APP_PIXMAP_STOCK));
    rb_define_const(gnoApp, "PIXMAP_DATA", INT2FIX(GNOME_APP_PIXMAP_DATA));
    rb_define_const(gnoApp, "PIXMAP_FILENAME", INT2FIX(GNOME_APP_PIXMAP_FILENAME));

    /* */
    rb_define_module_function(mGnomeUIInfo, "separator", uiinfo_separator, 0);
    rb_define_module_function(mGnomeUIInfo, "item", uiinfo_item, 4);
    rb_define_module_function(mGnomeUIInfo, "item_stock", uiinfo_item_stock, 4);
    rb_define_module_function(mGnomeUIInfo, "item_none", uiinfo_item_none, 3);
    rb_define_module_function(mGnomeUIInfo, "item_data", uiinfo_item_data, 5);
    rb_define_module_function(mGnomeUIInfo, "toggleitem", uiinfo_toggleitem, 4);
    rb_define_module_function(mGnomeUIInfo, "toggleitem_data", uiinfo_toggleitem_data, 5);
    rb_define_module_function(mGnomeUIInfo, "help", uiinfo_help, 1);
    rb_define_module_function(mGnomeUIInfo, "subtree", uiinfo_subtree, 2);
    rb_define_module_function(mGnomeUIInfo, "subtree_hint", uiinfo_subtree_hint, 3);
    rb_define_module_function(mGnomeUIInfo, "subtree_stock", uiinfo_subtree_stock, 3);
    rb_define_module_function(mGnomeUIInfo, "include", uiinfo_include, 1);
    rb_define_module_function(mGnomeUIInfo, "radiolist", uiinfo_radiolist, 1);
    rb_define_module_function(mGnomeUIInfo, "radioitem", uiinfo_radioitem, 4);
    rb_define_module_function(mGnomeUIInfo, "radioitem_data", uiinfo_radioitem_data, 5);

    /* The 'File' menu */
    rb_define_module_function(mGnomeUIInfo, "menu_new_item", uiinfo_menu_new_item, 4);
    rb_define_module_function(mGnomeUIInfo, "menu_new_subtree", uiinfo_menu_new_subtree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_open_item", uiinfo_menu_open_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_save_item", uiinfo_menu_save_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_save_as_item", uiinfo_menu_save_as_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_revert_item", uiinfo_menu_revert_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_print_item", uiinfo_menu_print_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_print_setup_item", uiinfo_menu_print_setup_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_close_item", uiinfo_menu_close_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_quit_item", uiinfo_menu_quit_item, 2);
    /* The "Edit" menu */
    rb_define_module_function(mGnomeUIInfo, "menu_cut_item", uiinfo_menu_cut_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_copy_item", uiinfo_menu_copy_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_paste_item", uiinfo_menu_paste_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_select_all_item", uiinfo_menu_select_all_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_clear_item", uiinfo_menu_clear_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_undo_item", uiinfo_menu_undo_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_redo_item", uiinfo_menu_redo_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_find_item", uiinfo_menu_find_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_find_again_item", uiinfo_menu_find_again_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_replace_item", uiinfo_menu_replace_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_properties_item", uiinfo_menu_properties_item, 2);
    /* The Settings menu */
    rb_define_module_function(mGnomeUIInfo, "menu_preferences_item", uiinfo_menu_preferences_item, 2);
    /* The Windows menu */
    rb_define_module_function(mGnomeUIInfo, "menu_new_window_item", uiinfo_menu_new_window_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_close_window_item", uiinfo_menu_close_window_item, 2);
    /* The "Help" menu */
    rb_define_module_function(mGnomeUIInfo, "menu_about_item", uiinfo_menu_about_item, 2);
    /* The "Game" menu */
    rb_define_module_function(mGnomeUIInfo, "menu_new_game_item", uiinfo_menu_new_game_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_pause_game_item", uiinfo_menu_pause_game_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_restart_game_item", uiinfo_menu_restart_game_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_undo_move_item", uiinfo_menu_undo_move_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_redo_move_item", uiinfo_menu_redo_move_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_hint_item", uiinfo_menu_hint_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_scores_item", uiinfo_menu_scores_item, 2);
    rb_define_module_function(mGnomeUIInfo, "menu_end_game_item", uiinfo_menu_end_game_item, 2);

    rb_define_module_function(mGnomeUIInfo, "menu_file_tree", uiinfo_menu_file_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_edit_tree", uiinfo_menu_edit_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_view_tree", uiinfo_menu_view_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_settings_tree", uiinfo_menu_settings_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_files_tree", uiinfo_menu_files_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_windows_tree", uiinfo_menu_windows_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_help_tree", uiinfo_menu_help_tree, 1);
    rb_define_module_function(mGnomeUIInfo, "menu_game_tree", uiinfo_menu_game_tree, 1);

    /*
     * instance methods
     */
    rb_define_method(gnoApp, "create_menus", app_create_menus, 1);
/*     rb_define_method(gtkToolBar, "fill_toolbar", app_fill_toolbar, 1); */
    rb_define_method(gnoApp, "create_toolbar", app_create_toolbar, 1);
/*     rb_define_method(gnoApp, "find_menu_pos", app_find_menu_pos, ?); */
    rb_define_method(gnoApp, "remove_menus", app_remove_menus, 2);
    rb_define_method(gnoApp, "remove_menu_range", app_remove_menu_range, 3);
    rb_define_method(gnoApp, "insert_menus", app_insert_menus, 2);

    rb_define_method(gnoAppBar, "install_menu_hints", app_install_appbar_menu_hints, 1);
    rb_define_method(gtkStatusBar, "install_menu_hints", app_install_statusbar_menu_hints, 1);
}
