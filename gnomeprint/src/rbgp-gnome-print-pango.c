#include "rbgp.h"
#include <libgnomeprint/gnome-print-pango.h>

#define _SELF(self) (GP_CONTEXT(self))

static VALUE
gp_pango_font_map_new(VALUE self)
{
  return GOBJ2RVAL(gnome_print_pango_font_map_new());
}

static VALUE
gp_pango_get_default_font_map(VALUE self)
{
  return GOBJ2RVAL(gnome_print_pango_get_default_font_map());
}

static VALUE
gp_pango_create_context(VALUE self, VALUE fontmap)
{
  return GOBJ2RVAL(gnome_print_pango_create_context(RVAL2GOBJ(fontmap)));
}


static VALUE
gp_pango_update_context(VALUE self, VALUE context)
{
  gnome_print_pango_update_context(RVAL2GOBJ(context), _SELF(self));
  return self;
}

static VALUE
gp_pango_create_layout(VALUE self)
{
  return GOBJ2RVAL(gnome_print_pango_create_layout(_SELF(self)));
}

static VALUE
gp_pango_glyph_string(VALUE self, VALUE font, VALUE glyphs)
{
  gnome_print_pango_glyph_string(_SELF(self),
                                 RVAL2GOBJ(font),
                                 RVAL2GOBJ(glyphs));
  return self;
}

static VALUE
gp_pango_layout_line(VALUE self, VALUE line)
{
  gnome_print_pango_layout_line(_SELF(self), RVAL2GOBJ(line));
  return self;
}

static VALUE
gp_pango_layout(VALUE self, VALUE layout)
{
  gnome_print_pango_layout(_SELF(self), RVAL2GOBJ(layout));
  return self;
}


VALUE
Init_gnome_print_pango(VALUE mGnome, VALUE mGP, VALUE cGPC)
{
  VALUE mGPP = rb_define_module_under(mGnome, "PrintPango");

  rb_define_module_function(mGPP, "font_map_new", gp_pango_font_map_new, 0);
  rb_define_module_function(mGPP, "default_font_map",
                            gp_pango_get_default_font_map, 0);
  rb_define_module_function(mGPP, "create_context", gp_pango_create_context, 1);
  
  rb_define_method(mGPP, "update_context", gp_pango_update_context, 1);
  rb_define_method(mGPP, "create_layout", gp_pango_create_layout, 0);
  rb_define_method(mGPP, "glyph_string", gp_pango_glyph_string, 2);
  rb_define_method(mGPP, "layout_line", gp_pango_layout_line, 1);
  rb_define_method(mGPP, "layout", gp_pango_layout, 1);

  return mGPP;
}
