/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-score.c,v 1.2 2003/02/02 12:51:06 tkubo Exp $ */
/* based on libgnome/gnome-score.h */

/* Gnome::Score module for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
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

static ID id_at;

static VALUE
gscore_init(self, gamename)
    VALUE self, gamename;
{
    gint result = gnome_score_init(NIL_P(gamename) ? NULL : RVAL2CSTR(gamename));
    if (result)
        rb_raise(rb_eRuntimeError, "cannot initialize score for %s", RVAL2CSTR(gamename));
    return Qtrue;
}

static VALUE
gscore_log(self, score, level, higher_to_lower_score_order)
    VALUE self, score, level, higher_to_lower_score_order;
{
    gint result = gnome_score_log(NUM2DBL(score),
                                  NIL_P(level) ? NULL : RVAL2CSTR(level),
                                  RTEST(higher_to_lower_score_order));
    return INT2NUM(result);
}

static VALUE
gscore_get_notable(self, gamename, level)
    VALUE self, gamename, level;
{
    gchar **names = NULL;
    gfloat *scores = NULL;
    time_t *scoretimes = NULL;
    gint result;
    int i;
    VALUE ary;
    VALUE entry;

    result = gnome_score_get_notable(NIL_P(gamename) ? NULL : RVAL2CSTR(gamename),
                                     NIL_P(level) ? NULL : RVAL2CSTR(level),
                                     &names, &scores, &scoretimes);
    if (result > 0) {
        ary = rb_ary_new2(result);
        for (i = 0; i < result; i++) {
            entry = rb_ary_new3(3, rb_str_new2(names[i]), rb_float_new(scores[i]),
                                rb_funcall(rb_cTime, id_at, 1, LONG2NUM(scoretimes[i])));
            rb_ary_push(ary, entry);
        }
    } else {
        ary = Qnil;
    }
    g_strfreev(names);
    g_free(scores);
    g_free(scoretimes);
    return ary;
}

void
Init_gnome_score(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeScore = rb_define_module_under(mGnome, "Score");

    id_at = rb_intern("at");

    rb_define_module_function(mGnomeScore, "init", gscore_init, 1);
    rb_define_module_function(mGnomeScore, "log", gscore_log, 3);
    rb_define_module_function(mGnomeScore, "get_notable", gscore_get_notable, 2);
}
