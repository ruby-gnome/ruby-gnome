/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-app.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::App widget for Ruby/Gnome
 * Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
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

/*
 * Class Gnome::App
 *
 *   The toplevel GNOME container.
 *
 *   Toplevel GNOME applications would normally use one GnomeApp widget
 *   as their toplevel window. You can create as many GnomeApp widgets
 *   as you want, for example, some people use one GnomeApp per document
 *   their application loads.
 *
 *   Once you have created one instance of this widget, you would add
 *   your main application view information to this window by using
 *   set_contents() method.
 *
 *   The GnomeApp has support for including a menubar one or more
 *   toolbars and a statusbar for your application, it also takes
 *   care of intalling the accelerators for you when used in conjuction
 *   with the gnome-app-helper routines. The toolbars are inserted into
 *   GnomeDock widgets.
 *
 *   The gnome-app-helper module provides various helper routines
 *   to simplify the configuration of your menus and toolbars, but you
 *   can create those yourself and use the set_menus(), add_toolbar(),
 *   set_toolbar(), add_dock_item(), and add_docked().
 */

/*
 * Gnome::App#initialize(appname, title)
 *
 * Initialize a new (empty) application window. You must specify the
 * application's name (used internally as an identifier). title can
 * be left as NULL, in which case the window's title will not be set.
 *
 * arguments:
 *   appname - Name of program, using in file names and paths.
 *   title - Window title for application.
 */
static VALUE
app_initialize(self, appname, title)
    VALUE self, appname, title;
{
    RBGTK_INITIALIZE(self, gnome_app_new(RVAL2CSTR(appname), RVAL2CSTR(title)));
    return Qnil;
}

/*
 * Gnome::App#set_menus(menubar)
 *
 * Sets the menu bar of the application window.
 *
 * argument:
 *   menubar - Menu bar widget for main app window.
 */
static VALUE
app_set_menus(self, menubar)
    VALUE self, menubar;
{
    gnome_app_set_menus(_SELF(self),
                        GTK_MENU_BAR(RVAL2GOBJ(menubar)));
    return self;
}

/*
 * Gnome::App#set_toolbar(toolbar)
 *
 * Sets the main toolbar of the application window.
 *
 * argument:
 *   toolbar - Toolbar widget for main app window.
 */
static VALUE
app_set_toolbar(self, toolbar)
    VALUE self, toolbar;
{
    gnome_app_set_toolbar(_SELF(self),
                          GTK_TOOLBAR(RVAL2GOBJ(toolbar)));
    return self;
}

/*
 * Gnome::App#set_statusbar(statusbar)
 *
 * Sets the status bar of the application window.
 *
 * argument:
 *   statusbar - Statusbar widget for main app window
 */
static VALUE
app_set_statusbar(self, statusbar)
    VALUE self, statusbar;
{
    gnome_app_set_statusbar(_SELF(self),
                            GTK_WIDGET(RVAL2GOBJ(statusbar)));
    return self;
}

/*
 * Gnome::App#set_statusbar_custom(container, statusbar)
 *
 * Sets the status bar of the application window, but uses the given
 * container widget rather than creating a new one.
 */
static VALUE
app_set_statusbar_custom(self, container, statusbar)
    VALUE self, container, statusbar;
{
    gnome_app_set_statusbar_custom(_SELF(self),
                                   GTK_WIDGET(RVAL2GOBJ(container)),
                                   GTK_WIDGET(RVAL2GOBJ(statusbar)));
    return self;
}

/*
 * Gnome::App#set_contents(contents)
 *
 * Sets the content area of the GNOME app's main window.
 *
 * argument:
 *   contents - Widget to be application content area.
 */
static VALUE
app_set_contents(self, contents)
    VALUE self, contents;
{
    gnome_app_set_contents(_SELF(self),
                           GTK_WIDGET(RVAL2GOBJ(contents)));
    return self;
}

/*
 * Gnome::App#add_toolbar(toolbar, name, behavior, placement,
 *                        band_num, band_position, offset)
 *
 * Create a new &GnomeDockItem widget containing toolbar, and add it
 * to app's dock with the specified layout information. Notice that,
 * if automatic layout configuration is enabled, the layout is
 * overridden by the saved configuration, if any.
 *
 * arguments:
 *   toolbar - Toolbar to be added to app's dock
 *   name - Name for the dock item that will contain toolbar
 *   behavior - Behavior for the new dock item
 *   placement - Placement for the new dock item
 *   band_num - Number of the band where the dock item should be placed
 *   band_position - Position of the new dock item in band band_num
 *   offset - Offset from the previous dock item in the band;
 *            if there is no previous item, offset from the beginning
 *            of the band.
 */
static VALUE
app_add_toolbar(self, toolbar, name, behavior, placement,
                band_num, band_position, offset)
    VALUE self, toolbar, name, behavior, placement;
    VALUE band_num, band_position, offset;
{
    gnome_app_add_toolbar(_SELF(self),
                          GTK_TOOLBAR(RVAL2GOBJ(toolbar)),
                          RVAL2CSTR(name),
                          NUM2INT(behavior),
                          NUM2INT(placement),
                          NUM2INT(band_num),
                          NUM2INT(band_position),
                          NUM2INT(offset));
    return self;
}

/*
 * Gnome::App::docked(widget, name, behavior, placement,
 *                    band_num, band_position, offset)
 *
 * Add widget as a dock item according to the specified layout
 * information. Notice that, if automatic layout configuration is
 * enabled, the layout is overridden by the saved configuration,
 * if any.
 *
 * arguments:
 *   widget - Widget to be added to the &GnomeApp
 *   name - Name for the dock item
 *   behavior - Behavior for the new dock item
 *   placement - Placement for the new dock item
 *   band_num - Number of the band where the dock item should be placed
 *   band_position - Position of the new dock item in band band_num
 *   offset - Offset from the previous dock item in the band;
 *            if there is no previous item, offset from the beginning
 *            of the band.
 */
static VALUE
app_add_docked(self, widget, name, behavior, placement,
               band_num, band_position, offset)
    VALUE self, widget, name, behavior, placement;
    VALUE band_num, band_position, offset;
{
    gnome_app_add_docked(_SELF(self),
                         GTK_WIDGET(widget),
                         RVAL2CSTR(name),
                         NUM2INT(behavior),
                         NUM2INT(placement),
                         NUM2INT(band_num),
                         NUM2INT(band_position),
                         NUM2INT(offset));
    return self;			 
}

/*
 * Gnome::App#add_dock_item(item, placement, band_num, band_position, offset)
 */
static VALUE
app_add_dock_item(self, item, placement,
                  band_num, band_position, offset)
    VALUE self, item, placement;
    VALUE band_num, band_position, offset;
{
    gnome_app_add_dock_item(_SELF(self),
                            BONOBO_DOCK_ITEM(RVAL2GOBJ(item)),
                            NUM2INT(placement),
                            NUM2INT(band_num),
                            NUM2INT(band_position),
                            NUM2INT(offset));
    return self;			 
}

/*
 * Gnome::App#enable_layout_config(enable)
 *
 * Specify whether app should automatically save the dock's layout
 * configuration via gnome-config whenever it changes or not.
 *
 * argument:
 *   enable - Boolean specifying whether automatic configuration
 *            saving is enabled
 */
static VALUE
app_enable_layout_config(self, enable)
    VALUE self, enable;
{
    gnome_app_enable_layout_config(_SELF(self), RTEST(enable));
    return self;
}

/*
 * Gnome::App#get_dock
 *
 * Retrieves the &GnomeDock widget contained in the &GnomeApp.
 *
 * return:
 *   The &Gnome::Dock widget.
 */
static VALUE
app_get_dock(self)
    VALUE self;
{
    BonoboDock* result;
    result = gnome_app_get_dock(_SELF(self));
    return result ? GOBJ2RVAL(result) : Qnil;
}

/*
 * Gnome::App#get_dock_item_by_name(name)
 *
 * Retrieve the dock item whose name matches name.
 *
 * argument:
 *   name - Name of the dock item to retrieve
 * return:
 *   The retrieved dock item.
 */
static VALUE
app_get_dock_item_by_name(self, name)
    VALUE self, name;
{
    BonoboDockItem* result;
    result = gnome_app_get_dock_item_by_name(_SELF(self),
					                         RVAL2CSTR(name));
    return result ? GOBJ2RVAL(result) : Qnil;
}

void
Init_gnome_app(mGnome)
    VALUE mGnome;
{
    VALUE gnoApp = G_DEF_CLASS(GNOME_TYPE_APP, "App", mGnome);

    /*
     * instance methods
     */
    rb_define_method(gnoApp, "initialize", app_initialize, 2);
    rb_define_method(gnoApp, "set_menus", app_set_menus, 1);
    rb_define_method(gnoApp, "set_toolbar", app_set_toolbar, 1);
    rb_define_method(gnoApp, "set_statusbar", app_set_statusbar, 1);
    rb_define_method(gnoApp, "set_statusbar_custom", app_set_statusbar_custom, 2);
    rb_define_method(gnoApp, "set_contents", app_set_contents, 1);
    rb_define_method(gnoApp, "add_toolbar", app_add_toolbar, 7);
    rb_define_method(gnoApp, "add_docked", app_add_docked, 7);
    rb_define_method(gnoApp, "add_dock_item", app_add_dock_item, 5);
    rb_define_method(gnoApp, "enable_layout_config", app_enable_layout_config, 1);
    rb_define_method(gnoApp, "get_dock", app_get_dock, 0);
    rb_define_method(gnoApp, "get_dock_item_by_name", app_get_dock_item_by_name, 1);

    Init_gnome_app_helper(mGnome, gnoApp);
}
