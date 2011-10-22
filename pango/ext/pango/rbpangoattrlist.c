/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbpangoprivate.h"

static ID id_call;

#define _SELF(self) ((PangoAttrList*)RVAL2BOXED(self, PANGO_TYPE_ATTR_LIST))

static VALUE
attrlist_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_attr_list_new());
    return Qnil;
}

static VALUE
attrlist_insert(VALUE self, VALUE attr)
{
    pango_attr_list_insert(_SELF(self), RVAL2ATTR(attr));
    return self;
}

static VALUE
attrlist_insert_before(VALUE self, VALUE attr)
{
    pango_attr_list_insert_before(_SELF(self), RVAL2ATTR(attr));
    return self;
}

static VALUE
attrlist_change(VALUE self, VALUE attr)
{
    pango_attr_list_change(_SELF(self), RVAL2ATTR(attr));
    return self;
}

static VALUE
attrlist_splice(VALUE self, VALUE other, VALUE pos, VALUE len)
{
    pango_attr_list_splice(_SELF(self), _SELF(other), NUM2INT(pos), NUM2INT(len));
    return self;
}

#if PANGO_CHECK_VERSION(1,4,0)
static gboolean
filter_func(PangoAttribute *attr, gpointer data)
{
    return CBOOL2RVAL(rb_funcall((VALUE)data, id_call, 1, ATTR2RVAL(attr)));
}

static VALUE
attrlist_filter(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return BOXED2RVAL(pango_attr_list_filter(_SELF(self), 
                                             (PangoAttrFilterFunc)filter_func, 
                                             (gpointer)func),
                      PANGO_TYPE_ATTR_LIST);
}
#endif

static VALUE
attrlist_get_iterator(VALUE self)
{
    return BOXED2RVAL(pango_attr_list_get_iterator(_SELF(self)), PANGO_TYPE_ATTR_ITERATOR);
}

void
Init_pango_attrlist(void)
{
    VALUE pAttrlist = G_DEF_CLASS(PANGO_TYPE_ATTR_LIST, "AttrList", mPango);

    id_call = rb_intern("call");

    rb_define_method(pAttrlist, "initialize", attrlist_initialize, 0);
    rb_define_method(pAttrlist, "insert", attrlist_insert, 1);
    rb_define_method(pAttrlist, "insert_before", attrlist_insert_before, 1);
    rb_define_method(pAttrlist, "change", attrlist_change, 1);
    rb_define_method(pAttrlist, "splice", attrlist_splice, 3);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pAttrlist, "filter", attrlist_filter, 0);
#endif
    rb_define_method(pAttrlist, "iterator", attrlist_get_iterator, 0);

}
