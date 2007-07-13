/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart_uta.c - Art::Uta class for ruby

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2005  Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbart.h"

VALUE artUta;

#define _SELF(obj) ((ArtUta *)(RDATA(obj)->data))

/*************************************************/
ArtUta *
get_art_uta(r_obj)
    VALUE r_obj;
{
    if (!rb_obj_is_instance_of(r_obj, artUta))
        rb_raise(rb_eTypeError, "not an Art::Uta");
    return _SELF(r_obj);
}

VALUE
make_art_uta(art)
    ArtUta *art;
{
    return Data_Wrap_Struct(artUta, 0, art_uta_free, art);
}
/*************************************************/

/**********************************/
#define ATTR_INT(name)\
static VALUE \
uta_get_ ## name (self)\
    VALUE self;\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
uta_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = NUM2INT(val);\
    return self;\
}
/**********************************/

static VALUE
uta_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE x0, y0, x1, y1, coords;
    ArtUta* uta;

    rb_scan_args(argc, argv, "41", &x0, &y0, &x1, &y1, &coords);

    if (NIL_P(coords)) coords = Qfalse;

    if (RVAL2CBOOL(coords)){
        uta = art_uta_new_coords(NUM2INT(x0), NUM2INT(y0), NUM2INT(x1), NUM2INT(y1));
    } else {
        uta = art_uta_new(NUM2INT(x0), NUM2INT(y0), NUM2INT(x1), NUM2INT(y1));
    }
    
    RDATA(self)->data = uta;
    return Qnil;
}

ATTR_INT(x0);
ATTR_INT(y0);
ATTR_INT(width);
ATTR_INT(height);

static VALUE
uta_utiles(self)
    VALUE self;
{
    int i;
    ArtUta* uta = _SELF(self);
    int size = uta->width * uta->height * sizeof(ArtUtaBbox);
    ArtUtaBbox* utiles = uta->utiles;

    VALUE ary = rb_ary_new();
    for (i = 0; i < size; i++){
        rb_ary_push(ary, UINT2NUM(utiles[i]));
    }
    return ary;
}

static VALUE
uta_to_a(self)
    VALUE self;
{
    ArtUta* uta = _SELF(self);

    return rb_ary_new3(5, INT2NUM(uta->x0), INT2NUM(uta->y0),
                       INT2NUM(uta->width), INT2NUM(uta->height),
                       uta_utiles(self));
}

static VALUE
uta_to_s(self)
    VALUE self;
{

  VALUE ret;
  VALUE to_s = rb_intern("to_s");

  ret = rb_str_new2("<");
  rb_str_cat2(ret, rb_class2name(CLASS_OF(self)));
  rb_str_cat2(ret, ":"); 
  rb_str_cat2(ret, " x0:"); 
  rb_str_append(ret, rb_funcall(uta_get_x0(self), to_s, 0, 0));
  rb_str_cat2(ret, " y0:");
  rb_str_append(ret, rb_funcall(uta_get_y0(self), to_s, 0, 0));
  rb_str_cat2(ret, " width:"); 
  rb_str_append(ret, rb_funcall(uta_get_width(self), to_s, 0, 0));
  rb_str_cat2(ret, " height:");
  rb_str_append(ret, rb_funcall(uta_get_height(self), to_s, 0, 0));
  rb_str_cat2(ret, " utiles:");
  rb_str_append(ret, rb_funcall(uta_utiles(self), rb_intern("inspect"), 0, 0));
  rb_str_cat2(ret, ">");

  return ret;
}

void
Init_art_uta(mArt)
    VALUE mArt;
{
    artUta = rb_define_class_under(mArt, "Uta", rb_cData);

    RBART_INIT_FUNC(artUta);

    rb_define_method(artUta, "initialize", uta_initialize, -1);

    rb_define_method(artUta, "set_x0", uta_set_x0, 1);
    rb_define_method(artUta, "set_y0", uta_set_y0, 1);
    rb_define_method(artUta, "set_width", uta_set_width, 1);
    rb_define_method(artUta, "set_height", uta_set_height, 1);

    rb_define_alias(artUta, "x0=", "set_x0");
    rb_define_alias(artUta, "y0=", "set_y0");
    rb_define_alias(artUta, "width=", "set_width");
    rb_define_alias(artUta, "height=", "set_height");

    rb_define_method(artUta, "x0", uta_get_x0, 0);
    rb_define_method(artUta, "y0", uta_get_y0, 0);
    rb_define_method(artUta, "width", uta_get_width, 0);
    rb_define_method(artUta, "height", uta_get_height, 0);
    rb_define_method(artUta, "utiles", uta_utiles, 0);

    rb_define_method(artUta, "to_a", uta_to_a, 0);
    rb_define_method(artUta, "to_s", uta_to_s, 0);
}
