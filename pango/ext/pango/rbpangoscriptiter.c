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

#define RG_TARGET_NAMESPACE cScriptIter
#define _SELF(r) (RVAL2PANGOSCRIPTITER(r))

static VALUE
rg_initialize(VALUE self, VALUE text)
{
    PangoScriptIter* iter;

    StringValue(text);
    iter = pango_script_iter_new(RSTRING_PTR(text), RSTRING_LEN(text));
    G_INITIALIZE(self, iter);
    return Qnil;
}

static VALUE
rg_range(VALUE self)
{
    G_CONST_RETURN char* start;
    G_CONST_RETURN char* end;
    PangoScript script;

    pango_script_iter_get_range(_SELF(self), &start, &end, &script);

    return rb_ary_new3(3, CSTR2RVAL(start), CSTR2RVAL(end), 
                       PANGOSCRIPT2RVAL(script));
}

static VALUE
rg_next_bang(VALUE self)
{
    return CBOOL2RVAL(pango_script_iter_next(_SELF(self)));
}
#endif

void
Init_pango_script_iter(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_SCRIPT_ITER, "ScriptIter", mPango);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(range, 0);
    RG_DEF_METHOD_BANG(next, 0);
#endif
}
