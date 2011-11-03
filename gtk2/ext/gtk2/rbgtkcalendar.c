/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cCalendar
#define _SELF(self) (GTK_CALENDAR(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_calendar_new());
    return Qnil;
}

static VALUE
rg_select_month(VALUE self, VALUE month, VALUE year)
{
    gint ret = gtk_calendar_select_month(_SELF(self),
                                         NUM2INT(month)-1,
                                         NUM2INT(year));
    return INT2FIX(ret);
}

static VALUE
rg_select_day(VALUE self, VALUE day)
{
    gtk_calendar_select_day(_SELF(self), NUM2INT(day));
    return self;
}

static VALUE
rg_mark_day(VALUE self, VALUE day)
{
    gtk_calendar_mark_day(_SELF(self), NUM2INT(day));
    return self;
}

static VALUE
rg_unmark_day(VALUE self, VALUE day)
{
    gtk_calendar_unmark_day(_SELF(self), NUM2INT(day));
    return self;
}

static VALUE
rg_clear_marks(VALUE self)
{
    gtk_calendar_clear_marks(_SELF(self));
    return self;
}

static VALUE
rg_date(VALUE self)
{
    VALUE ret;
    guint year, month, day;

    gtk_calendar_get_date(_SELF(self), &year, &month, &day);
    ret = rb_ary_new();
    rb_ary_push(ret, INT2FIX(year));
    rb_ary_push(ret, INT2FIX(month+1));
    rb_ary_push(ret, INT2FIX(day));
    return ret;
}

static VALUE
rg_freeze(VALUE self)
{
    gtk_calendar_freeze(_SELF(self));
    return self;
}

static VALUE
rg_thaw(VALUE self)
{
    gtk_calendar_thaw(_SELF(self));
    return self;
}

static VALUE
rg_display_options(int argc, VALUE *argv, VALUE self)
{
    VALUE flags;
    rb_scan_args(argc, argv, "01", &flags);

    if NIL_P(flags){
#if GTK_CHECK_VERSION(2,4,0)
        return GFLAGS2RVAL(gtk_calendar_get_display_options(_SELF(self)),
                           GTK_TYPE_CALENDAR_DISPLAY_OPTIONS);
#else
        rb_warn("Gtk::Calendar#display_options has supported since GTK+-2.4.");
#endif
    } else {
        /* This is for backward compatibility. */
        rb_warning("Gtk::Calendar#display_options(flags) has been deprecated. Use Gtk::Calendar#set_display_options(flags).");
        gtk_calendar_display_options(_SELF(self), 
                                     RVAL2GFLAGS(flags, GTK_TYPE_CALENDAR_DISPLAY_OPTIONS));
    }
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_set_display_options(VALUE self, VALUE flags)
{
    gtk_calendar_set_display_options(_SELF(self), 
                                     RVAL2GFLAGS(flags, 
                                                 GTK_TYPE_CALENDAR_DISPLAY_OPTIONS));
    return self;
}
#endif

void 
Init_gtk_calendar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_CALENDAR, "Calendar", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(select_month, 2);
    RG_DEF_METHOD(select_day, 1);
    RG_DEF_METHOD(mark_day, 1);
    RG_DEF_METHOD(unmark_day, 1);
    RG_DEF_METHOD(clear_marks, 0);
    RG_DEF_METHOD(date, 0);
    RG_DEF_METHOD(freeze, 0);
    RG_DEF_METHOD(thaw, 0);
    RG_DEF_METHOD(display_options, -1);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_display_options, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "display_options");
#endif

    G_DEF_CLASS(GTK_TYPE_CALENDAR_DISPLAY_OPTIONS, "DisplayOptions", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_CALENDAR_DISPLAY_OPTIONS, "GTK_CALENDAR_");
}
