
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

/*
 * Class: Gda::QuarkList
 * Quark lists (lists of KEY=VALUE's).
 */
VALUE cGdaQuarkList;

/*
 * Class method: new(string=nil)
 * string: a connection string.
 *
 * Creates a new Gda::QuarkList, which is a set of key->value pairs, very
 * similar to GLib's GHashTable, but with the only purpose to make easier the
 * parsing and creation of data source connection strings.
 *
 * You can optionally pass a connection string to initialize the quark list.
 *
 * Returns: a newly created Gda::QuarkList object.
 */
static VALUE rb_gda_quarklist_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GdaQuarkList *list;
    VALUE string;

    rb_scan_args(argc, argv, "01", &string);
    if (NIL_P(string)) {
        list = gda_quark_list_new();
    }
    else {
        list = gda_quark_list_new_from_string(RVAL2CSTR(string));
    }
   
    if (list != NULL) {
        G_INITIALIZE(self, list);
    }
    
    return Qnil;
}

/*
 * Method: add_from_string(string, cleanup=false)
 * string: a connection string.
 * cleanup: whether to cleanup the previous content or not.
 *
 * Adds new key->value pairs from the given string. If cleanup is set to true,
 * the previous contents will be discarded before adding the new pairs.
 *
 * Returns: self.
 */
static VALUE rb_gda_quarklist_add_from_string(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE string, cleanup;

    rb_scan_args(argc, argv, "11", &string, &cleanup);
    
    gda_quark_list_add_from_string(RGDA_QUARKLIST(self),
                                   RVAL2CSTR(string),
                                   NIL_P(cleanup) ? FALSE : RVAL2CBOOL(cleanup));
    return self;
}

/*
 * Method: find(name)
 * name: the name of the value to search for.
 *
 * Searches for the value identified by name in the quark list.
 *
 * Returns: the value associated with the given key if found, or nil if not
 * found.
 */
static VALUE rb_gda_quarklist_find(self, name)
    VALUE self, name;
{
    const gchar *res = gda_quark_list_find(RGDA_QUARKLIST(self),
                                           RVAL2CSTR(name));
    return res != NULL
        ? CSTR2RVAL(res)
        : Qnil;
}

/*
 * Method: clear
 *
 * Removes all key->pair values from the quark list.
 *
 * Returns: self.
 */
static VALUE rb_gda_quarklist_clear(self)   
    VALUE self;
{
    gda_quark_list_clear(RGDA_QUARKLIST(self));
    return self;
}

void Init_gda_quarklist(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_QUARK_LIST, "QuarkList", mGda);
    rb_define_method(c, "initialize", rb_gda_quarklist_new, -1);
    rb_define_method(c, "add_from_string", rb_gda_quarklist_add_from_string, -1);
    rb_define_alias(c, "add", "add_from_string");
    rb_define_method(c, "find", rb_gda_quarklist_find, 1);
    rb_define_method(c, "clear", rb_gda_quarklist_clear, 0);

    cGdaQuarkList = c;
}

