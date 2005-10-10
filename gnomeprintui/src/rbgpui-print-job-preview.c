/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-job-preview.c -

  $Author: ktou $
  $Date: 2005/10/10 01:42:03 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#include <libgnomeprintui/gnome-print-job-preview.h>

#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
gpui_job_preview_new(VALUE self, VALUE job, VALUE title)
{
  RBGTK_INITIALIZE(self,
                   gnome_print_job_preview_new(RVAL2GOBJ(job),
                                               (const guchar*)RVAL2CSTR(title)));
  return Qnil;
}

void
Init_gnome_print_job_preview(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_JOB_PREVIEW,
                        "PrintJobPreview", mGnome);

  rb_define_method(c, "initialize", gpui_job_preview_new, 2);
}
