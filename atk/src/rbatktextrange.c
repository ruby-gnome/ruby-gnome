/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatktextrange.c -

  $Author: mutoh $
  $Date: 2004/10/19 13:33:43 $

  Copyright (C) 2004 Masao Mutoh

  This file is derived from rbgdkrange.c.
  rbgdkrange.c -
  Copyright (C) 2002,2003 Masao Mutoh

  rbgdkrange.c file is derived from rbgdkregion.c.
  rbgdkregion.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "rbatk.h"

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#define _SELF(r) ((AtkTextRange*)RVAL2BOXED(r, ATK_TYPE_TEXT_RANGE))

/**********************************/
static AtkTextRange*
atk_text_range_copy(const AtkTextRange* val)
{
    AtkTextRange* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(AtkTextRange, 1);
    *new_val = *val;
    return new_val;
}

GType
atk_text_range_get_type(void)
{
    static GType our_type = 0;
                                                                                
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("AtkTextRange",
                    (GBoxedCopyFunc)atk_text_range_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/
/* Struct accessors */
static VALUE
atktextrange_start_offset(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->start_offset);
}

static VALUE
atktextrange_end_offset(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->end_offset);
}

static VALUE
atktextrange_content(self)
    VALUE self;
{
    return CSTR2RVAL(_SELF(self)->content);
}
#endif

void
Init_atk_text_range()
{
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
    VALUE range = G_DEF_CLASS4(ATK_TYPE_TEXT_RANGE, "TextRange", 
                               mAtk, GTYPE2CLASS(ATK_TYPE_TEXT_RECTANGLE));

    rb_define_method(range, "start_offset", atktextrange_start_offset, 0);
    rb_define_method(range, "end_offset", atktextrange_end_offset, 0);
    rb_define_method(range, "content", atktextrange_content, 0);
#endif
}
