/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
#include "rbgnomecanvas.h"

static VALUE
points_s_new(klass, num)
    VALUE klass, num;
{
    GnomeCanvasPoints *gcp = gnome_canvas_points_new(NUM2INT(num));
    return BOXED2RVAL(gcp, GNOME_TYPE_CANVAS_POINTS);
}

static VALUE
points_aref(self, offset)
    VALUE self, offset;
{
    GnomeCanvasPoints *gcp = (GnomeCanvasPoints*)RVAL2BOXED(self);
    int i = NUM2INT(offset);
    if (i < 0 || gcp->num_points * 2 <= i) {
        rb_raise(rb_eIndexError, "index %d out of coordinate", i);
    }
    return rb_float_new(gcp->coords[i]);
}

static VALUE
points_aset(self, offset, val)
    VALUE self, offset, val;
{
    GnomeCanvasPoints *gcp =(GnomeCanvasPoints*)RVAL2BOXED(self);
    int i = NUM2INT(offset);
    if (i < 0 || gcp->num_points * 2 <= i) {
        rb_raise(rb_eIndexError, "index %d out of coordinate", i);
    }
    gcp->coords[i] = NUM2DBL(val);
    return self;
}

void
Init_gnome_canvas_util(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasPoints = G_DEF_CLASS(GNOME_TYPE_CANVAS_POINTS, "CanvasPoints", mGnome);

    rb_define_singleton_method(gnoCanvasPoints, "new", points_s_new, 1);
    rb_define_method(gnoCanvasPoints, "[]", points_aref, 1);
    rb_define_method(gnoCanvasPoints, "[]=", points_aset, 2);
}
