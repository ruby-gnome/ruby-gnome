/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.h -

  $Author: mutoh $
  $Date: 2002/09/30 14:53:08 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#ifndef __RBGUTIL_H__
#define __RBGUTIL_H__

#include <glib.h>
#include <glib-object.h>
#include "ruby.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define G_SET_PROPERTIES(self, hash) (rbgutil_set_properties(self, hash))

#define GLIST2ARY(list)              (rbgutil_glist2ary(list))
#define GLIST2ARY2(list, gtype)      (rbgutil_glist2ary_boxed(list, gtype))
#define GSLIST2ARY(list)             (rbgutil_gslist2ary(list))
#define GSLIST2ARY2(list, gtype)     (rbgutil_gslist2ary_boxed(list, gtype))

    extern void rbgutil_set_properties(VALUE self, VALUE hash);
    extern VALUE rbgutil_glist2ary(GList* list);
    extern VALUE rbgutil_glist2ary_boxed(GList* list, GType gtype);
    extern VALUE rbgutil_gslist2ary(GSList* list);
    extern VALUE rbgutil_gslist2ary_boxed(GSList* list, GType gtype);
    extern void Init_gutil();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
