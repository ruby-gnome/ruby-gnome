
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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

VALUE cGdaValue;

GdaQuarkList *gda_quark_list_copy(list)
    GdaQuarkList *list;
{
    /* FIXME */
    return list;
}

GType gda_quarklist_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaQuarkList",
            (GBoxedCopyFunc)gda_quark_list_copy,
            (GBoxedFreeFunc)gda_quark_list_free);
    }
    return our_type;
}

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

static VALUE rb_gda_quarklist_find(self, name)
    VALUE self, name;
{
    const gchar *res = gda_quark_list_find(RGDA_QUARKLIST(self),
                                           RVAL2CSTR(name));
    return res != NULL
        ? CSTR2RVAL(res)
        : Qnil;
}

static VALUE rb_gda_quarklist_clear(self)   
    VALUE self;
{
    gda_quark_list_clear(RGDA_QUARKLIST(self));
    return self;
}

void Init_gda_quarklist(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_QUARKLIST, "QuarkList", mGda);
    rb_define_method(c, "initialize", rb_gda_quarklist_new, -1);
    rb_define_method(c, "add_from_string", rb_gda_quarklist_add_from_string, -1);
    rb_define_alias(c, "add", "add_from_string");
    rb_define_method(c, "find", rb_gda_quarklist_find, 1);
    rb_define_method(c, "clear", rb_gda_quarklist_clear, 0);
}

