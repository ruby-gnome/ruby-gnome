/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkconst.c -

  $Author: mutoh $
  $Date: 2002/12/25 14:33:58 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define RBGTK_MAJOR_VERSION 0
#define RBGTK_MINOR_VERSION 0
#define RBGTK_MICRO_VERSION 2

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
    rb_define_const(mGtk, "ACCEL_VISIBLE", INT2FIX(GTK_ACCEL_VISIBLE));
    rb_define_const(mGtk, "ACCEL_LOCKED", INT2FIX(GTK_ACCEL_LOCKED));
    rb_define_const(mGtk, "ACCEL_MASK", INT2FIX(GTK_ACCEL_MASK));

    /* GtkAnchorType */
    rb_define_const(mGtk, "ANCHOR_CENTER", INT2FIX(GTK_ANCHOR_CENTER));
    rb_define_const(mGtk, "ANCHOR_NORTH", INT2FIX(GTK_ANCHOR_NORTH));
    rb_define_const(mGtk, "ANCHOR_NORTH_WEST", INT2FIX(GTK_ANCHOR_NORTH_WEST));
    rb_define_const(mGtk, "ANCHOR_NORTH_EAST", INT2FIX(GTK_ANCHOR_NORTH_EAST));
    rb_define_const(mGtk, "ANCHOR_SOUTH", INT2FIX(GTK_ANCHOR_SOUTH));
    rb_define_const(mGtk, "ANCHOR_SOUTH_WEST", INT2FIX(GTK_ANCHOR_SOUTH_WEST));
    rb_define_const(mGtk, "ANCHOR_SOUTH_EAST", INT2FIX(GTK_ANCHOR_SOUTH_EAST));
    rb_define_const(mGtk, "ANCHOR_WEST", INT2FIX(GTK_ANCHOR_WEST));
    rb_define_const(mGtk, "ANCHOR_EAST", INT2FIX(GTK_ANCHOR_EAST));
    rb_define_const(mGtk, "ANCHOR_N", INT2FIX(GTK_ANCHOR_N));
    rb_define_const(mGtk, "ANCHOR_NW", INT2FIX(GTK_ANCHOR_NW));
    rb_define_const(mGtk, "ANCHOR_NE", INT2FIX(GTK_ANCHOR_NE));
    rb_define_const(mGtk, "ANCHOR_S", INT2FIX(GTK_ANCHOR_S));
    rb_define_const(mGtk, "ANCHOR_SW", INT2FIX(GTK_ANCHOR_SW));
    rb_define_const(mGtk, "ANCHOR_SE", INT2FIX(GTK_ANCHOR_SE));
    rb_define_const(mGtk, "ANCHOR_W", INT2FIX(GTK_ANCHOR_W));
    rb_define_const(mGtk, "ANCHOR_E", INT2FIX(GTK_ANCHOR_E));

    /* GtkAttachOptions */
    rb_define_const(mGtk, "EXPAND", INT2FIX(GTK_EXPAND));
    rb_define_const(mGtk, "SHRINK", INT2FIX(GTK_SHRINK));
    rb_define_const(mGtk, "FILL", INT2FIX(GTK_FILL));

    /* GtkCornerType */
    rb_define_const(mGtk, "CORNER_TOP_LEFT", INT2FIX(GTK_CORNER_TOP_LEFT));
    rb_define_const(mGtk, "CORNER_BOTTOM_LEFT", INT2FIX(GTK_CORNER_BOTTOM_LEFT));
    rb_define_const(mGtk, "CORNER_TOP_RIGHT", INT2FIX(GTK_CORNER_TOP_RIGHT));
    rb_define_const(mGtk, "CORNER_BOTTOM_RIGHT", INT2FIX(GTK_CORNER_BOTTOM_RIGHT));

    /* GtkDeleteType */
    rb_define_const(mGtk, "DELETE_CHARS", INT2FIX(GTK_DELETE_CHARS));
    rb_define_const(mGtk, "DELETE_WORD_ENDS", INT2FIX(GTK_DELETE_WORD_ENDS));
    rb_define_const(mGtk, "DELETE_WORDS", INT2FIX(GTK_DELETE_WORDS));
    rb_define_const(mGtk, "DELETE_DISPLAY_LINES", INT2FIX(GTK_DELETE_DISPLAY_LINES));
    rb_define_const(mGtk, "DELETE_DISPLAY_LINE_ENDS", INT2FIX(GTK_DELETE_DISPLAY_LINE_ENDS));
    rb_define_const(mGtk, "DELETE_PARAGRAPH_ENDS", INT2FIX(GTK_DELETE_PARAGRAPH_ENDS));
    rb_define_const(mGtk, "DELETE_PARAGRAPHS", INT2FIX(GTK_DELETE_PARAGRAPHS));
    rb_define_const(mGtk, "DELETE_WHITESPACE", INT2FIX(GTK_DELETE_WHITESPACE));

    /* GtkDirectionType */
    rb_define_const(mGtk, "DIR_TAB_FORWARD", INT2FIX(GTK_DIR_TAB_FORWARD));
    rb_define_const(mGtk, "DIR_TAB_BACKWARD", INT2FIX(GTK_DIR_TAB_BACKWARD));
    rb_define_const(mGtk, "DIR_UP", INT2FIX(GTK_DIR_UP));
    rb_define_const(mGtk, "DIR_DOWN", INT2FIX(GTK_DIR_DOWN));
    rb_define_const(mGtk, "DIR_LEFT", INT2FIX(GTK_DIR_LEFT));
    rb_define_const(mGtk, "DIR_RIGHT", INT2FIX(GTK_DIR_RIGHT));

    /* GtkExpanderStyle */
    rb_define_const(mGtk, "EXPANDER_COLLAPSED", INT2FIX(GTK_EXPANDER_COLLAPSED));
    rb_define_const(mGtk, "EXPANDER_SEMI_COLLAPSED", INT2FIX(GTK_EXPANDER_SEMI_COLLAPSED));
    rb_define_const(mGtk, "EXPANDER_SEMI_EXPANDED", INT2FIX(GTK_EXPANDER_SEMI_EXPANDED));
    rb_define_const(mGtk, "EXPANDER_EXPANDED", INT2FIX(GTK_EXPANDER_EXPANDED));

    /* GtkJustification */
    rb_define_const(mGtk, "JUSTIFY_LEFT", INT2FIX(GTK_JUSTIFY_LEFT));
    rb_define_const(mGtk, "JUSTIFY_RIGHT", INT2FIX(GTK_JUSTIFY_RIGHT));
    rb_define_const(mGtk, "JUSTIFY_CENTER", INT2FIX(GTK_JUSTIFY_CENTER));
    rb_define_const(mGtk, "JUSTIFY_FILL", INT2FIX(GTK_JUSTIFY_FILL));

    /* GtkMetricType */
    rb_define_const(mGtk, "PIXELS", INT2FIX(GTK_PIXELS));
    rb_define_const(mGtk, "INCHES", INT2FIX(GTK_INCHES));
    rb_define_const(mGtk, "CENTIMETERS", INT2FIX(GTK_CENTIMETERS));

    /* GtkMovementStep */
    rb_define_const(mGtk, "MOVEMENT_LOGICAL_POSITIONS", INT2FIX(GTK_MOVEMENT_LOGICAL_POSITIONS));
    rb_define_const(mGtk, "MOVEMENT_VISUAL_POSITIONS", INT2FIX(GTK_MOVEMENT_VISUAL_POSITIONS));
    rb_define_const(mGtk, "MOVEMENT_WORDS", INT2FIX(GTK_MOVEMENT_WORDS));
    rb_define_const(mGtk, "MOVEMENT_DISPLAY_LINES", INT2FIX(GTK_MOVEMENT_DISPLAY_LINES));
    rb_define_const(mGtk, "MOVEMENT_DISPLAY_LINE_ENDS", INT2FIX(GTK_MOVEMENT_DISPLAY_LINE_ENDS));
    rb_define_const(mGtk, "MOVEMENT_PARAGRAPHS", INT2FIX(GTK_MOVEMENT_PARAGRAPHS));
    rb_define_const(mGtk, "MOVEMENT_PARAGRAPH_ENDS", INT2FIX(GTK_MOVEMENT_PARAGRAPH_ENDS));
    rb_define_const(mGtk, "MOVEMENT_PAGES", INT2FIX(GTK_MOVEMENT_PAGES));
    rb_define_const(mGtk, "MOVEMENT_BUFFER_ENDS", INT2FIX(GTK_MOVEMENT_BUFFER_ENDS));

    /* GtkOrientation */
    rb_define_const(mGtk, "ORIENTATION_HORIZONTAL", INT2FIX(GTK_ORIENTATION_HORIZONTAL));
    rb_define_const(mGtk, "ORIENTATION_VERTICAL", INT2FIX(GTK_ORIENTATION_VERTICAL));

    /* GtkPackType */
    rb_define_const(mGtk, "PACK_START", INT2FIX(GTK_PACK_START));
    rb_define_const(mGtk, "PACK_END", INT2FIX(GTK_PACK_END));

    /* GtkPathPriorityType */
    rb_define_const(mGtk, "PATH_PRIO_LOWEST", INT2FIX(GTK_PATH_PRIO_LOWEST));
    rb_define_const(mGtk, "PATH_PRIO_GTK", INT2FIX(GTK_PATH_PRIO_GTK));
    rb_define_const(mGtk, "PATH_PRIO_APPLICATION", INT2FIX(GTK_PATH_PRIO_APPLICATION));
    rb_define_const(mGtk, "PATH_PRIO_THEME", INT2FIX(GTK_PATH_PRIO_THEME));
    rb_define_const(mGtk, "PATH_PRIO_RC", INT2FIX(GTK_PATH_PRIO_RC));
    rb_define_const(mGtk, "PATH_PRIO_HIGHEST", INT2FIX(GTK_PATH_PRIO_HIGHEST));

    /* GtkPathType */
    rb_define_const(mGtk, "PATH_WIDGET", INT2FIX(GTK_PATH_WIDGET));
    rb_define_const(mGtk, "PATH_WIDGET_CLASS", INT2FIX(GTK_PATH_WIDGET_CLASS));
    rb_define_const(mGtk, "PATH_CLASS", INT2FIX(GTK_PATH_CLASS));

    /* GtkPolicyType */
    rb_define_const(mGtk, "POLICY_ALWAYS", INT2FIX(GTK_POLICY_ALWAYS));
    rb_define_const(mGtk, "POLICY_AUTOMATIC", INT2FIX(GTK_POLICY_AUTOMATIC));
    rb_define_const(mGtk, "POLICY_NEVER", INT2FIX(GTK_POLICY_NEVER));

    /* GtkPositionType */
    rb_define_const(mGtk, "POS_LEFT", INT2FIX(GTK_POS_LEFT));
    rb_define_const(mGtk, "POS_RIGHT", INT2FIX(GTK_POS_RIGHT));
    rb_define_const(mGtk, "POS_TOP", INT2FIX(GTK_POS_TOP));
    rb_define_const(mGtk, "POS_BOTTOM", INT2FIX(GTK_POS_BOTTOM));

    /* GtkReliefStyle */
    rb_define_const(mGtk, "RELIEF_NORMAL", INT2FIX(GTK_RELIEF_NORMAL));
    rb_define_const(mGtk, "RELIEF_HALF", INT2FIX(GTK_RELIEF_HALF));
    rb_define_const(mGtk, "RELIEF_NONE", INT2FIX(GTK_RELIEF_NONE));

    /* GtkResizeMode */
    rb_define_const(mGtk, "RESIZE_PARENT", INT2FIX(GTK_RESIZE_PARENT));
    rb_define_const(mGtk, "RESIZE_QUEUE", INT2FIX(GTK_RESIZE_QUEUE));
    rb_define_const(mGtk, "RESIZE_IMMEDIATE", INT2FIX(GTK_RESIZE_IMMEDIATE));

    /* GtkScrollType */
    rb_define_const(mGtk, "SCROLL_NONE", INT2FIX(GTK_SCROLL_NONE));
    rb_define_const(mGtk, "SCROLL_STEP_BACKWARD", INT2FIX(GTK_SCROLL_STEP_BACKWARD));
    rb_define_const(mGtk, "SCROLL_STEP_FORWARD", INT2FIX(GTK_SCROLL_STEP_FORWARD));
    rb_define_const(mGtk, "SCROLL_PAGE_BACKWARD", INT2FIX(GTK_SCROLL_PAGE_BACKWARD));
    rb_define_const(mGtk, "SCROLL_PAGE_FORWARD", INT2FIX(GTK_SCROLL_PAGE_FORWARD));
    rb_define_const(mGtk, "SCROLL_STEP_UP", INT2FIX(GTK_SCROLL_STEP_UP));
    rb_define_const(mGtk, "SCROLL_STEP_DOWN", INT2FIX(GTK_SCROLL_STEP_DOWN));
    rb_define_const(mGtk, "SCROLL_PAGE_UP", INT2FIX(GTK_SCROLL_PAGE_UP));
    rb_define_const(mGtk, "SCROLL_PAGE_DOWN", INT2FIX(GTK_SCROLL_PAGE_DOWN));
    rb_define_const(mGtk, "SCROLL_STEP_LEFT", INT2FIX(GTK_SCROLL_STEP_LEFT));
    rb_define_const(mGtk, "SCROLL_STEP_RIGHT", INT2FIX(GTK_SCROLL_STEP_RIGHT));
    rb_define_const(mGtk, "SCROLL_PAGE_LEFT", INT2FIX(GTK_SCROLL_PAGE_LEFT));
    rb_define_const(mGtk, "SCROLL_PAGE_RIGHT", INT2FIX(GTK_SCROLL_PAGE_RIGHT));
    rb_define_const(mGtk, "SCROLL_START", INT2FIX(GTK_SCROLL_START));
    rb_define_const(mGtk, "SCROLL_END", INT2FIX(GTK_SCROLL_END));

    /* GtkSelectionMode */
    rb_define_const(mGtk, "SELECTION_NONE", INT2FIX(GTK_SELECTION_NONE));
    rb_define_const(mGtk, "SELECTION_SINGLE", INT2FIX(GTK_SELECTION_SINGLE));
    rb_define_const(mGtk, "SELECTION_BROWSE", INT2FIX(GTK_SELECTION_BROWSE));
    rb_define_const(mGtk, "SELECTION_MULTIPLE", INT2FIX(GTK_SELECTION_MULTIPLE));

    /* GtkShadowType */
    rb_define_const(mGtk, "SHADOW_NONE", INT2FIX(GTK_SHADOW_NONE));
    rb_define_const(mGtk, "SHADOW_IN", INT2FIX(GTK_SHADOW_IN));
    rb_define_const(mGtk, "SHADOW_OUT", INT2FIX(GTK_SHADOW_OUT));
    rb_define_const(mGtk, "SHADOW_ETCHED_IN", INT2FIX(GTK_SHADOW_ETCHED_IN));
    rb_define_const(mGtk, "SHADOW_ETCHED_OUT", INT2FIX(GTK_SHADOW_ETCHED_OUT));

    /* GtkStateType */
    rb_define_const(mGtk, "STATE_NORMAL", INT2FIX(GTK_STATE_NORMAL));
    rb_define_const(mGtk, "STATE_ACTIVE", INT2FIX(GTK_STATE_ACTIVE));
    rb_define_const(mGtk, "STATE_PRELIGHT", INT2FIX(GTK_STATE_PRELIGHT));
    rb_define_const(mGtk, "STATE_SELECTED", INT2FIX(GTK_STATE_SELECTED));
    rb_define_const(mGtk, "STATE_INSENSITIVE", INT2FIX(GTK_STATE_INSENSITIVE));

    /* GtkSubmenuDirection */
    rb_define_const(mGtk, "DIRECTION_LEFT", INT2FIX(GTK_DIRECTION_LEFT));
    rb_define_const(mGtk, "DIRECTION_RIGHT", INT2FIX(GTK_DIRECTION_RIGHT));

    /* GtkSubmenuPlacement */
    rb_define_const(mGtk, "TOP_BOTTOM", INT2FIX(GTK_TOP_BOTTOM));
    rb_define_const(mGtk, "LEFT_RIGHT", INT2FIX(GTK_LEFT_RIGHT));

    /* GtkUpdateType */
    rb_define_const(mGtk, "UPDATE_CONTINUOUS", INT2FIX(GTK_UPDATE_CONTINUOUS));
    rb_define_const(mGtk, "UPDATE_DISCONTINUOUS", INT2FIX(GTK_UPDATE_DISCONTINUOUS));
    rb_define_const(mGtk, "UPDATE_DELAYED", INT2FIX(GTK_UPDATE_DELAYED));

    /* GtkVisibility */
    rb_define_const(mGtk, "VISIBILITY_NONE", INT2FIX(GTK_VISIBILITY_NONE));
    rb_define_const(mGtk, "VISIBILITY_PARTIAL", INT2FIX(GTK_VISIBILITY_PARTIAL));
    rb_define_const(mGtk, "VISIBILITY_FULL", INT2FIX(GTK_VISIBILITY_FULL));

    /* GtkSortType */
    rb_define_const(mGtk, "SORT_ASCENDING", INT2FIX(GTK_SORT_ASCENDING));
    rb_define_const(mGtk, "SORT_DESCENDING", INT2FIX(GTK_SORT_DESCENDING));

}
