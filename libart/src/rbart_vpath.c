/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_vpath.c -

   $Author: mutoh $
   $Date: 2004/11/13 11:19:13 $

   Copyright (C) 2004 Ruby-GNOME2 Project Team
   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/
#include "rbart.h"

VALUE artVpath;

#define Vpath_Ptr(r_obj) ((ArtVpath *)(RDATA(r_obj)->data))

/*************************************************/
ArtVpath *
get_art_vpath(r_obj)
    VALUE r_obj;
{
    if (!rb_obj_is_instance_of(r_obj, artVpath))
        rb_raise(rb_eTypeError, "not an Art::Vpath");
    return Vpath_Ptr(r_obj);
}

VALUE
make_art_vpath(vpath)
    ArtVpath *vpath;
{
    return Data_Wrap_Struct(artVpath, 0, art_free, vpath);
}
/*************************************************/

/*
 * Art::Vpath.new([[pathcode1, x, y,..], [pathcode1, x, y,..]])
 * Or Art::Bpath.new([pathcode1, x, y,..], [pathcode1, x, y,..])
 * [pathcode1, x, y,..] are:
 *   [Art::MOVETO, x, y]
 *   [Art::MOVETO_OPEN, x, y]
 *   [Art::LINETO, x, y]
 *   [Art::END] - Always required at the end of parameters.
 */
static VALUE
vpath_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    ArtVpath *vpath;
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
    vpath = art_new(ArtVpath, RARRAY(r_array)->len);
    for (i = 0; i < RARRAY(r_array)->len; ++i)
    {
        VALUE r_point = RARRAY(r_array)->ptr[i];
        Check_Type(r_point, T_ARRAY);
        if (RARRAY(r_point)->len < 1)
            rb_raise(rb_eTypeError, "wrong size of Array (expect 1 or 3)");
        vpath[i].code = NUM2INT(RARRAY(r_point)->ptr[0]);
        switch (vpath[i].code)
        {
          case ART_MOVETO:
          case ART_MOVETO_OPEN:
          case ART_LINETO:
            if (RARRAY(r_point)->len != 3)
                rb_raise(rb_eTypeError, "wrong size of Array (expect 3)");
            vpath[i].x = NUM2DBL(RARRAY(r_point)->ptr[1]);
            vpath[i].y = NUM2DBL(RARRAY(r_point)->ptr[2]);
            break;
          case ART_CURVETO:
            rb_raise(rb_eTypeError, "wrong path code for Art::Vpath (Art::CURVETO).");
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
    DATA_PTR(self) = vpath;
    return Qnil;
}

static VALUE
vpath_s_circle(r_klass, r_x, r_y, r_r)
    VALUE r_klass;
    VALUE r_x;
    VALUE r_y;
    VALUE r_r;
{
    return make_art_vpath(art_vpath_new_circle(NUM2DBL(r_x), NUM2DBL(r_y), NUM2DBL(r_r)));
}

#if 0
static VALUE
vpath_add_point(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE rcode, rx, ry;
    ArtVpath *vpath;
    int n_points;
    int n_points_max;
    ArtPathcode code;
    double x = 0.0;
    double y = 0.0;

    rb_scan_args(argc, argv, "12", &rcode, &rx, &ry);

    code = NUM2INT(rcode);
    switch (code)
    {
      case ART_MOVETO:
      case ART_MOVETO_OPEN:
      case ART_LINETO:
        x = NUM2DBL(rx);
        y = NUM2DBL(ry);
        break;
      case ART_CURVETO:
        rb_raise(rb_eTypeError, "wrong path code for Art::Vpath (Art::CURVETO).");
        break;
      case ART_END:
        break;
      default:
        rb_raise(rb_eTypeError, "invalid code");
        break;
    }
    vpath = get_art_vpath(self);
    art_vpath_add_point(&vpath, &n_points, &n_points_max, code, x, y);
    DATA_PTR(self) = vpath;
    return self;
}
#endif

static VALUE
vpath_affine_transform(r_self, r_affine)
    VALUE r_self;
    VALUE r_affine;
{
    return make_art_vpath(art_vpath_affine_transform(get_art_vpath(r_self), get_art_affine(r_affine)));
}

static VALUE
vpath_dash(r_self, r_dash)
    VALUE r_self;
    VALUE r_dash;
{
    return make_art_vpath(art_vpath_dash(get_art_vpath(r_self), get_art_vpath_dash(r_dash)));
}

static VALUE
vpath_perturb(r_self)
    VALUE r_self;
{
    return make_art_vpath(art_vpath_perturb(get_art_vpath(r_self)));
}

static VALUE
vpath_stroke(r_self, r_join, r_cap, r_line_width, r_miter_limit, r_flatness)
    VALUE r_self;
    VALUE r_join;
    VALUE r_cap;
    VALUE r_line_width;
    VALUE r_miter_limit;
    VALUE r_flatness;
{
    return make_art_svp(art_svp_vpath_stroke(get_art_vpath(r_self), NUM2INT(r_join), NUM2INT(r_cap), NUM2DBL(r_line_width), NUM2DBL(r_miter_limit), NUM2DBL(r_flatness)));
}

static VALUE
vpath_to_a(r_self)
    VALUE r_self;
{
    VALUE r_result;
    ArtVpath *vpath;
    r_result = rb_ary_new();
    for (vpath = get_art_vpath(r_self); vpath->code != ART_END; ++vpath)
        switch (vpath->code)
        {
          case ART_MOVETO:
          case ART_MOVETO_OPEN:
          case ART_LINETO:
            rb_ary_push(r_result, rb_ary_new3(3, INT2NUM(vpath->code), rb_float_new(vpath->x), rb_float_new(vpath->y)));
            break;
          default:
            rb_raise(rb_eTypeError, "invalid code");
            break;
        }
    rb_ary_push(r_result, rb_ary_new3(1, INT2NUM(ART_END)));
    return r_result;
}

static VALUE
vpath_to_svp(r_self)
    VALUE r_self;
{
    return make_art_svp(art_svp_from_vpath(get_art_vpath(r_self)));
}

void
Init_art_vpath(mArt)
    VALUE mArt;
{
    rb_define_const(mArt, "MOVETO", INT2NUM(ART_MOVETO));
    rb_define_const(mArt, "MOVETO_OPEN", INT2NUM(ART_MOVETO_OPEN));
    rb_define_const(mArt, "CURVETO", INT2NUM(ART_CURVETO));
    rb_define_const(mArt, "LINETO", INT2NUM(ART_LINETO));
    rb_define_const(mArt, "END", INT2NUM(ART_END));

    artVpath = rb_define_class_under(mArt, "Vpath", rb_cData);

    RBART_INIT_FUNC(artVpath);

    rb_define_singleton_method(artVpath, "circle", vpath_s_circle, 3);

    rb_define_method(artVpath, "initialize", vpath_initialize, -1);
    rb_define_method(artVpath, "affine_transform", vpath_affine_transform, 1);
    rb_define_method(artVpath, "dash", vpath_dash, 1);
    rb_define_method(artVpath, "perturb", vpath_perturb, 0);
    rb_define_method(artVpath, "stroke", vpath_stroke, 5);
    rb_define_method(artVpath, "to_a", vpath_to_a, 0);
    rb_define_method(artVpath, "to_svp", vpath_to_svp, 0);
#if 0
    rb_define_method(artVpath, "add", vpath_add_point, -1);
    rb_define_alias(artVpath, "<<", "add");
#endif

}
