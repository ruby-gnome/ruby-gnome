/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  global.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE
glist2ary(list)
    GList *list; 
{
    VALUE ary = rb_ary_new();

    while (list) {
		rb_ary_push(ary, GOBJ2RVAL(list->data));
		list = list->next;
    }

    return ary;
}

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

VALUE
gslist2ary(list)
    GSList *list; 
{
    VALUE ary = rb_ary_new();

    while (list) {
		rb_ary_push(ary, GOBJ2RVAL(list->data));
		list = list->next;
    }

    return ary;
}

