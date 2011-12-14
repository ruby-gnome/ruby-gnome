/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

static VALUE
rbg_filename_gslist_to_array_free_body(VALUE list)
{
    VALUE ary = rb_ary_new();
    GSList *p;

    for (p = (GSList *)list; p != NULL; p = g_slist_next(p))
        rb_ary_push(ary, CSTRFILENAME2RVAL(p->data));

    return ary;
}

static VALUE
rbg_filename_gslist_to_array_free_ensure(VALUE val)
{
    GSList *list = (GSList *)val;
    GSList *p;

    for (p = list; p != NULL; p = g_slist_next(p))
        g_free((gchar *)p->data);

    g_slist_free(list);

    return Qnil;
}

VALUE
rbg_filename_gslist_to_array_free(GSList *list)
{
    return rb_ensure(rbg_filename_gslist_to_array_free_body, (VALUE)list,
                     rbg_filename_gslist_to_array_free_ensure, (VALUE)list);
}

