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
rg_s_now_local(VALUE self)
{
    GDateTime *date = NULL;
    date = g_date_time_new_now_local();
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
    RG_DEF_SMETHOD(now_local, 0);
    RG_DEF_METHOD(format, 1);
}
