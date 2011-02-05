/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangocoverage.c -

  $Author: sakai $
  $Date: 2007/07/08 02:53:10 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoCoverage*)RVAL2BOXED(self, PANGO_TYPE_COVERAGE))


/**********************************/
GType
pango_coverage_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoCoverage",
                    (GBoxedCopyFunc)pango_coverage_copy,
                    (GBoxedFreeFunc)pango_coverage_unref);
    return our_type;
}
/**********************************/

static VALUE
coverage_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_coverage_new());
    return Qnil;
}

static VALUE
coverage_get_level(VALUE self, VALUE index_)
{
    return GENUM2RVAL(pango_coverage_get(_SELF(self), NUM2INT(index_)), 
                      PANGO_TYPE_COVERAGE_LEVEL);
}

static VALUE
coverage_max(VALUE self, VALUE other)
{
    pango_coverage_max(_SELF(self), _SELF(other));
    return self;
}

static VALUE
coverage_set(VALUE self, VALUE index_, VALUE level)
{
    pango_coverage_set(_SELF(self), NUM2INT(index_), 
                       RVAL2GENUM(level, PANGO_TYPE_COVERAGE_LEVEL));
    return self;
}

static VALUE
coverage_to_bytes(VALUE self)
{
    guchar* bytes;
    int n_bytes;
    VALUE ret;

    pango_coverage_to_bytes(_SELF(self), &bytes, &n_bytes);
    ret = rb_str_new((const char*)bytes, n_bytes);
    g_free(bytes);

    return ret;
}

static VALUE
coverage_s_from_bytes(VALUE self, VALUE bytes)
{
    StringValue(bytes);
    return BOXED2RVAL(pango_coverage_from_bytes((guchar*)RVAL2CSTR(bytes), 
                                                RSTRING_LEN(bytes)),
                      PANGO_TYPE_COVERAGE);
}

void
Init_pango_coverage()
{
    VALUE pCoverage = G_DEF_CLASS(PANGO_TYPE_COVERAGE, "Coverage", mPango);
    
    rb_define_method(pCoverage, "initialize", coverage_initialize, 0);
    rb_define_method(pCoverage, "get_level", coverage_get_level, 0);
    rb_define_method(pCoverage, "max!", coverage_max, 1);
    rb_define_method(pCoverage, "set", coverage_set, 2);
    rb_define_method(pCoverage, "to_bytes", coverage_to_bytes, 0);
    rb_define_singleton_method(pCoverage, "from_bytes", coverage_s_from_bytes, 1);

    /* PangoCoverageLevel */
    G_DEF_CLASS(PANGO_TYPE_COVERAGE_LEVEL, "Level", pCoverage);
    G_DEF_CONSTANTS(pCoverage, PANGO_TYPE_COVERAGE_LEVEL, "PANGO_COVERAGE_");
}
