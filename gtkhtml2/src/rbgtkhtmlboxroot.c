/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxroot.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"
#include <libgtkhtml/layout/htmlboxroot.h>

#define _SELF(s) (HTML_BOX_ROOT(RVAL2GOBJ(s)))

static VALUE
rb_html_box_root_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_root_new());
    return Qnil;
}

static VALUE
rb_html_box_root_get_float_left_list(self)
    VALUE self;
{
    return GSLIST2ARY(html_box_root_get_float_left_list(_SELF(self)));
}

static VALUE
rb_html_box_root_get_float_right_list(self)
    VALUE self;
{
    return GSLIST2ARY(html_box_root_get_float_right_list(_SELF(self)));
}

static VALUE
rb_html_box_root_get_positioned_list(self)
    VALUE self;
{
    return GSLIST2ARY(html_box_root_get_positioned_list(_SELF(self)));
}

static VALUE
rb_html_box_root_clear_float_left_list(self)
    VALUE self;
{
    html_box_root_clear_float_left_list(_SELF(self));
    return self;
}

static VALUE
rb_html_box_root_clear_float_right_list(self)
    VALUE self;
{
    html_box_root_clear_float_right_list(_SELF(self));
    return self;
}

static VALUE
rb_html_box_root_clear_positioned_list(self)
    VALUE self;
{
    html_box_root_clear_positioned_list(_SELF(self));
    return self;
}

static VALUE
rb_html_box_root_add_float(self, box)
    VALUE self, box;
{
    html_box_root_add_float(_SELF(self), HTML_BOX(RVAL2GOBJ(box)));
    return self;
}

static VALUE
rb_html_box_root_add_positioned(self, box)
    VALUE self, box;
{
    html_box_root_add_positioned(_SELF(self), HTML_BOX(RVAL2GOBJ(box)));
    return self;
}

static VALUE
rb_html_box_root_mark_floats_relayouted(self, box)
    VALUE self, box;
{
    html_box_root_mark_floats_relayouted(_SELF(self), HTML_BOX(RVAL2GOBJ(box)));
    return self;
}

static VALUE
rb_html_box_root_mark_floats_unrelayouted(self, box)
    VALUE self, box;
{
    html_box_root_mark_floats_unrelayouted(_SELF(self), HTML_BOX(RVAL2GOBJ(box)));
    return self;
}

/*
void    html_box_root_paint_fixed_list         (HtmlPainter *painter, HtmlBox *root, gint tx, gint ty, GSList *list);
*/

void 
Init_html_box_root(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE boxroot = G_DEF_CLASS(HTML_TYPE_BOX_ROOT, "HtmlBoxRoot", mGtkHtml2);

    rb_define_method(boxroot, "initialize", rb_html_box_root_initialize, 0);

    rb_define_method(boxroot, "float_left_list", rb_html_box_root_get_float_left_list, 0);
    rb_define_method(boxroot, "float_right_list", rb_html_box_root_get_float_right_list, 0);
    rb_define_method(boxroot, "positioned_list", rb_html_box_root_get_positioned_list, 0);

    rb_define_method(boxroot, "clear_float_left_list", rb_html_box_root_clear_float_left_list, 0);
    rb_define_method(boxroot, "clear_float_right_list", rb_html_box_root_clear_float_right_list, 0);
    rb_define_method(boxroot, "clear_positioned_list", rb_html_box_root_clear_positioned_list, 0);
    rb_define_method(boxroot, "add_float", rb_html_box_root_add_float, 1);
    rb_define_method(boxroot, "add_positioned", rb_html_box_root_add_positioned, 1);
    rb_define_method(boxroot, "mark_floats_relayouted", rb_html_box_root_mark_floats_relayouted, 1);
    rb_define_method(boxroot, "mark_floats_unrelayouted", rb_html_box_root_mark_floats_unrelayouted, 1);

}

