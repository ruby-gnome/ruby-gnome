/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkconst.c -

  $Author: mutoh $
  $Date: 2003/11/21 16:06:03 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define RBGTK_MAJOR_VERSION 0
#define RBGTK_MINOR_VERSION 8
#define RBGTK_MICRO_VERSION 1

/*
 * constants
 */
void 
Init_gtk_const()
{
    /*
     * version information
     */
    rb_define_const(mGtk, "BINDING_VERSION",
		    rb_ary_new3(3, INT2FIX(RBGTK_MAJOR_VERSION),
				INT2FIX(RBGTK_MINOR_VERSION),
				INT2FIX(RBGTK_MICRO_VERSION)));
    rb_define_const(mGtk, "VERSION",
		    rb_ary_new3(3, INT2FIX(gtk_major_version),
				INT2FIX(gtk_minor_version),
				INT2FIX(gtk_micro_version)));
    rb_define_const(mGtk, "MAJOR_VERSION", INT2FIX(gtk_major_version));
    rb_define_const(mGtk, "MINOR_VERSION", INT2FIX(gtk_minor_version));
    rb_define_const(mGtk, "MICRO_VERSION", INT2FIX(gtk_micro_version));
    rb_define_const(mGtk, "BINARY_AGE", INT2FIX(gtk_binary_age));
    rb_define_const(mGtk, "INTERFACE_AGE", INT2FIX(gtk_interface_age));

    /*
     * standard enumeration
     */

    /* GtkAccelFlags */
    G_DEF_CLASS(GTK_TYPE_ACCEL_FLAGS, "AccelFlags", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_ACCEL_FLAGS, "GTK_");

    /* GtkAnchorType */
    G_DEF_CLASS(GTK_TYPE_ANCHOR_TYPE, "AnchorType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_ANCHOR_TYPE, "GTK_");

    /* GtkAttachOptions */
    G_DEF_CLASS(GTK_TYPE_ATTACH_OPTIONS, "AttachOptions", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_ATTACH_OPTIONS, "GTK_");

    /* GtkCornerType */
    G_DEF_CLASS(GTK_TYPE_CORNER_TYPE, "CornerType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_CORNER_TYPE, "GTK_");

    /* GtkDeleteType */
    G_DEF_CLASS(GTK_TYPE_DELETE_TYPE, "DeleteType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_DELETE_TYPE, "GTK_");

    /* GtkDirectionType */
    G_DEF_CLASS(GTK_TYPE_DIRECTION_TYPE, "DirectionType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_DIRECTION_TYPE, "GTK_");

    /* GtkExpanderStyle */
    G_DEF_CLASS(GTK_TYPE_EXPANDER_STYLE, "ExpanderStyle", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_EXPANDER_STYLE, "GTK_");

    /* GtkJustification */
    G_DEF_CLASS(GTK_TYPE_JUSTIFICATION, "Justification", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_JUSTIFICATION, "GTK_");

    /* GtkMetricType */
    G_DEF_CLASS(GTK_TYPE_METRIC_TYPE, "MetricType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_METRIC_TYPE, "GTK_");

    /* GtkMovementStep */
    G_DEF_CLASS(GTK_TYPE_MOVEMENT_STEP, "MovementStep", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_MOVEMENT_STEP, "GTK_");

    /* GtkOrientation */
    G_DEF_CLASS(GTK_TYPE_ORIENTATION, "Orientation", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_ORIENTATION, "GTK_");

    /* GtkPackType */
    G_DEF_CLASS(GTK_TYPE_PACK_TYPE, "PackType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_PACK_TYPE, "GTK_");

    /* GtkPathPriorityType */
    G_DEF_CLASS(GTK_TYPE_PATH_PRIORITY_TYPE, "PathPriorityType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_PATH_PRIORITY_TYPE, "GTK_");

    /* GtkPathType */
    G_DEF_CLASS(GTK_TYPE_PATH_TYPE, "PathType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_PATH_TYPE, "GTK_");

    /* GtkPolicyType */
    G_DEF_CLASS(GTK_TYPE_POLICY_TYPE, "PolicyType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_POLICY_TYPE, "GTK_");

    /* GtkPositionType */
    G_DEF_CLASS(GTK_TYPE_POSITION_TYPE, "PositionType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_POSITION_TYPE, "GTK_");

    /* GtkReliefStyle */
    G_DEF_CLASS(GTK_TYPE_RELIEF_STYLE, "ReliefStyle", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_RELIEF_STYLE, "GTK_");

    /* GtkResizeMode */
    G_DEF_CLASS(GTK_TYPE_RESIZE_MODE, "ResizeMode", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_RESIZE_MODE, "GTK_");

    /* GtkScrollType */
    G_DEF_CLASS(GTK_TYPE_SCROLL_TYPE, "ScrollType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SCROLL_TYPE, "GTK_");

    /* GtkSelectionMode */
    G_DEF_CLASS(GTK_TYPE_SELECTION_MODE, "SelectionMode", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SELECTION_MODE, "GTK_");

    /* GtkShadowType */
    G_DEF_CLASS(GTK_TYPE_SHADOW_TYPE, "ShadowType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SHADOW_TYPE, "GTK_");

    /* GtkStateType */
    G_DEF_CLASS(GTK_TYPE_STATE_TYPE, "StateType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_STATE_TYPE, "GTK_");

    /* GtkSubmenuDirection */
    G_DEF_CLASS(GTK_TYPE_SUBMENU_DIRECTION, "SubmenuDirection", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SUBMENU_DIRECTION, "GTK_");

    /* GtkSubmenuPlacement */
    G_DEF_CLASS(GTK_TYPE_SUBMENU_PLACEMENT, "SubmenuPlacement", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SUBMENU_PLACEMENT, "GTK_");

    /* GtkUpdateType */
    G_DEF_CLASS(GTK_TYPE_UPDATE_TYPE, "UpdateType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_UPDATE_TYPE, "GTK_");

    /* GtkVisibility */
    G_DEF_CLASS(GTK_TYPE_VISIBILITY, "Visibility", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_VISIBILITY, "GTK_");

    /* GtkSortType */
    G_DEF_CLASS(GTK_TYPE_SORT_TYPE, "SortType", mGtk);
    G_DEF_CONSTANTS(mGtk, GTK_TYPE_SORT_TYPE, "GTK_");
}
