/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cAnalysis
#define _SELF(s) (RVAL2PANGOANALYSIS(s))

/**********************************/
static PangoAnalysis*
ana_copy(const PangoAnalysis* val)
{
    PangoAnalysis* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(PangoAnalysis, 1);
    *new_val = *val;
    return new_val;
}

GType
pango_analysis_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoAnalysis",
                    (GBoxedCopyFunc)ana_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
rg_initialize(VALUE self)
{
    PangoAnalysis ana = { NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL };
    G_INITIALIZE(self, &ana);
    return Qnil;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rg_set_shape_engine(VALUE self, VALUE engine)
{
    _SELF(self)->shape_engine = RVAL2PANGOENGINESHAPE(engine);
    return self;
}
static VALUE
rg_shape_engine(VALUE self)
{
    VALUE ret;
    if (_SELF(self)->shape_engine){
        const gchar* gname = G_OBJECT_TYPE_NAME(_SELF(self)->shape_engine);
        gchar* name = g_strdup(gname);
        name[0] = g_ascii_toupper(name[0]);
        G_DEF_CLASS3(gname, name, mPango);
        ret = GOBJ2RVAL(_SELF(self)->shape_engine);
    } else {
        ret = Qnil;
    }
    return ret;
}

static VALUE
rg_set_lang_engine(VALUE self, VALUE engine)
{
    _SELF(self)->lang_engine = RVAL2PANGOENGINELANG(engine);
    return self;
}
static VALUE
rg_lang_engine(VALUE self)
{
    VALUE ret;
    if (_SELF(self)->lang_engine){
        const gchar* gname = G_OBJECT_TYPE_NAME(_SELF(self)->lang_engine);
        gchar* name = g_strdup(gname);
        name[0] = g_ascii_toupper(name[0]);
        G_DEF_CLASS3(name, name, mPango);
        ret = GOBJ2RVAL(_SELF(self)->lang_engine);
    } else {
        ret = Qnil;
    }
    return ret;
}
#endif

static VALUE
rg_set_font(VALUE self, VALUE font)
{
    _SELF(self)->font = RVAL2PANGOFONT(font);
    return self;
}

static VALUE
rg_font(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->font);
}

static VALUE
rg_set_level(VALUE self, VALUE level)
{
    _SELF(self)->level = NUM2UINT(level);
    return self;
}

static VALUE
rg_level(VALUE self)
{
    return UINT2NUM(_SELF(self)->level);
}

static VALUE
rg_set_language(VALUE self, VALUE lang)
{
    _SELF(self)->language = RVAL2PANGOLANGUAGE(lang);
    return self;
}

static VALUE
rg_language(VALUE self)
{
    return PANGOLANGUAGE2RVAL(_SELF(self)->language);
}

struct ana_set_extra_attrs_args {
    PangoAnalysis *analysis;
    VALUE ary;
    long n;
    GSList *result;
};

static VALUE
ana_set_extra_attrs_body(VALUE value)
{
    struct ana_set_extra_attrs_args *args = (struct ana_set_extra_attrs_args *)value;
    long i;

    for (i = 0; i < args->n; i++)
        args->result = g_slist_append(args->result, RVAL2ATTR(RARRAY_PTR(args->ary)[i]));

    args->analysis->extra_attrs = args->result;

    return Qnil;
}

static G_GNUC_NORETURN VALUE
ana_set_extra_attrs_rescue(VALUE value)
{
    g_slist_free(((struct ana_set_extra_attrs_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static VALUE
rg_set_extra_attrs(VALUE self, VALUE attrs)
{
    struct ana_set_extra_attrs_args args;
    args.analysis = _SELF(self);
    args.ary = rb_ary_to_ary(attrs);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(ana_set_extra_attrs_body, (VALUE)&args,
              ana_set_extra_attrs_rescue, (VALUE)&args);

    return self;
}

static VALUE
rg_extra_attrs(VALUE self)
{
    VALUE ary = rb_ary_new();
    GSList* list = _SELF(self)->extra_attrs;

    while (list) {
        rb_ary_push(ary, ATTR2RVAL(list->data));
        list = list->next;
    }
    return ary;
}

void
Init_pango_analysis(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_ANALYSIS, "Analysis", mPango);

    RG_DEF_METHOD(initialize, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_METHOD(set_shape_engine, 1);
    RG_DEF_METHOD(shape_engine, 0);
    RG_DEF_METHOD(set_lang_engine, 1);
    RG_DEF_METHOD(lang_engine, 0);
#endif
    RG_DEF_METHOD(set_font, 1);
    RG_DEF_METHOD(font, 0);
    RG_DEF_METHOD(set_level, 1);
    RG_DEF_METHOD(level, 0);
    RG_DEF_METHOD(set_language, 1);
    RG_DEF_METHOD(language, 0);

    RG_DEF_METHOD(set_extra_attrs, 1);
    RG_DEF_METHOD(extra_attrs, 0);
}
