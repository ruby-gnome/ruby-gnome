#include "rbgnome.h"
extern VALUE cImlibImage;
VALUE gnoCanvasPoints;

static VALUE
points_s_new(klass, num)
    VALUE klass, num;
{
    GnomeCanvasPoints *gcp = gnome_canvas_points_new(NUM2INT(num));
    return Data_Wrap_Struct(klass, 0, gnome_canvas_points_free, gcp);
}

static VALUE
points_ref(self)
    VALUE self;
{
    GnomeCanvasPoints *gcp;
    Data_Get_Struct(self, GnomeCanvasPoints, gcp);
    gnome_canvas_points_ref(gcp);
    return Qnil;
}

static VALUE
points_free(self)
    VALUE self;
{
    GnomeCanvasPoints *gcp;
    Data_Get_Struct(self, GnomeCanvasPoints, gcp);
    if (gcp->ref_count == 1) {
	gnome_canvas_points_free(gcp);
	DATA_PTR(self) = NULL;
    } else {
	gnome_canvas_points_free(gcp);
    }
    return Qnil;
}

static VALUE
points_aref(self, offset)
    VALUE self, offset;
{
    GnomeCanvasPoints *gcp;
    int i = NUM2INT(offset);
    Data_Get_Struct(self, GnomeCanvasPoints, gcp);
    if (gcp == NULL) {
	rb_raise(rb_eRuntimeError, "object is already freed.");
    }
    if (i < 0 || gcp->num_points * 2 <= i) {
	rb_raise(rb_eIndexError, "index %d out of coordinate", i);
    }
    return rb_float_new(gcp->coords[i]);
}

static VALUE
points_aset(self, offset, val)
    VALUE self, offset, val;
{
    GnomeCanvasPoints *gcp;
    int i = NUM2INT(offset);
    Data_Get_Struct(self, GnomeCanvasPoints, gcp);
    if (gcp == NULL) {
	rb_raise(rb_eRuntimeError, "object is already freed.");
    }
    if (i < 0 || gcp->num_points * 2 <= i) {
	rb_raise(rb_eIndexError, "index %d out of coordinate", i);
    }
    gcp->coords[i] = NUM2DBL(val);
    return Qnil;
}

static VALUE
s_load_alpha(klass, file)
    VALUE klass, file;
{
    GdkImlibImage *im;

    Check_Type(file, T_STRING); 
    im = gnome_canvas_load_alpha(STR2CSTR(file));
    if(im == NULL && errno == ENOMEM){
        rb_gc();
        im = gnome_canvas_load_alpha(STR2CSTR(file));
    }
    if(im == NULL){
	rb_raise(rb_eRuntimeError, "could not load %s\n", STR2CSTR(file));
    }
    return Data_Wrap_Struct(cImlibImage, 0, gnome_canvas_destroy_image, im);
}

void
Init_gnome_canvas_util()
{
    gnoCanvasPoints = rb_define_class_under(mGnome, "CanvasPoints", rb_cObject);

    rb_define_singleton_method(gnoCanvasPoints, "new", points_s_new, 1);
    rb_define_method(gnoCanvasPoints, "ref", points_ref, 0);
    rb_define_method(gnoCanvasPoints, "free", points_free, 0);
    rb_define_alias(gnoCanvasPoints, "unref", "free");
    rb_define_method(gnoCanvasPoints, "[]", points_aref, 1);
    rb_define_method(gnoCanvasPoints, "[]=", points_aset, 2);

    rb_define_singleton_method(gnoCanvas, "load_alpha", s_load_alpha, 1);
}
