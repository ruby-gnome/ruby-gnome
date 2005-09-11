/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgp.h"
#include "rbgp-gnome-print-pango.h"
#include <libgnomeprint/gnome-print.h>
#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gnome-print-private.h>
#undef WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/libgnomeprint-enum-types.h>

#include <rbart.h>

#define _SELF(self) (GP_CONTEXT(self))
#define RVAL2GPRC(r_obj) (RVAL2GENUM(r_obj, GNOME_TYPE_PRINT_PRINT_RETURN_CODE))


static VALUE
gp_rc_ok_to_s(VALUE self)
{
  return CSTR2RVAL("OK");
}

static VALUE
gp_rc_unknown_to_s(VALUE self)
{
  return CSTR2RVAL("unknown error");
}

static VALUE
gp_rc_badvalue_to_s(VALUE self)
{
  return CSTR2RVAL("bad value");
}

static VALUE
gp_rc_nocurrentpoint_to_s(VALUE self)
{
  return CSTR2RVAL("no current point");
}

static VALUE
gp_rc_nocurrentpath_to_s(VALUE self)
{
  return CSTR2RVAL("no current path");
}

static VALUE
gp_rc_textcorrupt_to_s(VALUE self)
{
  return CSTR2RVAL("text corrupt");
}

static VALUE
gp_rc_badcontext_to_s(VALUE self)
{
  return CSTR2RVAL("bad context");
}

static VALUE
gp_rc_nopage_to_s(VALUE self)
{
  return CSTR2RVAL("no page");
}

static VALUE
gp_rc_nomatch_to_s(VALUE self)
{
  return CSTR2RVAL("no match");
}

static VALUE
gp_rc_to_s(VALUE self)
{
  switch RVAL2GPRC(self) {
  case GNOME_PRINT_OK:
    return gp_rc_ok_to_s(self);
    break;
  case GNOME_PRINT_ERROR_UNKNOWN:
    return gp_rc_unknown_to_s(self);
    break;
  case GNOME_PRINT_ERROR_BADVALUE:
    return gp_rc_badvalue_to_s(self);
    break;
  case GNOME_PRINT_ERROR_NOCURRENTPOINT:
    return gp_rc_nocurrentpoint_to_s(self);
    break;
  case GNOME_PRINT_ERROR_NOCURRENTPATH:
    return gp_rc_nocurrentpath_to_s(self);
    break;
  case GNOME_PRINT_ERROR_TEXTCORRUPT:
    return gp_rc_textcorrupt_to_s(self);
    break;
  case GNOME_PRINT_ERROR_BADCONTEXT:
    return gp_rc_badcontext_to_s(self);
    break;
  case GNOME_PRINT_ERROR_NOPAGE:
    return gp_rc_nopage_to_s(self);
    break;
  case GNOME_PRINT_ERROR_NOMATCH:
    return gp_rc_nomatch_to_s(self);
    break;
  default:
    return rb_call_super(0, (VALUE *)NULL);
    break;
  }
}

static gdouble *
rb_array_to_gdouble_array(VALUE array)
{
  int i;
  gint size = RARRAY(array)->len;
  gdouble *g_array;

  g_array = ALLOC_N(gdouble, size);

  for (i = 0; i < size; i++) {
    g_array[i] = NUM2DBL(RARRAY(array)->ptr[i]);
  }

  return g_array;
}


static VALUE
gp_context_new(int argc, VALUE *argv, VALUE self)
{
  VALUE config;
  GnomePrintConfig *gp_config;
  
  rb_scan_args(argc, argv, "01", &config);

  if (NIL_P(config)) {
    gp_config = gnome_print_config_default();
  } else {
    gp_config = GP_CONFIG(config);
  }
  
  G_INITIALIZE(self, gnome_print_context_new(gp_config));
  return Qnil;
}


static VALUE
gp_context_close(VALUE self)
{
  return rbgp_check_return_code(gnome_print_context_close(_SELF(self)));
}

static VALUE
gp_context_create_transport(VALUE self)
{
  return rbgp_check_return_code(gnome_print_context_create_transport(_SELF(self)));
}


static VALUE
gp_newpath(VALUE self)
{
  return rbgp_check_return_code(gnome_print_newpath(_SELF(self)));
}

static VALUE
gp_moveto(VALUE self, VALUE x, VALUE y)
{
  return rbgp_check_return_code(gnome_print_moveto(_SELF(self),
                                                   NUM2DBL(x),
                                                   NUM2DBL(y)));
}

static VALUE
gp_lineto(VALUE self, VALUE x, VALUE y)
{
  return rbgp_check_return_code(gnome_print_lineto(_SELF(self),
                                                   NUM2DBL(x),
                                                   NUM2DBL(y)));
}

static VALUE
gp_curveto(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2, VALUE x3, VALUE y3)
{
  return rbgp_check_return_code(gnome_print_curveto(_SELF(self),
                                                    NUM2DBL(x1),
                                                    NUM2DBL(y1),
                                                    NUM2DBL(x2),
                                                    NUM2DBL(y2),
                                                    NUM2DBL(x3),
                                                    NUM2DBL(y3)));
}

static VALUE
gp_closepath(VALUE self)
{
  return rbgp_check_return_code(gnome_print_closepath(_SELF(self)));
}

static VALUE
gp_strokepath(VALUE self)
{
  return rbgp_check_return_code(gnome_print_strokepath(_SELF(self)));
}

static VALUE
_gp_bpath(VALUE self, VALUE bpath, VALUE append)
{
  return rbgp_check_return_code(gnome_print_bpath(_SELF(self),
                                             get_art_bpath(bpath),
                                             RTEST(append)));
}

static VALUE
gp_bpath(int argc, VALUE *argv, VALUE self)
{
  VALUE bpath, append;

  if (rb_scan_args(argc, argv, "11", &bpath, &append) == 1) {
    append = Qtrue;
  }

  return _gp_bpath(self, bpath, append);
}

static VALUE
_gp_vpath(VALUE self, VALUE vpath, VALUE append)
{
  return rbgp_check_return_code(gnome_print_vpath(_SELF(self),
                                                  get_art_vpath(vpath),
                                                  RTEST(append)));
}

static VALUE
gp_vpath(int argc, VALUE *argv, VALUE self)
{
  VALUE vpath, append;

  if (rb_scan_args(argc, argv, "11", &vpath, &append) == 1) {
    append = Qtrue;
  }
  
  return _gp_vpath(self, vpath, append);
}

static VALUE
gp_path(int argc, VALUE *argv, VALUE self)
{
  VALUE path, append;

  if (rb_scan_args(argc, argv, "11", &path, &append) == 1) {
    append = Qtrue;
  }
  
  if (rb_obj_is_kind_of(path, artBpath)) {
    return _gp_bpath(self, path, append);
  } else if (rb_obj_is_kind_of(path, artVpath)) {
    return _gp_vpath(self, path, append);
  } else {
    rb_raise(rb_eTypeError, "not an Art::Bpath or an Art::Vpath");
  }
}

static VALUE
gp_arcto(VALUE self, VALUE x, VALUE y, VALUE radius,
         VALUE angle1, VALUE angle2, VALUE clockwise)
{
  return rbgp_check_return_code(gnome_print_arcto(_SELF(self),
                                                  NUM2DBL(x),
                                                  NUM2DBL(y),
                                                  NUM2DBL(radius),
                                                  NUM2DBL(angle1),
                                                  NUM2DBL(angle2),
                                                  RTEST(clockwise) ? 1 : 0));
}

static VALUE
gp_setrgbcolor(VALUE self, VALUE r, VALUE g, VALUE b)
{
  return rbgp_check_return_code(gnome_print_setrgbcolor(_SELF(self),
                                                        NUM2DBL(r),
                                                        NUM2DBL(g),
                                                        NUM2DBL(b)));
}

static VALUE
gp_setopacity(VALUE self, VALUE opacity)
{
  return rbgp_check_return_code(gnome_print_setopacity(_SELF(self),
                                                       NUM2DBL(opacity)));
}

static VALUE
gp_setlinewidth(VALUE self, VALUE width)
{
  return rbgp_check_return_code(gnome_print_setlinewidth(_SELF(self),
                                                         NUM2DBL(width)));
}

static VALUE
gp_setmiterlimit(VALUE self, VALUE limit)
{
  return rbgp_check_return_code(gnome_print_setmiterlimit(_SELF(self),
                                                          NUM2DBL(limit)));
}

static VALUE
gp_setlinejoin(VALUE self, VALUE jointype)
{
  return rbgp_check_return_code(gnome_print_setlinejoin(_SELF(self),
                                                        NUM2INT(jointype)));
}

static VALUE
gp_setlinecap(VALUE self, VALUE captype)
{
  return rbgp_check_return_code(gnome_print_setlinecap(_SELF(self),
                                                       NUM2INT(captype)));
}

static VALUE
gp_setdash(VALUE self, VALUE values, VALUE offset)
{
  gint ret;
  gdouble *g_values = rb_array_to_gdouble_array(values);
  ret = gnome_print_setdash(_SELF(self),
                            RARRAY(values)->len,
                            g_values,
                            NUM2INT(offset));
  free(g_values);
  return rbgp_check_return_code(ret);
}

static VALUE
gp_clip(VALUE self)
{
  return rbgp_check_return_code(gnome_print_clip(_SELF(self)));
}

static VALUE
gp_eoclip(VALUE self)
{
  return rbgp_check_return_code(gnome_print_eoclip(_SELF(self)));
}

static VALUE
gp_concat(VALUE self, VALUE matrix)
{
  return rbgp_check_return_code(gnome_print_concat(_SELF(self),
                                                   get_art_affine(matrix)));
}

static VALUE
gp_scale(VALUE self, VALUE sx, VALUE sy)
{
  return rbgp_check_return_code(gnome_print_scale(_SELF(self),
                                                  NUM2DBL(sx),
                                                  NUM2DBL(sy)));
}

static VALUE
gp_rotate(VALUE self, VALUE theta)
{
  return rbgp_check_return_code(gnome_print_rotate(_SELF(self),
                                                   NUM2DBL(theta)));
}

static VALUE
gp_translate(VALUE self, VALUE x, VALUE y)
{
  return rbgp_check_return_code(gnome_print_translate(_SELF(self),
                                                      NUM2DBL(x),
                                                      NUM2DBL(y)));
}

static VALUE
gp_grestore(VALUE self)
{
  return rbgp_check_return_code(gnome_print_grestore(_SELF(self)));
}

static VALUE
gp_gsave(VALUE self)
{
  VALUE result;
  
  result = rbgp_check_return_code(gnome_print_gsave(_SELF(self)));
  if (rb_block_given_p()) {
    result = rb_ensure(rb_yield, self, gp_grestore, self);
  }
      
  return result;
}

static VALUE
gp_fill(VALUE self)
{
  return rbgp_check_return_code(gnome_print_fill(_SELF(self)));
}

static VALUE
gp_eofill(VALUE self)
{
  return rbgp_check_return_code(gnome_print_eofill(_SELF(self)));
}

static VALUE
gp_stroke(VALUE self)
{
  return rbgp_check_return_code(gnome_print_stroke(_SELF(self)));
}

static VALUE
gp_show(VALUE self, VALUE text)
{
  return rbgp_check_return_code(gnome_print_show(_SELF(self),
                                                 (const guchar *)RVAL2CSTR(text)));
}

static VALUE
gp_show_sized(VALUE self, VALUE text, VALUE bytes)
{
  return rbgp_check_return_code(gnome_print_show_sized(_SELF(self),
                                                       (const guchar *)RVAL2CSTR(text),
                                                       NUM2INT(bytes)));
}


static VALUE
gp_grayimage(VALUE self, VALUE data,
             VALUE width, VALUE height, VALUE rowstride)
{
  return rbgp_check_return_code(gnome_print_grayimage(_SELF(self),
                                                      (const guchar *)RVAL2CSTR(data),
                                                      NUM2INT(width),
                                                      NUM2INT(height),
                                                      NUM2INT(rowstride)));
}

static VALUE
gp_rgbimage(VALUE self, VALUE data,
            VALUE width, VALUE height, VALUE rowstride)
{
  return rbgp_check_return_code(gnome_print_rgbimage(_SELF(self),
                                                     (const guchar *)RVAL2CSTR(data),
                                                     NUM2INT(width),
                                                     NUM2INT(height),
                                                     NUM2INT(rowstride)));
}

static VALUE
gp_rgbaimage(VALUE self, VALUE data,
             VALUE width, VALUE height, VALUE rowstride)
{
  return rbgp_check_return_code(gnome_print_rgbaimage(_SELF(self),
                                                      (const guchar *)RVAL2CSTR(data),
                                                      NUM2INT(width),
                                                      NUM2INT(height),
                                                      NUM2INT(rowstride)));
}


static VALUE
gp_showpage(VALUE self)
{
  return rbgp_check_return_code(gnome_print_showpage(_SELF(self)));
}

static VALUE
gp_beginpage(int argc, VALUE *argv, VALUE self)
{
  VALUE name, result;
  guchar *c_name;
  
  rb_scan_args(argc, argv, "01", &name);
  
  if (NIL_P(name)) {
    c_name = NULL;
  } else {
    c_name = (guchar*)RVAL2CSTR(name);
  }

  result = rbgp_check_return_code(gnome_print_beginpage(_SELF(self), c_name));
  if (rb_block_given_p()) {
    rb_yield(result);
    result = gp_showpage(self);
  }
    
  return result;
}

static VALUE
gp_end_doc(VALUE self)
{
  return rbgp_check_return_code(gnome_print_end_doc(_SELF(self)));
}


static VALUE
gp_line_stroked(VALUE self, VALUE x0, VALUE y0, VALUE x1, VALUE y1)
{
  return rbgp_check_return_code(gnome_print_line_stroked(_SELF(self),
                                                         NUM2DBL(x0),
                                                         NUM2DBL(y0),
                                                         NUM2DBL(x1),
                                                         NUM2DBL(y1)));
}

static VALUE
gp_rect_stroked(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
  return rbgp_check_return_code(gnome_print_rect_stroked(_SELF(self),
                                                         NUM2DBL(x),
                                                         NUM2DBL(y),
                                                         NUM2DBL(width),
                                                         NUM2DBL(height)));
}

static VALUE
gp_rect_filled(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
  return rbgp_check_return_code(gnome_print_rect_filled(_SELF(self),
                                                        NUM2DBL(x),
                                                        NUM2DBL(y),
                                                        NUM2DBL(width),
                                                        NUM2DBL(height)));
}


static VALUE
gp_context_pages(VALUE self)
{
  return INT2NUM(_SELF(self)->pages);
}


void
Init_gnome_print(VALUE mGnome)
{
  VALUE cGPC = G_DEF_CLASS(GNOME_TYPE_PRINT_CONTEXT, "PrintContext", mGnome);
  VALUE mGP = rb_define_module_under(mGnome, "Print");
  VALUE mGPP = Init_gnome_print_pango(mGnome);
  
  VALUE cGPRC = G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_RETURN_CODE,
                            "PrintReturnCode", mGnome);
  G_DEF_CONSTANTS(mGP, GNOME_TYPE_PRINT_PRINT_RETURN_CODE, "GNOME_PRINT_");

  rb_include_module(cGPC, mGPP);
  

  rb_define_method(cGPRC, "to_s", gp_rc_to_s, 0);

  
  rb_define_method(cGPC, "initialize", gp_context_new, -1);

  rb_define_method(cGPC, "close", gp_context_close, 0);
  rb_define_method(cGPC, "create_transport", gp_context_create_transport, 0);

/* Path manipulation */
  rb_define_method(cGPC, "new_path", gp_newpath, 0);
  rb_define_method(cGPC, "move_to", gp_moveto, 2);
  rb_define_method(cGPC, "line_to", gp_lineto, 2);
  rb_define_method(cGPC, "curve_to", gp_curveto, 6);
  rb_define_method(cGPC, "close_path", gp_closepath, 0);
  rb_define_method(cGPC, "stroke_path", gp_strokepath, 0);
  rb_define_method(cGPC, "bpath", gp_bpath, -1);
  rb_define_method(cGPC, "vpath", gp_vpath, -1);
  rb_define_method(cGPC, "path", gp_path, -1);
  rb_define_method(cGPC, "arc_to", gp_arcto, 6);

/* Graphic state manipulation */
  rb_define_method(cGPC, "set_rgb_color", gp_setrgbcolor, 3);
  rb_define_method(cGPC, "set_opacity", gp_setopacity, 1);
  rb_define_method(cGPC, "set_line_width", gp_setlinewidth, 1);
  rb_define_method(cGPC, "set_miter_limit", gp_setmiterlimit, 1);
  rb_define_method(cGPC, "set_line_join", gp_setlinejoin, 1);
  rb_define_method(cGPC, "set_line_cap", gp_setlinecap, 1);
  rb_define_method(cGPC, "set_dash", gp_setdash, 2);
  rb_define_method(cGPC, "clip", gp_clip, 0);
  rb_define_method(cGPC, "eoclip", gp_eoclip, 0);

/* CTM manipulation */
  rb_define_method(cGPC, "concat", gp_concat, 1);
  rb_define_method(cGPC, "scale", gp_scale, 2);
  rb_define_method(cGPC, "rotate", gp_rotate, 1);
  rb_define_method(cGPC, "translate", gp_translate, 2);

/* Stack */
  rb_define_method(cGPC, "save", gp_gsave, 0);
  rb_define_method(cGPC, "restore", gp_grestore, 0);

/* Painting */
  rb_define_method(cGPC, "fill", gp_fill, 0);
  rb_define_method(cGPC, "eofill", gp_eofill, 0);
  rb_define_method(cGPC, "stroke", gp_stroke, 0);

/* Text drawing */
  rb_define_method(cGPC, "show", gp_show, 1);
  rb_define_method(cGPC, "show_sized", gp_show_sized, 2);

/* Images */
  rb_define_method(cGPC, "gray_image", gp_grayimage, 4);
  rb_define_method(cGPC, "rgb_image", gp_rgbimage, 4);
  rb_define_method(cGPC, "rgba_image", gp_rgbaimage, 4);

/* Page */
  rb_define_method(cGPC, "begin_page", gp_beginpage, -1);
  rb_define_method(cGPC, "show_page", gp_showpage, 0);
  rb_define_method(cGPC, "end_doc", gp_end_doc, 0);

/*
 * Convenience methods for printing straight lines and rectangles.
 */
  rb_define_method(cGPC, "line_stroked", gp_line_stroked, 4);
  rb_define_method(cGPC, "rect_stroked", gp_rect_stroked, 4);
  rb_define_method(cGPC, "rect_filled", gp_rect_filled, 4);

/* only for Ruby/GnomePrint */
  rb_define_method(cGPC, "pages", gp_context_pages, 0);

  
  G_DEF_SETTERS(cGPC);
}
