/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_bpath.c -

   $Author: mutoh $
   $Date: 2004/11/13 11:19:13 $

   Copyright (C) 2004 Ruby-GNOME2 Project Team
   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/

#include "rbart.h"

VALUE artBpath;

#define Bpath_Ptr(r_obj) ((ArtBpath *)(RDATA(r_obj)->data))

/*************************************************/
ArtBpath *
get_art_bpath(r_obj)
    VALUE r_obj;
{
    if (!rb_obj_is_instance_of(r_obj, artBpath))
        rb_raise(rb_eTypeError, "not an Art::Bpath");
    return Bpath_Ptr(r_obj);
}

VALUE
make_art_bpath(bpath)
    ArtBpath *bpath;
{
    return Data_Wrap_Struct(artBpath, 0, art_free, bpath);
}
/*************************************************/
/*
 * Art::Bpath.new([[pathcode1, x, y,..], [pathcode1, x, y,..]])
 * Or Art::Bpath.new([pathcode1, x, y,..], [pathcode1, x, y,..])
 * [pathcode1, x, y,..] are:
 *   [Art::MOVETO, x, y]
 *   [Art::MOVETO_OPEN, x, y]
 *   [Art::CURVETO, x1, y1, x2, y2, x3, y3]
 *   [Art::LINETO, x, y]
 *   [Art::END] - Always required at the end of parameters.
 */
static VALUE
bpath_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    ArtBpath *bpath;
    int i;    
    VALUE r_array;

    if (argc == 1){
        VALUE ary;
        rb_scan_args(argc, argv, "1", &ary);
        if (TYPE(RARRAY(ary)->ptr) == T_ARRAY){
            r_array = (VALUE)(RARRAY(ary)->ptr[0]);
        } else {
            r_array = ary;
        }
    } else {
        rb_scan_args(argc, argv, "*", &r_array);
    }

    Check_Type(r_array, T_ARRAY);
    bpath = art_new(ArtBpath, RARRAY(r_array)->len);

    for (i = 0; i < RARRAY(r_array)->len; i++)
    {
        VALUE r_point = RARRAY(r_array)->ptr[i];
        Check_Type(r_point, T_ARRAY);
        if (RARRAY(r_point)->len < 1)
            rb_raise(rb_eTypeError, "wrong size of Array (expect 1, 3 or 7)");
        bpath[i].code = NUM2INT(RARRAY(r_point)->ptr[0]);
        switch (bpath[i].code)
        {
          case ART_MOVETO:
          case ART_MOVETO_OPEN:
          case ART_LINETO:
            if (RARRAY(r_point)->len != 3)
                rb_raise(rb_eTypeError, "wrong size of Array (expect 3)");
            bpath[i].x1 = NUM2DBL(RARRAY(r_point)->ptr[1]);
            bpath[i].y1 = NUM2DBL(RARRAY(r_point)->ptr[2]);
            break;
          case ART_CURVETO:
            if (RARRAY(r_point)->len != 7)
                rb_raise(rb_eTypeError, "wrong size of Array (expect 7)");
            bpath[i].x1 = NUM2DBL(RARRAY(r_point)->ptr[1]);
            bpath[i].y1 = NUM2DBL(RARRAY(r_point)->ptr[2]);
            bpath[i].x2 = NUM2DBL(RARRAY(r_point)->ptr[3]);
            bpath[i].y2 = NUM2DBL(RARRAY(r_point)->ptr[4]);
            bpath[i].x3 = NUM2DBL(RARRAY(r_point)->ptr[5]);
            bpath[i].y3 = NUM2DBL(RARRAY(r_point)->ptr[6]);
            break;
          case ART_END:
            if (RARRAY(r_point)->len != 1)
                rb_raise(rb_eTypeError, "wrong size of Array (expect 1)");
            break;
          default:
            rb_raise(rb_eTypeError, "invalid code");
            break;
        }
    }
    DATA_PTR(self) = bpath;
    return Qnil;
}

static VALUE
bpath_affine_transform(r_self, r_affine)
    VALUE r_self;
    VALUE r_affine;
{
    return make_art_bpath(art_bpath_affine_transform(get_art_bpath(r_self), get_art_affine(r_affine)));
}

static VALUE
bpath_to_a(r_self)
    VALUE r_self;
{
    VALUE r_result;
    ArtBpath *bpath;
    r_result = rb_ary_new();
    for (bpath = get_art_bpath(r_self); bpath->code != ART_END; ++bpath)
        switch (bpath->code)
        {
          case ART_MOVETO:
          case ART_MOVETO_OPEN:
          case ART_LINETO:
            rb_ary_push(r_result, rb_ary_new3(3, INT2NUM(bpath->code), rb_float_new(bpath->x1), rb_float_new(bpath->y1)));
            break;
          case ART_CURVETO:
            rb_ary_push(r_result, rb_ary_new3(7, INT2NUM(bpath->code), rb_float_new(bpath->x1), rb_float_new(bpath->y1), rb_float_new(bpath->x2), rb_float_new(bpath->y2), rb_float_new(bpath->x3), rb_float_new(bpath->y3)));
            break;
          default:
            rb_raise(rb_eTypeError, "invalid code");
            break;
        }
    rb_ary_push(r_result, rb_ary_new3(1, INT2NUM(ART_END)));
    return r_result;
}

static VALUE
bpath_to_vpath(r_self, r_flatness)
    VALUE r_self;
    VALUE r_flatness;
{
    return make_art_vpath(art_bez_path_to_vec(get_art_bpath(r_self), NUM2DBL(r_flatness)));
}

void
Init_art_bpath(mArt)
    VALUE mArt;
{
    artBpath = rb_define_class_under(mArt, "Bpath", rb_cData);

    RBART_INIT_FUNC(artBpath);

    rb_define_method(artBpath, "initialize", bpath_initialize, -1);
    rb_define_method(artBpath, "affine_transform", bpath_affine_transform, 1);
    rb_define_method(artBpath, "to_a", bpath_to_a, 0);
    rb_define_method(artBpath, "to_vpath", bpath_to_vpath, 1);
}
