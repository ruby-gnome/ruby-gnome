/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

   rbart_canvas.c -

   $Author: mutoh $
   $Date: 2003/02/18 17:01:21 $

   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>

**********************************************************************/

#include "rbart.h"

#include <jpeglib.h>
#include <png.h>

#define ART_CANVAS_PNG_INTERLACE_ADAM7 1
#define ART_CANVAS_ALPHA_MASK 2

VALUE artCanvas;
VALUE artCanvasColor;

typedef struct {
    int flags;
    int width;
    int height;
    art_u8 *rgb;
    art_u8 *mask;
} ArtCanvas;

#define Canvas_Ptr(r_obj) ((ArtCanvas *)RDATA(r_obj)->data)

ArtCanvas *
rbart_get_art_canvas(r_obj)
    VALUE r_obj;
{
    if (!rb_obj_is_instance_of(r_obj, artCanvas))
        rb_raise(rb_eTypeError, "not an Art::Canvas");
    return Canvas_Ptr(r_obj);
}


static void
canvas_free(canvas)
    ArtCanvas *canvas;
{
    xfree(canvas->rgb);
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
        xfree(canvas->mask);
    xfree(canvas);
}

static VALUE
canvas_s_new(argc, argv, r_klass)
    int argc;
    VALUE *argv;
    VALUE r_klass;
{
    VALUE r_width;
    VALUE r_height;
    VALUE r_color;
    VALUE r_flags;
    ArtCanvas *canvas;
    art_u32 color;
    rb_scan_args(argc, argv, "22", &r_width, &r_height, &r_color, &r_flags);
    canvas = (ArtCanvas *) xmalloc(sizeof(ArtCanvas));
    canvas->flags = r_flags == Qnil ? 0 : NUM2INT(r_flags);
    canvas->width = NUM2INT(r_width);
    canvas->height = NUM2INT(r_height);
    canvas->rgb = (art_u8 *) xcalloc(3 * canvas->width * canvas->height, sizeof(art_u8));
    color = r_color == Qnil ? 0xffffffff : NUM2ULONG(r_color);
    art_rgb_run_alpha(canvas->rgb, color >> 24, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, canvas->width * canvas->height);
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
    {
        canvas->mask = (art_u8 *) xcalloc(3 * canvas->width * canvas->height, sizeof(art_u8));
        art_rgb_run_alpha(canvas->mask, 0x00, 0x00, 0x00, color & 0xff, canvas->width * canvas->height);
    }
    else
        canvas->mask = NULL;
    return Data_Wrap_Struct(artCanvas, 0, canvas_free, canvas);
}

static VALUE
canvas_aref(r_self, r_x, r_y)
    VALUE r_self;
    VALUE r_x;
    VALUE r_y;
{
    ArtCanvas *canvas;
    int x;
    int y;
    art_u8 *rgb_pixel;
    art_u8 mask_value;
    canvas = rbart_get_art_canvas(r_self);
    x = NUM2INT(r_x);
    y = NUM2INT(r_y);
    if (x < 0 || canvas->width <= x || y < 0 || canvas->height <= y)
        rb_raise(rb_eIndexError, "index out of range");
    rgb_pixel = canvas->rgb + 3 * (x + y * canvas->width);
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
        mask_value = canvas->mask[3 * (x + y * canvas->width)];
    else
        mask_value = 0xff;
    return ULONG2NUM((rgb_pixel[0] << 24) | (rgb_pixel[1] << 16) | (rgb_pixel[2] << 8) | mask_value);
}

static VALUE
canvas_aref_set(r_self, r_x, r_y, r_color)
    VALUE r_self;
    VALUE r_x;
    VALUE r_y;
    VALUE r_color;
{
    ArtCanvas *canvas;
    int x;
    int y;
    art_u32 color;
    art_u8 *pixel;
    canvas = rbart_get_art_canvas(r_self);
    x = NUM2INT(r_x);
    y = NUM2INT(r_y);
    if (x < 0 || canvas->width <= x || y < 0 || canvas->height <= y)
        rb_raise(rb_eIndexError, "index out of range");
    color = NUM2ULONG(r_color);
    pixel = canvas->rgb + 3 * (x + y * canvas->width);
    pixel[0] = color >> 24;
    pixel[1] = (color >> 16) & 0xff;
    pixel[2] = (color >> 8) & 0xff;
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
    {
        art_u8 *mask_pixel;
        mask_pixel = canvas->mask + 3 * (x + y * canvas->width);
        mask_pixel[0] = mask_pixel[1] = mask_pixel[2] = color & 0xff;
    }
    return r_color;
}

static VALUE
canvas_flags(r_self)
    VALUE r_self;
{
    return INT2NUM(rbart_get_art_canvas(r_self)->flags);
}

static VALUE
canvas_height(r_self)
    VALUE r_self;
{
    return INT2NUM(rbart_get_art_canvas(r_self)->height);
}

static VALUE
canvas_render_svp(r_self, r_svp, r_color)
    VALUE r_self;
    VALUE r_svp;
    VALUE r_color;
{
    ArtCanvas *canvas;
    ArtSVP *svp;
    art_u32 color;
    canvas = rbart_get_art_canvas(r_self);
    svp = get_art_svp(r_svp);
    color = NUM2ULONG(r_color);
    art_rgb_svp_alpha(svp, 0, 0, canvas->width, canvas->height, color, canvas->rgb, 3 * canvas->width, NULL);
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
        art_rgb_svp_alpha(svp, 0, 0, canvas->width, canvas->height, 0xffffff00 | (color & 0xff), canvas->mask, 3 * canvas->width, NULL);
    return r_self;
}

#if 0
static void
user_init_destination(cinfo)
    j_compress_ptr cinfo;
{
    VALUE r_buffer;
    r_buffer = (VALUE) cinfo->client_data;
    cinfo->dest->next_output_byte = (JOCTET *) RSTRING(r_buffer)->ptr;
    cinfo->dest->free_in_buffer = RSTRING(r_buffer)->aux.capa;
}

static boolean
user_empty_output_buffer(cinfo)
    j_compress_ptr cinfo;
{
    VALUE r_buffer;
    int old_capa;
    r_buffer = (VALUE) cinfo->client_data;
    old_capa = RSTRING(r_buffer)->aux.capa;
    rb_str_resize(r_buffer, old_capa + 16384);
    cinfo->dest->next_output_byte = (JOCTET *) RSTRING(r_buffer)->ptr + old_capa;
    cinfo->dest->free_in_buffer += RSTRING(r_buffer)->aux.capa - old_capa;
    return TRUE;
}

static void
user_term_destination(cinfo)
    j_compress_ptr cinfo;
{
    VALUE r_buffer;
    r_buffer = (VALUE) cinfo->client_data;
    RSTRING(r_buffer)->len = RSTRING(r_buffer)->aux.capa - cinfo->dest->free_in_buffer;
    RSTRING(r_buffer)->ptr[RSTRING(r_buffer)->len] = '\0';
}
#endif

struct client_data {
    VALUE r_result;
    JOCTET buffer[BUFSIZ];
};

static void
user_init_destination(cinfo)
    j_compress_ptr cinfo;
{
    struct client_data *cd;
    cd = (struct client_data *) cinfo->client_data;
    cinfo->dest->next_output_byte = cd->buffer;
    cinfo->dest->free_in_buffer = BUFSIZ;
}

static boolean
user_empty_output_buffer(cinfo)
    j_compress_ptr cinfo;
{
    struct client_data *cd;
    cd = (struct client_data *) cinfo->client_data;
    rb_str_cat(cd->r_result, cd->buffer, BUFSIZ);
    cinfo->dest->next_output_byte = cd->buffer;
    cinfo->dest->free_in_buffer = BUFSIZ;
    return TRUE;
}

static void
user_term_destination(cinfo)
    j_compress_ptr cinfo;
{
    struct client_data *cd;
    cd = (struct client_data *) cinfo->client_data;
    rb_str_cat(cd->r_result, cd->buffer, cinfo->dest->next_output_byte - cd->buffer);
}


static VALUE
canvas_to_jpeg(argc, argv, r_self)
    int argc;
    VALUE *argv;
    VALUE r_self;
{
    VALUE r_quality;
    ArtCanvas *canvas;
    struct jpeg_error_mgr err;
    struct jpeg_compress_struct cinfo;
    struct jpeg_destination_mgr dest;
    JSAMPLE **row_pointers;
    int i;
    struct client_data cd;

    rb_scan_args(argc, argv, "01", &r_quality);
    canvas = rbart_get_art_canvas(r_self);

    cinfo.err = jpeg_std_error(&err);
    jpeg_create_compress(&cinfo);

    cinfo.image_width = canvas->width;
    cinfo.image_height = canvas->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    if (r_quality != Qnil)
        jpeg_set_quality(&cinfo, NUM2INT(r_quality), TRUE);
    cinfo.optimize_coding = TRUE;

    cd.r_result = rb_str_new(NULL, 0);
    cinfo.client_data = (void *) &cd;

    dest.next_output_byte = NULL;
    dest.free_in_buffer = 0;
    dest.init_destination = user_init_destination;
    dest.empty_output_buffer = user_empty_output_buffer;
    dest.term_destination = user_term_destination;
    cinfo.dest = &dest;

    jpeg_start_compress(&cinfo, TRUE);
    row_pointers = (JSAMPLE **) xcalloc(canvas->height, sizeof(JSAMPLE *));
    for (i = 0; i < canvas->height; ++i)
        row_pointers[i] = canvas->rgb + 3 * i * canvas->width;
    jpeg_write_scanlines(&cinfo, row_pointers, canvas->height);
    xfree(row_pointers);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return cd.r_result;
}

static void
user_write_data(png_ptr, data, length)
    png_structp png_ptr;
    png_bytep data;
    png_uint_32 length;
{
    VALUE r_buffer;
    r_buffer = (VALUE) png_get_io_ptr(png_ptr);
    rb_str_cat(r_buffer, data, length);
}

static void
user_flush_data(png_ptr)
    png_structp png_ptr;
{
}

static VALUE
canvas_to_png(argc, argv, r_self)
    int argc;
    VALUE *argv;
    VALUE r_self;
{
    VALUE r_flags;
    int flags;
    ArtCanvas *canvas;
    png_structp png_ptr;
    png_infop info_ptr;
    VALUE r_result;
    art_u8 *rgba = NULL;
    png_bytepp row_pointers;
    int i;

    rb_scan_args(argc, argv, "01", &r_flags);
    flags = r_flags == Qnil ? 0 : NUM2INT(r_flags);
    canvas = rbart_get_art_canvas(r_self);

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
        return Qnil;

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_write_struct(&png_ptr, NULL);
        return Qnil;
    }

    if (setjmp(png_ptr->jmpbuf))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return Qnil;
    }

    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    r_result = rb_str_new(NULL, 0);
    png_set_write_fn(png_ptr, (void *) r_result, user_write_data, user_flush_data);
    png_set_IHDR(png_ptr, info_ptr, canvas->width, canvas->height, 8,
                 canvas->flags & ART_CANVAS_ALPHA_MASK ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB,
                 flags & ART_CANVAS_PNG_INTERLACE_ADAM7 ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    row_pointers = (png_bytepp) xcalloc(canvas->height, sizeof(png_bytep));
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
    {
        rgba = (art_u8 *) xcalloc(4 * canvas->width * canvas->height, sizeof(art_u8));
        for (i = 0; i < canvas->width * canvas->height; ++i)
        {
            rgba[4 * i + 0] = canvas->rgb[3 * i + 0];
            rgba[4 * i + 1] = canvas->rgb[3 * i + 1];
            rgba[4 * i + 2] = canvas->rgb[3 * i + 2];
            rgba[4 * i + 3] = canvas->mask[3 * i + 0];
        }
        for (i = 0; i < canvas->height; ++i)
            row_pointers[i] = rgba + 4 * i * canvas->width;
    }
    else
        for (i = 0; i < canvas->height; ++i)
            row_pointers[i] = canvas->rgb + 3 * i * canvas->width;
    png_write_image(png_ptr, row_pointers);
    xfree(row_pointers);
    if (canvas->flags & ART_CANVAS_ALPHA_MASK)
        xfree(rgba);
    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return r_result;
}

static VALUE
canvas_width(r_self)
    VALUE r_self;
{
    return INT2NUM(rbart_get_art_canvas(r_self)->width);
}


static VALUE
color_new(argc, argv, r_klass)
    int argc;
    VALUE *argv;
    VALUE r_klass;
{
    art_u8 components[4];
    int i;
    components[3] = 0xff;
    switch (argc)
    {
#if 0
      case 2:
        switch (TYPE(argv[1]))
        {
          case T_BIGNUM:
          case T_FIXNUM:
            components[3] = NUM2INT(argv[1]);
            break;
          case T_FLOAT:
            components[3] = 0xff * NUM2DBL(argv[1]) + 0.5;
            break;
          default:
            rb_raise(rb_eTypeError, "expect an Integer or Float");
            break;
        }
      case 1:
        switch (TYPE(argv[0]))
        {
          case T_STRING:
            if ((RSTRING(argv[0])->len - 1) % 3 != 0 || RSTRING(argv[0])->ptr[0] != '#'))
            rb_raise(rb_eTypeError, "invalid color");
                                                
            break;
      default:
        rb_raise(rb_eTypeError, "expect a String");
        break;
    }
    break;
#endif
  case 3:
  case 4:
    for (i = 0; i < argc; ++i)
        switch (TYPE(argv[i]))
        {
          case T_BIGNUM:
          case T_FIXNUM:
            components[i] = NUM2INT(argv[i]);
            break;
          case T_FLOAT:
            components[i] = 0xff * NUM2DBL(argv[i]) + 0.5;
            break;
          default:
            rb_raise(rb_eTypeError, "expect an Integer or Float");
            break;
        }
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (expect 1, 3, or 4)");
    break;
}
return ULONG2NUM((components[0] << 24) | (components[1] << 16) | (components[2] << 8) | (components[3]));
}

void
Init_art_canvas(mArt)
    VALUE mArt;
{
    artCanvas = rb_define_class_under(mArt, "Canvas", rb_cObject);
    rb_define_const(artCanvas, "ALPHA_MASK", INT2NUM(ART_CANVAS_ALPHA_MASK));
    rb_define_const(artCanvas, "PNG_INTERLACE_ADAM7", INT2NUM(ART_CANVAS_PNG_INTERLACE_ADAM7));
    rb_define_singleton_method(artCanvas, "new", canvas_s_new, -1);
    rb_define_method(artCanvas, "[]", canvas_aref, 2);
    rb_define_method(artCanvas, "[]=", canvas_aref_set, 3);
    rb_define_method(artCanvas, "flags", canvas_flags, 0);
    rb_define_method(artCanvas, "height", canvas_height, 0);
    rb_define_method(artCanvas, "render_svp", canvas_render_svp, 2);
    rb_define_method(artCanvas, "to_jpeg", canvas_to_jpeg, -1);
    rb_define_method(artCanvas, "to_png", canvas_to_png, -1);
    rb_define_method(artCanvas, "width", canvas_width, 0);

    artCanvasColor = rb_define_module_under(artCanvas, "Color");
    rb_define_const(artCanvasColor, "BLACK", ULONG2NUM(0x000000ff));
    rb_define_const(artCanvasColor, "BLUE", ULONG2NUM(0x0000ffff));
    rb_define_const(artCanvasColor, "CYAN", ULONG2NUM(0x00ffffff));
    rb_define_const(artCanvasColor, "GREEN", ULONG2NUM(0x00ff00ff));
    rb_define_const(artCanvasColor, "MAGENTA", ULONG2NUM(0xff00ffff));
    rb_define_const(artCanvasColor, "RED", ULONG2NUM(0xff0000ff));
    rb_define_const(artCanvasColor, "WHITE", ULONG2NUM(0xffffffff));
    rb_define_const(artCanvasColor, "YELLOW", ULONG2NUM(0xffff00ff));
    rb_define_singleton_method(artCanvasColor, "new", color_new, -1);
}
