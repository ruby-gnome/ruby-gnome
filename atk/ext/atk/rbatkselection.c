/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkselection.c -

  $Author: mutoh $
  $Date: 2004/03/05 15:33:48 $

  Copyright (C) 2004 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_SELECTION(RVAL2GOBJ(s)))

static VALUE
rbatksel_add_selection(VALUE self, VALUE i)
{
    gboolean ret = atk_selection_add_selection(_SELF(self), NUM2INT(i));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't add selection");
    return self;
}

static VALUE
rbatksel_clear_selection(VALUE self)
{
    gboolean ret = atk_selection_clear_selection(_SELF(self));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't clear selection");
    return self;
}

static VALUE
rbatksel_ref_selection(VALUE self, VALUE i)
{
    return GOBJ2RVAL(atk_selection_ref_selection(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatksel_get_selection_count(VALUE self)
{
    return INT2NUM(atk_selection_get_selection_count(_SELF(self)));
}

static VALUE
rbatksel_is_child_selected(VALUE self, VALUE i)
{
    return CBOOL2RVAL(atk_selection_is_child_selected(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatksel_remove_selection(VALUE self, VALUE i)
{
    gboolean ret = atk_selection_remove_selection(_SELF(self), NUM2INT(i));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't remove selection");
    return self;
}

static VALUE
rbatksel_select_all_selection(VALUE self)
{
    return CBOOL2RVAL(atk_selection_select_all_selection(_SELF(self)));
}

void
Init_atk_selection()
{
    VALUE sel = G_DEF_INTERFACE(ATK_TYPE_SELECTION, "Selection", mAtk);

    rb_define_method(sel, "add_selection", rbatksel_add_selection, 1);
    rb_define_method(sel, "clear_selection", rbatksel_clear_selection, 0);
    rb_define_method(sel, "ref_selection", rbatksel_ref_selection, 1);
    rb_define_method(sel, "selection_count", rbatksel_get_selection_count, 0);
    rb_define_method(sel, "child_selected?", rbatksel_is_child_selected, 1);
    rb_define_method(sel, "remove_selection", rbatksel_remove_selection, 1);
    rb_define_method(sel, "select_all_selection", rbatksel_select_all_selection, 0);
}
