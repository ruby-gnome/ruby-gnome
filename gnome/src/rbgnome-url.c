/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-url.c,v 1.6 2005/09/24 18:02:43 mutoh Exp $ */

/* Gnome::URL module for Ruby/Gnome
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

static VALUE
url_show(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
  VALUE url, env;

    GError *error = NULL;
    gboolean result;

    rb_scan_args(argc, argv, "11", &url, &env);

    if (NIL_P(env)){
        result = gnome_url_show((const char*)RVAL2CSTR(url), &error);
    } else {
#if GNOME_CHECK_VERSION(2,6,0)
      int i, genc;
      char** genvp;
      
      Check_Type(env, T_ARRAY);
      genc = RARRAY_LEN(env);

      genvp = ALLOCA_N(gchar*, genc + 1);
      
      for (i = 0; i < genc; i++) {
        if (TYPE(RARRAY_PTR(env)[i]) == T_STRING) {
          genvp[i] = RVAL2CSTR(RARRAY_PTR(env)[i]);
        }
        else {
          genvp[i] = "";
        }
        genvp[genc] = (gchar*)NULL;
      }
      result = gnome_url_show_with_env((const char*)RVAL2CSTR(url), genvp, &error);
#else
      rb_warn("Ignored 5th argument under this environment. Because it has been supported since GNOME-2.6.");
      result = gnome_url_show((const char*)RVAL2CSTR(url), &error);
#endif      
    }
    if (!result)
        RAISE_GERROR(error);
    return self;
}

void
Init_gnome_url(mGnome)
     VALUE mGnome;
{
    VALUE mGnomeURL = rb_define_module_under(mGnome, "URL");
    rb_define_module_function(mGnomeURL, "show", url_show, -1);

    G_DEF_ERROR(GNOME_URL_ERROR, "URLError", mGnome, rb_eRuntimeError,
                GNOME_TYPE_URL_ERROR);

}
