/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

#include "rbgprivate.h"

VALUE
rbgutil_glist2ary(const GList *const list)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_and_free_body(VALUE data)
{
    VALUE ary;
    GList *i;

    ary = rb_ary_new();
    for (i = (GList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_and_free_ensure(VALUE data)
{
    g_list_free((GList *)data);

    return Qnil;
}

VALUE
rbgutil_glist2ary_and_free(GList *const list)
{
    return rb_ensure(rbgutil_glist2ary_and_free_body, (VALUE)list,
                     rbgutil_glist2ary_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_glist2ary_boxed(const GList *const list, GType gtype)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, gtype));

    return ary;
}

struct rbgutil_glist2ary_boxed_and_free_data
{
    GList *const list;
    GType gtype;
};

static VALUE
rbgutil_glist2ary_boxed_and_free_body(VALUE data)
{
    struct rbgutil_glist2ary_boxed_and_free_data *real;
    VALUE ary;
    GList *i;

    real = (struct rbgutil_glist2ary_boxed_and_free_data *)data;
    ary = rb_ary_new();
    for (i = real->list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, real->gtype));

    return ary;
}

static VALUE
rbgutil_glist2ary_boxed_and_free_ensure(VALUE data)
{
    g_list_free(((struct rbgutil_glist2ary_boxed_and_free_data *)data)->list);

    return Qnil;
}

VALUE
rbgutil_glist2ary_boxed_and_free(GList *const list, GType gtype)
{
    struct rbgutil_glist2ary_boxed_and_free_data data = { list, gtype };

    return rb_ensure(rbgutil_glist2ary_boxed_and_free_body, (VALUE)&data,
                     rbgutil_glist2ary_boxed_and_free_ensure, (VALUE)&data);
}

VALUE
rbgutil_glist2ary_string(const GList *const list)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, CSTR2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_string_and_free_body(VALUE data)
{
    VALUE ary;
    GList *i;

    ary = rb_ary_new();
    for (i = (GList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, CSTR2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_string_and_free_ensure(VALUE data)
{
    GList *i;

    for (i = (GList *)data; i != NULL; i = i->next)
        g_free(i->data);
    g_list_free((GList *)data);

    return Qnil;
}

VALUE
rbgutil_glist2ary_string_and_free(GList *const list)
{
    return rb_ensure(rbgutil_glist2ary_string_and_free_body, (VALUE)list,
                     rbgutil_glist2ary_string_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_gslist2ary(const GSList *const list)
{
    VALUE ary;
    const GSList *i;
    
    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_gslist2ary_and_free_body(VALUE data)
{
    VALUE ary;
    GSList *i;

    ary = rb_ary_new();
    for (i = (GSList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_gslist2ary_and_free_ensure(VALUE data)
{
    g_slist_free((GSList *)data);

    return Qnil;
}

VALUE
rbgutil_gslist2ary_and_free(GSList *const list)
{
    return rb_ensure(rbgutil_gslist2ary_and_free_body, (VALUE)list,
                     rbgutil_gslist2ary_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_gslist2ary_boxed(const GSList *const list, GType gtype)
{
    VALUE ary;
    const GSList *i;
    
    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, gtype));

    return ary;
}

struct rbgutil_gslist2ary_boxed_and_free_data
{
    GSList *const list;
    GType gtype;
};

static VALUE
rbgutil_gslist2ary_boxed_and_free_body(VALUE data)
{
    struct rbgutil_gslist2ary_boxed_and_free_data *real;
    VALUE ary;
    GSList *i;

    real = (struct rbgutil_gslist2ary_boxed_and_free_data *)data;
    ary = rb_ary_new();
    for (i = real->list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, real->gtype));

    return ary;
}

static VALUE
rbgutil_gslist2ary_boxed_and_free_ensure(VALUE data)
{
    g_slist_free(((struct rbgutil_gslist2ary_boxed_and_free_data *)data)->list);

    return Qnil;
}

VALUE
rbgutil_gslist2ary_boxed_and_free(GSList *const list, GType gtype)
{
    struct rbgutil_gslist2ary_boxed_and_free_data data = { list, gtype };

    return rb_ensure(rbgutil_gslist2ary_boxed_and_free_body, (VALUE)&data,
                     rbgutil_gslist2ary_boxed_and_free_ensure, (VALUE)&data);
}

