/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoengine.c -

  $Author: mutoh $
  $Date: 2005/10/15 04:32:01 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#ifdef PANGO_TYPE_ENGINE

#define _SELF(self) (PANGO_ENGINE(RVAL2GOBJ(self)))

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
Init_pango_engine()
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

