/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcalendar.c -

  $Author: sakai $
  $Date: 2002/08/02 13:44:31 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Calendar
 */

static VALUE
cal_init(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_calendar_new());
    return Qnil;
}

static VALUE
cal_select_month(self, month, year)
    VALUE self, month, year;
{
    gint ret;

    ret = gtk_calendar_select_month(GTK_CALENDAR(RVAL2GOBJ(self)),
				    NUM2INT(month)-1,
				    NUM2INT(year));
    return INT2FIX(ret);
}

static VALUE
cal_select_day(self, day)
    VALUE self, day;
{
    gtk_calendar_select_day(GTK_CALENDAR(RVAL2GOBJ(self)), NUM2INT(day));
    return self;
}

static VALUE
cal_mark_day(self, day)
    VALUE self, day;
{
    gint ret;

    ret = gtk_calendar_mark_day(GTK_CALENDAR(RVAL2GOBJ(self)), NUM2INT(day));
    return INT2FIX(ret);
}

static VALUE
cal_unmark_day(self, day)
    VALUE self, day;
{
    gint ret;

    ret = gtk_calendar_unmark_day(GTK_CALENDAR(RVAL2GOBJ(self)),
				  NUM2INT(day));
    return INT2FIX(ret);
}

static VALUE
cal_clear_marks(self)
    VALUE self;
{
    gtk_calendar_clear_marks(GTK_CALENDAR(RVAL2GOBJ(self)));
    return self;
}

static VALUE
cal_display_options(self, flags)
    VALUE self, flags;
{
    gtk_calendar_display_options(GTK_CALENDAR(RVAL2GOBJ(self)),
				 NUM2INT(flags));
    return self;
}

static VALUE
cal_get_date(self)
    VALUE self;
{
    VALUE ret;
    guint year, month, day;

    gtk_calendar_get_date(GTK_CALENDAR(RVAL2GOBJ(self)),
			  &year, &month, &day);
    ret = rb_ary_new();
    rb_ary_push(ret, INT2FIX(year));
    rb_ary_push(ret, INT2FIX(month+1));
    rb_ary_push(ret, INT2FIX(day));
    return ret;
}

static VALUE
cal_get_year(self)
    VALUE self;
{
    guint year, month, day;

    gtk_calendar_get_date(GTK_CALENDAR(RVAL2GOBJ(self)),
			  &year, &month, &day);
    return INT2FIX(year);
}

static VALUE
cal_get_month(self)
    VALUE self;
{
    guint year, month, day;

    gtk_calendar_get_date(GTK_CALENDAR(RVAL2GOBJ(self)),
			  &year, &month, &day);
    return INT2FIX(month+1);
}

static VALUE
cal_get_day(self)
    VALUE self;
{
    guint year, month, day;

    gtk_calendar_get_date(GTK_CALENDAR(RVAL2GOBJ(self)),
			  &year, &month, &day);
    return INT2FIX(day);
}

static VALUE
cal_freeze(self)
    VALUE self;
{
    gtk_calendar_freeze(GTK_CALENDAR(RVAL2GOBJ(self)));
    return self;
}

static VALUE
cal_thaw(self)
    VALUE self;
{
    gtk_calendar_thaw(GTK_CALENDAR(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_calendar()
{
    VALUE gCalendar = G_DEF_CLASS(GTK_TYPE_CALENDAR, "Calendar", mGtk);

    rb_define_const(gCalendar, "SHOW_HEADING",
		    INT2FIX(GTK_CALENDAR_SHOW_HEADING));
    rb_define_const(gCalendar, "SHOW_DAY_NAMES",
		    INT2FIX(GTK_CALENDAR_SHOW_DAY_NAMES));
    rb_define_const(gCalendar, "NO_MONTH_CHANGE",
		    INT2FIX(GTK_CALENDAR_NO_MONTH_CHANGE));
    rb_define_const(gCalendar, "SHOW_WEEK_NUMBERS",
		    INT2FIX(GTK_CALENDAR_SHOW_WEEK_NUMBERS));
    rb_define_const(gCalendar, "WEEK_START_MONDAY",
		    INT2FIX(GTK_CALENDAR_WEEK_START_MONDAY));

    rb_define_method(gCalendar, "initialize", cal_init, 0);
    rb_define_method(gCalendar, "select_month", cal_select_month, 2);
    rb_define_method(gCalendar, "select_day", cal_select_day, 1);
    rb_define_method(gCalendar, "mark_day", cal_mark_day, 1);
    rb_define_method(gCalendar, "unmark_day", cal_unmark_day, 1);
    rb_define_method(gCalendar, "clear_marks", cal_clear_marks, 0);
    rb_define_method(gCalendar, "display_options", cal_display_options, 1);
    rb_define_method(gCalendar, "get_date", cal_get_date, 0);
    rb_define_method(gCalendar, "get_year", cal_get_year, 0);
    rb_define_method(gCalendar, "get_month", cal_get_month, 0);
    rb_define_method(gCalendar, "get_day", cal_get_day, 0);
    rb_define_method(gCalendar, "freeze", cal_freeze, 0);
    rb_define_method(gCalendar, "thaw", cal_thaw, 0);

    rb_define_alias(gCalendar, "date", "get_date");
    rb_define_alias(gCalendar, "year", "get_year");
    rb_define_alias(gCalendar, "month", "get_month");
    rb_define_alias(gCalendar, "day", "get_day");
    rb_define_alias(gCalendar, "mmyyyy=", "select_month");
    rb_define_alias(gCalendar, "day=", "select_day");
}
