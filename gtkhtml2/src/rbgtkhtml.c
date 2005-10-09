/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtml.c

  $Author: mutoh $
  $Date: 2005/10/09 17:43:57 $

  Copyright (C) 2003-2005  Ruby-GNOME2 Project Team
************************************************/

#include "rbgtkhtml.h"

void
Init_gtkhtml2(void)
{ 
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
