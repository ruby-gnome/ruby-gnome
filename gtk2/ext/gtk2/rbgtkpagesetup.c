/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cPageSetup
#define _SELF(s) (GTK_PAGE_SETUP(RVAL2GOBJ(s)))

#define RVAL2SIZE(o) (RVAL2BOXED(o, GTK_TYPE_PAPER_SIZE))
#define RVAL2UNIT(o) (RVAL2GENUM(o, GTK_TYPE_UNIT))

static VALUE
ps_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_page_setup_new());
    return Qnil;
}

static VALUE
ps_copy(VALUE self)
{
    return GOBJ2RVALU(gtk_page_setup_copy(_SELF(self)));
}

static VALUE
ps_get_orientation(VALUE self)
{
    return GENUM2RVAL(gtk_page_setup_get_orientation(_SELF(self)), 
                      GTK_TYPE_PAGE_ORIENTATION);
}

static VALUE
ps_set_orientation(VALUE self, VALUE orientation)
{
    gtk_page_setup_set_orientation(_SELF(self), 
                                   RVAL2GENUM(orientation, GTK_TYPE_PAGE_ORIENTATION));
    return self;
}

static VALUE
ps_get_paper_size(VALUE self)
{
    return BOXED2RVAL(gtk_page_setup_get_paper_size(_SELF(self)), GTK_TYPE_PAPER_SIZE);
}

static VALUE
ps_set_paper_size(VALUE self, VALUE paper_size)
{
    gtk_page_setup_set_paper_size(_SELF(self), RVAL2SIZE(paper_size));
    return self;
}

static VALUE
ps_get_top_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_top_margin(_SELF(self),
                                                      RVAL2UNIT(unit)));
}

static VALUE
ps_set_top_margin(VALUE self, VALUE margin, VALUE unit)
{
    gtk_page_setup_set_top_margin(_SELF(self), NUM2DBL(margin),
                                  RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_bottom_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_bottom_margin(_SELF(self),
                                                         RVAL2UNIT(unit)));
}

static VALUE
ps_set_bottom_margin(VALUE self, VALUE margin, VALUE unit)
{
    gtk_page_setup_set_bottom_margin(_SELF(self), NUM2DBL(margin),
                                     RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_left_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_left_margin(_SELF(self),
                                                       RVAL2UNIT(unit)));
}

static VALUE
ps_set_left_margin(VALUE self, VALUE margin, VALUE unit)
{
    gtk_page_setup_set_left_margin(_SELF(self), NUM2DBL(margin),
                                   RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_get_right_margin(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_right_margin(_SELF(self),
                                                        RVAL2UNIT(unit)));
}

static VALUE
ps_set_right_margin(VALUE self, VALUE margin, VALUE unit)
{
    gtk_page_setup_set_right_margin(_SELF(self), NUM2DBL(margin),
                                    RVAL2UNIT(unit));
    return self;
}

static VALUE
ps_set_paper_size_and_default_margins(VALUE self, VALUE size)
{
    gtk_page_setup_set_paper_size_and_default_margins(_SELF(self),
                                                      RVAL2SIZE(size));
    return self;
}

/* These take orientation, but not margins into consideration */
static VALUE
ps_get_paper_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_paper_width(_SELF(self),
                                                       RVAL2UNIT(unit)));
}

static VALUE
ps_get_paper_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_paper_height(_SELF(self),
                                                        RVAL2UNIT(unit)));
}

/* These take orientation, and margins into consideration */
static VALUE
ps_get_page_width(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_page_width(_SELF(self),
                                                      RVAL2UNIT(unit)));
}

static VALUE
ps_get_page_height(VALUE self, VALUE unit)
{
    return rb_float_new(gtk_page_setup_get_page_height(_SELF(self),
                                                        RVAL2UNIT(unit)));
}
#endif

void
Init_gtk_page_setup(void)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PAGE_SETUP, "PageSetup", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", ps_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "dup", ps_copy, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "orientation", ps_get_orientation, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_orientation", ps_set_orientation, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "paper_size", ps_get_paper_size, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_paper_size", ps_set_paper_size, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_top_margin", ps_get_top_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_top_margin", ps_set_top_margin, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_bottom_margin", ps_get_bottom_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_bottom_margin", ps_set_bottom_margin, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_left_margin", ps_get_left_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_left_margin", ps_set_left_margin, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_right_margin", ps_get_right_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_right_margin", ps_set_right_margin, 2);

    rb_define_method(RG_TARGET_NAMESPACE, "set_paper_size_and_default_margins",
                     ps_set_paper_size_and_default_margins, 1);

    /* These take orientation, but not margins into consideration */
    rb_define_method(RG_TARGET_NAMESPACE, "get_paper_width", ps_get_paper_width, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_paper_height", ps_get_paper_height, 1);

    /* These take orientation, and margins into consideration */
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_width", ps_get_page_width, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_height", ps_get_page_height, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
