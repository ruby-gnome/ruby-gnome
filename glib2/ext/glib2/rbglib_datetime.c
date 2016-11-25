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
    gchar *format = RVAL2CSTR(rb_format);
    return CSTR2RVAL(g_date_time_format(_SELF(self), format));
}

void
Init_glib_date_time(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_DATE_TIME, "DateTime", mGLib);
    RG_DEF_SMETHOD(now, -1);
    RG_DEF_METHOD(format, 1);
}
