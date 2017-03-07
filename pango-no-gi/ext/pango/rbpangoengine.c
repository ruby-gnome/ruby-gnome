/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbpangoprivate.h"

#ifdef PANGO_TYPE_ENGINE

#define _SELF(self) (RVAL2PANGOENGINE(self))

/* FIXME
static VALUE
rbpango_s_engine_list(VALUE self)
{
    PangoEngineInfo* engines = g_new(PangoEngineInfo, 1);
    int i, n;
    VALUE ary;

    script_engine_list(&engines, &n);
    ary = rb_ary_new();

    for(i = 0; i < n; i++){
        rb_ary_push(ary, GOBJ2RVAL(&engines[i]));
    }
    return ary;
}

static VALUE
rbpango_s_engine_create(VALUE self, VALUE id)
{
    return GOBJ2RVAL(script_engine_create(RVAL2CSTR(id)));
}
*/
/*
void        script_engine_init              (GTypeModule *module);
void        script_engine_exit              (void);
PangoEngine* script_engine_create           (const char *id);
*/

#endif

void
Init_pango_engine(VALUE mPango)
{
#ifdef PANGO_TYPE_ENGINE
    G_DEF_CLASS(PANGO_TYPE_ENGINE, "Engine", mPango);

/* FIXME
    rb_define_singleton_method(engine, "engines", rbpango_s_engine_list, 0);
    rb_define_singleton_method(engine, "create", rbpango_s_engine_create, 1);
*/
    G_DEF_CLASS(PANGO_TYPE_ENGINE_LANG, "EngineLang", mPango);
    G_DEF_CLASS(PANGO_TYPE_ENGINE_SHAPE, "EngineShape", mPango);
#endif
}

