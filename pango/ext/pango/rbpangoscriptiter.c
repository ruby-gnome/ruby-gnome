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

#if PANGO_CHECK_VERSION(1,4,0)

/**********************************/
static PangoScriptIter*
rbpango_script_iter_copy(PangoScriptIter *ref)
{
/*
  PangoScriptIter* new_ref = pango_script_iter_new("", 0);
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoScriptIter, 1);
  *new_ref = *ref;
  return new_ref;
*/
    return ref;
}

GType
pango_script_iter_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoScriptIter",
                    (GBoxedCopyFunc)rbpango_script_iter_copy,
                    (GBoxedFreeFunc)pango_script_iter_free);
    return our_type;
}
/**********************************/

#define _SELF(r) ((PangoScriptIter*)RVAL2BOXED(r, PANGO_TYPE_SCRIPT_ITER))

static VALUE
rbpango_scriptiter_initialize(VALUE self, VALUE text)
{
    PangoScriptIter* iter;

    StringValue(text);
    iter = pango_script_iter_new(RSTRING_PTR(text), RSTRING_LEN(text));
    G_INITIALIZE(self, iter);
    return Qnil;
}

static VALUE
rbpango_scriptiter_get_range(VALUE self)
{
    G_CONST_RETURN char* start;
    G_CONST_RETURN char* end;
    PangoScript script;

    pango_script_iter_get_range(_SELF(self), &start, &end, &script);

    return rb_ary_new3(3, CSTR2RVAL(start), CSTR2RVAL(end), 
                       GENUM2RVAL(script, PANGO_TYPE_SCRIPT));
}

static VALUE
rbpango_scriptiter_next(VALUE self)
{
    return CBOOL2RVAL(pango_script_iter_next(_SELF(self)));
}
#endif

void
Init_pango_script_iter(void)
{
#if PANGO_CHECK_VERSION(1,4,0)
    VALUE scriter = G_DEF_CLASS(PANGO_TYPE_SCRIPT_ITER, "ScriptIter", mPango);

    rb_define_method(scriter, "initialize", rbpango_scriptiter_initialize, 1);
    rb_define_method(scriter, "range", rbpango_scriptiter_get_range, 0);
    rb_define_method(scriter, "next!", rbpango_scriptiter_next, 0);
#endif
}

