/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_svp.c -

   $Author: mutoh $
   $Date: 2004/11/13 11:19:13 $

   Copyright (C) 2004 Ruby-GNOME2 Project Team
   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/
#include "rbart.h"

VALUE artSVP;

#define SVP_Ptr(r_obj) ((ArtSVP *)(RDATA(r_obj)->data))

/*************************************************/
ArtSVP *
get_art_svp(r_obj)
    VALUE r_obj;
{
    if (!rb_obj_is_instance_of(r_obj, artSVP))
        rb_raise(rb_eTypeError, "not an Art::SVP");
    return SVP_Ptr(r_obj);
}

VALUE
make_art_svp(vpath)
    ArtSVP *vpath;
{
    return Data_Wrap_Struct(artSVP, 0, art_free, vpath);
}
/*************************************************/

static VALUE
svp_diff(r_self, r_other)
    VALUE r_self;
    VALUE r_other;
{
    return make_art_svp(art_svp_diff(get_art_svp(r_self), get_art_svp(r_other)));
}

static VALUE
svp_intersect(r_self, r_other)
    VALUE r_self;
    VALUE r_other;
{
    return make_art_svp(art_svp_intersect(get_art_svp(r_self), get_art_svp(r_other)));
}

static VALUE
svp_to_vpath(r_self)
    VALUE r_self;
{
    return make_art_vpath(art_vpath_from_svp(get_art_svp(r_self)));
}

static VALUE
svp_union(r_self, r_other)
    VALUE r_self;
    VALUE r_other;
{
    return make_art_svp(art_svp_union(get_art_svp(r_self), get_art_svp(r_other)));
}

void
Init_art_svp(mArt)
    VALUE mArt;
{
    artSVP = rb_define_class_under(mArt, "SVP", rb_cData);
    rb_define_method(artSVP, "diff", svp_diff, 1);
    rb_define_method(artSVP, "intersect", svp_intersect, 1);
    rb_define_method(artSVP, "to_vpath", svp_to_vpath, 0);
    rb_define_method(artSVP, "union", svp_union, 1);
    rb_define_alias(artSVP, "|", "union");
    rb_define_alias(artSVP, "^", "diff");
    rb_define_alias(artSVP, "&", "intersect");
}
