#ifndef _RBGTKHTML2_H
#define _RBGTKHTML2_H 1
#include "ruby.h"
#include "rbgtk.h"
#include <libgtkhtml/gtkhtml.h>
#include <libgtkhtml/dom/core/dom-document.h>
#include <libgtkhtml/dom/core/dom-element.h>

extern void Init_html_context(VALUE);

extern void Init_html_document(VALUE);
extern void Init_html_stream(VALUE);

extern void Init_html_view(VALUE);

extern void Init_html_parser(VALUE);

extern void Init_html_box(VALUE);
extern void Init_html_box_block(VALUE);
extern void Init_html_box_root(VALUE);
extern void Init_html_box_table(VALUE);
extern void Init_html_box_table_caption(VALUE);
extern void Init_html_box_table_row(VALUE);
extern void Init_html_box_table_row_group(VALUE);
extern void Init_html_box_text(VALUE);
extern void Init_html_box_embedded(VALUE);
extern void Init_html_box_form(VALUE);
extern void Init_html_embedded(VALUE);

#endif /* _RBGTKHTML2_H */
