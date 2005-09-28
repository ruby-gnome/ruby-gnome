/* -*- c-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-help.c,v 1.7 2005/09/28 16:36:39 mutoh Exp $ */
/* based on libgnome/gnome-help.h */

/* Gnome::Help for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
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
gnohelp_m_display(self, file_name, link_id)
    VALUE self, file_name, link_id;
{
    GError *error = NULL;
    gboolean result = gnome_help_display(RVAL2CSTR(file_name),
                                         NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                         &error);
    if (!result)
        RAISE_GERROR(error);
    return self;
}

static VALUE
gnohelp_m_display_with_doc_id(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GError *error = NULL;
    gboolean result;
    VALUE pgm, doc_id, file_name, link_id, env;

    rb_scan_args(argc, argv, "41", &pgm, &doc_id, &file_name, &link_id, &env);

    if (NIL_P(env)){
      result = gnome_help_display_with_doc_id(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                              NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                              RVAL2CSTR(file_name),
                                              NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                              &error);
    } else {
#if GNOME_CHECK_VERSION(2,6,0)
      int i, genc;
      char** genvp;

      Check_Type(env, T_ARRAY);
      genc = RARRAY(env)->len;

      genvp = ALLOCA_N(gchar*, genc + 1);

      for (i = 0; i < genc; i++) {
        if (TYPE(RARRAY(env)->ptr[i]) == T_STRING) {
          genvp[i] = RVAL2CSTR(RARRAY(env)->ptr[i]);
        }
        else {
          genvp[i] = "";
        }
        genvp[genc] = (gchar*)NULL;
      }
      result = gnome_help_display_with_doc_id_and_env(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                                      NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                                      RVAL2CSTR(file_name),
                                                      NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                                      genvp,
                                                      &error);
#else
      rb_warn("Ignored 5th argument under this environment. Because it has been supported since GNOME-2.6.");
      result = gnome_help_display_with_doc_id(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                              NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                              RVAL2CSTR(file_name),
                                              NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                              &error);
#endif
    }

    if (!result)
        RAISE_GERROR(error);
    return self;
}

static VALUE
gnohelp_m_display_desktop(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE pgm, doc_id, file_name, link_id, env;

    GError *error = NULL;
    gboolean result;

    rb_scan_args(argc, argv, "41", &pgm, &doc_id, &file_name, &link_id, &env);

    if (NIL_P(env)){
      result = gnome_help_display_desktop(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                          NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                          RVAL2CSTR(file_name),
                                          NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                          &error);
    } else {
#if GNOME_CHECK_VERSION(2,6,0)
      int i, genc;
      char** genvp;
      
      Check_Type(env, T_ARRAY);
      genc = RARRAY(env)->len;

      genvp = ALLOCA_N(gchar*, genc + 1);
      
      for (i = 0; i < genc; i++) {
        if (TYPE(RARRAY(env)->ptr[i]) == T_STRING) {
          genvp[i] = RVAL2CSTR(RARRAY(env)->ptr[i]);
        }
        else {
          genvp[i] = "";
        }
        genvp[genc] = (gchar*)NULL;
      }
      result = gnome_help_display_desktop_with_env(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                                   NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                                   RVAL2CSTR(file_name),
                                                   NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                                   genvp,
                                                   &error);
#else
      rb_warn("Ignored 5th argument under this environment. Because it has been supported since GNOME-2.6.");
      result = gnome_help_display_desktop(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                          NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                          RVAL2CSTR(file_name),
                                          NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                          &error);
#endif      
    }
    if (!result)
        RAISE_GERROR(error);
    return self;
}

static VALUE
gnohelp_m_display_uri(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
  VALUE help_uri, env;

    GError *error = NULL;
    gboolean result;

    rb_scan_args(argc, argv, "11", &help_uri, &env);

    if (NIL_P(env)){
      result = gnome_help_display_uri((const char*)RVAL2CSTR(help_uri), &error);
    } else {
#if GNOME_CHECK_VERSION(2,6,0)
      int i, genc;
      char** genvp;
      
      Check_Type(env, T_ARRAY);
      genc = RARRAY(env)->len;

      genvp = ALLOCA_N(gchar*, genc + 1);
      
      for (i = 0; i < genc; i++) {
        if (TYPE(RARRAY(env)->ptr[i]) == T_STRING) {
          genvp[i] = RVAL2CSTR(RARRAY(env)->ptr[i]);
        }
        else {
          genvp[i] = "";
        }
        genvp[genc] = (gchar*)NULL;
      }
      result = gnome_help_display_uri_with_env((const char*)RVAL2CSTR(help_uri), genvp, &error);
#else
      rb_warn("Ignored 5th argument under this environment. Because it has been supported since GNOME-2.6.");
      result = gnome_help_display_uri((const char*)RVAL2CSTR(help_uri), &error);
#endif      
    }
    if (!result)
        RAISE_GERROR(error);
    return self;
}

void
Init_gnome_help(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeHelp = rb_define_module_under(mGnome, "Help");

    rb_define_module_function(mGnomeHelp, "display", gnohelp_m_display, 2);
    rb_define_module_function(mGnomeHelp, "display_with_doc_id", gnohelp_m_display_with_doc_id, -1);
    rb_define_module_function(mGnomeHelp, "display_desktop", gnohelp_m_display_desktop, -1);
    rb_define_module_function(mGnomeHelp, "display_uri", gnohelp_m_display_uri, -1);

    G_DEF_ERROR(GNOME_HELP_ERROR, "HelpError", mGnome, rb_eRuntimeError,
                GNOME_TYPE_HELP_ERROR);

}
