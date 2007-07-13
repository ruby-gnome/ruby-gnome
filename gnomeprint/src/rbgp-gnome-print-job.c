/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gnome-print-job.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:30 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"
#include <libgnomeprint/gnome-print-job.h>

#define _SELF(self) (GP_JOB(self))

static VALUE
gp_job_new(int argc, VALUE *argv, VALUE self)
{
    VALUE config;
    GnomePrintJob *job;
  
    rb_scan_args(argc, argv, "01", &config);

    job = gnome_print_job_new(GP_CONFIG(config));
    if (job) {
        G_INITIALIZE(self, job);
    } else {
        rbgp_check_return_code(GNOME_PRINT_ERROR_UNKNOWN);
    }
  
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
    return rbgp_check_return_code(gnome_print_job_close(_SELF(self)));
}

static VALUE
gp_job_print(VALUE self)
{
    return rbgp_check_return_code(gnome_print_job_print(_SELF(self)));
}

static VALUE
gp_job_render(VALUE self, VALUE context)
{
    return rbgp_check_return_code(gnome_print_job_render(_SELF(self),
                                                         GP_CONTEXT(context)));
}

/* pageops: whether you want to send begingpage/showpage to output */
static VALUE
gp_job_render_page(VALUE self, VALUE context, VALUE page, VALUE pageops)
{
    return rbgp_check_return_code(gnome_print_job_render_page(_SELF(self),
                                                              GP_CONTEXT(context),
                                                              NUM2INT(page),
                                                              RVAL2CBOOL(pageops)));
}

static VALUE
gp_job_get_pages(VALUE self)
{
    return INT2NUM(gnome_print_job_get_pages(_SELF(self)));
}

static VALUE
gp_job_print_to_file(VALUE self, VALUE output)
{
    gchar *g_output = NULL;

    if (!NIL_P(output)) {
        g_output = RVAL2CSTR(output);
    }
  
    return rbgp_check_return_code(gnome_print_job_print_to_file(_SELF(self),
                                                                g_output));
}

static VALUE
gp_job_get_page_size(VALUE self)
{
    gdouble width, height;
    gboolean ret;
  
    ret = gnome_print_job_get_page_size(_SELF(self), &width, &height);
  
    if (ret) {
        return rb_ary_new3(2, rb_float_new(width), rb_float_new(height));
    } else {
        return Qnil;
    }
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
