/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_enums.c -

  $Author: sakai $
  $Date: 2002/07/27 14:46:35 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
enums_to_ruby(const GValue* from)
{
    return UINT2NUM(g_value_get_enum(from));
}

static VALUE
flags_to_ruby(const GValue* from)
{
    return UINT2NUM(g_value_get_flags(from));
}

void
Init_gobject_genums()
{
    rbgobj_register_g2r_func(G_TYPE_ENUM,  enums_to_ruby);
    rbgobj_register_g2r_func(G_TYPE_FLAGS, flags_to_ruby);
} 
