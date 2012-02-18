/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifndef __RBGOOCANVASPRIVATE_H__
#define __RBGOOCANVASPRIVATE_H__

#include <rbgdk-pixbuf.h>
#include "rbgoocanvas.h"

/* TODO: move to rbgutil.h */
#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rbg_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rbg_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

void Init_goocanvas(void); /* Goo::Canvas */
G_GNUC_INTERNAL void Init_goocanvasitem(VALUE mGoo); /* Goo::CanvasItem */
G_GNUC_INTERNAL void Init_goocanvastext(VALUE mGoo); /* Goo::CanvasText */
G_GNUC_INTERNAL void Init_goocanvasrect(VALUE mGoo); /* Goo::CanvasRect */
G_GNUC_INTERNAL void Init_goocanvasellipse(VALUE mGoo); /* Goo::CanvasEllipse */
G_GNUC_INTERNAL void Init_goocanvaspolyline(VALUE mGoo); /* Goo::CanvasPolyline */
G_GNUC_INTERNAL void Init_goocanvaspoints(VALUE mGoo); /* Goo::CanvasPoints */
G_GNUC_INTERNAL void Init_goocanvasimage(VALUE mGoo); /* Goo::CanvasImage */
G_GNUC_INTERNAL void Init_goocanvastable(VALUE mGoo); /* Goo::CanvasTable */
G_GNUC_INTERNAL void Init_goocanvaswidget(VALUE mGoo); /* Goo::CanvasWidget */
G_GNUC_INTERNAL void Init_goocanvasstyle(VALUE mGoo); /* Goo::CanvasStyle */
G_GNUC_INTERNAL void Init_goocanvasgroup(VALUE mGoo); /* Goo::CanvasGroup */
G_GNUC_INTERNAL void Init_goo(void);
G_GNUC_INTERNAL void Init_goocairopattern(VALUE mCairo);
G_GNUC_INTERNAL void Init_goocairomatrix(VALUE mCairo);

#endif /* __RBGOOCANVASPRIVATE_H__ */
