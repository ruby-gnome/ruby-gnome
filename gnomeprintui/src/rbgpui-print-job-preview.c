/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
