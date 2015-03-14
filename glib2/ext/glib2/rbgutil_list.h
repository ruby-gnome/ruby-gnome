/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2015  Ruby-GNOME2 Project Team
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

#ifndef __RBGUTIL_LIST_H__
#define __RBGUTIL_LIST_H__

G_BEGIN_DECLS

#define CSTRGLIST2RVAL(list) \
        CSTRGLIST2RVAL_FREE(list, NULL, NULL)
#define CSTRGLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_glist2rval(list, (RBGRValueFunc)rbg_cstr2rval, \
                       (GFreeFunc)free_list, (GFreeFunc)free_elem)
#define CSTRGSLIST2RVAL(list) \
        CSTRGSLIST2RVAL_FREE(list, NULL, NULL)
#define CSTRGSLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_gslist2rval(list, (RBGRValueFunc)rbg_cstr2rval, \
                        (GFreeFunc)free_list, (GFreeFunc)free_elem)

#define FILENAMEGLIST2RVAL(list) \
        FILENAMEGLIST2RVAL_FREE(list, NULL, NULL)
#define FILENAMEGLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_glist2rval(list, (RBGRValueFunc)rbg_filename_to_ruby, \
                       (GFreeFunc)free_list, (GFreeFunc)free_elem)
#define FILENAMEGSLIST2RVAL(list) \
        FILENAMEGSLIST2RVAL_FREE(list, NULL, NULL)
#define FILENAMEGSLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_gslist2rval(list, (RBGRValueFunc)rbg_filename_to_ruby, \
                        (GFreeFunc)free_list, (GFreeFunc)free_elem)

#define GOBJGLIST2RVAL(list) \
        GOBJGLIST2RVAL_FREE(list, NULL, NULL)
#define GOBJGLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_glist2rval(list, (RBGRValueFunc)rbgobj_ruby_object_from_instance, \
                       (GFreeFunc)free_list, (GFreeFunc)free_elem)
#define GOBJGSLIST2RVAL(list) \
        GOBJGSLIST2RVAL_FREE(list, NULL, NULL)
#define GOBJGSLIST2RVAL_FREE(list, free_list, free_elem) \
        rbg_gslist2rval(list, (RBGRValueFunc)rbgobj_ruby_object_from_instance, \
                        (GFreeFunc)free_list, (GFreeFunc)free_elem)

#define BOXEDGLIST2RVAL(list, gtype) \
        BOXEDGLIST2RVAL_FREE(list, gtype, NULL, NULL)
#define BOXEDGLIST2RVAL_FREE(list, gtype, free_list, free_elem) \
        rbg_glist2rval_with_type(list, (RBGRValueFuncWithType)rbgobj_make_boxed, gtype, \
                                 (GFreeFunc)free_list, (GFreeFunc)free_elem)
#define BOXEDGSLIST2RVAL(list, gtype) \
        BOXEDGSLIST2RVAL_FREE(list, gtype, NULL, NULL)
#define BOXEDGSLIST2RVAL_FREE(list, gtype, free_list, free_elem) \
        rbg_gslist2rval_with_type(list, (RBGRValueFuncWithType)rbgobj_make_boxed, gtype, \
                                  (GFreeFunc)free_list, (GFreeFunc)free_elem)

#define RVAL2GOBJGLIST(rb_array)                \
    rbg_rval2glist(rb_array)
#define RVAL2GOBJGSLIST(rb_array)               \
    rbg_rval2gslist(rb_array)


typedef VALUE (*RBGRValueFunc)(gpointer obj);
typedef VALUE (*RBGRValueFuncWithType)(gpointer obj, GType gtype);

extern VALUE rbg_glist2rval(GList *const list, RBGRValueFunc conv,
                            GFreeFunc free_list, GFreeFunc free_elem);
extern VALUE rbg_gslist2rval(GSList *const list, RBGRValueFunc conv,
                             GFreeFunc free_list, GFreeFunc free_elem);
extern VALUE rbg_glist2rval_with_type(GList *const list, RBGRValueFuncWithType conv, GType gtype,
                                      GFreeFunc free_list, GFreeFunc free_elem);
extern VALUE rbg_gslist2rval_with_type(GSList *const list, RBGRValueFuncWithType conv, GType gtype,
                                       GFreeFunc free_list, GFreeFunc free_elem);

extern GList *rbg_rval2glist(VALUE rb_array);
extern GSList *rbg_rval2gslist(VALUE rb_array);

G_END_DECLS

#endif /* __RBGUTIL_LIST_H__ */

