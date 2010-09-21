/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkhypertext.c -

  $Author: mutoh $
  $Date: 2003/12/07 17:18:16 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_HYPERTEXT(RVAL2GOBJ(s)))

static VALUE
rbatk_ht_get_link(self, link_index)
    VALUE self, link_index;
{
    return GOBJ2RVAL(atk_hypertext_get_link(_SELF(self), NUM2INT(link_index)));
}

static VALUE
rbatk_ht_get_n_links(self)
    VALUE self;
{
    return INT2NUM(atk_hypertext_get_n_links(_SELF(self)));
}

static VALUE
rbatk_ht_get_link_index(self, char_index)
    VALUE self, char_index;
{
    return INT2NUM(atk_hypertext_get_link_index(_SELF(self), NUM2INT(char_index)));
}

void
Init_atk_hypertext()
{
    VALUE ht = G_DEF_INTERFACE(ATK_TYPE_HYPERTEXT, "Hypertext", mAtk);

    rb_define_method(ht, "get_link", rbatk_ht_get_link, 1);
    rb_define_method(ht, "n_links", rbatk_ht_get_n_links, 0);
    rb_define_method(ht, "link_index", rbatk_ht_get_link_index, 1);
}
