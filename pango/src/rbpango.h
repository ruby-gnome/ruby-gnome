/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.h -

  $Author: mutoh $
  $Date: 2003/01/10 19:22:13 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "ruby.h"
#include <pango/pango.h>
#include "rbgobject.h"

extern void Init_pango_inits();
extern VALUE mPango;

#define CBOOL2RVAL(b)   ((b) ? Qtrue : Qfalse)

#define PANGO_TYPE_ITEM (pango_item_get_type())
#define PANGO_TYPE_LAYOUT_ITER (pango_layout_iter_get_type())
#define PANGO_TYPE_LAYOUT_LINE (pango_layout_line_get_type())
#define PANGO_TYPE_RECTANGLE (pango_rectangle_get_type())
#define PANGO_TYPE_ATTR_ITERATOR (pango_attr_iter_get_type())

#define ATTR2RVAL(attr) (pango_make_attribute(attr))
#define RVAL2ATTR(attr) (pango_get_attribute(attr))
#define ATTRTYPE2CLASS(attr_type) (pango_get_attribute_klass(attr_type))
#define RBPANGO_ADD_ATTRIBUTE(type, klass) (pango_add_attribute(type, klass))

extern VALUE pango_make_attribute(PangoAttribute* attr);
extern PangoAttribute* pango_get_attribute(VALUE attr);

extern GType pango_item_get_type();
extern GType pango_layout_iter_get_type();
extern GType pango_layout_line_get_type();
extern GType pango_rectangle_get_type();
extern GType pango_attr_iter_get_type();
extern VALUE pango_get_attribute_klass(VALUE attr_type);
extern void pango_add_attribute(int attr_type, VALUE klass);
