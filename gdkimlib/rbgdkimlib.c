/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkimlib.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

  Copyright (C) 1998-2000 NAKAMURA Hideki,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "rbgdkimlib.h"
#include <errno.h>
#include <gdk_imlib.h>

VALUE mImlib;
VALUE cImlibImage;
VALUE cImlibBorder;
VALUE cImlibShape;
VALUE cImlibModifier;
VALUE cImlibSaveInfo;

#ifndef RBGDKIMLIB_RENDER_LIMIT
#define RBGDKIMLIB_RENDER_LIMIT 1000000
#endif

static unsigned long rb_gdkimlib_render_limit = 1000000;

/*
 * GdkImlib module function
 */
static VALUE
im_get_render_type(self)
    VALUE self;
{
    return INT2FIX(gdk_imlib_get_render_type());
}

static VALUE
im_set_render_type(self, rend_type)
    VALUE self, rend_type;
{
    gdk_imlib_set_render_type(NUM2INT(rend_type));
    return Qnil;
}

static VALUE
im_load_colors(self, filename)
    VALUE self, filename;
{
    Check_Type(filename, T_STRING);
    if(gdk_imlib_load_colors(STR2CSTR(filename)) == 0){
	rb_raise(rb_eRuntimeError, "Connot load palette file %s\n", STR2CSTR(filename));
    }
    return Qnil;
}

static VALUE
im_free_colors(self)
    VALUE self;
{
    gdk_imlib_free_colors();
    return Qnil;
}

static VALUE
im_get_fallback(self)
    VALUE self;
{
    return INT2FIX(gdk_imlib_get_fallback());
}

static VALUE
im_set_fallback(self, fallback)
    VALUE self, fallback;
{
    Check_Type(fallback, T_FIXNUM);
    if(FIX2INT(fallback) != 0 && FIX2INT(fallback) != 1){
	rb_raise(rb_eRuntimeError, "0 or 1 for fallback\n");
    }
    gdk_imlib_set_fallback(FIX2INT(fallback));
    return Qnil;
}

static VALUE
im_push_visual(self)
    VALUE self;
{
    gtk_widget_push_visual(gdk_imlib_get_visual());
    gtk_widget_push_colormap(gdk_imlib_get_colormap());
    return self;
}

static VALUE
im_pop_visual(self)
    VALUE self;
{
    gtk_widget_pop_visual();
    gtk_widget_pop_colormap();
    return self;
}

static void
im_free_sysconfig(sysconfig)
    gchar *sysconfig;
{
    free(sysconfig);
}

static VALUE
im_get_sysconfig(self)
    VALUE self;
{
    gchar *sysconfig;

    sysconfig = gdk_imlib_get_sysconfig();
    if(sysconfig == NULL){
        rb_gc();
        sysconfig = gdk_imlib_get_sysconfig();
    }
    if(sysconfig == NULL){
        rb_raise(rb_eRuntimeError, "Connot get sysconfig\n");
    }
    return Data_Wrap_Struct(cImlibImage, 0, im_free_sysconfig, sysconfig);
}

/*
 * GdkImlibImage method
 */
static VALUE
imlib_s_new(klass, file)
    VALUE klass, file;
{
    GdkImlibImage *im;

    im = gdk_imlib_load_image(STR2CSTR(file));
    if(im == NULL && errno == ENOMEM){
        rb_gc();
        im = gdk_imlib_load_image(STR2CSTR(file));
    }
    if(im == NULL){
	rb_raise(rb_eRuntimeError, "could not load %s\n", STR2CSTR(file));
    }
    return Data_Wrap_Struct(cImlibImage, 0, gdk_imlib_destroy_image, im);
}

static VALUE
im_s_get_render_limit(self)
    VALUE self;
{
    return INT2NUM(rb_gdkimlib_render_limit);
}

static VALUE
im_s_set_render_limit(self, limit)
    VALUE self, limit;
{
    int old_limit;
    if (NUM2INT(limit) < 0) {
        rb_raise(rb_eArgError, "negative limit size");
    }
    old_limit = rb_gdkimlib_render_limit;
    rb_gdkimlib_render_limit = NUM2INT(limit);
    return INT2NUM(old_limit);
}

static VALUE
im_render(self, w, h)
    VALUE self, w, h;
{
    static unsigned long render_count = 0;

    GdkImlibImage *im;
    gint ret;

    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);

    render_count += FIX2INT(w) * FIX2INT(h);
    if(render_count > rb_gdkimlib_render_limit){
        rb_gc();
        render_count = 0;
    }
    
    Data_Get_Struct(self, GdkImlibImage, im);
    ret = gdk_imlib_render(im, FIX2INT(w), FIX2INT(h));
    if(ret == 0 && errno == ENOMEM){
        rb_gc();
        ret = gdk_imlib_render(im, FIX2INT(w), FIX2INT(h));
    }
    if(ret == 0){
	rb_raise(rb_eRuntimeError, "renderring error\n");
    }
    return Qnil;
}

static void
im_free_border(borp)
    GdkImlibBorder *borp;
{
    free(borp);
}

static VALUE
im_get_image_border(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibBorder *borp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibBorder, GdkImlibBorder, 0, im_free_border, borp);
    gdk_imlib_get_image_border(im, borp);
    return obj;
}

static VALUE
im_set_image_border(self, border)
    VALUE self, border;
{
    GdkImlibImage *im;
    GdkImlibBorder *borp;

    if(!rb_obj_is_kind_of(border, cImlibBorder)){
	rb_raise(rb_eTypeError, "not a ImlibBorder\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(border, GdkImlibBorder, borp);
    gdk_imlib_set_image_border(im, borp);
    return Qnil;
}

static void
im_free_shape(colp)
    GdkImlibColor *colp;
{
    free(colp);
}

static VALUE
im_get_image_shape(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibColor *colp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibShape, GdkImlibColor, 0, im_free_shape, colp);
    gdk_imlib_get_image_shape(im, colp);
    return obj;
}

static VALUE
im_set_image_shape(self, shape)
    VALUE self, shape;
{
    GdkImlibImage *im;
    GdkImlibColor *colp;

    if(!rb_obj_is_kind_of(shape, cImlibShape)){
	rb_raise(rb_eTypeError, "not a ImlibShape\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(shape, GdkImlibColor, colp);
    gdk_imlib_set_image_shape(im, colp);
    return Qnil;
}

static VALUE
im_save_image_to_eim(self, file)
    VALUE self, file;
{
    /* gdk_imlib_save_image_to_eim() */
    rb_notimplement();
}

static VALUE
im_add_image_to_eim(self, file)
    VALUE self, file;
{
    /* gdk_imlib_add_image_to_eim() */
    rb_notimplement();
}

static VALUE
im_save_image_to_ppm(self, file)
    VALUE self, file;
{
    /* gdk_imlib_save_image_to_ppm() */
    rb_notimplement();
}

static VALUE
im_set_image_modifier(self, mod)
    VALUE self, mod;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;

    if(!rb_obj_is_kind_of(mod, cImlibModifier)){
	rb_raise(rb_eTypeError, "not a ImlibModifier\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(mod, GdkImlibColorModifier, modp);
    gdk_imlib_set_image_modifier(im, modp);
    return Qnil;
}

static VALUE
im_set_image_red_modifier(self, mod)
    VALUE self, mod;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;

    if(!rb_obj_is_kind_of(mod, cImlibModifier)){
	rb_raise(rb_eTypeError, "not a ImlibModifier\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(mod, GdkImlibColorModifier, modp);
    gdk_imlib_set_image_red_modifier(im, modp);
    return Qnil;
}

static VALUE
im_set_image_green_modifier(self, mod)
    VALUE self, mod;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;

    if(!rb_obj_is_kind_of(mod, cImlibModifier)){
	rb_raise(rb_eTypeError, "not a ImlibModifier\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(mod, GdkImlibColorModifier, modp);
    gdk_imlib_set_image_green_modifier(im, modp);
    return Qnil;
}

static VALUE
im_set_image_blue_modifier(self, mod)
    VALUE self, mod;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;

    if(!rb_obj_is_kind_of(mod, cImlibModifier)){
	rb_raise(rb_eTypeError, "not a ImlibModifier\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(mod, GdkImlibColorModifier, modp);
    gdk_imlib_set_image_blue_modifier(im, modp);
    return Qnil;
}

static void
im_free_modifier(modp)
    GdkImlibColorModifier *modp;
{
    free(modp);
}

static VALUE
im_get_image_modifier(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibModifier, GdkImlibColorModifier, 0, im_free_modifier, modp);
    gdk_imlib_get_image_modifier(im, modp);
    return obj;
}

static VALUE
im_get_image_red_modifier(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibModifier, GdkImlibColorModifier, 0, im_free_modifier, modp);
    gdk_imlib_get_image_red_modifier(im, modp);
    return obj;
}

static VALUE
im_get_image_green_modifier(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibModifier, GdkImlibColorModifier, 0, im_free_modifier, modp);
    gdk_imlib_get_image_green_modifier(im, modp);
    return obj;
}

static VALUE
im_get_image_blue_modifier(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkImlibColorModifier *modp;
    VALUE obj;

    Data_Get_Struct(self, GdkImlibImage, im);
    obj = Data_Make_Struct(cImlibModifier, GdkImlibColorModifier, 0, im_free_modifier, modp);
    gdk_imlib_get_image_blue_modifier(im, modp);
    return obj;
}

static void
im_check_curve_data(mod, modf)
    VALUE mod;
    unsigned char *modf;
{
    int ii;

    Check_Type(mod, T_ARRAY);
    if(RARRAY(mod)->len != 256){
	rb_raise(rb_eRuntimeError, "curve data 256 element array\n");
    }
    for(ii=0; ii<256; ii++){
	Check_Type(RARRAY(mod)->ptr[ii], T_FIXNUM);
	if(FIX2INT(RARRAY(mod)->ptr[ii]) < 0 || 255 < FIX2INT(RARRAY(mod)->ptr[ii])){
	    rb_raise(rb_eRuntimeError, "curve data 0-255\n");
	}
	modf[ii] = FIX2INT(RARRAY(mod)->ptr[ii]);
    }
}

static VALUE
im_set_image_red_curve(self, mod)
    VALUE self, mod;
{
    unsigned char modf[256];
    GdkImlibImage *im;

    im_check_curve_data(mod, modf);
    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_set_image_red_curve(im, modf);
    return Qnil;
}

static VALUE
im_set_image_green_curve(self, mod)
    VALUE self, mod;
{
    unsigned char modf[256];
    GdkImlibImage *im;

    im_check_curve_data(mod, modf);
    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_set_image_green_curve(im, modf);
    return Qnil;
}

static VALUE
im_set_image_blue_curve(self, mod)
    VALUE self, mod;
{
    unsigned char modf[256];
    GdkImlibImage *im;

    im_check_curve_data(mod, modf);
    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_set_image_blue_curve(im, modf);
    return Qnil;
}

static VALUE
im_get_curve_data(self, get_image_curve)
    VALUE self;
    void (*get_image_curve)();
{
    GdkImlibImage *im;
    unsigned char modf[256];
    VALUE obj;
    int ii;
        
    Data_Get_Struct(self, GdkImlibImage, im);
    (*get_image_curve)(im, modf);
    obj = rb_ary_new2(256);
    for(ii=0; ii<256; ii++){
	rb_ary_push(obj, INT2FIX(modf));
    }
    return obj;
}
        
static VALUE
im_get_image_red_curve(self)
    VALUE self;
{
    return im_get_curve_data(self, gdk_imlib_get_image_red_curve);
}

static VALUE
im_get_image_green_curve(self)
    VALUE self;
{
    return im_get_curve_data(self, gdk_imlib_get_image_green_curve);
}

static VALUE
im_get_image_blue_curve(self)
    VALUE self;
{
    return im_get_curve_data(self, gdk_imlib_get_image_blue_curve);
}

static VALUE
im_apply_modifiers_to_rgb(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_apply_modifiers_to_rgb(im);
    return Qnil;
}

static VALUE
im_changed_image(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_changed_image(im);
    return Qnil;
}

static VALUE
im_apply_image(self, win)
    VALUE self, win;
{
    GdkImlibImage *im;
    GdkWindow *winp;

    if(!rb_obj_is_kind_of(win, gdkWindow)){
	rb_raise(rb_eTypeError, "not a GdkWindow\n");
    }
    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(win, GdkWindow, winp);
    gdk_imlib_apply_image(im, winp);
    return Qnil;
}

static VALUE
im_paste_image(self, win, x, y, w, h)
    VALUE self, win, x, y, w, h;
{
    GdkImlibImage *im;
    GdkWindow *winp;

    if(!rb_obj_is_kind_of(win, gdkWindow)){
	rb_raise(rb_eTypeError, "not a GdkWindow\n");
    }
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);

    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(win, GdkWindow, winp);
    gdk_imlib_paste_image(im, winp, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

static VALUE
im_paste_image_border(self, win, x, y, w, h)
    VALUE self, win, x, y, w, h;
{
    GdkImlibImage *im;
    GdkWindow *winp;

    if(!rb_obj_is_kind_of(win, gdkWindow)){
	rb_raise(rb_eTypeError, "not a GdkWindow\n");
    }
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);

    Data_Get_Struct(self, GdkImlibImage, im);
    Data_Get_Struct(win, GdkWindow, winp);
    gdk_imlib_paste_image_border(im, winp, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

static VALUE
im_flip_image_horizontal(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_flip_image_horizontal(im);
    return Qnil;
}

static VALUE
im_flip_image_vertical(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_flip_image_vertical(im);
    return Qnil;
}

static VALUE
im_rotate_image(self, d)
    VALUE self, d;
{
    GdkImlibImage *im;

    Check_Type(d, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_rotate_image(im, FIX2INT(d));
    return Qnil;
}

static VALUE
im_create_image_from_data(self, data, alpha, w, h)
    VALUE self, data, alpha, w, h;
{
    /* gdk_imlib_create_image_from_data() */
    rb_notimplement();
}

static VALUE
im_create_image_from_drawable(self, win, mask, x, y, w, h)
     VALUE self, win, mask, x, y, w, h;
{
    GdkImlibImage *im;
    GdkWindow     *gwin;
    GdkBitmap     *gmask;



    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    Data_Get_Struct(win, GdkWindow, gwin);
    Data_Get_Struct(mask, GdkBitmap, gmask);

    im = gdk_imlib_create_image_from_drawable( gwin, gmask, x, y, w, h);

    if(im == NULL && errno == ENOMEM){
        rb_gc();
    im = gdk_imlib_create_image_from_drawable( gwin, gmask, x, y, w, h);
    }
    if(im == NULL){
	rb_raise(rb_eRuntimeError, "could not create\n");
    }
    return Data_Wrap_Struct(cImlibImage, 0, gdk_imlib_destroy_image, im);
}

static VALUE
im_clone_image(self)
    VALUE self;
{
    GdkImlibImage *im, *newp;

    Data_Get_Struct(self, GdkImlibImage, im);
    newp = gdk_imlib_clone_image(im);
    if(newp == NULL || errno == ENOMEM) {
        rb_gc();
        newp = gdk_imlib_clone_image(im);
    }
    if(newp == NULL){
	rb_raise(rb_eRuntimeError, "could not clone image\n");
    }
    return Data_Wrap_Struct(cImlibImage, 0, gdk_imlib_destroy_image, newp);
}

static VALUE
im_clone_scaled_image(self, w, h)
    VALUE self, w, h;
{
    GdkImlibImage *im, *newp;

    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibImage, im);
    newp = gdk_imlib_clone_scaled_image(im, FIX2INT(w), FIX2INT(h));
    if(newp == NULL || errno == ENOMEM) {
        rb_gc();
        newp = gdk_imlib_clone_scaled_image(im, FIX2INT(w), FIX2INT(h));
    }
    if(newp == NULL){
	rb_raise(rb_eRuntimeError, "could not clone scaled image\n");
    }
    return Data_Wrap_Struct(cImlibImage, 0, gdk_imlib_destroy_image, newp);
}

static VALUE
im_crop_image(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    gdk_imlib_crop_image(im, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return Qnil;
}

static VALUE
im_crop_and_clone_image(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    GdkImlibImage *im, *newp;

    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);

    Data_Get_Struct(self, GdkImlibImage, im);
    newp = gdk_imlib_crop_and_clone_image(im, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    if(newp == NULL || errno == ENOMEM) {
        rb_gc();
        newp = gdk_imlib_crop_and_clone_image(im, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    }
    if(newp == NULL){
	rb_raise(rb_eRuntimeError, "could not clone image\n");
    }
    return Data_Wrap_Struct(cImlibImage, 0, gdk_imlib_destroy_image, newp);
}

static VALUE
im_save_image(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE filename;
    VALUE si;
    GdkImlibSaveInfo *sip;
    GdkImlibImage *im;

    if(rb_scan_args(argc, argv, "11", &filename, &si) == 1){
	sip = NULL;
    } else if(NIL_P(si)){
	sip = NULL;
    } else {
	Check_Type(si, T_DATA);
	Data_Get_Struct(si, GdkImlibSaveInfo, sip);
    }
    Check_Type(filename, T_STRING);

    Data_Get_Struct(self, GdkImlibImage, im);
    if(gdk_imlib_save_image(im, STR2CSTR(filename), sip) == 0){
	rb_raise(rb_eRuntimeError, "file save faild %s\n", STR2CSTR(filename));
    }
    return Qnil;
}

static VALUE
im_get_rgb_width(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    return INT2FIX(im->rgb_width);
}

static VALUE
im_get_rgb_height(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    return INT2FIX(im->rgb_height);
}

static VALUE
im_get_filename(self)
    VALUE self;
{
    GdkImlibImage *im;

    Data_Get_Struct(self, GdkImlibImage, im);
    if(im->filename == NULL){
	return Qnil;
    }
    return rb_str_new2(im->filename);
}

static VALUE
im_get_pixmap(self)
    VALUE self;
{
    GdkImlibImage *im;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    VALUE pixmap_data, mask_data;

    Data_Get_Struct(self, GdkImlibImage, im);
    pixmap = gdk_imlib_move_image(im);
    mask = gdk_imlib_move_mask(im);

    if (pixmap == NULL) {
	pixmap_data = Qnil;
    } else {
	pixmap_data =
	    Data_Wrap_Struct(gdkPixmap, 0, gdk_imlib_free_pixmap, pixmap);
    } 
    if (mask == NULL) {
	mask_data = Qnil;
    } else {
	mask_data = Data_Wrap_Struct(gdkBitmap, 0, 0, mask);
    }
    return rb_assoc_new(pixmap_data, mask_data);
}

/*
 * GdkImlibBorder method
 */
static VALUE
im_s_border_new(self)
    VALUE self;
{
    GdkImlibBorder *borp;
    VALUE obj;

    obj = Data_Make_Struct(cImlibBorder, GdkImlibBorder, 0, im_free_border, borp);
    borp->left = borp->right = borp->top = borp->bottom = 0;
    return obj;
}

static VALUE
im_get_border_left(self)
    VALUE self;
{
    GdkImlibBorder *borp;

    Data_Get_Struct(self, GdkImlibBorder, borp);
    return INT2FIX(borp->left);
}

static VALUE
im_get_border_right(self)
    VALUE self;
{
    GdkImlibBorder *borp;

    Data_Get_Struct(self, GdkImlibBorder, borp);
    return INT2FIX(borp->right);
}

static VALUE
im_get_border_top(self)
    VALUE self;
{
    GdkImlibBorder *borp;

    Data_Get_Struct(self, GdkImlibBorder, borp);
    return INT2FIX(borp->top);
}

static VALUE
im_get_border_bottom(self)
    VALUE self;
{
    GdkImlibBorder *borp;

    Data_Get_Struct(self, GdkImlibBorder, borp);
    return INT2FIX(borp->bottom);
}

static VALUE
im_set_border_left(self, left)
    VALUE self, left;
{
    GdkImlibBorder *borp;

    Check_Type(left, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibBorder, borp);
    borp->left = FIX2INT(left);
    return Qnil;
}

static VALUE
im_set_border_right(self, right)
    VALUE self, right;
{
    GdkImlibBorder *borp;

    Check_Type(right, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibBorder, borp);
    borp->right = FIX2INT(right);
    return Qnil;
}

static VALUE
im_set_border_top(self, top)
    VALUE self, top;
{
    GdkImlibBorder *borp;

    Check_Type(top, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibBorder, borp);
    borp->top = FIX2INT(top);
    return Qnil;
}

static VALUE
im_set_border_bottom(self, bottom)
    VALUE self, bottom;
{
    GdkImlibBorder *borp;

    Check_Type(bottom, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibBorder, borp);
    borp->bottom = FIX2INT(bottom);
    return Qnil;
}

/*
 * GdkImlibShape method
 */
static VALUE
im_s_shape_new(self)
    VALUE self;
{
    GdkImlibColor *colp;
    VALUE obj;

    obj = Data_Make_Struct(cImlibShape, GdkImlibColor, 0, im_free_shape, colp);
    colp->r = colp->g = colp->b = -1;
    colp->pixel = 0;
    return obj;
}

static VALUE
im_get_shape_r(self)
    VALUE self;
{
    GdkImlibColor *colp;

    Data_Get_Struct(self, GdkImlibColor, colp);
    return INT2FIX(colp->r);
}

static VALUE
im_get_shape_g(self)
    VALUE self;
{
    GdkImlibColor *colp;

    Data_Get_Struct(self, GdkImlibColor, colp);
    return INT2FIX(colp->g);
}

static VALUE
im_get_shape_b(self)
    VALUE self;
{
    GdkImlibColor *colp;

    Data_Get_Struct(self, GdkImlibColor, colp);
    return INT2FIX(colp->b);
}

static VALUE
im_get_shape_pixel(self)
    VALUE self;
{
    GdkImlibColor *colp;

    Data_Get_Struct(self, GdkImlibColor, colp);
    return INT2FIX(colp->pixel);
}

static VALUE
im_set_shape_r(self, r)
    VALUE self, r;
{
    GdkImlibColor *colp;

    Check_Type(r, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColor, colp);
    colp->r = FIX2INT(r);
    return Qnil;
}

static VALUE
im_set_shape_g(self, g)
    VALUE self, g;
{
    GdkImlibColor *colp;

    Check_Type(g, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColor, colp);
    colp->g = FIX2INT(g);
    return Qnil;
}

static VALUE
im_set_shape_b(self, b)
    VALUE self, b;
{
    GdkImlibColor *colp;

    Check_Type(b, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColor, colp);
    colp->b = FIX2INT(b);
    return Qnil;
}

static VALUE
im_set_shape_pixel(self, pixel)
    VALUE self, pixel;
{
    GdkImlibColor *colp;

    Check_Type(pixel, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColor, colp);
    colp->pixel = FIX2INT(pixel);
    return Qnil;
}

/*
 * GdkImlibModifier method
 */
static VALUE
im_s_modifier_new(self)
    VALUE self;
{
    GdkImlibColorModifier *modp;
    VALUE obj;

    obj = Data_Make_Struct(cImlibModifier, GdkImlibColorModifier, 0, im_free_modifier, modp);
    modp->gamma = modp->brightness = modp->contrast = 256;
    return obj;
}

static VALUE
im_get_modifier_gamma(self)
    VALUE self;
{
    GdkImlibColorModifier *modp;

    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    return INT2FIX(modp->gamma);
}

static VALUE
im_get_modifier_brightness(self)
    VALUE self;
{
    GdkImlibColorModifier *modp;

    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    return INT2FIX(modp->brightness);
}

static VALUE
im_get_modifier_contrast(self)
    VALUE self;
{
    GdkImlibColorModifier *modp;

    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    return INT2FIX(modp->contrast);
}

static VALUE
im_set_modifier_gamma(self, gamma)
    VALUE self, gamma;
{
    GdkImlibColorModifier *modp;

    Check_Type(gamma, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    modp->gamma = FIX2INT(gamma);
    return Qnil;
}

static VALUE
im_set_modifier_brightness(self, brightness)
    VALUE self, brightness;
{
    GdkImlibColorModifier *modp;

    Check_Type(brightness, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    modp->brightness = FIX2INT(brightness);
    return Qnil;
}

static VALUE
im_set_modifier_contrast(self, contrast)
    VALUE self, contrast;
{
    GdkImlibColorModifier *modp;

    Check_Type(contrast, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibColorModifier, modp);
    modp->contrast = FIX2INT(contrast);
    return Qnil;
}

/*
 * GdkImlibSaveInfo method
 */
static void
im_free_saveinfo(si)
    GdkImlibSaveInfo *si;
{
    free(si);
}

static VALUE
im_s_saveinfo_new(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;
    VALUE obj;

    obj = Data_Make_Struct(cImlibSaveInfo, GdkImlibSaveInfo, 0, im_free_saveinfo, sip);
    /* default value setting (imlib/gdk_imlib/save.c) */
    sip->quality = 208;
    sip->scaling = 1024;
    sip->xjustification = 512;
    sip->yjustification = 512;
    sip->page_size = PAGE_SIZE_LETTER;
    sip->color = 1;
    return obj;
}

static VALUE
im_set_saveinfo_quality(self, quality)
    VALUE self, quality;
{
    GdkImlibSaveInfo *sip;

    Check_Type(quality, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->quality = FIX2INT(quality);
    return Qnil;
}

static VALUE
im_set_saveinfo_scaling(self, scaling)
    VALUE self, scaling;
{
    GdkImlibSaveInfo *sip;

    Check_Type(scaling, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->scaling = FIX2INT(scaling);
    return Qnil;
}

static VALUE
im_set_saveinfo_xjustification(self, xjustification)
    VALUE self, xjustification;
{
    GdkImlibSaveInfo *sip;

    Check_Type(xjustification, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->xjustification = FIX2INT(xjustification);
    return Qnil;
}

static VALUE
im_set_saveinfo_yjustification(self, yjustification)
    VALUE self, yjustification;
{
    GdkImlibSaveInfo *sip;

    Check_Type(yjustification, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->yjustification = FIX2INT(yjustification);
    return Qnil;
}

static VALUE
im_set_saveinfo_page_size(self, page_size)
    VALUE self, page_size;
{
    GdkImlibSaveInfo *sip;

    Check_Type(page_size, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->page_size = FIX2INT(page_size);
    return Qnil;
}

static VALUE
im_set_saveinfo_color(self, color)
    VALUE self, color;
{
    GdkImlibSaveInfo *sip;

    Check_Type(color, T_FIXNUM);
    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    sip->color = (char)FIX2INT(color);
    return Qnil;
}

static VALUE
im_get_saveinfo_quality(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX(sip->quality);
}

static VALUE
im_get_saveinfo_scaling(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX(sip->scaling);
}

static VALUE
im_get_saveinfo_xjustification(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX(sip->xjustification);
}

static VALUE
im_get_saveinfo_yjustification(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX(sip->yjustification);
}

static VALUE
im_get_saveinfo_page_size(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX(sip->page_size);
}

static VALUE
im_get_saveinfo_color(self)
    VALUE self;
{
    GdkImlibSaveInfo *sip;

    Data_Get_Struct(self, GdkImlibSaveInfo, sip);
    return INT2FIX((int)sip->color);
}

/*
 * GdkImlib initialize
 */
void
Init_gdk_imlib()
{
    gdk_imlib_init();
    gtk_widget_push_visual(gdk_imlib_get_visual());
    gtk_widget_push_colormap(gdk_imlib_get_colormap());

    /* not implement
       gdk_imlib_init_params()
    */

    /*
     * GdkImlib module
     */
    mImlib = rb_define_module("GdkImlib");

    rb_define_const(mImlib, "RT_PLAIN_PALETTE", INT2FIX(RT_PLAIN_PALETTE));
    rb_define_const(mImlib, "RT_PLAIN_PALETTE_FAST", INT2FIX(RT_PLAIN_PALETTE_FAST));
    rb_define_const(mImlib, "RT_DITHER_PALETTE", INT2FIX(RT_DITHER_PALETTE));
    rb_define_const(mImlib, "RT_DITHER_PALETTE_FAST", INT2FIX(RT_DITHER_PALETTE_FAST));
    rb_define_const(mImlib, "RT_PLAIN_TRUECOL", INT2FIX(RT_PLAIN_TRUECOL));
    rb_define_const(mImlib, "RT_DITHER_TRUECOL", INT2FIX(RT_DITHER_TRUECOL));

    rb_define_module_function(mImlib, "render_type", im_get_render_type, 0);
    rb_define_module_function(mImlib, "render_type=", im_set_render_type, 1);
    rb_define_module_function(mImlib, "load_colors", im_load_colors, 1);
    rb_define_module_function(mImlib, "free_colors", im_free_colors, 0);
    rb_define_module_function(mImlib, "fallback", im_get_fallback, 0);
    rb_define_module_function(mImlib, "fallback=", im_set_fallback, 1);
    rb_define_module_function(mImlib, "sysconfig", im_get_sysconfig, 0);
    rb_define_module_function(mImlib, "push_visual", im_push_visual, 0);
    rb_define_module_function(mImlib, "pop_visual", im_pop_visual, 0);

    /*
     * GdkImlib::Image
     */
    cImlibImage = rb_define_class_under(mImlib, "Image", rb_cData);
    rb_define_singleton_method(cImlibImage, "new", imlib_s_new, 1);
    rb_define_singleton_method(cImlibImage, "create_from_data", im_create_image_from_data, 4);
    rb_define_singleton_method(cImlibImage, "create_from_drawable", im_create_image_from_drawable, 6);
    rb_define_singleton_method(cImlibImage, "render_limit", im_s_get_render_limit, 0);
    rb_define_singleton_method(cImlibImage, "render_limit=", im_s_set_render_limit, 1);
    rb_define_method(cImlibImage, "render", im_render, 2);
    rb_define_method(cImlibImage, "border", im_get_image_border, 0);
    rb_define_method(cImlibImage, "border=", im_set_image_border, 1);
    rb_define_method(cImlibImage, "shape", im_get_image_shape, 0);
    rb_define_method(cImlibImage, "shape=", im_set_image_shape, 1);
    rb_define_method(cImlibImage, "save_to_eim", im_save_image_to_eim, 1);
    rb_define_method(cImlibImage, "add_to_eim", im_add_image_to_eim, 1);
    rb_define_method(cImlibImage, "save_to_ppm", im_save_image_to_ppm, 1);
    rb_define_method(cImlibImage, "modifier=", im_set_image_modifier, 1);
    rb_define_method(cImlibImage, "red_modifier=", im_set_image_red_modifier, 1);
    rb_define_method(cImlibImage, "green_modifier=", im_set_image_green_modifier, 1);
    rb_define_method(cImlibImage, "blue_modifier=", im_set_image_blue_modifier, 1);
    rb_define_method(cImlibImage, "modifier", im_get_image_modifier, 0);
    rb_define_method(cImlibImage, "red_modifier", im_get_image_red_modifier, 0);
    rb_define_method(cImlibImage, "green_modifier", im_get_image_green_modifier, 0);
    rb_define_method(cImlibImage, "blue_modifier", im_get_image_blue_modifier, 0);
    rb_define_method(cImlibImage, "red_curve=", im_set_image_red_curve, 1);
    rb_define_method(cImlibImage, "green_curve=", im_set_image_green_curve, 1);
    rb_define_method(cImlibImage, "blue_curve=", im_set_image_blue_curve, 1);
    rb_define_method(cImlibImage, "red_curve", im_get_image_red_curve, 0);
    rb_define_method(cImlibImage, "green_curve", im_get_image_green_curve, 0);
    rb_define_method(cImlibImage, "blue_curve", im_get_image_blue_curve, 0);
    rb_define_method(cImlibImage, "apply_modifiers_to_rgb", im_apply_modifiers_to_rgb, 0);
    rb_define_method(cImlibImage, "changed", im_changed_image, 0);
    rb_define_method(cImlibImage, "apply", im_apply_image, 1);
    rb_define_method(cImlibImage, "paste", im_paste_image, 5);
    rb_define_method(cImlibImage, "paste_border", im_paste_image_border, 5);
    rb_define_method(cImlibImage, "flip_horizontal", im_flip_image_horizontal, 0);
    rb_define_method(cImlibImage, "flip_vertical", im_flip_image_vertical, 0);
    rb_define_method(cImlibImage, "rotate", im_rotate_image, 1);
    rb_define_method(cImlibImage, "clone", im_clone_image, 0);
    rb_define_method(cImlibImage, "clone_scaled", im_clone_scaled_image, 2);
    rb_define_method(cImlibImage, "crop", im_crop_image, 4);
    rb_define_method(cImlibImage, "crop_and_clone", im_crop_and_clone_image, 4);
    rb_define_method(cImlibImage, "save", im_save_image, -1);
    rb_define_method(cImlibImage, "rgb_width", im_get_rgb_width, 0);
    rb_define_method(cImlibImage, "rgb_height", im_get_rgb_height, 0);
    rb_define_method(cImlibImage, "get_filename", im_get_filename, 0);
    rb_define_method(cImlibImage, "get_pixmap", im_get_pixmap, 0);

    /*
     * GdkImlib::Border
     */
    cImlibBorder = rb_define_class_under(mImlib, "Border", rb_cData);
    rb_define_singleton_method(cImlibBorder, "new", im_s_border_new, 0);
    rb_define_method(cImlibBorder, "right", im_get_border_right, 0);
    rb_define_method(cImlibBorder, "left", im_get_border_left, 0);
    rb_define_method(cImlibBorder, "top", im_get_border_top, 0);
    rb_define_method(cImlibBorder, "bottom", im_get_border_bottom, 0);
    rb_define_method(cImlibBorder, "right=", im_set_border_right, 1);
    rb_define_method(cImlibBorder, "left=", im_set_border_left, 1);
    rb_define_method(cImlibBorder, "top=", im_set_border_top, 1);
    rb_define_method(cImlibBorder, "bottom=", im_set_border_bottom, 1);

    /*
     * GdkImlib::Shape
     */
    cImlibShape = rb_define_class_under(mImlib, "Shape", rb_cData);
    rb_define_singleton_method(cImlibShape, "new", im_s_shape_new, 0);
    rb_define_method(cImlibShape, "red", im_get_shape_r, 0);
    rb_define_method(cImlibShape, "green", im_get_shape_g, 0);
    rb_define_method(cImlibShape, "blue", im_get_shape_b, 0);
    rb_define_method(cImlibShape, "pixel", im_get_shape_pixel, 0);
    rb_define_method(cImlibShape, "red=", im_set_shape_r, 1);
    rb_define_method(cImlibShape, "green=", im_set_shape_g, 1);
    rb_define_method(cImlibShape, "blue=", im_set_shape_b, 1);
    rb_define_method(cImlibShape, "pixel=", im_set_shape_pixel, 0);

    /*
     * GdkImlib::Modifier
     */
    cImlibModifier = rb_define_class_under(mImlib, "Modifier", rb_cData);
    rb_define_singleton_method(cImlibModifier, "new", im_s_modifier_new, 0);
    rb_define_method(cImlibModifier, "gamma", im_get_modifier_gamma, 0);
    rb_define_method(cImlibModifier, "brightness", im_get_modifier_brightness, 0);
    rb_define_method(cImlibModifier, "contrast", im_get_modifier_contrast, 0);
    rb_define_method(cImlibModifier, "gamma=", im_set_modifier_gamma, 1);
    rb_define_method(cImlibModifier, "brightness=", im_set_modifier_brightness, 1);
    rb_define_method(cImlibModifier, "contrast=", im_set_modifier_contrast, 1);

    /*
     * GdkImlib::SaveInfo
     */
    cImlibSaveInfo = rb_define_class_under(mImlib, "SaveInfo", rb_cData);
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_EXECUTIVE", INT2FIX(PAGE_SIZE_EXECUTIVE));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_LETTER", INT2FIX(PAGE_SIZE_LETTER));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_LEGAL", INT2FIX(PAGE_SIZE_LEGAL));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_A4", INT2FIX(PAGE_SIZE_A4));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_A3", INT2FIX(PAGE_SIZE_A3));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_A5", INT2FIX(PAGE_SIZE_A5));
    rb_define_const(cImlibSaveInfo, "PAGE_SIZE_FOLIO", INT2FIX(PAGE_SIZE_FOLIO));
    rb_define_singleton_method(cImlibSaveInfo, "new", im_s_saveinfo_new, 0);
    rb_define_method(cImlibSaveInfo, "quality", im_get_saveinfo_quality, 0);
    rb_define_method(cImlibSaveInfo, "scaling", im_get_saveinfo_scaling, 0);
    rb_define_method(cImlibSaveInfo, "xjustification", im_get_saveinfo_xjustification, 0);
    rb_define_method(cImlibSaveInfo, "yjustification", im_get_saveinfo_yjustification, 0);
    rb_define_method(cImlibSaveInfo, "page_size", im_get_saveinfo_page_size, 0);
    rb_define_method(cImlibSaveInfo, "color", im_get_saveinfo_color, 0);
    rb_define_method(cImlibSaveInfo, "quality=", im_set_saveinfo_quality, 1);
    rb_define_method(cImlibSaveInfo, "scaling=", im_set_saveinfo_scaling, 1);
    rb_define_method(cImlibSaveInfo, "xjustification=", im_set_saveinfo_xjustification, 1);
    rb_define_method(cImlibSaveInfo, "yjustification=", im_set_saveinfo_yjustification, 1);
    rb_define_method(cImlibSaveInfo, "page_size=", im_set_saveinfo_page_size, 1);
    rb_define_method(cImlibSaveInfo, "color=", im_set_saveinfo_color, 1);

}
