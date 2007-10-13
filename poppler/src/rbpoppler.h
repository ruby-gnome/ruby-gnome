#ifndef __RBPOPPLER_H__
#define __RBPOPPLER_H__

#include "rbpopplerversion.h"

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
extern GType poppler_dest_get_type (void) G_GNUC_CONST;
#  define POPPLER_TYPE_DEST (poppler_dest_get_type ())
#  define RB_POPPLER_TYPE_DEST_NOT_DEFINED
#endif

#if defined(HAVE_RB_CAIRO_H) && defined(POPPLER_HAS_CAIRO)
#  define RB_POPPLER_CAIRO_AVAILABLE 1
#  include <rb_cairo.h>
#endif

#define RECT2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_RECTANGLE))
#define RVAL2RECT(obj) ((PopplerRectangle *)RVAL2BOXED(obj, POPPLER_TYPE_RECTANGLE))
#define REGION2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_REGION))
#define RVAL2DEST(obj) ((PopplerDest *)RVAL2BOXED(obj, POPPLER_TYPE_DEST))
#define DEST2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_DEST))
#define TRANS2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION))
#define RVAL2TRANS(obj) ((PopplerPageTransition *)RVAL2BOXED(obj, POPPLER_TYPE_PAGE_TRANSITION))

#define DESTTYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_DEST_TYPE))
#define ACTIONTYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ACTION_TYPE))
#define RVAL2ACTIONTYPE(obj) (RVAL2GENUM(obj, POPPLER_TYPE_ACTION_TYPE))
#define SELSTYLE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_SELECTION_STYLE))
#define RVAL2SELSTYLE(obj) (RVAL2GENUM(obj, POPPLER_TYPE_SELECTION_STYLE))
#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))

#define ACTION2RVAL(obj) (rb_poppler_ruby_object_from_action(obj))
#define RVAL2ACTION(obj) (rb_poppler_action_from_ruby_object(obj))
#define FF2RVAL(obj) (rb_poppler_ruby_object_from_form_field(obj))

extern VALUE rb_poppler_ruby_object_from_action(PopplerAction *action);
extern PopplerAction *rb_poppler_action_from_ruby_object(VALUE action);
#if POPPLER_CHECK_VERSION(0, 6, 0)
extern VALUE rb_poppler_ruby_object_from_form_field(PopplerFormField *field);
#endif

#define RVAL2GDK_PIXBUF(pixbuf) (GDK_PIXBUF(RVAL2GOBJ(pixbuf)))


extern void Init_poppler_document(VALUE mPoppler);
extern void Init_poppler_page(VALUE mPoppler);
extern void Init_poppler_attachment(VALUE mPoppler);
extern void Init_poppler_action(VALUE mPoppler);

#endif
