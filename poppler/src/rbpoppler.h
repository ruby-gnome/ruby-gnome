#ifndef __RBPOPPLER_H__
#define __RBPOPPLER_H__

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#ifndef GDK_TYPE_REGION
extern GType gdk_region_get_type(void);
#  define GDK_TYPE_REGION (gdk_region_get_type())
#endif

#include <poppler.h>

#ifndef POPPLER_TYPE_INDEX_ITER
#  define POPPLER_TYPE_INDEX_ITER (poppler_index_iter_get_type ())
#endif
#ifndef POPPLER_TYPE_FONTS_ITER
#  define POPPLER_TYPE_FONTS_ITER (poppler_fonts_iter_get_type ())
#endif

#ifndef POPPLER_TYPE_DEST
GType poppler_dest_get_type (void) G_GNUC_CONST;
#  define POPPLER_TYPE_DEST (poppler_dest_get_type ())
#  define RB_POPPLER_TYPE_DEST_NOT_DEFINED
#endif

#if defined(HAVE_RB_CAIRO_H) && defined(POPPLER_HAS_CAIRO)
#  define RB_POPPLER_CAIRO_AVAILABLE 1
#  include <rb_cairo.h>
#endif

#define ACTION2RVAL(obj) (GOBJ2RVAL(obj))
#define RVAL2ACTION(obj) (RVAL2GOBJ(obj))
#define RECT2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_RECTANGLE))
#define RVAL2RECT(obj) ((PopplerRectangle *)RVAL2BOXED(obj, POPPLER_TYPE_RECTANGLE))
#define REGION2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_REGION))
#define RVAL2DEST(obj) ((PopplerDest *)RVAL2BOXED(obj, POPPLER_TYPE_DEST))
#define DESTTYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_DEST_TYPE))
#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))

void Init_poppler_document(VALUE mPoppler);
void Init_poppler_page(VALUE mPoppler);
void Init_poppler_attachment(VALUE mPoppler);
void Init_poppler_action(VALUE mPoppler);

#endif
