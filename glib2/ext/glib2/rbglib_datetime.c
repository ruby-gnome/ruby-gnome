/*
 *  Copyright (C) 2016  Ruby-GNOME2 Project Team
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

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE cDateTime
#define _SELF(s) ((GDateTime*)RVAL2BOXED(s, G_TYPE_DATE_TIME))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    /*
     * Not implemented:
     * GDateTime * 	g_date_time_new_from_timeval_local ()
     * GDateTime * 	g_date_time_new_from_timeval_utc ()
     * https://developer.gnome.org/glib/stable/glib-Date-and-Time-Functions.html#GTimeVal
     * */
    VALUE rb_options;
    VALUE rb_unix_local;
    VALUE rb_unix_utc;
    VALUE rb_timezone;
    VALUE rb_year;
    VALUE rb_month;
    VALUE rb_day;
    VALUE rb_hour;
    VALUE rb_minute;
    VALUE rb_seconds;
    VALUE rb_offset;
    gint year = 0;
    gint month = 0;
    gint day = 0;
    gint hour = 0;
    gint minute = 0;
    gdouble seconds = 0.0;
    GTimeZone *timezone = NULL;
    GDateTime *datetime = NULL;

    rb_scan_args(argc, argv, "10", &rb_options);
    rbg_scan_options(rb_options,
                     "unix_local", &rb_unix_local,
                     "unix_utc", &rb_unix_utc,
                     "timezone", &rb_timezone,
                     "year", &rb_year,
                     "month", &rb_month,
                     "day", &rb_day,
                     "hour", &rb_hour,
                     "minute", &rb_minute,
                     "second", &rb_seconds,
                     "offset", &rb_offset,
                     NULL);

    int full_info = !NIL_P(rb_year) && !NIL_P(rb_month) && !NIL_P(rb_hour) &&
                    !NIL_P(rb_minute) && !NIL_P(rb_seconds);

    if (!NIL_P(rb_unix_local))
        datetime = g_date_time_new_from_unix_local(rbglib_num_to_int64(rb_unix_local));
    else if (!NIL_P(rb_unix_utc))
        datetime = g_date_time_new_from_unix_utc(rbglib_num_to_int64(rb_unix_utc));
    else if (full_info) {
        year = NUM2INT(rb_year);
        month = NUM2INT(rb_month);
        day = NUM2INT(rb_day);
        hour = NUM2INT(rb_hour);
        minute = NUM2INT(rb_minute);
        seconds = NUM2DBL(rb_seconds);
        if (!NIL_P(rb_timezone)) {
            timezone = RVAL2GTIMEZONE(rb_timezone);
            datetime = g_date_time_new(timezone, year, month, day, hour,
                                       minute, seconds);
        }
        else if (strcmp(RVAL2CSTR(rb_offset), "utc"))
            datetime = g_date_time_new_utc(year, month, day, hour, minute, seconds);

        else if (strcmp(RVAL2CSTR(rb_offset), "local"))
            datetime = g_date_time_new_local(year, month, day, hour, minute, seconds);

        else
            rb_raise(rb_eArgError, "wrong options: offset value (\":utc\" or \":local\"))");
    }
    else
        rb_raise(rb_eArgError, "wrong options usage");

    G_INITIALIZE(self, datetime);
    return Qnil;
}

static VALUE
rg_year(VALUE self)
{
    return INT2NUM(g_date_time_get_year(_SELF(self)));
}

static VALUE
rg_month(VALUE self)
{
    return INT2NUM(g_date_time_get_month(_SELF(self)));
}

static VALUE
rg_day_of_month(VALUE self)
{
    return INT2NUM(g_date_time_get_day_of_month(_SELF(self)));
}

static VALUE
rg_hour(VALUE self)
{
    return INT2NUM(g_date_time_get_hour(_SELF(self)));
}

static VALUE
rg_minute(VALUE self)
{
    return INT2NUM(g_date_time_get_minute(_SELF(self)));
}

static VALUE
rg_second(VALUE self)
{
    return DBL2NUM(g_date_time_get_second(_SELF(self)));
}

static VALUE
rg_s_now(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    GDateTime *date = NULL;
    VALUE rb_timezone;
    ID id_equal;
    ID id_local;
    ID id_utc;

    rb_scan_args(argc, argv, "01", &rb_timezone);

    CONST_ID(id_equal, "==");
    CONST_ID(id_local, "local");
    CONST_ID(id_utc, "utc");
    if (NIL_P(rb_timezone) ||
        RVAL2CBOOL(rb_funcall(rb_timezone, id_equal, 1,
                              rb_id2sym(id_local)))) {
        date = g_date_time_new_now_local();
    } else if (RVAL2CBOOL(rb_funcall(rb_timezone, id_equal, 1,
                                     rb_id2sym(id_utc)))) {
        date = g_date_time_new_now_utc();
/* TODO: Support GLib::TimeZone */
    } else {
        rb_raise(rb_eArgError,
                 "timezone must be nil, :local, :utc or GLib::TimeZone: "
                 "%+" PRIsVALUE,
                 rb_timezone);
    }

    return GDATETIME2RVAL(date);
}

static VALUE
rg_format(VALUE self, VALUE rb_format)
{
    const gchar *format = RVAL2CSTR(rb_format);
    return CSTR2RVAL(g_date_time_format(_SELF(self), format));
}

void
Init_glib_date_time(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_DATE_TIME, "DateTime", mGLib);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(year, 0);
    RG_DEF_METHOD(month, 0);
    RG_DEF_METHOD(day_of_month, 0);
    RG_DEF_METHOD(hour, 0);
    RG_DEF_METHOD(minute, 0);
    RG_DEF_METHOD(second, 0);
    RG_DEF_SMETHOD(now, -1);
    RG_DEF_METHOD(format, 1);
}
