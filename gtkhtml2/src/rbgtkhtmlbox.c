/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlbox.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"
#include <libgtkhtml/view/htmlevent.h>

#define _SELF(s) (HTML_BOX(RVAL2GOBJ(s)))

static VALUE
rb_html_box_append_child(self, child)
    VALUE self, child;
{
    html_box_append_child(_SELF(self), _SELF(child));
    return self;
}

static VALUE
rb_html_box_remove(self)
    VALUE self;
{
    html_box_remove(_SELF(self));
    return self;
}

static VALUE
rb_html_box_insert_after(self, box)
    VALUE self, box;
{
    html_box_insert_after(_SELF(self), _SELF(box));
    return self;
}

static VALUE
rb_html_box_horizontal_mbp_sum(self)
    VALUE self;
{
    return INT2NUM(html_box_horizontal_mbp_sum(_SELF(self)));
}

static VALUE
rb_html_box_vertical_mbp_sum(self)
    VALUE self;
{
    return INT2NUM(html_box_vertical_mbp_sum(_SELF(self)));
}

static VALUE
rb_html_box_top_mbp_sum(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_top_mbp_sum(_SELF(self), NUM2INT(width)));
}

static VALUE
rb_html_box_bottom_mbp_sum(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_bottom_mbp_sum(_SELF(self), NUM2INT(width)));
}

static VALUE
rb_html_box_left_mbp_sum(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_left_mbp_sum(_SELF(self), NUM2INT(width)));
}

static VALUE
rb_html_box_right_mbp_sum(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_right_mbp_sum(_SELF(self), NUM2INT(width)));
}

static VALUE
rb_html_box_get_absolute_x(self)
    VALUE self;
{
    return INT2NUM(html_box_get_absolute_x(_SELF(self)));
}

static VALUE
rb_html_box_get_absolute_y(self)
    VALUE self;
{
    return INT2NUM(html_box_get_absolute_y(_SELF(self)));
}

static VALUE
rb_html_box_check_min_max_width_height(self)
    VALUE self;
{
    gint boxwidth, boxheight;
    html_box_check_min_max_width_height(_SELF(self), &boxwidth, &boxheight);
    return rb_ary_new3(2, INT2NUM(boxwidth), INT2NUM(boxheight));
}
/*
void html_box_set_style                  (HtmlBox *box, HtmlStyle *style);
void html_box_relayout (HtmlBox *self, HtmlRelayout *relayout);
void html_box_paint    (HtmlBox *self, HtmlPainter *painter, GdkRectangle *area, gint tx, gint ty);
*/
  
static VALUE
rb_html_box_get_ascent(self)
    VALUE self;
{
    return INT2NUM(html_box_get_ascent(_SELF(self)));
}

static VALUE
rb_html_box_get_descent(self)
    VALUE self;
{
    return INT2NUM(html_box_get_descent(_SELF(self)));
}

/*
HtmlStyle *html_box_get_style             (HtmlBox *self);
*/

static VALUE
rb_html_box_get_bidi_level(self)
    VALUE self;
{
    return INT2NUM(html_box_get_bidi_level(_SELF(self)));
}

static VALUE
rb_html_box_set_unrelayouted_up(self)
    VALUE self;
{
    html_box_set_unrelayouted_up(_SELF(self));
    return self;
}

static VALUE
rb_html_box_set_unrelayouted_down(self)
    VALUE self;
{
    html_box_set_unrelayouted_down(_SELF(self));
    return self;
}

static VALUE
rb_html_box_get_containing_block_width(self)
    VALUE self;
{
    return INT2NUM(html_box_get_containing_block_width(_SELF(self)));
}

static VALUE
rb_html_box_get_containing_block_height(self)
    VALUE self;
{
    return INT2NUM(html_box_get_containing_block_height(_SELF(self)));
}

static VALUE
rb_html_box_apply_positioned_offset(self)
    VALUE self;
{
    gint tx, ty;
    html_box_apply_positioned_offset(_SELF(self), &tx, &ty);
    return rb_ary_new3(2, INT2NUM(tx), INT2NUM(ty));
}

static VALUE
rb_html_box_left_margin(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_left_margin(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_left_padding(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_left_padding(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_left_border_width(self)
    VALUE self;
{
    return INT2NUM(html_box_left_border_width(_SELF(self)));
}

static VALUE
rb_html_box_top_margin(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_top_margin(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_top_padding(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_top_padding(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_top_border_width(self)
    VALUE self;
{
    return INT2NUM(html_box_top_border_width(_SELF(self)));
}

static VALUE
rb_html_box_right_margin(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_right_margin(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_right_padding(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_right_padding(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_right_border_width(self)
    VALUE self;
{
    return INT2NUM(html_box_right_border_width(_SELF(self)));
}

static VALUE
rb_html_box_bottom_margin(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_bottom_margin(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_bottom_padding(self, width)
    VALUE self, width;
{
    return INT2NUM(html_box_bottom_padding(_SELF(self), NUM2INT(width)));
}
static VALUE
rb_html_box_bottom_border_width(self)
    VALUE self;
{
    return INT2NUM(html_box_bottom_border_width(_SELF(self)));
}

static VALUE
rb_html_box_is_parent(self, parent)
    VALUE self, parent;
{
    return CBOOL2RVAL(html_box_is_parent(_SELF(self), _SELF(self)));
}

static VALUE
rb_html_box_handles_events(self)
    VALUE self;
{
    return CBOOL2RVAL(html_box_handles_events(_SELF(self)));
}

/*
void     html_box_handle_html_properties (HtmlBox *box, xmlNode *n);
*/

static VALUE
rb_html_box_get_containing_block(self)
    VALUE self;
{
    return GOBJ2RVAL(html_box_get_containing_block(_SELF(self)));
}

static VALUE
rb_html_box_get_after(self)
    VALUE self;
{
    return GOBJ2RVAL(html_box_get_after(_SELF(self)));
}

static VALUE
rb_html_box_get_before(self)
    VALUE self;
{
    return GOBJ2RVAL(html_box_get_before(_SELF(self)));
}

static VALUE
rb_html_box_set_after(self, box)
    VALUE self, box;
{
    html_box_set_after(_SELF(self), _SELF(box));
    return self;
}

static VALUE
rb_html_box_set_before(self, box)
    VALUE self, box;
{
    html_box_set_before(_SELF(self), _SELF(box));
    return self;
}

/*
gboolean html_box_should_paint (HtmlBox *box, GdkRectangle *area, gint tx, gint ty);
*/

/* from htmlevent.c */
static VALUE
rb_html_event_find_root_box(self, x, y)
    VALUE self, x, y;
{
    html_event_find_root_box(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}
  
void 
Init_html_box(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE box = G_DEF_CLASS(HTML_TYPE_BOX, "HtmlBox", mGtkHtml2);

    rb_define_method(box, "append_child", rb_html_box_append_child, 1);
    rb_define_method(box, "box_remove", rb_html_box_remove, 0);
    rb_define_method(box, "insert_after", rb_html_box_insert_after, 1);
    rb_define_method(box, "horizontal_mbp_sum", rb_html_box_horizontal_mbp_sum, 0);
    rb_define_method(box, "vertical_mbp_sum", rb_html_box_vertical_mbp_sum, 0);
    rb_define_method(box, "top_mbp_sum", rb_html_box_top_mbp_sum, 1);
    rb_define_method(box, "bottom_mbp_sum", rb_html_box_bottom_mbp_sum, 1);
    rb_define_method(box, "left_mbp_sum", rb_html_box_left_mbp_sum, 1);
    rb_define_method(box, "right_mbp_sum", rb_html_box_right_mbp_sum, 1);
    rb_define_method(box, "absolute_x", rb_html_box_get_absolute_x, 0);
    rb_define_method(box, "absolute_y", rb_html_box_get_absolute_y, 0);
    rb_define_method(box, "check_min_max_width_height", rb_html_box_check_min_max_width_height, 0);
    rb_define_method(box, "ascent", rb_html_box_get_ascent, 0);
    rb_define_method(box, "descent", rb_html_box_get_descent, 0);
    rb_define_method(box, "bidi_level", rb_html_box_get_bidi_level, 0);
    rb_define_method(box, "unrelayouted_up", rb_html_box_set_unrelayouted_up, 0);
    rb_define_method(box, "unrelayouted_down", rb_html_box_set_unrelayouted_down, 0);
    rb_define_method(box, "containing_block_width", rb_html_box_get_containing_block_width, 0);
    rb_define_method(box, "containing_block_height", rb_html_box_get_containing_block_height, 0);
    rb_define_method(box, "apply_positioned_offset", rb_html_box_apply_positioned_offset, 0);
    rb_define_method(box, "left_margin", rb_html_box_left_margin, 1);
    rb_define_method(box, "left_padding", rb_html_box_left_padding, 1);
    rb_define_method(box, "left_border_width", rb_html_box_left_border_width, 0);
    rb_define_method(box, "top_margin", rb_html_box_top_margin, 1);
    rb_define_method(box, "top_padding", rb_html_box_top_padding, 1);
    rb_define_method(box, "top_border_width", rb_html_box_top_border_width, 0);
    rb_define_method(box, "right_margin", rb_html_box_right_margin, 1);
    rb_define_method(box, "right_padding", rb_html_box_right_padding, 1);
    rb_define_method(box, "right_border_width", rb_html_box_right_border_width, 0);
    rb_define_method(box, "bottom_margin", rb_html_box_bottom_margin, 1);
    rb_define_method(box, "bottom_padding", rb_html_box_bottom_padding, 1);
    rb_define_method(box, "bottom_border_width", rb_html_box_bottom_border_width, 0);
    rb_define_method(box, "parent?", rb_html_box_is_parent, 1);
    rb_define_method(box, "handles_events", rb_html_box_handles_events, 0);
    rb_define_method(box, "containing_block", rb_html_box_get_containing_block, 0);
    rb_define_method(box, "after", rb_html_box_get_after, 0);
    rb_define_method(box, "before", rb_html_box_get_before, 0);
    rb_define_method(box, "set_after", rb_html_box_set_after, 1);
    rb_define_method(box, "set_before", rb_html_box_set_before, 1);

    rb_define_method(box, "find_root_box", rb_html_event_find_root_box, 2);

    G_DEF_SETTERS(box);
}

