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

#include "rbgp.h"
#include <libgnomeprint/gnome-print-job.h>

#define _SELF(self) (GP_JOB(self))

static VALUE
gp_job_new(int argc, VALUE *argv, VALUE self)
{
  VALUE config;
  rb_scan_args(argc, argv, "01", &config);
  
  G_INITIALIZE(self, gnome_print_job_new(GP_CONFIG(config)));
  return Qnil;
}

static VALUE
gp_job_get_context(VALUE self)
{
  return GOBJ2RVAL(gnome_print_job_get_context(_SELF(self)));
}

static VALUE
gp_job_get_config(VALUE self)
{
  return GOBJ2RVAL(gnome_print_job_get_config(_SELF(self)));
}

static VALUE
gp_job_close(VALUE self)
{
  return check_return_code(gnome_print_job_close(_SELF(self)));
}

static VALUE
gp_job_print(VALUE self)
{
  return check_return_code(gnome_print_job_print(_SELF(self)));
}

static VALUE
gp_job_render(VALUE self, VALUE context)
{
  return check_return_code(gnome_print_job_render(_SELF(self),
                                                  GP_CONTEXT(context)));
}

/* pageops: whether you want to send begingpage/showpage to output */
static VALUE
gp_job_render_page(VALUE self, VALUE context, VALUE page, VALUE pageops)
{
  return check_return_code(gnome_print_job_render_page(_SELF(self),
                                                       GP_CONTEXT(context),
                                                       NUM2INT(page),
                                                       RTEST(pageops)));
}

static VALUE
gp_job_get_pages(VALUE self)
{
  return INT2NUM(gnome_print_job_get_pages(_SELF(self)));
}

static VALUE
gp_job_print_to_file(VALUE self, VALUE output)
{
  return check_return_code(gnome_print_job_print_to_file(_SELF(self),
                                                         RVAL2CSTR(output)));
}

static VALUE
gp_job_get_page_size(VALUE self)
{
  gdouble width, height;
  gboolean ret;
  
  ret = gnome_print_job_get_page_size(_SELF(self), &width, &height);
  
  return rb_ary_new3(2, rb_float_new(width), rb_float_new(height));
}

void
Init_gnome_print_job(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_JOB, "PrintJob", mGnome);

  rb_define_method(c, "initialize", gp_job_new, -1);
  
  rb_define_method(c, "context", gp_job_get_context, 0);
  rb_define_method(c, "config", gp_job_get_config, 0);
  rb_define_method(c, "close", gp_job_close, 0);
  rb_define_method(c, "print", gp_job_print, 0);
  rb_define_method(c, "render", gp_job_render, 1);
  rb_define_method(c, "render_page", gp_job_render_page, 3);
  rb_define_method(c, "pages", gp_job_get_pages, 0);
  rb_define_method(c, "print_to_file", gp_job_print_to_file, 1);
  rb_define_method(c, "page_size", gp_job_get_page_size, 0);
}
