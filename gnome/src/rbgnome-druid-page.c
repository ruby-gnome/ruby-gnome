/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid-page.c,v 1.1 2002/10/20 07:33:51 tkubo Exp $ */

#include "rbgnome.h"

#define _SELF(self) GNOME_DRUID_PAGE(RVAL2GOBJ(self))

static VALUE
dpage_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_druid_page_new());
    return Qnil;
}

static VALUE
dpage_next(self)
    VALUE self;
{
    return gnome_druid_page_next(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
dpage_prepare(self)
    VALUE self;
{
    gnome_druid_page_prepare(_SELF(self));
    return self;
}

static VALUE
dpage_back(self)
    VALUE self;
{
    return gnome_druid_page_back(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
dpage_cancel(self)
    VALUE self;
{
    return gnome_druid_page_cancel(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
dpage_finish(self)
    VALUE self;
{
    gnome_druid_page_finish(_SELF(self));
    return self;
}

void
Init_gnome_druid_page(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruidPage = G_DEF_CLASS(GNOME_TYPE_DRUID_PAGE, "DruidPage", mGnome);

    rb_define_method(gnoDruidPage, "initialize", dpage_initialize, 0);
    rb_define_method(gnoDruidPage, "next", dpage_next, 0);
    rb_define_method(gnoDruidPage, "prepare", dpage_prepare, 0);
    rb_define_method(gnoDruidPage, "back", dpage_back, 0);
    rb_define_method(gnoDruidPage, "cancel", dpage_cancel, 0);
    rb_define_method(gnoDruidPage, "finish", dpage_finish, 0);
}
