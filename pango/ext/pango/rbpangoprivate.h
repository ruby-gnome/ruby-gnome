#ifndef RB_PANGO_PRIVATE_H
#define RB_PANGO_PRIVATE_H

#include "rbpango.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

G_BEGIN_DECLS

G_GNUC_INTERNAL void Init_pango_analysis(void);
G_GNUC_INTERNAL void Init_pango_attribute(void);
G_GNUC_INTERNAL void Init_pango_attriterator(void);
G_GNUC_INTERNAL void Init_pango_attrlist(void);
G_GNUC_INTERNAL void Init_pango_cairo(void);
G_GNUC_INTERNAL void Init_pango_color(void);
G_GNUC_INTERNAL void Init_pango_context(void);
G_GNUC_INTERNAL void Init_pango_coverage(void);
G_GNUC_INTERNAL void Init_pango_engine(void);
G_GNUC_INTERNAL void Init_pango_font(void);
G_GNUC_INTERNAL void Init_pango_font_description(void);
G_GNUC_INTERNAL void Init_pango_font_face(void);
G_GNUC_INTERNAL void Init_pango_font_family(void);
G_GNUC_INTERNAL void Init_pango_font_map(void);
G_GNUC_INTERNAL void Init_pango_font_metrics(void);
G_GNUC_INTERNAL void Init_pango_fontset(void);
G_GNUC_INTERNAL void Init_pango_fontset_simple(void);
G_GNUC_INTERNAL void Init_pango_glyph_info(void);
G_GNUC_INTERNAL void Init_pango_glyph_item(void);
G_GNUC_INTERNAL void Init_pango_glyph_string(void);
G_GNUC_INTERNAL void Init_pango_gravity(void);
G_GNUC_INTERNAL void Init_pango_item(void);
G_GNUC_INTERNAL void Init_pango_language(void);
G_GNUC_INTERNAL void Init_pango_layout(void);
G_GNUC_INTERNAL void Init_pango_layout_iter(void);
G_GNUC_INTERNAL void Init_pango_layout_line(void);
G_GNUC_INTERNAL void Init_pango_logattr(void);
G_GNUC_INTERNAL void Init_pango_main(void);
G_GNUC_INTERNAL void Init_pango_matrix(void);
G_GNUC_INTERNAL void Init_pango_rectangle(void);
G_GNUC_INTERNAL void Init_pangorenderer(void);
G_GNUC_INTERNAL void Init_pango_script(void);
G_GNUC_INTERNAL void Init_pango_script_iter(void);
G_GNUC_INTERNAL void Init_pango_array(void);

G_END_DECLS

#endif /* RB_PANGO_PRIVATE_H */
