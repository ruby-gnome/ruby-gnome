/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  global.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

GList*
ary2glist(ary)
    VALUE ary;
{
    long i;
    GList *glist = NULL;

    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY(ary)->len; i++) {
		glist = g_list_prepend(glist,RVAL2GOBJ(RARRAY(ary)->ptr[i]));
    }

    return g_list_reverse(glist);
}

GSList*
ary2gslist(ary)
    VALUE ary;
{
    long i;
    GSList *glist = NULL;

    if (NIL_P(ary)) return NULL;
    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY(ary)->len; i++) {
		glist = g_slist_append(glist,RVAL2GOBJ(RARRAY(ary)->ptr[i]));
    }

    return glist;
}
