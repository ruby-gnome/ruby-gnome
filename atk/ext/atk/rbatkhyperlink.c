/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkhyperlink.c -

  $Author: mutoh $
  $Date: 2004/10/17 23:06:07 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_HYPERLINK(RVAL2GOBJ(s)))

static VALUE
rbatk_hl_get_uri(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_hyperlink_get_uri(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_hl_get_object(VALUE self, VALUE i)
{
    return GOBJ2RVAL(atk_hyperlink_get_object(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_hl_get_end_index(VALUE self)
{
    return INT2NUM(atk_hyperlink_get_end_index(_SELF(self)));
}

static VALUE
rbatk_hl_get_start_index(VALUE self)
{
    return INT2NUM(atk_hyperlink_get_start_index(_SELF(self)));
}

static VALUE
rbatk_hl_is_valid(VALUE self)
{
    return CBOOL2RVAL(atk_hyperlink_is_valid(_SELF(self)));
}

#ifdef HAVE_ATK_HYPERLINK_IS_INLINE
static VALUE
rbatk_hl_is_inline(VALUE self)
{
    return CBOOL2RVAL(atk_hyperlink_is_inline(_SELF(self)));
}
#endif

static VALUE
rbatk_hl_get_n_anchors(VALUE self)
{
    return INT2NUM(atk_hyperlink_get_n_anchors(_SELF(self)));
}

#ifdef HAVE_ATK_HYPERLINK_IS_SELECTED_LINK
static VALUE
rbatk_hl_is_selected_link(VALUE self)
{
    return CBOOL2RVAL(atk_hyperlink_is_selected_link(_SELF(self)));
}
#endif

void
Init_atk_hyperlink()
{
    VALUE hl = G_DEF_CLASS(ATK_TYPE_HYPERLINK, "Hyperlink", mAtk);

    rb_define_method(hl, "get_uri", rbatk_hl_get_uri, 1);
    rb_define_method(hl, "get_object", rbatk_hl_get_object, 1);
    rb_define_method(hl, "end_index", rbatk_hl_get_end_index, 0);
    rb_define_method(hl, "start_index", rbatk_hl_get_start_index, 0);
    rb_define_method(hl, "valid?", rbatk_hl_is_valid, 0);
#ifdef HAVE_ATK_HYPERLINK_IS_INLINE
    rb_define_method(hl, "inline?", rbatk_hl_is_inline, 0);
#endif
    rb_define_method(hl, "n_anchors", rbatk_hl_get_n_anchors, 0);
#ifdef HAVE_ATK_HYPERLINK_IS_SELECTED_LINK
    rb_define_method(hl, "selected_link?", rbatk_hl_is_selected_link, 0);
#endif
}
