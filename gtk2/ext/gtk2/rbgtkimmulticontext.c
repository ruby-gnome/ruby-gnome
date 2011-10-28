/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cIMMulticontext
#define _SELF(self) (GTK_IM_MULTICONTEXT(RVAL2GOBJ(self)))

static VALUE 
immulti_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_im_multicontext_new());
    return Qnil;
}

static VALUE
immulti_append_menuitems(VALUE self, VALUE menushell)
{
    gtk_im_multicontext_append_menuitems(_SELF(self), RVAL2GOBJ(menushell));
    return self;
}

void 
Init_gtk_im_multicontext(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IM_MULTICONTEXT, "IMMulticontext", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", immulti_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "append_menuitems", immulti_append_menuitems, 1);
}
