/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-settings.c -

  $Author: ktou $
  $Date: 2005/10/10 02:07:41 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-settings.h>

#define _SELF(self) (GP_GPA_SETTINGS(self))

static VALUE
gp_gpa_settings_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE model, name, id;
    rb_scan_args(argc, argv, "21", &model, &name, &id);

    /*
    if (NIL_P(model)) {
        model = name;
        name = id;
        G_INITIALIZE(self,
                     gpa_settings_new_empty(RVAL2CSTR(name),
                     RVAL2CSTR(id)));
    } else */ if (NIL_P(id)) {
        VALUE tree = id;
        G_INITIALIZE(self,
                     gpa_settings_new_from_model_and_tree(RVAL2GOBJ(model),
                                                          RVAL2GOBJ(tree)));
    } else {
        G_INITIALIZE(self, gpa_settings_new(RVAL2GOBJ(model),
                                            (const guchar *)RVAL2CSTR(name),
                                            (const guchar *)RVAL2CSTR(id)));
    }
    return Qnil;
}

void
Init_gnome_print_gpa_settings(VALUE mGnome)
{
    VALUE c = G_DEF_CLASS(GPA_TYPE_SETTINGS, "GPASettings", mGnome);
  
    rb_define_method(c, "initialize", gp_gpa_settings_initialize, -1);
}
