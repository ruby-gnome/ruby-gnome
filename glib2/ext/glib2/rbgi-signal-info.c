/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

#ifdef HAVE_GIREPOSITORY
#include "rbgi-private.h"

#define RG_TARGET_NAMESPACE rb_cGISignalInfo
#define SELF(self) (RVAL2GI_SIGNAL_INFO(self))

static VALUE
rg_flags(VALUE self)
{
    GISignalInfo *info = SELF(self);
    return GFLAGS2RVAL(gi_signal_info_get_flags(info), G_TYPE_SIGNAL_FLAGS);
}

static VALUE
rg_class_closure(VALUE self)
{
    GISignalInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_signal_info_get_class_closure(info));
}

static VALUE
rg_true_stops_emit_p(VALUE self)
{
    GISignalInfo *info = SELF(self);
    return CBOOL2RVAL(gi_signal_info_true_stops_emit(info));
}

void
rbgi_signal_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_SIGNAL_INFO, "SignalInfo", rb_mGLib);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(class_closure, 0);
    RG_DEF_METHOD_P(true_stops_emit, 0);
}
#endif
