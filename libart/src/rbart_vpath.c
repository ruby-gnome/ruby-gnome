/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_vpath.c -

   $Author: mutoh $
   $Date: 2003/02/08 18:51:44 $

   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/
#include "rbart.h"

VALUE artVpath;
VALUE artVpathDash;

#define Vpath_Ptr(r_obj) ((ArtVpath *)(RDATA(r_obj)->data))
#define VpathDash_Ptr(r_obj) ((ArtVpathDash *)(RDATA(r_obj)->data))

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

static VALUE
vpath_s_new(r_klass, r_array)
	VALUE r_klass;
	VALUE r_array;
{
	ArtVpath *vpath;
	int i;
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
			case ART_END:
				if (RARRAY(r_point)->len != 1)
					rb_raise(rb_eTypeError, "wrong size of Array (expect 1)");
				break;
			default:
				rb_raise(rb_eTypeError, "invalid code");
				break;
		}
	}
	return make_art_vpath(vpath);
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
vpath_add_point(r_self, r_point)
	VALUE r_self;
	VALUE r_point;
{
	ArtVpath *vpath;
	int n_points;
	int n_points_max;
	ArtPathcode code;
	double x;
	double y;
	Check_Type(r_point, T_ARRAY);
	if (RARRAY(r_point)->len < 1)
		rb_raise(rb_eTypeError, "wrong size of Array (expect 1 or 3)");
	code = NUM2INT(RARRAY(r_point)->ptr[0]);
	switch (code)
	{
		case ART_MOVETO:
		case ART_MOVETO_OPEN:
		case ART_LINETO:
			if (RARRAY(r_point)->len != 3)
				rb_raise(rb_eTypeError, "wrong size of Array (expect 3)");
			x = NUM2DBL(RARRAY(r_point)->ptr[1]);
			y = NUM2DBL(RARRAY(r_point)->ptr[2]);
			break;
		case ART_END:
			if (RARRAY(r_point)->len != 1)
				rb_raise(rb_eTypeError, "wrong size of Array (expect 1)");
			break;
		default:
			rb_raise(rb_eTypeError, "invalid code");
			break;
	}
	vpath = get_art_vpath(r_self);
	art_vpath_add_point(&vpath, &n_points, &n_points_max, code, x, y);
	Vpath_Ptr(r_self) = vpath;
	return r_self;
}
#endif

static VALUE
vpath_affine_transform(r_self, r_affine)
	VALUE r_self;
	VALUE r_affine;
{
	return make_art_vpath(art_vpath_affine_transform(get_art_vpath(r_self), rbart_get_art_affine(r_affine)));
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

ArtVpathDash *
get_art_vpath_dash(r_obj)
	VALUE r_obj;
{
	if (!rb_obj_is_instance_of(r_obj, artVpathDash))
		rb_raise(rb_eTypeError, "not an Art::Vpath::Dash");
	return VpathDash_Ptr(r_obj);
}

static void
vpath_dash_free(ArtVpathDash *dash)
{
	art_free(dash->dash);
	art_free(dash);
}

VALUE
make_art_vpath_dash(dash)
	ArtVpathDash *dash;
{
	return Data_Wrap_Struct(artVpathDash, 0, vpath_dash_free, dash);
}

static VALUE
vpath_dash_s_new(r_klass, r_offset, r_dash)
	VALUE r_klass;
	VALUE r_offset;
	VALUE r_dash;
{
	ArtVpathDash *dash;
	int i;
	Check_Type(r_dash, T_ARRAY);
	dash = (ArtVpathDash *) art_alloc(sizeof(ArtVpathDash));
	dash->offset = NUM2DBL(r_offset);
	dash->n_dash = RARRAY(r_dash)->len;
	dash->dash = art_new(double, dash->n_dash);
	for (i = 0; i < dash->n_dash; ++i)
	    dash->dash[i] = NUM2DBL(RARRAY(r_dash)->ptr[i]);
	return make_art_vpath_dash(dash);
}

static VALUE
vpath_dash_to_a(r_self)
    VALUE r_self;
{
	ArtVpathDash *dash;
	VALUE r_dash;
	int i;
	dash = get_art_vpath_dash(r_self);
	r_dash = rb_ary_new2(dash->n_dash);
	for (i = 0; i < dash->n_dash; ++i)
		RARRAY(r_dash)->ptr[i] = rb_float_new(dash->dash[i]);
	RARRAY(r_dash)->len = dash->n_dash;
	return rb_ary_new3(2, rb_float_new(dash->offset), r_dash);
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

	rb_define_const(mArt, "PATH_STROKE_CAP_BUTT", INT2NUM(ART_PATH_STROKE_CAP_BUTT));
	rb_define_const(mArt, "PATH_STROKE_CAP_ROUND", INT2NUM(ART_PATH_STROKE_CAP_ROUND));
	rb_define_const(mArt, "PATH_STROKE_CAP_SQUARE", INT2NUM(ART_PATH_STROKE_CAP_SQUARE));

	rb_define_const(mArt, "PATH_STROKE_JOIN_MITER", INT2NUM(ART_PATH_STROKE_JOIN_MITER));
	rb_define_const(mArt, "PATH_STROKE_JOIN_ROUND", INT2NUM(ART_PATH_STROKE_JOIN_ROUND));
	rb_define_const(mArt, "PATH_STROKE_JOIN_BEVEL", INT2NUM(ART_PATH_STROKE_JOIN_BEVEL));

	artVpath = rb_define_class_under(mArt, "Vpath", rb_cObject);
	rb_define_singleton_method(artVpath, "new", vpath_s_new, 1);
	rb_define_singleton_method(artVpath, "circle", vpath_s_circle, 3);
	rb_define_method(artVpath, "affine_transform", vpath_affine_transform, 1);
	rb_define_method(artVpath, "dash", vpath_dash, 1);
	rb_define_method(artVpath, "perturb", vpath_perturb, 0);
	rb_define_method(artVpath, "stroke", vpath_stroke, 5);
	rb_define_method(artVpath, "to_a", vpath_to_a, 0);
	rb_define_method(artVpath, "to_svp", vpath_to_svp, 0);
#if 0
	rb_define_method(artVpath, "add_point!", vpath_add_point, 1);
	rb_define_alias(artVpath, "<<", "add_point!");
#endif

	artVpathDash = rb_define_class_under(artVpath, "Dash", rb_cObject);
	rb_define_singleton_method(artVpathDash, "new", vpath_dash_s_new, 2);
	rb_define_method(artVpathDash, "to_a", vpath_dash_to_a, 0);
}
