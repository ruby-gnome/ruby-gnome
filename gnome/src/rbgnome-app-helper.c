/* $Id: rbgnome-app-helper.c,v 1.3 2002/08/04 14:03:59 mutoh Exp $ */

/* Gnome::UIInfo module for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
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

VALUE mGnomeUIInfo;

#if 0
static VALUE
uiinfo_end(self)
    VALUE self;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ENDOFINFO));	/* type */
    rb_ary_push(ary, Qnil);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, Qnil);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}
#endif

static VALUE
uiinfo_separator(self)
    VALUE self;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SEPARATOR));	/* type */
    rb_ary_push(ary, Qnil);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, Qnil);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_item(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_item_stock(self, label, tooltip, callback, stock_id)
    VALUE self, label, tooltip, callback, stock_id;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_STOCK));	/* pixmap_type */
    rb_ary_push(ary, stock_id);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_item_none(self, label, tooltip, callback)
    VALUE self, label, tooltip, callback;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_NONE));	/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_item_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, user_data);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_toggleitem(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_TOGGLEITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_toggleitem_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_TOGGLEITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, user_data);			/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_help(self, app_name)
    VALUE self, app_name;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_HELP));	/* type */
    rb_ary_push(ary, Qnil);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, app_name);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_subtree(self, label, tree)
    VALUE self, label, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, tree);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_subtree_hint(self, label, hint, tree)
    VALUE self, label, hint, tree;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, hint);				/* hint */
    rb_ary_push(ary, tree);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_subtree_stock(self, label, tree, stock_id)
    VALUE self, label, tree, stock_id;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_SUBTREE));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, tree);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_STOCK));	/* pixmap_type */
    rb_ary_push(ary, stock_id);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_radiolist(self, list)
    VALUE self, list;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_RADIOITEMS));	/* type */
    rb_ary_push(ary, Qnil);				/* label */
    rb_ary_push(ary, Qnil);				/* hint */
    rb_ary_push(ary, list);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(0));			/* pixmap_type */
    rb_ary_push(ary, Qnil);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_radioitem(self, label, tooltip, callback, xpm_data)
    VALUE self, label, tooltip, callback, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, Qnil);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static VALUE
uiinfo_radioitem_data(self, label, tooltip, callback, user_data, xpm_data)
    VALUE self, label, tooltip, callback, user_data, xpm_data;
{
    VALUE ary;
    ary = rb_ary_new2(10);
    rb_ary_push(ary, INT2FIX(GNOME_APP_UI_ITEM));	/* type */
    rb_ary_push(ary, label);				/* label */
    rb_ary_push(ary, tooltip);				/* hint */
    rb_ary_push(ary, callback);				/* moreinfo */
    rb_ary_push(ary, user_data);				/* user_data */
    rb_ary_push(ary, INT2FIX(GNOME_APP_PIXMAP_DATA));	/* pixmap_type */
    rb_ary_push(ary, xpm_data);				/* pixmap_info */
    rb_ary_push(ary, INT2FIX(0));			/* accelerator_key */
    rb_ary_push(ary, INT2FIX(0));			/* ac_mods */
    rb_ary_push(ary, Qnil);				/* widget */
    return ary;
}

static void
do_ui_signal_connect(uiinfo, signal_name, uibdata)
    GnomeUIInfo *uiinfo;
    gchar *signal_name;
    GnomeUIBuilderData *uibdata;
{
    VALUE args, data;
    int id;

    args = rb_ary_new3(1, (VALUE)uiinfo->user_data);
    id = rb_intern(signal_name);
    data = rb_ary_new3(3, (VALUE)uiinfo->moreinfo, INT2NUM(id), args);
    add_relative((VALUE)uiinfo->moreinfo, data);
    add_relative((VALUE)uibdata->data, (VALUE)uiinfo->moreinfo);
    gtk_signal_connect_full(GTK_OBJECT(uiinfo->widget),
			    signal_name, 0,
			    signal_callback,
			    (gpointer)data,
			    0, FALSE, 0);
}

static const GnomeUIBuilderData RbGnome_UIBuilder = {
  do_ui_signal_connect,
  0,               /* these are basically ignored when */
  0,
  0,
  0
};

static void
set_uibdata(uibdata, self)
    GnomeUIBuilderData *uibdata;
    VALUE self;
{
    memcpy(uibdata, &RbGnome_UIBuilder, sizeof(GnomeUIBuilderData));
    uibdata->data = (gpointer)self;
}

static GnomeUIInfo *
ary_to_ui_info(ary, inc_uibd, uibdata)
   VALUE ary;
   gboolean inc_uibd;
   GnomeUIBuilderData *uibdata;
{
    GnomeUIInfo *ret, *inf, *sub;
    VALUE item, moreinfo;
    int len, i;
    int type, pix_type;

    Check_Type(ary, T_ARRAY);
    len = RARRAY(ary)->len;

    if (inc_uibd) {
	inf = g_new(GnomeUIInfo, len + 2);
	inf[0].type = GNOME_APP_UI_BUILDER_DATA;
	inf[0].label = inf[0].hint = NULL;
	inf[0].moreinfo = &RbGnome_UIBuilder;
	ret = &inf[1];
    } else {
	ret = inf = g_new(GnomeUIInfo, len + 1);
    }
    ret[len].type = GNOME_APP_UI_ENDOFINFO; /* set terminator */

    for (i = 0; i < len; i++) {
	item = RARRAY(ary)->ptr[i];
	if (TYPE(item) != T_ARRAY || RARRAY(item)->len < 10) {
	    g_free(inf);
	    return 0;
	}
	
	ret[i].type = NUM2INT(RARRAY(item)->ptr[0]);
	ret[i].label = NIL_P(RARRAY(item)->ptr[1])?
	    0:STR2CSTR(RARRAY(item)->ptr[1]);
	ret[i].hint = NIL_P(RARRAY(item)->ptr[2])?
	    0:STR2CSTR(RARRAY(item)->ptr[2]);
	moreinfo = RARRAY(item)->ptr[3];
	ret[i].user_data = (gpointer)RARRAY(item)->ptr[4];
	ret[i].unused_data = 0;
	ret[i].pixmap_type = NUM2INT(RARRAY(item)->ptr[5]);
	ret[i].pixmap_info = NIL_P(RARRAY(item)->ptr[6])?
	    0:STR2CSTR(RARRAY(item)->ptr[6]);
	ret[i].accelerator_key = NUM2INT(RARRAY(item)->ptr[7]);
	ret[i].ac_mods = NUM2INT(RARRAY(item)->ptr[8]);
	ret[i].widget = NIL_P(RARRAY(item)->ptr[9])?
	    0:get_widget(RARRAY(ary)->ptr[9]);

	if ((VALUE)ret[i].user_data == Qnil) {
	    ret[i].user_data = 0;
	}

	type = ret[i].type;
	pix_type = ret[i].pixmap_type;
	if (type < GNOME_APP_UI_ENDOFINFO
	    || type == GNOME_APP_UI_BUILDER_DATA
	    || type > GNOME_APP_UI_SUBTREE_STOCK
	    || pix_type < GNOME_APP_PIXMAP_NONE
	    || pix_type > GNOME_APP_PIXMAP_FILENAME
	    || pix_type == GNOME_APP_PIXMAP_DATA) {
	    g_free(inf);
	    rb_raise(rb_eArgError,
		     "bad info or pixmap type for label \"%s\"\n",
		     STR2CSTR(RARRAY(item)->ptr[1]));
	}

	switch(type) {
	case GNOME_APP_UI_ENDOFINFO:
	case GNOME_APP_UI_SEPARATOR:
	    ret[i].moreinfo = 0;
	    break;
	case GNOME_APP_UI_ITEM:
	case GNOME_APP_UI_TOGGLEITEM:
	    if (NIL_P(moreinfo)) {
		ret[i].moreinfo = 0;
	    }
	    ret[i].moreinfo = (gpointer)moreinfo;
	    break;
	case GNOME_APP_UI_RADIOITEMS:
	    sub = ary_to_ui_info(moreinfo, FALSE, uibdata);
	    if (!sub) {
		g_free(inf);
		return 0;
	    }
	    ret[i].moreinfo = sub;
	    break;
	case GNOME_APP_UI_SUBTREE:
	case GNOME_APP_UI_SUBTREE_STOCK:
	    sub = ary_to_ui_info(moreinfo, FALSE, uibdata);
	    if (!sub) {
		g_free(inf);
		return 0;
	    }
	    ret[i].moreinfo = sub;
	    break;
	case GNOME_APP_UI_HELP:
	    if (!NIL_P(moreinfo) && TYPE(moreinfo) != T_STRING) {
		rb_raise(rb_eArgError,
			 "additional info should be a string for help");
		g_free(inf);
		return 0;
	    }
	    ret[i].moreinfo = (NIL_P(moreinfo) ? 0 : STR2CSTR(moreinfo));
            break;
	case GNOME_APP_UI_ITEM_CONFIGURABLE:
            ret[i].moreinfo = (NIL_P(moreinfo) ? 0 : (gpointer)moreinfo);
            break;
	}
    }
    return inf;
}

static void
free_ui_info(inf)
    GnomeUIInfo *inf;
{
    int i = 0;
  
    while (inf[i].type != GNOME_APP_UI_ENDOFINFO) {
	if (inf[i].type == GNOME_APP_UI_RADIOITEMS
	    || inf[i].type == GNOME_APP_UI_SUBTREE)
	    if (inf[i].moreinfo !=  NULL)
		free_ui_info(inf[i].moreinfo);
	i++;
    }
    g_free(inf);
}

static VALUE
app_create_menus(self, menuinfo)
    VALUE self, menuinfo;
{
    GnomeUIInfo *uiinfo;
    GnomeUIBuilderData uibdata;
    uiinfo = ary_to_ui_info(menuinfo, FALSE, &uibdata);
    if (!uiinfo) {
	return Qnil;
    }

    set_uibdata(&uibdata, self);
    gnome_app_create_menus_custom(GNOME_APP(get_widget(self)),
				  uiinfo,
				  &uibdata);
    free_ui_info(uiinfo);

    return self;
}

static VALUE
app_create_toolbar(self, menuinfo)
    VALUE self, menuinfo;
{
    GnomeUIInfo *uiinfo;
    GnomeUIBuilderData uibdata;
    uiinfo = ary_to_ui_info(menuinfo, FALSE, &uibdata);
    if (!uiinfo) {
	return Qnil;
    }

    set_uibdata(&uibdata, self);
    gnome_app_create_toolbar_custom(GNOME_APP(get_widget(self)),
				    uiinfo,
				    &uibdata);
    free_ui_info(uiinfo);

    return self;
}

static VALUE
app_remove_menus(self, path, items)
    VALUE self, path, items;
{
    gnome_app_remove_menus(GNOME_APP(get_widget(self)),
			   STR2CSTR(path),
			   NUM2INT(items));
    return self;
}

static VALUE
app_remove_menu_range(self, path, start, items)
    VALUE self, path, start, items;
{
    gnome_app_remove_menu_range(GNOME_APP(get_widget(self)),
				STR2CSTR(path),
				NUM2INT(start),
				NUM2INT(items));
    return self;
}

static VALUE
app_insert_menus(self, path, menuinfo)
    VALUE self, path, menuinfo;
{
    GnomeUIInfo *uiinfo;
    GnomeUIBuilderData uibdata;
    uiinfo = ary_to_ui_info(menuinfo, FALSE, &uibdata);
    if (!uiinfo) {
	return Qnil;
    }

    set_uibdata(&uibdata, self);
    gnome_app_insert_menus_custom(GNOME_APP(get_widget(self)),
				  STR2CSTR(path),
				  uiinfo,
				  &uibdata);
    free_ui_info(uiinfo);

    return self;
}

void
Init_gnome_app_helper()
{
    mGnomeUIInfo = rb_define_module_under(mGnome, "UIInfo");

    /* GnomeUIInfoType */
    rb_define_const(gnoApp, "UI_ENDOFINFO", INT2FIX(GNOME_APP_UI_ENDOFINFO));
    rb_define_const(gnoApp, "UI_ITEM", INT2FIX(GNOME_APP_UI_ITEM));
    rb_define_const(gnoApp, "UI_TOGGLEITEM", INT2FIX(GNOME_APP_UI_TOGGLEITEM));
    rb_define_const(gnoApp, "UI_RADIOITEMS", INT2FIX(GNOME_APP_UI_RADIOITEMS));
    rb_define_const(gnoApp, "UI_SUBTREE", INT2FIX(GNOME_APP_UI_SUBTREE));
    rb_define_const(gnoApp, "UI_SEPARATOR", INT2FIX(GNOME_APP_UI_SEPARATOR));
    rb_define_const(gnoApp, "UI_HELP", INT2FIX(GNOME_APP_UI_HELP));
    rb_define_const(gnoApp, "UI_BUILDER_DATA",
		    INT2FIX(GNOME_APP_UI_BUILDER_DATA));
    rb_define_const(gnoApp, "UI_ITEM_CONFIGURABLE",
		    INT2FIX(GNOME_APP_UI_ITEM_CONFIGURABLE));
    rb_define_const(gnoApp, "GNOME_APP_UI_SUBTREE_STOCK",
		    INT2FIX(GNOME_APP_UI_SUBTREE_STOCK));

    /* GnomeUIInfoConfigurableTypes */
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_OPEN",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_OPEN));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SAVE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SAVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SAVE_AS",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SAVE_AS));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REVERT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REVERT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PRINT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PRINT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PRINT_SETUP",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PRINT_SETUP));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLOSE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLOSE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_EXIT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_EXIT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CUT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CUT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_COPY",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_COPY));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PASTE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PASTE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLEAR",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLEAR));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_UNDO",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_UNDO));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REDO",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REDO));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_FIND",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_FIND));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_FIND_AGAIN",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_FIND_AGAIN));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REPLACE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REPLACE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PROPERTIES",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PROPERTIES));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PREFERENCES",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PREFERENCES));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_ABOUT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_ABOUT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SELECT_ALL",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SELECT_ALL));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW_WINDOW",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW_WINDOW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_CLOSE_WINDOW",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_CLOSE_WINDOW));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_NEW_GAME",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_NEW_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_PAUSE_GAME",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_PAUSE_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_RESTART_GAME",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_RESTART_GAME));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_UNDO_MOVE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_UNDO_MOVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_REDO_MOVE",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_REDO_MOVE));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_HINT",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_HINT));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_SCORES",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_SCORES));
    rb_define_const(gnoApp, "CONFIGURABLE_ITEM_END_GAME",
		    INT2FIX(GNOME_APP_CONFIGURABLE_ITEM_END_GAME));

    /* GnomeUIPixmapType */
    rb_define_const(gnoApp, "PIXMAP_NONE", INT2FIX(GNOME_APP_PIXMAP_NONE));
    rb_define_const(gnoApp, "PIXMAP_STOCK", INT2FIX(GNOME_APP_PIXMAP_STOCK));
    rb_define_const(gnoApp, "PIXMAP_DATA", INT2FIX(GNOME_APP_PIXMAP_DATA));
    rb_define_const(gnoApp, "PIXMAP_FILENAME",
		    INT2FIX(GNOME_APP_PIXMAP_FILENAME));

    /* */
#if 0
    rb_define_module_function(mGnomeUIInfo, "end", uiinfo_end, 0);
#endif
    rb_define_module_function(mGnomeUIInfo, "separator",
			      uiinfo_separator, 0);
    rb_define_module_function(mGnomeUIInfo, "item",
			      uiinfo_item, 4);
    rb_define_module_function(mGnomeUIInfo, "item_stock",
			      uiinfo_item_stock, 4);
    rb_define_module_function(mGnomeUIInfo, "item_none",
			      uiinfo_item_none, 3);
    rb_define_module_function(mGnomeUIInfo, "item_data",
			      uiinfo_item_data, 5);
    rb_define_module_function(mGnomeUIInfo, "toggleitem",
			      uiinfo_toggleitem, 4);
    rb_define_module_function(mGnomeUIInfo, "toggleitem_data",
			      uiinfo_toggleitem_data, 5);
    rb_define_module_function(mGnomeUIInfo, "help",
			      uiinfo_help, 1);
    rb_define_module_function(mGnomeUIInfo, "subtree",
			      uiinfo_subtree, 2);
    rb_define_module_function(mGnomeUIInfo, "subtree_hint",
			      uiinfo_subtree_hint, 3);
    rb_define_module_function(mGnomeUIInfo, "subtree_stock",
			      uiinfo_subtree_stock, 3);
    rb_define_module_function(mGnomeUIInfo, "radiolist",
			      uiinfo_radiolist, 1);
    rb_define_module_function(mGnomeUIInfo, "radioitem",
			      uiinfo_radioitem, 4);
    rb_define_module_function(mGnomeUIInfo, "radioitem_data",
			      uiinfo_radioitem_data, 5);

    /*
     * instance methods
     */
    rb_define_method(gnoApp, "create_menus", app_create_menus, 1);
    rb_define_method(gnoApp, "create_toolbar", app_create_toolbar, 1);
    rb_define_method(gnoApp, "remove_menus", app_remove_menus, 2);
    rb_define_method(gnoApp, "remove_menu_range", app_remove_menu_range, 3);
    rb_define_method(gnoApp, "insert_menus", app_insert_menus, 2);
}
