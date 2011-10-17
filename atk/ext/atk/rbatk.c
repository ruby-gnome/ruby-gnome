/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatk.c -

  $Author: mutoh $
  $Date: 2005/09/28 17:38:25 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "rbatkprivate.h"

VALUE mAtk;

extern void Init_atk(void);

void
Init_atk(void)
{
    mAtk = rb_define_module("Atk");

    rb_define_const(mAtk, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(ATK_MAJOR_VERSION),
                                INT2FIX(ATK_MINOR_VERSION),
                                INT2FIX(ATK_MICRO_VERSION)));

    Init_atk_action();
    Init_atk_component();
    Init_atk_document();
    Init_atk_editabletext();
    Init_atk_gobjectaccessible();
    Init_atk_hyperlink();
    Init_atk_hypertext();
    Init_atk_image();
    Init_atk_implementor();
    Init_atk_noopobject();
    Init_atk_noopobjectfactory();
    Init_atk_object();
    Init_atk_objectfactory();
    Init_atk_registry();
    Init_atk_relation();
    Init_atk_relation_set();
    Init_atk_selection();
    Init_atk_state();
    Init_atk_state_set();
    Init_atk_streamable_content();
    Init_atk_table();
    Init_atk_text();
    Init_atk_text_range();
    Init_atk_text_rectangle();
    Init_atk_util();
    Init_atk_value();
}
