/* $Id: rbgtk-xmhtml.c,v 1.4 2002/08/27 11:54:51 mutoh Exp $ */

/* Gtk::XmHTML widget for Ruby/Gnome
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

#include <gtk-xmhtml/gtk-xmhtml.h>
#include "rbgnome.h"

VALUE gXmHTML;

GtkWidget *gtk_xmhtml_new _((void));

static VALUE
xmhtml_initialize(self)
    VALUE self;
{
    GtkWidget *xmhtml;
    xmhtml = gtk_xmhtml_new();
    set_widget(self, xmhtml);
    return Qnil;
}

static VALUE
xmhtml_freeze(self)
    VALUE self;
{
    gtk_xmhtml_freeze(GTK_XMHTML(get_widget(self)));
    return self;
}

static VALUE
xmhtml_thaw(self)
    VALUE self;
{
    gtk_xmhtml_thaw(GTK_XMHTML(get_widget(self)));
    return self;
}

static VALUE
xmhtml_source(self, source)
    VALUE self, source;
{
    gtk_xmhtml_source(GTK_XMHTML(get_widget(self)), STR2CSTR(source));
    return self;
}

static VALUE
xmhtml_string_direction(self, direction)
    VALUE self, direction;
{
    gtk_xmhtml_set_string_direction(GTK_XMHTML(get_widget(self)),
				    NUM2INT(direction));
    return self;
}

static VALUE
xmhtml_set_alignment(self, alignment)
    VALUE self, alignment;
{
    gtk_xmhtml_set_alignment(GTK_XMHTML(get_widget(self)),
			     NUM2INT(alignment));
    return self;
}

static VALUE
xmhtml_outline(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_outline(GTK_XMHTML(get_widget(self)),
			   RTEST(flag));
    return self;
}

static VALUE
xmhtml_set_font_familty(self, family, sizes)
    VALUE self, family, sizes;
{
    gtk_xmhtml_set_font_familty(GTK_XMHTML(get_widget(self)),
				STR2CSTR(family),
				STR2CSTR(sizes));
    return self;
}

static VALUE
xmhtml_set_font_familty_fixed(self, family, sizes)
    VALUE self, family, sizes;
{
    gtk_xmhtml_set_font_familty_fixed(GTK_XMHTML(get_widget(self)),
				      STR2CSTR(family),
				      STR2CSTR(sizes));
    return self;
}

static VALUE
xmhtml_set_font_charset(self, charset)
    VALUE self, charset;
{
    gtk_xmhtml_set_font_charset(GTK_XMHTML(get_widget(self)),
				STR2CSTR(charset));
    return self;
}

static VALUE
xmhtml_set_allow_body_colors(self, enable)
    VALUE self, enable;
{
    gtk_xmhtml_set_allow_body_colors(GTK_XMHTML(get_widget(self)),
				     RTEST(enable));
    return self;
}

static VALUE
xmhtml_set_hilight_on_enter(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_hilight_on_enter(GTK_XMHTML(get_widget(self)),
				    RTEST(flag));
    return self;
}
     
static VALUE
xmhtml_set_anchor_underline_type(self, underline_type)
    VALUE self, underline_type;
{
    gtk_xmhtml_set_anchor_underline_type(GTK_XMHTML(get_widget(self)),
					 NUM2INT(underline_type));
    return self;
}

static VALUE
xmhtml_set_anchor_visited_underline_type(self, underline_type)
    VALUE self, underline_type;
{
    gtk_xmhtml_set_anchor_visited_underline_type(GTK_XMHTML(get_widget(self)),
						 NUM2INT(underline_type));
    return self;
}

static VALUE
xmhtml_set_anchor_target_underline_type(self, underline_type)
    VALUE self, underline_type;
{
    gtk_xmhtml_set_anchor_target_underline_type(GTK_XMHTML(get_widget(self)),
						NUM2INT(underline_type));
    return self;
}

static VALUE
xmhtml_set_allow_color_switching(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_allow_color_switching(GTK_XMHTML(get_widget(self)),
					 RTEST(flag));
    return self;
}

#if 0
static VALUE
xmhtml_set_dithering(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_dithering(GTK_XMHTML(get_widget(self)),
			     /* XmHTMLDitherType flag */);
    return self;
}
#endif

static VALUE
xmhtml_set_allow_font_switching(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_allow_font_switching(GTK_XMHTML(get_widget(self)),
					RTEST(flag));
    return self;
}

static VALUE
xmhtml_set_max_image_colors(self, max_colors)
    VALUE self, max_colors;
{
    gtk_xmhtml_set_max_image_colors(GTK_XMHTML(get_widget(self)),
				    NUM2INT(max_colors));
    return self;
}

static VALUE
xmhtml_set_allow_images(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_allow_images(GTK_XMHTML(get_widget(self)),
				RTEST(flag));
    return self;
}

static VALUE
xmhtml_set_plc_intervals(self, min_delay, max_delay, def_delay)
    VALUE self, min_delay, max_delay, def_delay;
{
    gtk_xmhtml_set_plc_intervals(GTK_XMHTML(get_widget(self)),
				 NUM2INT(min_delay),
				 NUM2INT(max_delay),
				 NUM2INT(def_delay));
    return self;
}

static VALUE
xmhtml_set_def_body_image_url(self, url)
    VALUE self, url;
{
    gtk_xmhtml_set_def_body_image_url(GTK_XMHTML(get_widget(self)),
				      STR2CSTR(url));
    return self;
}

static VALUE
xmhtml_set_anchor_buttons(self, flag)
    VALUE self, flag;
{
    gtk_xmhtml_set_anchor_buttons(GTK_XMHTML(get_widget(self)),
				  RTEST(flag));
    return self;
}

#if 0
static VALUE
xmhtml_set_anchor_cursor(self, cursor, flag)
    VALUE self, cursor, flag;
{
    gtk_xmhtml_set_anchor_cursor(GTK_XMHTML(get_widget(self)),
				 /* get_gdkcursor(cursor) */,
				 RTEST(flag));
    return self;
}
#endif

#if 0
void gtk_xmhtml_set_topline                       (GtkXmHTML *html,
						   int line);

int gtk_xmhtml_get_topline                        (GtkXmHTML *html);

void gtk_xmhtml_set_freeze_animations             (GtkXmHTML *html,
						   int flag);

char *gtk_xmhtml_get_source                       (GtkXmHTML *html);

void gtk_xmhtml_set_screen_gamma                  (GtkXmHTML *html,
						   float     gamma);
#endif

#if 0
static VALUE
xmhtml_set_image_procs(self, image_proc, gif_proc, get_data, end_data)
    VALUE self, image_proc, gif_proc, get_data, end_data;
{
    gtk_xmhtml_set_image_procs(GTK_XMHTML(get_widget(self)),
			       /* XmImageProc image_proc */,
			       /* XmImageGifProc gif_proc */,
			       /* XmHTMLGetDataProc get_data */,
			       /* XmHTMLEndDataProc end_data */);
    return self;
}

static VALUE
xmhtml_set_event_proc(self, event_proc)
    VALUE self, event_proc;
{
    gtk_xmhtml_set_event_proc(GTK_XMHTML(get_widget(self)),
			      /* XmHTMLEventProc event_proc*/ );
    return self;
}
#endif /* 0 */

#if 0
void gtk_xmhtml_set_perfect_colors                (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_uncompress_command            (GtkXmHTML *html, char *cmd);
void gtk_xmhtml_set_strict_checking               (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_bad_html_warnings             (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_allow_form_coloring           (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_imagemap_draw                 (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_mime_type                     (GtkXmHTML *html, char *mime_type);
void gtk_xmhtml_set_alpha_processing              (GtkXmHTML *html, int flag);
void gtk_xmhtml_set_rgb_conv_mode                 (GtkXmHTML *html, int val);
#endif

void
Init_gtk_xmhtml()
{
    gXmHTML = rb_define_class_under(mGtk, "XmHTML", gContainer);

    /* Instance methods */

    /* Signals */
    rb_define_const(gXmHTML, "SIGNAL_ACTIVATE", rb_str_new2("activate"));
    rb_define_const(gXmHTML, "SIGNAL_ARM", rb_str_new2("arm"));
    rb_define_const(gXmHTML, "SIGNAL_ANCHOR_TRACK",
		    rb_str_new2("anchor_track"));
    rb_define_const(gXmHTML, "SIGNAL_FRAME", rb_str_new2("frame"));
    rb_define_const(gXmHTML, "SIGNAL_FORM", rb_str_new2("form"));
    rb_define_const(gXmHTML, "SIGNAL_INPUT", rb_str_new2("input"));
    rb_define_const(gXmHTML, "SIGNAL_LINK", rb_str_new2("link"));
    rb_define_const(gXmHTML, "SIGNAL_MOTION", rb_str_new2("motion"));
    rb_define_const(gXmHTML, "SIGNAL_IMAGEMAP", rb_str_new2("imagemap"));
    rb_define_const(gXmHTML, "SIGNAL_DOCUMENT", rb_str_new2("document"));
    rb_define_const(gXmHTML, "SIGNAL_FOCUS", rb_str_new2("_focus"));
    rb_define_const(gXmHTML, "SIGNAL_LOSING_FOCUS",
		    rb_str_new2("losing_focus"));
    rb_define_const(gXmHTML, "SIGNAL_MOTION_TRACK",
		    rb_str_new2("motion_track"));
    rb_define_const(gXmHTML, "SIGNAL_HTML_EVENT", rb_str_new2("html_event"));
    rb_define_const(gXmHTML, "SIGNAL_ANCHOR_VISITED",
		    rb_str_new2("anchor_visited"));
}
