#include "rbgnome.h"
extern VALUE cImlibImage;
VALUE gnoCanvasPoints;

GnomeCanvasPoints*
get_gnome_canvas_points(obj)
    VALUE obj;
{
    GnomeCanvasPoints *gcp;
    if (!rb_obj_is_instance_of(obj, gnoCanvasPoints)) {
	rb_raise(rb_eTypeError, "not a GnomeCanvasPoints");
    }
    Data_Get_Struct(obj, GnomeCanvasPoints, gcp);
    return gcp;
}

/* This function is for newly created GnomeCanvasPoints pointer.
 * If it was already wrapped by other ruby object, you must call
 * gnome_canvas_points_ref in advance.
 */
VALUE
make_gnome_canvas_points(gcp)
    GnomeCanvasPoints* gcp;
{
    return Data_Wrap_Struct(gnoCanvasPoints, 0, gnome_canvas_points_free, gcp);
}

static VALUE
points_s_new(klass, num)
    VALUE klass, num;
{
    GnomeCanvasPoints *gcp = gnome_canvas_points_new(NUM2INT(num));
    return Data_Wrap_Struct(klass, 0, gnome_canvas_points_free, gcp);
}

static VALUE
points_aref(self, offset)
    VALUE self, offset;
{
    GnomeCanvasPoints *gcp;
    int i = NUM2INT(offset);
    Data_Get_Struct(self, GnomeCanvasPoints, gcp);
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
    rb_define_method(gnoCanvasPoints, "[]", points_aref, 1);
    rb_define_method(gnoCanvasPoints, "[]=", points_aset, 2);

    rb_define_singleton_method(gnoCanvas, "load_alpha", s_load_alpha, 1);
}
