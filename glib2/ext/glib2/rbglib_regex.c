/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#ifdef G_OS_WIN32
#include <windows.h>
#endif

#define RG_TARGET_NAMESPACE cRegex
#define _SELF(s) ((GRegex*)RVAL2BOXED(s, G_TYPE_REGEX))
  
static VALUE
rg_initialize(gint argc, VALUE *argv, VALUE self)
{
    VALUE pattern, compile_options, match_options;
    GError *error = NULL;
    GRegex *regex = NULL;

    rb_secure(4);
    rb_scan_args(argc, argv, "31", &pattern, &compile_options, &match_options);

    regex = g_regex_new(RVAL2CSTR(pattern), 
                        NUM2UINT(compile_options),
                        NUM2UINT(match_options),
                        &error);
    
    if(error)
      RAISE_GERROR(error);

    G_INITIALIZE(self, regex); 
    return Qnil;
}
void
Init_glib_regex(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_REGEX, "Regex", mGLib); 

    RG_DEF_METHOD(initialize, -1);
}

