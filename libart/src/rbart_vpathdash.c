/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_vpathdash.c -

   $Author: mutoh $
   $Date: 2004/11/13 11:19:13 $

   Copyright (C) 2004 Ruby-GNOME2 Project Team
   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/
#include "rbart.h"

VALUE artVpathDash;

static ID id_flatten;

#define VpathDash_Ptr(r_obj) ((ArtVpathDash *)(RDATA(r_obj)->data))

/*************************************************/
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
/*************************************************/

static VALUE
vpath_dash_s_allocate(klass)
    VALUE klass;
{
    return Data_Wrap_Struct(klass, 0, vpath_dash_free, 0);
}

static VALUE
vpath_dash_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int i;    
    VALUE offset;
    VALUE ary;
    ArtVpathDash *dash;

    rb_scan_args(argc, argv, "1*", &offset, &ary);

    rb_funcall(ary, id_flatten, 0);

    dash = (ArtVpathDash *) art_alloc(sizeof(ArtVpathDash));
    dash->offset = NUM2DBL(offset);
    dash->n_dash = RARRAY(ary)->len;
    dash->dash = art_new(double, dash->n_dash);
    for (i = 0; i < dash->n_dash; ++i)
        dash->dash[i] = NUM2DBL(RARRAY(ary)->ptr[i]);
    DATA_PTR(self) = dash;
    return Qnil;
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
Init_art_vpathdash(mArt)
    VALUE mArt;
{
    artVpathDash = rb_define_class_under(mArt, "VpathDash", rb_cData);
    RBART_INIT_FUNC2(artVpathDash, vpath_dash_s_allocate);
    
    rb_define_method(artVpathDash, "initialize", vpath_dash_initialize, -1);
    rb_define_method(artVpathDash, "to_a", vpath_dash_to_a, 0);

    id_flatten = rb_intern("flatten!");

    /* ArtPathStrokeCapType */
    rb_define_const(mArt, "PATH_STROKE_CAP_BUTT", INT2NUM(ART_PATH_STROKE_CAP_BUTT));
    rb_define_const(mArt, "PATH_STROKE_CAP_ROUND", INT2NUM(ART_PATH_STROKE_CAP_ROUND));
    rb_define_const(mArt, "PATH_STROKE_CAP_SQUARE", INT2NUM(ART_PATH_STROKE_CAP_SQUARE));

    /* ArtPathStrokeJoinType */
    rb_define_const(mArt, "PATH_STROKE_JOIN_MITER", INT2NUM(ART_PATH_STROKE_JOIN_MITER));
    rb_define_const(mArt, "PATH_STROKE_JOIN_ROUND", INT2NUM(ART_PATH_STROKE_JOIN_ROUND));
    rb_define_const(mArt, "PATH_STROKE_JOIN_BEVEL", INT2NUM(ART_PATH_STROKE_JOIN_BEVEL));

}
