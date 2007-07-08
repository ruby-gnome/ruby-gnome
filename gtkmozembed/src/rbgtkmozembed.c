/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

/************************************************

  rbgtkmozembed.c -

    Ruby-GNOME2 Gtk::MozEmbed - Ruby bindings for GtkMozEmbed

    Copyright (C) 2005-2006 Ruby-GNOME2 Project Team.
    Copyright (C) 2005 Mirko Maischberger, All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    ***

    $Author: sakai $
    $Date: 2007/07/08 03:02:08 $

    *** 

    Many thanks to Christopher Blizzard <blizzard@mozilla.org> for
    writing the gtkmozembed library and to the Mozilla Foundation.

    Thanks should as well go to the Gtk and GNOME developers, Yukihiro
    Matsumoto for Ruby and to the Ruby-GNOME2 Team.

************************************************/

#include "ruby.h"

#include "rbgtk.h"

#include "gtkmozembed.h"

#include "rbgtkmozembedversion.h"

/*
 * Class: Gtk::MozEmbed
 *
 * This class wraps the GtkMozEmbed widget in a Ruby class.  The
 * GtkMozEmbed is an HTML rendering widget based on Mozilla Gecko
 * provided by either Mozilla or Firefox.  You need to have
 * libgtkmozembed.so in your library path to use and
 * mozilla-gtkmozembed.pc and gtkmozembed.h in order to compile.
 *
 *  Gtk::MozEmbed.new
 *  ...
 *  Gtk::MozEmbed.location="http://ruby-gnome2.sf.net"
 *  ...
 *
 * Please see the sample application ruby-gecko for more info and
 * examples.
 *
 */

#define _SELF(s) (GTK_MOZ_EMBED(RVAL2GOBJ(s)))


/* 
 * Class method: new
 *
 * Constructor for the Gtk::MozEmbed object.
 *
 * Returns: a newly created Gtk::MozEmbed widget.
 *
 */
static VALUE
moz_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_moz_embed_new());
    return Qnil;
}

/* 
 * Class method: set_profile_path(profile_path, profile_name)
 *
 * profile_path: the path to store the profile (String).
 * profile_name: the name of the application using the widget (String).
 *
 * You should call this method before the constructor. The
 * profile_path parameter should be set to $ENV['HOME']+'/.mozilla'.
 *
 * A new profile directory with this name will be created in the
 * profile_path directory.
 *
 * Returns: nil.
 *
 */
static VALUE
moz_set_profile_path(self, profile_path, profile_name)
    VALUE self, profile_path, profile_name;
{
    gtk_moz_embed_set_profile_path(RVAL2CSTR(profile_path), 
                                   RVAL2CSTR(profile_name));
    return Qnil;
}

/* 
 * Class method: set_comp_path(path)
 *
 * path: the mozilla component path (String).
 *
 * You should call this method before the constructor. 
 *
 * The path should be something like /usr/lib/mozilla or
 * /usr/lib/mozilla-firefox depending on your installation and
 * distribution.
 *
 * We call this inside Init_gtkmozembed() and set the value at 
 * compile time before initializing XPCOM.
 *
 * Returns: nil.
 *
 */
static VALUE
moz_set_comp_path(self, path)
    VALUE self, path;
{
    gtk_moz_embed_set_comp_path(RVAL2CSTR(path));
    return Qnil;
}

/*
 * Method: load_url(url)
 *
 * url: the url to load (String).
 *
 * This method starts loading the given url, and returns
 * immediately. The url should be in the form "http://www.gnome.org".
 *
 * Returns: the object itself.
 *
 */

/*
 * Method: set_location(url)
 *
 * See Gtk::MozEmbed#load_url
 *
 * Returns: self.
 */

/*
 * Method: location=url
 *
 * See Gtk::MozEmbed#load_url
 *
 * Returns: self.
 */

static VALUE
moz_load_url(self, url)
    VALUE self, url;
{
    gtk_moz_embed_load_url(_SELF(self), RVAL2CSTR(url));
    return self;
}

/*
 * Method: stop_load
 *
 * Stops loading the current page.
 *
 * Returns: the object itself.
 *
 */
static VALUE
moz_stop_load(self)
    VALUE self;
{
    gtk_moz_embed_stop_load(_SELF(self));
    return self;
}

/*
 * Method: can_go_back?
 *
 * This method reflects the status of the browsing history.
 *
 * Returns: True if the browser can go back one page.
 *
 */
static VALUE
moz_can_go_back(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_moz_embed_can_go_back(_SELF(self)));
}

/*
 * Method: can_go_forward?
 *
 * This method reflects the status of the browsing history.
 *
 * Returns: True if the browser can go forward one page.
 *
 */
static VALUE
moz_can_go_forward(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_moz_embed_can_go_forward(_SELF(self)));
}

/*
 * Method: go_back
 *
 * Tells the browser to go back one page in the history.
 *
 * Returns: the object itself.
 *
 */
static VALUE
moz_go_back(self)
    VALUE self;
{
    gtk_moz_embed_go_back(_SELF(self));
    return self;
}

/*
 * Method: go_forward
 *
 * Tells the browser to go one page forward in the history.
 *
 * Returns: the object itself.
 *
 */
static VALUE
moz_go_forward(self)
    VALUE self;
{
    gtk_moz_embed_go_forward(_SELF(self));
    return self;
}

/*
 * Method: location
 *
 * The current location, usefull when the "location" signal is raised.
 *
 * Returns: the current location (String).
 *
 */
static VALUE
moz_get_location(self)
    VALUE self;
{
    char* url = gtk_moz_embed_get_location(_SELF(self));
    return CSTR2RVAL(url);
}


/*
 * Method: link_message
 *
 * The location of the selected link, useful when the "link_message"
 * signal is raised.
 *
 * Returns: the selected url.
 *
 */
static VALUE
moz_get_link_message(self)
    VALUE self;
{
    char *msg = gtk_moz_embed_get_link_message(_SELF(self));
    return CSTR2RVAL(msg);
}

/*
 * Method: js_status
 *
 * The JavaScript|ECMAScript status message, useful when the
 * "js_status" signal is raised.
 *
 * Returns: the selected url (String).
 *
 */
static VALUE
moz_get_js_status(self)
    VALUE self;
{
    char *msg = gtk_moz_embed_get_js_status(_SELF(self));
    return CSTR2RVAL(msg);
}

/*
 * Method: title
 *
 * The title of the currently loaded page, usefull when the "title"
 * signal is raised.
 *
 * Returns: the page title (String).
 *
 */
static VALUE
moz_get_title(self)
    VALUE self;
{
    char *title = gtk_moz_embed_get_title(_SELF(self));
    return CSTR2RVAL(title);
}

/*
 * Method: reload(flags)
 *
 * flags: one of the Gtk::MozEmbed::ReloadFlags
 * (Gtk::MozEmbed::RELOADNORMAL, Gtk::MozEmbed::RELOADBYPASSCACHE,
 * Gtk::MozEmbed::RELOADBYPASSPROXY, Gtk::MozEmbed::RELOADBYPASSPROXYANDCACHE).
 *
 * Forces a page reload of a specific type. 
 *
 * Returns: the object itself.
 *
 */
static VALUE
moz_reload(self, flags)
    VALUE self, flags;
{
    gtk_moz_embed_reload(_SELF(self), 
        RVAL2GFLAGS(flags, GTK_TYPE_MOZ_EMBED_RELOAD_FLAGS));
    return self;
}

/*
 * Method: set_chrome_mask(flags)
 *
 * flags: a combination of Gtk::MozEmbed::ChromeFlags.
 *
 * Set the chrome mask.
 *
 * Returns: the object itself.
 *
 */

static VALUE
moz_set_chrome_mask(self, flags)
    VALUE self, flags;
{
    gtk_moz_embed_set_chrome_mask(_SELF(self), 
        RVAL2GFLAGS(flags, GTK_TYPE_MOZ_EMBED_CHROME_FLAGS));
    return self;
}

/*
 * Method: chrome_mask
 *
 * Get the chrome mask. 
 *
 * Returns: a combination of Moz::GtkEmbed::ChromeFlags.
 *
 */
static VALUE
moz_get_chrome_mask(self)
    VALUE self;
{
    guint32 mask = gtk_moz_embed_get_chrome_mask(_SELF(self));
    return GFLAGS2RVAL(mask, GTK_TYPE_MOZ_EMBED_CHROME_FLAGS);
}

/*
 * Method: render_data(data, base_uri, mime_type)
 *
 * data: the data to be rendered.
 * base_uri: the base uri for the resolver.
 * mime_type: the mime/type of the data to be rendered.
 *
 * This function will allow you to take a chunk of random data and
 * render it into the Gtk::MozEmbed window. See also
 * Gtk::MozEmbed#open_stream, Gtk::MozEmbed#append_data and
 * Gtk::MozEmbed#close_stream.
 *
 * Returns: self.
 *
 */
static VALUE
moz_render_data(self, data, base_uri, mime_type)
    VALUE self, data, base_uri, mime_type;
{
    gtk_moz_embed_render_data(_SELF(self), 
                              RSTRING(data)->ptr, 
                              RSTRING(data)->len,
                              RVAL2CSTR(base_uri),
                              RVAL2CSTR(mime_type));
    return self;
}    




static gboolean stream_open = FALSE;

/*
 * Method: open_stream(base_uri, mime_type)
 *
 * base_uri: the uri that Gtk::MozEmbed will use to resolve
 *           relative addresses (String).
 * mime_type: the mime type of the data (String).
 * 
 * This method sould be called to visualize self feeded html.
 *
 * The base_uri must be a valid base uri, this should contain 
 * the protocol name and the path separated by a colon.
 *
 * The mime_type should be a valid mime-type and instucts 
 * GtkMozEmbed about the data that we'll send with "<<":
 *
 *  * "text/html" if an HTML is to be displayed.
 *  * "text/plain" if TXT data are to be displayed.
 *  * "image/png" if we want to send some PNG data.
 *  * "image/jpeg" if our data are in JPEG format.
 *  * "image/gif" if our data are in GIF format.
 *  * See /etc/mime.types for more.
 *
 *  moz.open_stream("file:///basepath/", "text/html")
 *  moz.append_data("
 *  <html>
 *  <body>
 *  test 
 *  <a href=\"hello.html\">this is resolved against base_path</a>
 *  </body> 
 *  </html>")
 *  moz.close_stream
 *
 * See also #render_data.
 *
 * Returns: self.
 * 
 */
static VALUE
moz_open_stream(self, base_uri, mime_type)
    VALUE self, base_uri, mime_type;
{
    gchar *bu = RVAL2CSTR(base_uri);
    
    if(!strchr(bu, ':'))
       rb_raise(rb_eArgError, 
                "The base_uri parameter must be a valid base uri. "
                "(e.g http://www.sitename.org/base/, file:///path/.../)");
    gtk_moz_embed_open_stream(_SELF(self), bu, RVAL2CSTR(mime_type));
    stream_open = TRUE;
    return self;
}

/*
 * Method: append_data(data)
 *
 * data: The data do be added to the rendering stream (String).
 *
 * You should feed data to the rendering stream via this method.
 * The data should be in the mime format if you want to send some
 * self drawn PNG or JPEG data from a Pixbuf please see:
 * http://ruby-gnome2.sourceforge.jp/hiki.cgi?Gdk%3A%3APixbuf#save_to_buffer
 *
 * Returns: self.
 */
static VALUE
moz_append_data(self, data)
    VALUE self, data;
{
    if(!stream_open) 
        rb_raise(rb_eRuntimeError, 
                 "Can't append date before open_stream"
                 " is called.");

    gtk_moz_embed_append_data(_SELF(self), 
                              RSTRING(data)->ptr, 
                              RSTRING(data)->len);
    return self;
}

/*
 * Method: close_stream
 *
 * This closes the stream. This way Gtk::MozEmbed knows we
 * are finished.
 *
 * Returns: self.
 *
 */
static VALUE
moz_close_stream(self)
    VALUE self;
{
    if(!stream_open) 
        rb_raise(rb_eRuntimeError, 
                 "You should open the stream before closing it.");

    gtk_moz_embed_close_stream(_SELF(self));
    stream_open = FALSE;
    return self;
}


/* 
 * Signals are automagically handled by Ruby-GNOME2.
 * except for new_window which needs a custom handling.
 *
 */


/* The new_window signal is handled separately. */

static gulong new_win_sig = 0;

static void
moz_custom_new_window_cb(embed, retval, chromemask, func)
    GtkMozEmbed *embed;
    GtkMozEmbed **retval;
    guint chromemask;
    VALUE func;
{
    VALUE new_moz = 
        rb_funcall(func, rb_intern("call"), 2,
                   GOBJ2RVAL(embed),
                   GFLAGS2RVAL(chromemask, GTK_TYPE_MOZ_EMBED_CHROME_FLAGS));
    *retval = RVAL2GOBJ(new_moz); 
}

/*
 * Method: on_new_window { |widget, chromemask| block } 
 * 
 * The new_window signal should not be connected directly, this method
 * should be used instead. The block should eval to the newly created
 * Gtk::MozEmbed widget. This method serves the purpose of creating
 * a new window around the widget when requested.
 *
 * Returns: self.
 */
static VALUE
set_on_new_window_block(self)
    VALUE self;
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func); 
    new_win_sig = 
        g_signal_connect(RVAL2GOBJ(self), "new_window",
                         G_CALLBACK(moz_custom_new_window_cb),
                         (void *)func); 
    return self;
} 

/*
 * Method: no_new_window
 * 
 * This method clears the code block associated via the
 * Gtk::MozEmbed#on_new_window method.
 *
 * Returns: self.
 */
static VALUE
clear_on_new_window_block(self)
    VALUE self;
{
    GObject *moz = RVAL2GOBJ(self);

    if(g_signal_handler_is_connected(moz, new_win_sig)) 
        g_signal_handler_disconnect(RVAL2GOBJ(self), new_win_sig);
    else
        rb_raise(rb_eRuntimeError, 
                 "#on_new_window code block not set so it cannot be removed.");

    return self;
} 


/*************************************************************/
/*  Some methods missing in the Gtk2 version of GtkMozEmbed  */

static GtkFlagValue gtk_moz_embed_reload_flags_values[] = {
    { GTK_MOZ_EMBED_FLAG_RELOADNORMAL,
      "GTK_MOZ_EMBED_FLAG_RELOADNORMAL", "reloadnormal" },
    { GTK_MOZ_EMBED_FLAG_RELOADBYPASSCACHE,
      "GTK_MOZ_EMBED_FLAG_RELOADBYPASSCACHE", "reloadbypasscache" },
    { GTK_MOZ_EMBED_FLAG_RELOADBYPASSPROXY,
      "GTK_MOZ_EMBED_FLAG_RELOADBYPASSPROXY", "reloadbypassproxy" },
    { GTK_MOZ_EMBED_FLAG_RELOADBYPASSPROXYANDCACHE,
      "GTK_MOZ_EMBED_FLAG_RELOADBYPASSPROXYANDCACHE",
      "reloadbypassproxyandcache" },
    { GTK_MOZ_EMBED_FLAG_RELOADCHARSETCHANGE,
      "GTK_MOZ_EMBED_FLAG_RELOADCHARSETCHANGE", "reloadcharset" },
    { 0,
      NULL, NULL }
};

static GtkFlagValue gtk_moz_embed_chrome_flags_values[] = {
    { GTK_MOZ_EMBED_FLAG_DEFAULTCHROME, 
      "GTK_MOZ_EMBED_FLAG_DEFAULTCHROME", "defaultchrome" },
    { GTK_MOZ_EMBED_FLAG_WINDOWBORDERSON, 
      "GTK_MOZ_EMBED_FLAG_WINDOWBORDERSON", "windowborderson" },
    { GTK_MOZ_EMBED_FLAG_WINDOWCLOSEON,
      "GTK_MOZ_EMBED_FLAG_WINDOWCLOSEON", "windowcloseon" },
    { GTK_MOZ_EMBED_FLAG_WINDOWRESIZEON,
      "GTK_MOZ_EMBED_FLAG_WINDOWRESIZEON", "windowresizeon" },
    { GTK_MOZ_EMBED_FLAG_MENUBARON, 
      "GTK_MOZ_EMBED_FLAG_MENUBARON", "menubaron" },
    { GTK_MOZ_EMBED_FLAG_TOOLBARON,
      "GTK_MOZ_EMBED_FLAG_TOOLBARON", "toolbaron" },
    { GTK_MOZ_EMBED_FLAG_LOCATIONBARON,
      "GTK_MOZ_EMBED_FLAG_LOCATIONBARON", "locationbaron" },
    { GTK_MOZ_EMBED_FLAG_STATUSBARON,
      "GTK_MOZ_EMBED_FLAG_STATUSBARON", "statusbaron" },
    { GTK_MOZ_EMBED_FLAG_PERSONALTOOLBARON,
      "GTK_MOZ_EMBED_FLAG_PERSONALTOOLBARON", "personaltoolbaron" },
    { GTK_MOZ_EMBED_FLAG_SCROLLBARSON,
      "GTK_MOZ_EMBED_FLAG_SCROLLBARSON", "scrollbarson" },
    { GTK_MOZ_EMBED_FLAG_TITLEBARON, 
      "GTK_MOZ_EMBED_FLAG_TITLEBARON", "titlebaron" },
    { GTK_MOZ_EMBED_FLAG_EXTRACHROMEON,
      "GTK_MOZ_EMBED_FLAG_EXTRACHROMEON", "extrachromeon" },
    { GTK_MOZ_EMBED_FLAG_ALLCHROME, 
      "GTK_MOZ_EMBED_FLAG_ALLCHROME", "allchrome" },
    { GTK_MOZ_EMBED_FLAG_WINDOWRAISED, 
      "GTK_MOZ_EMBED_FLAG_WINDOWRAISED", "windowraised" },
    { GTK_MOZ_EMBED_FLAG_WINDOWLOWERED,
      "GTK_MOZ_EMBED_FLAG_WINDOWLOWERED", "windowlowered" },
    { GTK_MOZ_EMBED_FLAG_CENTERSCREEN,
      "GTK_MOZ_EMBED_FLAG_CENTERSCREEN", "centerscreen" },
    { GTK_MOZ_EMBED_FLAG_DEPENDENT,
      "GTK_MOZ_EMBED_FLAG_DEPENDENT", "dependent" },
    { GTK_MOZ_EMBED_FLAG_MODAL,
      "GTK_MOZ_EMBED_FLAG_MODAL", "modal" },
    { GTK_MOZ_EMBED_FLAG_OPENASDIALOG,
      "GTK_MOZ_EMBED_FLAG_OPENASDIALOG", "openasdialog" },
    { GTK_MOZ_EMBED_FLAG_OPENASCHROME, 
      "GTK_MOZ_EMBED_FLAG_OPENASCHROME", "openaschrome" },
    { 0,
      NULL, NULL }
};

GtkType
gtk_moz_embed_reload_flags_get_type(void)
{
    static GtkType reload_flags_type = 0;

    if (!reload_flags_type)
        reload_flags_type =
            g_flags_register_static("GtkMozEmbedReloadFlags",
                                    gtk_moz_embed_reload_flags_values);
    return reload_flags_type;
}

GtkType
gtk_moz_embed_chrome_flags_get_type(void)
{
    static GtkType chrome_flags_type = 0;

    if (!chrome_flags_type)
        chrome_flags_type = 
            g_flags_register_static("GtkMozEmbedChromeFlags",
                                    gtk_moz_embed_chrome_flags_values);
    return chrome_flags_type;
}
/*                                                           */
/*************************************************************/

/** INIT **/

void 
Init_gtkmozembed()
{

    VALUE moz = G_DEF_CLASS(GTK_TYPE_MOZ_EMBED, "MozEmbed", mGtk);

    rb_define_module_function(moz, "set_profile_path", 
                              moz_set_profile_path, 2);
    rb_define_module_function(moz, "set_comp_path", 
                              moz_set_comp_path, 1);

    rb_define_method(moz, "initialize", moz_initialize, 0);
    rb_define_method(moz, "load_url", moz_load_url, 1);
    /* Alias method so that location & location= work in pair */
    rb_define_method(moz, "set_location", moz_load_url, 1);
    rb_define_method(moz, "stop_load", moz_stop_load, 0);
    rb_define_method(moz, "can_go_back?", moz_can_go_back, 0);
    rb_define_method(moz, "can_go_forward?", moz_can_go_forward, 0);
    rb_define_method(moz, "go_back", moz_go_back, 0);
    rb_define_method(moz, "go_forward", moz_go_forward, 0);
    rb_define_method(moz, "location", moz_get_location, 0);
    rb_define_method(moz, "js_status", moz_get_js_status, 0);
    rb_define_method(moz, "link_message", moz_get_link_message, 0);
    rb_define_method(moz, "title", moz_get_title, 0);
    rb_define_method(moz, "reload", moz_reload, 1);
    rb_define_method(moz, "chrome_mask", moz_get_chrome_mask, 0);
    rb_define_method(moz, "set_chrome_mask", moz_set_chrome_mask, 1);
    rb_define_method(moz, "render_data", moz_render_data, 3);
    rb_define_method(moz, "open_stream", moz_open_stream, 2);
    rb_define_method(moz, "append_data", moz_append_data, 1);
    /*
     * This overrides the Bin method so it's disabled:
     * rb_define_method(moz, "<<", moz_append_data, 1);
     */
    rb_define_method(moz, "close_stream", moz_close_stream, 0);

    /* overriding new_window signal to take care of retval ourself */
    rb_define_method(moz, "on_new_window", set_on_new_window_block, 0);
    rb_define_method(moz, "no_new_window", clear_on_new_window_block, 0);

    G_DEF_SETTERS(moz);

    G_DEF_CLASS(GTK_TYPE_MOZ_EMBED_RELOAD_FLAGS, "ReloadFlags", moz);
    G_DEF_CONSTANTS(moz, GTK_TYPE_MOZ_EMBED_RELOAD_FLAGS, 
                    "GTK_MOZ_EMBED_FLAG_");

    G_DEF_CLASS(GTK_TYPE_MOZ_EMBED_CHROME_FLAGS, "ChromeFlags", moz);
    G_DEF_CONSTANTS(moz, GTK_TYPE_MOZ_EMBED_CHROME_FLAGS, 
                    "GTK_MOZ_EMBED_FLAG_");

    rb_define_const(moz, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GTKMOZEMBED_MAJOR_VERSION),
                                INT2FIX(GTKMOZEMBED_MINOR_VERSION),
                                INT2FIX(GTKMOZEMBED_MICRO_VERSION)));
    
    // set default component path if defined
#ifdef DEFAULT_MOZILLA_FIVE_HOME
    gtk_moz_embed_set_comp_path(DEFAULT_MOZILLA_FIVE_HOME);
#endif

}
