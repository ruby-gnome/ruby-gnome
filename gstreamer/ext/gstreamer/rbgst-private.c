/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#include "rbgst.h"
#include "rbgst-private.h"

char *
_rbgst_inspect(VALUE object)
{
    VALUE inspected;
    inspected = rb_funcall(object, rb_intern("inspect"), 0);
    return StringValueCStr(inspected);
}

void
_rbgst_define_class_if_need(VALUE klass, GType type, const gchar *prefix)
{
    VALUE parent;
    const gchar *type_name;
    gchar *class_name = NULL;
    static ID id_gtype = 0;

    if (rb_class2name(klass)[0] != '#')
        return;

    type_name = g_type_name(type);
    if (g_str_has_prefix(type_name, "Gst"))
        type_name += 3;

    if (prefix)
        class_name = g_strconcat(prefix, type_name, NULL);
    G_DEF_CLASS(type, class_name ? class_name : type_name, mGst);
    g_free(class_name);

    parent = rb_ary_entry(rb_mod_ancestors(klass), 1);
    if (!id_gtype)
        id_gtype = rb_intern("gtype");
    if (rb_respond_to(parent, id_gtype))
        _rbgst_define_class_if_need(parent, CLASS2GTYPE(parent), prefix);
}

VALUE
_rbgst_collect_elements(GstIterator *iter)
{
    VALUE elements;
    gboolean done = FALSE;

    elements = rb_ary_new();
    while (!done) {
        gpointer element;
        switch (gst_iterator_next(iter, &element)) {
          case GST_ITERATOR_OK:
            rb_ary_push(elements, GST_ELEMENT2RVAL(element));
            gst_object_unref(element);
            break;
          case GST_ITERATOR_RESYNC:
            gst_iterator_resync(iter);
            break;
          case GST_ITERATOR_ERROR:
            done = TRUE;
            break;
          case GST_ITERATOR_DONE:
            done = TRUE;
            break;
        }
    }
    gst_iterator_free(iter);

    return elements;
}

