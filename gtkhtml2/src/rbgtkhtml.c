#include "rbgtkhtml.h"

void
Init_gtkhtml2(void)
{
    //VALUE mGtkHtml2 = rb_define_module("GtkHtml2");

    Init_html_document(mGtk);
    Init_html_view(mGtk);
}
