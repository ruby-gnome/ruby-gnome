#include "rbgtkhtml.h"

void
Init_gtkhtml2(void)
{
    //VALUE mGtkHtml2 = rb_define_module("GtkHtml2");

    Init_html_context(mGtk);

    Init_html_document(mGtk);
    Init_html_stream(mGtk);

    Init_html_parser(mGtk);

    Init_html_view(mGtk);

    Init_html_box(mGtk);
    Init_html_box_block(mGtk);
    Init_html_box_root(mGtk);
    Init_html_box_table(mGtk);
    Init_html_box_table_caption(mGtk);
    Init_html_box_table_row(mGtk);
    Init_html_box_table_row_group(mGtk);
    Init_html_box_text(mGtk);
    Init_html_box_embedded(mGtk);
    Init_html_box_form(mGtk);
    Init_html_embedded(mGtk);
}
