/*
 *  Copyright (C) 2016-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cTimeZone
#define _SELF(s) ((GTimeZone*)RVAL2BOXED(s, G_TYPE_TIME_ZONE))

static VALUE
rg_initialize(gint argc, VALUE *argv, VALUE self)
{
    VALUE rb_identifier = Qnil;
    const gchar *identifier = NULL;
    GTimeZone *time_zone = NULL;

    rb_scan_args(argc, argv, "01", &rb_identifier);

    if (!NIL_P(rb_identifier))
        identifier = RVAL2CSTR(rb_identifier);

    time_zone = g_time_zone_new(identifier);
    G_INITIALIZE(self, time_zone);
    return Qnil;
}

static VALUE
rg_s_local(G_GNUC_UNUSED VALUE self)
{
    GTimeZone *time_zone = NULL;
    time_zone = g_time_zone_new_local();
    return GTIMEZONE2RVAL(time_zone);
}

static VALUE
rg_s_utc(G_GNUC_UNUSED VALUE self)
{
    GTimeZone *time_zone = NULL;
    time_zone = g_time_zone_new_utc();
    return GTIMEZONE2RVAL(time_zone);
}

static VALUE
rg_abbreviation(VALUE self, VALUE rb_interval)
{
    gint interval = NUM2INT(rb_interval);
    return CSTR2RVAL(g_time_zone_get_abbreviation(_SELF(self), interval));
}

static VALUE
rg_offset(VALUE self, VALUE rb_interval)
{
    gint interval = NUM2INT(rb_interval);
    return INT2NUM(g_time_zone_get_offset(_SELF(self), interval));
}

void
Init_glib_time_zone(void)
{
    VALUE RG_TARGET_NAMESPACE;
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_TIME_ZONE, "TimeZone", mGLib);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_SMETHOD(local, 0);
    RG_DEF_SMETHOD(utc, 0);
    RG_DEF_METHOD(abbreviation, 1);
    RG_DEF_METHOD(offset, 1);
}
