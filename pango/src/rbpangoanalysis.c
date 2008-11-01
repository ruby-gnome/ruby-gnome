/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoanalysis.c -

  $Author: mutoh $
  $Date: 2005/09/17 17:09:13 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(s) ((PangoAnalysis*)RVAL2BOXED(s, PANGO_TYPE_ANALYSIS))

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
ana_initialize(self)
    VALUE self;
{
    PangoAnalysis ana = { NULL, NULL, NULL, 0 };
    G_INITIALIZE(self, &ana);
    return Qnil;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
ana_set_shape_engine(self, engine)
    VALUE self, engine;
{
    _SELF(self)->shape_engine = PANGO_ENGINE_SHAPE(RVAL2GOBJ(engine));
    return self;
}
static VALUE
ana_get_shape_engine(self)
    VALUE self;
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
ana_set_lang_engine(self, engine)
    VALUE self, engine;
{
    _SELF(self)->lang_engine = PANGO_ENGINE_LANG(RVAL2GOBJ(engine));
    return self;
}
static VALUE
ana_get_lang_engine(self)
    VALUE self;
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
ana_set_font(self, font)
    VALUE self, font;
{
    _SELF(self)->font = PANGO_FONT(RVAL2GOBJ(font));
    return self;
}

static VALUE
ana_get_font(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->font);
}

static VALUE
ana_set_level(self, level)
    VALUE self, level;
{
    _SELF(self)->level = NUM2UINT(level);
    return self;
}

static VALUE
ana_get_level(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->level);
}

static VALUE
ana_set_language(self, lang)
    VALUE self, lang;
{
    _SELF(self)->language = RVAL2BOXED(lang, PANGO_TYPE_LANGUAGE);
    return self;
}

static VALUE
ana_get_language(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->language, PANGO_TYPE_LANGUAGE);
}

static VALUE
ana_set_extra_attrs(self, attrs)
    VALUE self, attrs;
{
    gint i;
    gint len = RARRAY_LEN(attrs);
    GSList* gattrs = NULL;

    for (i = 0; i < len; i++){
        gattrs = g_slist_append(gattrs, RVAL2ATTR(RARRAY_PTR(attrs)[i]));
    }

    _SELF(self)->extra_attrs = gattrs;
    return self;
}

static VALUE
ana_get_extra_attrs(self)
    VALUE self;
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
Init_pango_analysis()
{
    VALUE pana = G_DEF_CLASS(PANGO_TYPE_ANALYSIS, "Analysis", mPango);
    
    rb_define_method(pana, "initialize", ana_initialize, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pana, "set_shape_engine", ana_set_shape_engine, 1);
    rb_define_method(pana, "shape_engine", ana_get_shape_engine, 0);
    rb_define_method(pana, "set_lang_engine", ana_set_lang_engine, 1);
    rb_define_method(pana, "lang_engine", ana_get_lang_engine, 0);
#endif
    rb_define_method(pana, "set_font", ana_set_font, 1);
    rb_define_method(pana, "font", ana_get_font, 0);
    rb_define_method(pana, "set_level", ana_set_level, 1);
    rb_define_method(pana, "level", ana_get_level, 0);
    rb_define_method(pana, "set_language", ana_set_language, 1);
    rb_define_method(pana, "language", ana_get_language, 0);

    rb_define_method(pana, "set_extra_attrs", ana_set_extra_attrs, 1);
    rb_define_method(pana, "extra_attrs", ana_get_extra_attrs, 0);

    G_DEF_SETTERS(pana);
}
