/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#ifndef __RBGUTILDEPRECATED_H__
#define __RBGUTILDEPRECATED_H__

G_BEGIN_DECLS

#define G_DEF_SETTER(klass, name) \
    rb_funcall(klass, rbgutil_id_module_eval, 1, rb_str_new2( \
    "def " name "=(val); set_" name "(val); val; end\n"))
#define G_DEF_SETTERS(klass) rbgutil_def_setters(klass)

#define GLIST2ARY(list)           (rbgutil_glist2ary(list))
#define GLIST2ARY_FREE(list)      (rbgutil_glist2ary_and_free(list))
#define GLIST2ARYF(list)          (GLIST2ARY_FREE(list))
#define GLIST2ARY2(list, gtype)   (rbgutil_glist2ary_boxed(list, gtype))
#define GLIST2ARY2F(list, gtype)  (rbgutil_glist2ary_boxed_and_free(list, gtype))
#define GLIST2ARY_STR(list)       (rbgutil_glist2ary_string(list))
#define GLIST2ARY_STR_FREE(list)  (rbgutil_glist2ary_string_and_free(list))
#define GSLIST2ARY(list)          (rbgutil_gslist2ary(list))
#define GSLIST2ARY_FREE(list)     (rbgutil_gslist2ary_and_free(list))
#define GSLIST2ARYF(list)         (GSLIST2ARY_FREE(list))
#define GSLIST2ARY2(list, gtype)  (rbgutil_gslist2ary_boxed(list, gtype))
#define GSLIST2ARY2F(list, gtype) (rbgutil_gslist2ary_boxed_and_free(list, gtype))

#define G_SET_SYMBOL_PROPERTY(gtype, name) \
     rbgobj_register_property_getter(gtype, name, rbgutil_sym_g2r_func)

#define G_BLOCK_PROC rb_block_proc

extern VALUE rbgutil_glist2ary(const GList *list);
extern VALUE rbgutil_glist2ary_and_free(GList* list);
extern VALUE rbgutil_glist2ary_boxed(const GList *list, GType gtype);
extern VALUE rbgutil_glist2ary_boxed_and_free(GList *list, GType gtype);
extern VALUE rbgutil_glist2ary_string(const GList *list);
extern VALUE rbgutil_glist2ary_string_and_free(GList *list);
extern VALUE rbgutil_gslist2ary(const GSList *list);
extern VALUE rbgutil_gslist2ary_and_free(GSList *list);
extern VALUE rbgutil_gslist2ary_boxed(const GSList *list, GType gtype);
extern VALUE rbgutil_gslist2ary_boxed_and_free(GSList *list, GType gtype);

G_END_DECLS

#endif /* __RBGUTILDEPRECATED_H__ */
