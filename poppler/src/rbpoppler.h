#ifndef __RBPOPPLER_H__
#define __RBPOPPLER_H__

#include "rbpopplerversion.h"

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <poppler.h>

#ifndef POPPLER_TYPE_INDEX_ITER
#  define POPPLER_TYPE_INDEX_ITER (poppler_index_iter_get_type ())
#endif
#ifndef POPPLER_TYPE_FONTS_ITER
#  define POPPLER_TYPE_FONTS_ITER (poppler_fonts_iter_get_type ())
#endif

#ifndef POPPLER_TYPE_DEST
extern GType poppler_dest_get_type (void) G_GNUC_CONST;
#  define POPPLER_TYPE_DEST (poppler_dest_get_type ())
#  define RB_POPPLER_TYPE_DEST_NOT_DEFINED
#endif

#if defined(HAVE_RB_CAIRO_H) && defined(POPPLER_HAS_CAIRO)
#  define RB_POPPLER_CAIRO_AVAILABLE 1
#  include <rb_cairo.h>
#endif

#define POPPLER_RECT2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_RECTANGLE))
#define RVAL2POPPLER_RECT(obj) ((PopplerRectangle *)RVAL2BOXED(obj, POPPLER_TYPE_RECTANGLE))

#ifdef POPPLER_TYPE_COLOR
extern PopplerColor *rb_poppler_ruby_object_to_color(VALUE color);
extern VALUE rb_poppler_ruby_object_from_color_with_free(PopplerColor *color);
#  define RVAL2POPPLER_COLOR(obj) (rb_poppler_ruby_object_to_color(obj))
#  define POPPLER_COLOR2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_COLOR))
#  define POPPLER_COLOR2RVAL_FREE(obj) (rb_poppler_ruby_object_from_color_with_free(obj))
#endif


#define POPPLER_ANNOT2RVAL(obj) (GOBJ2RVAL(obj))
#define RVAL2POPPLER_ANNOT(obj) (POPPLER_ANNOT(RVAL2GOBJ(obj)))

#define POPPLER_ACTION2RVAL(obj) (rb_poppler_ruby_object_from_action(obj))
#define RVAL2POPPLER_ACTION(obj) (rb_poppler_action_from_ruby_object(obj))
#define POPPLER_FORM_FIELD2RVAL(obj) (rb_poppler_ruby_object_from_form_field(obj))
#define RVAL2POPPLER_FORM_FIELD(obj) (POPPLER_FORM_FIELD(RVAL2GOBJ(obj)))

extern VALUE rb_poppler_ruby_object_from_action(PopplerAction *action);
extern PopplerAction *rb_poppler_action_from_ruby_object(VALUE action);
extern VALUE rb_poppler_ruby_object_from_form_field(PopplerFormField *field);

#endif
