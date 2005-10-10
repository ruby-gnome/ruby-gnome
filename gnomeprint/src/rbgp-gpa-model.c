/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-model.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-model.h>

void
Init_gnome_print_gpa_model(VALUE mGnome)
{
  G_DEF_CLASS(GPA_TYPE_MODEL, "GPAModel", mGnome);
}
