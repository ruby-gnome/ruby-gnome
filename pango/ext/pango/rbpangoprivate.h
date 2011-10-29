#ifndef RB_PANGO_PRIVATE_H
#define RB_PANGO_PRIVATE_H

#include "rbpango.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#if PANGO_CHECK_VERSION(1,10,0) && defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#endif

G_BEGIN_DECLS

G_GNUC_INTERNAL void Init_pango_analysis(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_attribute(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_attriterator(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_attrlist(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_cairo(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_cairo_context(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_color(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_context(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_coverage(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_engine(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font_description(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font_face(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font_family(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font_map(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_font_metrics(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_fontset(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_fontset_simple(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_glyph_info(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_glyph_item(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_glyph_string(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_gravity(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_item(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_language(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_layout(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_layout_iter(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_layout_line(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_logattr(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_matrix(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_rectangle(VALUE mPango);
G_GNUC_INTERNAL void Init_pangorenderer(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_script(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_script_iter(VALUE mPango);
G_GNUC_INTERNAL void Init_pango_array(VALUE mPango);

G_END_DECLS

#endif /* RB_PANGO_PRIVATE_H */
