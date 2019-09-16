/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "rbgtk3private.h"

void
Init_gtk_const(VALUE mGtk)
{
    rb_define_const(mGtk, "BINDING_VERSION",
		    rb_ary_new3(3, INT2FIX(RBGTK_MAJOR_VERSION),
				INT2FIX(RBGTK_MINOR_VERSION),
				INT2FIX(RBGTK_MICRO_VERSION)));

    rb_define_const(mGtk, "BUILD_VERSION",
		    rb_ary_new3(3, INT2FIX(GTK_MAJOR_VERSION),
				INT2FIX(GTK_MINOR_VERSION),
				INT2FIX(GTK_MICRO_VERSION)));

    rb_define_const(mGtk, "VERSION",
		    rb_ary_new3(3, INT2FIX(gtk_major_version),
				INT2FIX(gtk_minor_version),
				INT2FIX(gtk_micro_version)));
    rb_define_const(mGtk, "MAJOR_VERSION", INT2FIX(gtk_major_version));
    rb_define_const(mGtk, "MINOR_VERSION", INT2FIX(gtk_minor_version));
    rb_define_const(mGtk, "MICRO_VERSION", INT2FIX(gtk_micro_version));
    rb_define_const(mGtk, "BINARY_AGE", INT2FIX(gtk_binary_age));
    rb_define_const(mGtk, "INTERFACE_AGE", INT2FIX(gtk_interface_age));

    G_DEF_CLASS(GTK_TYPE_ACCEL_FLAGS, "AccelFlags", mGtk);
    G_DEF_CLASS(GTK_TYPE_ATTACH_OPTIONS, "AttachOptions", mGtk);
    G_DEF_CLASS(GTK_TYPE_CORNER_TYPE, "CornerType", mGtk);
    G_DEF_CLASS(GTK_TYPE_DELETE_TYPE, "DeleteType", mGtk);
    G_DEF_CLASS(GTK_TYPE_DIRECTION_TYPE, "DirectionType", mGtk);
    G_DEF_CLASS(GTK_TYPE_EXPANDER_STYLE, "ExpanderStyle", mGtk);
    G_DEF_CLASS(GTK_TYPE_JUSTIFICATION, "Justification", mGtk);
    G_DEF_CLASS(GTK_TYPE_MESSAGE_TYPE, "MessageType", mGtk);
    G_DEF_CLASS(GTK_TYPE_MOVEMENT_STEP, "MovementStep", mGtk);
    G_DEF_CLASS(GTK_TYPE_ORIENTATION, "Orientation", mGtk);
    G_DEF_CLASS(GTK_TYPE_PACK_TYPE, "PackType", mGtk);
    G_DEF_CLASS(GTK_TYPE_PATH_PRIORITY_TYPE, "PathPriorityType", mGtk);
    G_DEF_CLASS(GTK_TYPE_PATH_TYPE, "PathType", mGtk);
    G_DEF_CLASS(GTK_TYPE_POLICY_TYPE, "PolicyType", mGtk);
    G_DEF_CLASS(GTK_TYPE_POSITION_TYPE, "PositionType", mGtk);
    G_DEF_CLASS(GTK_TYPE_RELIEF_STYLE, "ReliefStyle", mGtk);
    G_DEF_CLASS(GTK_TYPE_RESIZE_MODE, "ResizeMode", mGtk);
    G_DEF_CLASS(GTK_TYPE_RESPONSE_TYPE, "ResponseType", mGtk);
    G_DEF_CLASS(GTK_TYPE_SCROLL_STEP, "ScrollStep", mGtk);
    G_DEF_CLASS(GTK_TYPE_SCROLL_TYPE, "ScrollType", mGtk);
    G_DEF_CLASS(GTK_TYPE_SELECTION_MODE, "SelectionMode", mGtk);
    G_DEF_CLASS(GTK_TYPE_SHADOW_TYPE, "ShadowType", mGtk);
    G_DEF_CLASS(GTK_TYPE_STATE_FLAGS, "StateFlags", mGtk);
    G_DEF_CLASS(GTK_TYPE_STATE_TYPE, "StateType", mGtk);
/* deprecated
    G_DEF_CLASS(GTK_TYPE_SUBMENU_DIRECTION, "SubmenuDirection", mGtk);
    G_DEF_CLASS(GTK_TYPE_SUBMENU_PLACEMENT, "SubmenuPlacement", mGtk);
*/
    G_DEF_CLASS(GTK_TYPE_SORT_TYPE, "SortType", mGtk);
}
