/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkconst.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk module constants
 */
void
Init_gtk_gdk_const()
{
    rb_define_const(mGdk, "CURRENT_TIME", INT2FIX(GDK_CURRENT_TIME));
    rb_define_const(mGdk, "PRIORITY_EVENTS", INT2FIX(GDK_PRIORITY_EVENTS));
    rb_define_const(mGdk, "NONE", INT2FIX(GDK_NONE));
    rb_define_const(mGdk, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));

    /* GdkWindowType */
    rb_define_const(mGdk, "WINDOW_ROOT", INT2FIX(GDK_WINDOW_ROOT));
    rb_define_const(mGdk, "WINDOW_TOPLEVEL", INT2FIX(GDK_WINDOW_TOPLEVEL));
    rb_define_const(mGdk, "WINDOW_CHILD", INT2FIX(GDK_WINDOW_CHILD));
    rb_define_const(mGdk, "WINDOW_DIALOG", INT2FIX(GDK_WINDOW_DIALOG));
    rb_define_const(mGdk, "WINDOW_TEMP", INT2FIX(GDK_WINDOW_TEMP));
    rb_define_const(mGdk, "WINDOW_FOREIGN", INT2FIX(GDK_WINDOW_FOREIGN));

    /* GdkWindowClass */
    rb_define_const(mGdk, "INPUT_OUTPUT", INT2FIX(GDK_INPUT_OUTPUT));
    rb_define_const(mGdk, "INPUT_ONLY", INT2FIX(GDK_INPUT_ONLY));

    /* GdkImageType */
    rb_define_const(mGdk, "IMAGE_NORMAL", INT2FIX(GDK_IMAGE_NORMAL));
    rb_define_const(mGdk, "IMAGE_SHARED", INT2FIX(GDK_IMAGE_SHARED));
    rb_define_const(mGdk, "IMAGE_FASTEST", INT2FIX(GDK_IMAGE_FASTEST));

    /* GdkVisualType */
    rb_define_const(mGdk, "VISUAL_STATIC_GRAY", INT2FIX(GDK_VISUAL_STATIC_GRAY));
    rb_define_const(mGdk, "VISUAL_GRAYSCALE", INT2FIX(GDK_VISUAL_GRAYSCALE));
    rb_define_const(mGdk, "VISUAL_STATIC_COLOR", INT2FIX(GDK_VISUAL_STATIC_COLOR));
    rb_define_const(mGdk, "VISUAL_PSEUDO_COLOR", INT2FIX(GDK_VISUAL_PSEUDO_COLOR));
    rb_define_const(mGdk, "VISUAL_TRUE_COLOR", INT2FIX(GDK_VISUAL_TRUE_COLOR));
    rb_define_const(mGdk, "VISUAL_DIRECT_COLOR", INT2FIX(GDK_VISUAL_DIRECT_COLOR));

    /* GdkFontType */
    rb_define_const(mGdk, "FONT_FONT", INT2FIX(GDK_FONT_FONT));
    rb_define_const(mGdk, "FONT_FONTSET", INT2FIX(GDK_FONT_FONTSET));

    /* GdkWindowAttibutesType */
    rb_define_const(mGdk, "WA_TITLE", INT2FIX(GDK_WA_TITLE));
    rb_define_const(mGdk, "WA_X", INT2FIX(GDK_WA_X));
    rb_define_const(mGdk, "WA_Y", INT2FIX(GDK_WA_Y));
    rb_define_const(mGdk, "WA_CURSOR", INT2FIX(GDK_WA_CURSOR));
    rb_define_const(mGdk, "WA_COLORMAP", INT2FIX(GDK_WA_COLORMAP));
    rb_define_const(mGdk, "WA_VISUAL", INT2FIX(GDK_WA_VISUAL));
    rb_define_const(mGdk, "WA_WMCLASS", INT2FIX(GDK_WA_WMCLASS));
    rb_define_const(mGdk, "WA_NOREDIR", INT2FIX(GDK_WA_NOREDIR));

    /* GdkFunction */
    rb_define_const(mGdk, "COPY", INT2FIX(GDK_COPY));
    rb_define_const(mGdk, "INVERT", INT2FIX(GDK_INVERT));
    rb_define_const(mGdk, "XOR", INT2FIX(GDK_XOR));
    rb_define_const(mGdk, "CLEAR", INT2FIX(GDK_CLEAR));
    rb_define_const(mGdk, "AND", INT2FIX(GDK_AND));
    rb_define_const(mGdk, "AND_REVERSE", INT2FIX(GDK_AND_REVERSE));
    rb_define_const(mGdk, "AND_INVERT", INT2FIX(GDK_AND_INVERT));
    rb_define_const(mGdk, "NOOP", INT2FIX(GDK_NOOP));
    rb_define_const(mGdk, "OR", INT2FIX(GDK_OR));
    rb_define_const(mGdk, "EQUIV", INT2FIX(GDK_EQUIV));
    rb_define_const(mGdk, "OR_REVERSE", INT2FIX(GDK_OR_REVERSE));
    rb_define_const(mGdk, "COPY_INVERT", INT2FIX(GDK_COPY_INVERT));
    rb_define_const(mGdk, "OR_INVERT", INT2FIX(GDK_OR_INVERT));
    rb_define_const(mGdk, "NAND", INT2FIX(GDK_NAND));
    rb_define_const(mGdk, "SET", INT2FIX(GDK_SET));

    /* GdkFill */
    rb_define_const(mGdk, "SOLID", INT2FIX(GDK_SOLID));
    rb_define_const(mGdk, "TILED", INT2FIX(GDK_TILED));
    rb_define_const(mGdk, "STIPPLED", INT2FIX(GDK_STIPPLED));
    rb_define_const(mGdk, "OPAQUE_STIPPLED", INT2FIX(GDK_OPAQUE_STIPPLED));

    /* GdkFillRule */
    rb_define_const(mGdk, "EVEN_ODD_RULE", INT2FIX(GDK_EVEN_ODD_RULE));
    rb_define_const(mGdk, "WINDING_RULE", INT2FIX(GDK_WINDING_RULE));

    /* GdkLineStyle */
    rb_define_const(mGdk, "LINE_SOLID", INT2FIX(GDK_LINE_SOLID));
    rb_define_const(mGdk, "LINE_ON_OFF_DASH", INT2FIX(GDK_LINE_ON_OFF_DASH));
    rb_define_const(mGdk, "LINE_DOUBLE_DASH", INT2FIX(GDK_LINE_DOUBLE_DASH));

    /* GdkCapStyle */
    rb_define_const(mGdk, "CAP_NOT_LAST", INT2FIX(GDK_CAP_NOT_LAST));
    rb_define_const(mGdk, "CAP_BUTT", INT2FIX(GDK_CAP_BUTT));
    rb_define_const(mGdk, "CAP_ROUND", INT2FIX(GDK_CAP_ROUND));
    rb_define_const(mGdk, "CAP_PROJECTING", INT2FIX(GDK_CAP_PROJECTING));

    /* GdkJoinStyle */
    rb_define_const(mGdk, "JOIN_MITER", INT2FIX(GDK_JOIN_MITER));
    rb_define_const(mGdk, "JOIN_ROUND", INT2FIX(GDK_JOIN_ROUND));
    rb_define_const(mGdk, "JOIN_BEVEL", INT2FIX(GDK_JOIN_BEVEL));

    /* GdkFilterReturn */
    rb_define_const(mGdk, "FILTER_CONTINUE", INT2FIX(GDK_FILTER_CONTINUE));
    rb_define_const(mGdk, "FILTER_TRANSLATE", INT2FIX(GDK_FILTER_TRANSLATE));
    rb_define_const(mGdk, "FILTER_REMOVE", INT2FIX(GDK_FILTER_REMOVE));

    /* GdkVisibilityState */
    rb_define_const(mGdk, "VISIBILITY_UNOBSCURED", INT2FIX(GDK_VISIBILITY_UNOBSCURED));
    rb_define_const(mGdk, "VISIBILITY_PARTIAL", INT2FIX(GDK_VISIBILITY_PARTIAL));
    rb_define_const(mGdk, "VISIBILITY_FULLY_OBSCURED", INT2FIX(GDK_VISIBILITY_FULLY_OBSCURED));

    /* GdkCrossingMode */
    rb_define_const(mGdk, "CROSSING_NORMAL", INT2FIX(GDK_CROSSING_NORMAL));
    rb_define_const(mGdk, "CROSSING_GRAB", INT2FIX(GDK_CROSSING_GRAB));
    rb_define_const(mGdk, "CROSSING_UNGRAB", INT2FIX(GDK_CROSSING_UNGRAB));

    /* GdkEvent */
    rb_define_const(mGdk, "NOTHING", INT2FIX(GDK_NOTHING));
    rb_define_const(mGdk, "EXPOSE", INT2FIX(GDK_EXPOSE));
    rb_define_const(mGdk, "NO_EXPOSE", INT2FIX(GDK_NO_EXPOSE));
    rb_define_const(mGdk, "VISIBILITY_NOTIFY", INT2FIX(GDK_VISIBILITY_NOTIFY));
    rb_define_const(mGdk, "MOTION_NOTIFY", INT2FIX(GDK_MOTION_NOTIFY));
    rb_define_const(mGdk, "BUTTON_PRESS", INT2FIX(GDK_BUTTON_PRESS));
    rb_define_const(mGdk, "BUTTON2_PRESS", INT2FIX(GDK_2BUTTON_PRESS));
    rb_define_const(mGdk, "BUTTON3_PRESS", INT2FIX(GDK_3BUTTON_PRESS));
    rb_define_const(mGdk, "BUTTON_RELEASE", INT2FIX(GDK_BUTTON_RELEASE));
    rb_define_const(mGdk, "KEY_PRESS", INT2FIX(GDK_KEY_PRESS));
    rb_define_const(mGdk, "KEY_RELEASE", INT2FIX(GDK_KEY_RELEASE));
    rb_define_const(mGdk, "FOCUS_CHANGE", INT2FIX(GDK_FOCUS_CHANGE));
    rb_define_const(mGdk, "CONFIGURE", INT2FIX(GDK_CONFIGURE));
    rb_define_const(mGdk, "PROPERTY_NOTIFY", INT2FIX(GDK_PROPERTY_NOTIFY));
    rb_define_const(mGdk, "SELECTION_CLEAR", INT2FIX(GDK_SELECTION_CLEAR));
    rb_define_const(mGdk, "SELECTION_REQUEST", INT2FIX(GDK_SELECTION_REQUEST));
    rb_define_const(mGdk, "SELECTION_NOTIFY", INT2FIX(GDK_SELECTION_NOTIFY));
    rb_define_const(mGdk, "PROXIMITY_IN", INT2FIX(GDK_PROXIMITY_IN));
    rb_define_const(mGdk, "PROXIMITY_OUT", INT2FIX(GDK_PROXIMITY_OUT));
    rb_define_const(mGdk, "DRAG_ENTER", INT2FIX(GDK_DRAG_ENTER));
    rb_define_const(mGdk, "DRAG_LEAVE", INT2FIX(GDK_DRAG_LEAVE));
    rb_define_const(mGdk, "DRAG_MOTION", INT2FIX(GDK_DRAG_MOTION));
    rb_define_const(mGdk, "DRAG_STATUS", INT2FIX(GDK_DRAG_STATUS));
    rb_define_const(mGdk, "DROP_START", INT2FIX(GDK_DROP_START));
    rb_define_const(mGdk, "DROP_FINISHED", INT2FIX(GDK_DROP_FINISHED));
    /* remove 1.2.x
       rb_define_const(mGdk, "DRAG_BEGIN", INT2FIX(GDK_DRAG_BEGIN));
       rb_define_const(mGdk, "DRAG_REQUEST", INT2FIX(GDK_DRAG_REQUEST));
       rb_define_const(mGdk, "DROP_ENTER", INT2FIX(GDK_DROP_ENTER));
       rb_define_const(mGdk, "DROP_LEAVE", INT2FIX(GDK_DROP_LEAVE));
       rb_define_const(mGdk, "DROP_DATA_AVAIL", INT2FIX(GDK_DROP_DATA_AVAIL));
    */
    rb_define_const(mGdk, "CLIENT_EVENT", INT2FIX(GDK_CLIENT_EVENT));
    rb_define_const(mGdk, "DELETE", INT2FIX(GDK_DELETE));
    rb_define_const(mGdk, "DESTROY", INT2FIX(GDK_DESTROY));
    rb_define_const(mGdk, "ENTER_NOTIFY", INT2FIX(GDK_ENTER_NOTIFY));
    rb_define_const(mGdk, "LEAVE_NOTIFY", INT2FIX(GDK_LEAVE_NOTIFY));
    rb_define_const(mGdk, "MAP", INT2FIX(GDK_MAP));
    rb_define_const(mGdk, "UNMAP", INT2FIX(GDK_UNMAP));

    /* GdkEventMask */
    rb_define_const(mGdk, "EXPOSURE_MASK", INT2FIX(GDK_EXPOSURE_MASK));
    rb_define_const(mGdk, "POINTER_MOTION_MASK", INT2FIX(GDK_POINTER_MOTION_MASK));
    rb_define_const(mGdk, "POINTER_MOTION_HINT_MASK", INT2FIX(GDK_POINTER_MOTION_HINT_MASK));
    rb_define_const(mGdk, "BUTTON_MOTION_MASK", INT2FIX(GDK_BUTTON_MOTION_MASK));
    rb_define_const(mGdk, "BUTTON1_MOTION_MASK", INT2FIX(GDK_BUTTON1_MOTION_MASK));
    rb_define_const(mGdk, "BUTTON2_MOTION_MASK", INT2FIX(GDK_BUTTON2_MOTION_MASK));
    rb_define_const(mGdk, "BUTTON3_MOTION_MASK", INT2FIX(GDK_BUTTON3_MOTION_MASK));
    rb_define_const(mGdk, "BUTTON_PRESS_MASK", INT2FIX(GDK_BUTTON_PRESS_MASK));
    rb_define_const(mGdk, "BUTTON_RELEASE_MASK", INT2FIX(GDK_BUTTON_RELEASE_MASK));
    rb_define_const(mGdk, "KEY_PRESS_MASK", INT2FIX(GDK_KEY_PRESS_MASK));
    rb_define_const(mGdk, "KEY_RELEASE_MASK", INT2FIX(GDK_KEY_RELEASE_MASK));
    rb_define_const(mGdk, "ENTER_NOTIFY_MASK", INT2FIX(GDK_ENTER_NOTIFY_MASK));
    rb_define_const(mGdk, "LEAVE_NOTIFY_MASK", INT2FIX(GDK_LEAVE_NOTIFY_MASK));
    rb_define_const(mGdk, "FOCUS_CHANGE_MASK", INT2FIX(GDK_FOCUS_CHANGE_MASK));
    rb_define_const(mGdk, "STRUCTURE_MASK", INT2FIX(GDK_STRUCTURE_MASK));
    rb_define_const(mGdk, "PROPERTY_CHANGE_MASK", INT2FIX(GDK_PROPERTY_CHANGE_MASK));
    rb_define_const(mGdk, "VISIBILITY_NOTIFY_MASK", INT2FIX(GDK_VISIBILITY_NOTIFY_MASK));
    rb_define_const(mGdk, "PROXIMITY_IN_MASK", INT2FIX(GDK_PROXIMITY_IN_MASK));
    rb_define_const(mGdk, "PROXIMITY_OUT_MASK", INT2FIX(GDK_PROXIMITY_OUT_MASK));
    rb_define_const(mGdk, "SUBSTRUCTURE_MASK", INT2FIX(GDK_SUBSTRUCTURE_MASK));
    rb_define_const(mGdk, "ALL_EVENTS_MASK", INT2FIX(GDK_ALL_EVENTS_MASK));

    /* GdkNotifyType */
    rb_define_const(mGdk, "NOTIFY_ANCESTOR", INT2FIX(GDK_NOTIFY_ANCESTOR));
    rb_define_const(mGdk, "NOTIFY_VIRTUAL", INT2FIX(GDK_NOTIFY_VIRTUAL));
    rb_define_const(mGdk, "NOTIFY_INFERIOR", INT2FIX(GDK_NOTIFY_INFERIOR));
    rb_define_const(mGdk, "NOTIFY_NONLINEAR", INT2FIX(GDK_NOTIFY_NONLINEAR));
    rb_define_const(mGdk, "NOTIFY_NONLINEAR_VIRTUAL", INT2FIX(GDK_NOTIFY_NONLINEAR_VIRTUAL));
    rb_define_const(mGdk, "NOTIFY_UNKNOWN", INT2FIX(GDK_NOTIFY_UNKNOWN));

    /* GdkModifierType */
    rb_define_const(mGdk, "SHIFT_MASK", INT2FIX(GDK_SHIFT_MASK));
    rb_define_const(mGdk, "LOCK_MASK", INT2FIX(GDK_LOCK_MASK));
    rb_define_const(mGdk, "CONTROL_MASK", INT2FIX(GDK_CONTROL_MASK));
    rb_define_const(mGdk, "MOD1_MASK", INT2FIX(GDK_MOD1_MASK));
    rb_define_const(mGdk, "MOD2_MASK", INT2FIX(GDK_MOD2_MASK));
    rb_define_const(mGdk, "MOD3_MASK", INT2FIX(GDK_MOD3_MASK));
    rb_define_const(mGdk, "MOD4_MASK", INT2FIX(GDK_MOD4_MASK));
    rb_define_const(mGdk, "MOD5_MASK", INT2FIX(GDK_MOD5_MASK));
    rb_define_const(mGdk, "BUTTON1_MASK", INT2FIX(GDK_BUTTON1_MASK));
    rb_define_const(mGdk, "BUTTON2_MASK", INT2FIX(GDK_BUTTON2_MASK));
    rb_define_const(mGdk, "BUTTON3_MASK", INT2FIX(GDK_BUTTON3_MASK));
    rb_define_const(mGdk, "BUTTON4_MASK", INT2FIX(GDK_BUTTON4_MASK));
    rb_define_const(mGdk, "BUTTON5_MASK", INT2FIX(GDK_BUTTON5_MASK));

    /* GdkSubwindowMode */
    rb_define_const(mGdk, "CLIP_BY_CHILDREN", INT2FIX(GDK_CLIP_BY_CHILDREN));
    rb_define_const(mGdk, "INCLUDE_INFERIORS", INT2FIX(GDK_INCLUDE_INFERIORS));

    /* GdkInputCondition */
    rb_define_const(mGdk, "INPUT_READ", INT2FIX(GDK_INPUT_READ));
    rb_define_const(mGdk, "INPUT_WRITE", INT2FIX(GDK_INPUT_WRITE));
    rb_define_const(mGdk, "INPUT_EXCEPTION", INT2FIX(GDK_INPUT_EXCEPTION));

    /* GdkStatus */
    rb_define_const(mGdk, "OK", INT2FIX(GDK_OK));
    rb_define_const(mGdk, "ERROR", INT2FIX(GDK_ERROR));
    rb_define_const(mGdk, "ERROR_PARAM", INT2FIX(GDK_ERROR_PARAM));
    rb_define_const(mGdk, "ERROR_FILE", INT2FIX(GDK_ERROR_FILE));
    rb_define_const(mGdk, "ERROR_MEM", INT2FIX(GDK_ERROR_MEM));

    /* GdkByteOrder */
    rb_define_const(mGdk, "LSB_FIRST", INT2FIX(GDK_LSB_FIRST));
    rb_define_const(mGdk, "MSB_FIRST", INT2FIX(GDK_MSB_FIRST));

    /* GdkGCValuesMask */
    rb_define_const(mGdk, "GC_FOREGROUND", INT2FIX(GDK_GC_FOREGROUND));
    rb_define_const(mGdk, "GC_BACKGROUND", INT2FIX(GDK_GC_BACKGROUND));
    rb_define_const(mGdk, "GC_FONT", INT2FIX(GDK_GC_FONT));
    rb_define_const(mGdk, "GC_FUNCTION", INT2FIX(GDK_GC_FUNCTION));
    rb_define_const(mGdk, "GC_FILL", INT2FIX(GDK_GC_FILL));
    rb_define_const(mGdk, "GC_TILE", INT2FIX(GDK_GC_TILE));
    rb_define_const(mGdk, "GC_STIPPLE", INT2FIX(GDK_GC_STIPPLE));
    rb_define_const(mGdk, "GC_CLIP_MASK", INT2FIX(GDK_GC_CLIP_MASK));
    rb_define_const(mGdk, "GC_SUBWINDOW", INT2FIX(GDK_GC_SUBWINDOW));
    rb_define_const(mGdk, "GC_TS_X_ORIGIN", INT2FIX(GDK_GC_TS_X_ORIGIN));
    rb_define_const(mGdk, "GC_TS_Y_ORIGIN", INT2FIX(GDK_GC_TS_Y_ORIGIN));
    rb_define_const(mGdk, "GC_CLIP_X_ORIGIN", INT2FIX(GDK_GC_CLIP_X_ORIGIN));
    rb_define_const(mGdk, "GC_CLIP_Y_ORIGIN", INT2FIX(GDK_GC_CLIP_Y_ORIGIN));
    rb_define_const(mGdk, "GC_EXPOSURES", INT2FIX(GDK_GC_EXPOSURES));
    rb_define_const(mGdk, "GC_LINE_WIDTH", INT2FIX(GDK_GC_LINE_WIDTH));
    rb_define_const(mGdk, "GC_LINE_STYLE", INT2FIX(GDK_GC_LINE_STYLE));
    rb_define_const(mGdk, "GC_CAP_STYLE", INT2FIX(GDK_GC_CAP_STYLE));
    rb_define_const(mGdk, "GC_JOIN_STYLE", INT2FIX(GDK_GC_JOIN_STYLE));

    /* GdkPropertyState */
    rb_define_const(mGdk, "PROPERTY_NEW_VALUE", INT2FIX(GDK_PROPERTY_NEW_VALUE));
    rb_define_const(mGdk, "PROPERTY_DELETE", INT2FIX(GDK_PROPERTY_DELETE));

    /* GdkPropMode */
    rb_define_const(mGdk, "PROP_MODE_REPLACE", INT2FIX(GDK_PROP_MODE_REPLACE));
    rb_define_const(mGdk, "PROP_MODE_PREPEND", INT2FIX(GDK_PROP_MODE_PREPEND));
    rb_define_const(mGdk, "PROP_MODE_APPEND", INT2FIX(GDK_PROP_MODE_APPEND));

    /* GdkDndType *
       rb_define_const(mGdk, "DNDTYPE_NOTDND", INT2FIX(GDK_DNDTYPE_NOTDND));
       rb_define_const(mGdk, "DNDTYPE_UNKNOWN", INT2FIX(GDK_DNDTYPE_UNKNOWN));
       rb_define_const(mGdk, "DNDTYPE_RAWDATA", INT2FIX(GDK_DNDTYPE_RAWDATA));
       rb_define_const(mGdk, "DNDTYPE_FILE", INT2FIX(GDK_DNDTYPE_FILE));
       rb_define_const(mGdk, "DNDTYPE_FILES", INT2FIX(GDK_DNDTYPE_FILES));
       rb_define_const(mGdk, "DNDTYPE_TEXT", INT2FIX(GDK_DNDTYPE_TEXT));
       rb_define_const(mGdk, "DNDTYPE_DIR", INT2FIX(GDK_DNDTYPE_DIR));
       rb_define_const(mGdk, "DNDTYPE_LINK", INT2FIX(GDK_DNDTYPE_LINK));
       rb_define_const(mGdk, "DNDTYPE_EXE", INT2FIX(GDK_DNDTYPE_EXE));
       rb_define_const(mGdk, "DNDTYPE_URL", INT2FIX(GDK_DNDTYPE_URL));
       rb_define_const(mGdk, "DNDTYPE_MIME", INT2FIX(GDK_DNDTYPE_MIME));
       rb_define_const(mGdk, "DNDTYPE_END", INT2FIX(GDK_DNDTYPE_END));
    */

    /* GdkInputSource */
    rb_define_const(mGdk, "SOURCE_MOUSE", INT2FIX(GDK_SOURCE_MOUSE));
    rb_define_const(mGdk, "SOURCE_PEN", INT2FIX(GDK_SOURCE_PEN));
    rb_define_const(mGdk, "SOURCE_ERASER", INT2FIX(GDK_SOURCE_ERASER));
    rb_define_const(mGdk, "SOURCE_CURSOR", INT2FIX(GDK_SOURCE_CURSOR));

    /* GdkInputMode */
    rb_define_const(mGdk, "MODE_DISABLED", INT2FIX(GDK_MODE_DISABLED));
    rb_define_const(mGdk, "MODE_SCREEN", INT2FIX(GDK_MODE_SCREEN));
    rb_define_const(mGdk, "MODE_WINDOW", INT2FIX(GDK_MODE_WINDOW));

    /* GdkAxisUse */
    rb_define_const(mGdk, "AXIS_IGNORE", INT2FIX(GDK_AXIS_IGNORE));
    rb_define_const(mGdk, "AXIS_X", INT2FIX(GDK_AXIS_X));
    rb_define_const(mGdk, "AXIS_Y", INT2FIX(GDK_AXIS_Y));
    rb_define_const(mGdk, "AXIS_PRESSURE", INT2FIX(GDK_AXIS_PRESSURE));
    rb_define_const(mGdk, "AXIS_XTILT", INT2FIX(GDK_AXIS_XTILT));
    rb_define_const(mGdk, "AXIS_YTILT", INT2FIX(GDK_AXIS_YTILT));
    rb_define_const(mGdk, "AXIS_LAST", INT2FIX(GDK_AXIS_LAST));

    /* GdkExtensionMode */
    rb_define_const(mGdk, "EXTENSION_EVENTS_NONE", INT2FIX(GDK_EXTENSION_EVENTS_NONE));
    rb_define_const(mGdk, "EXTENSION_EVENTS_ALL", INT2FIX(GDK_EXTENSION_EVENTS_ALL));
    rb_define_const(mGdk, "EXTENSION_EVENTS_CURSOR", INT2FIX(GDK_EXTENSION_EVENTS_CURSOR));

    /* GdkWMDecoration */
    rb_define_const(mGdk, "DECOR_ALL", INT2FIX(GDK_DECOR_ALL));
    rb_define_const(mGdk, "DECOR_BORDER", INT2FIX(GDK_DECOR_BORDER));
    rb_define_const(mGdk, "DECOR_RESIZEH", INT2FIX(GDK_DECOR_RESIZEH));
    rb_define_const(mGdk, "DECOR_TITLE", INT2FIX(GDK_DECOR_TITLE));
    rb_define_const(mGdk, "DECOR_MENU", INT2FIX(GDK_DECOR_MENU));
    rb_define_const(mGdk, "DECOR_MINIMIZE", INT2FIX(GDK_DECOR_MINIMIZE));
    rb_define_const(mGdk, "DECOR_MAXIMIZE", INT2FIX(GDK_DECOR_MAXIMIZE));

    /* GdkWMFunction */
    rb_define_const(mGdk, "FUNC_ALL", INT2FIX(GDK_FUNC_ALL));
    rb_define_const(mGdk, "FUNC_RESIZE", INT2FIX(GDK_FUNC_RESIZE));
    rb_define_const(mGdk, "FUNC_MOVE", INT2FIX(GDK_FUNC_MOVE));
    rb_define_const(mGdk, "FUNC_MINIMIZE", INT2FIX(GDK_FUNC_MINIMIZE));
    rb_define_const(mGdk, "FUNC_MAXIMIZE", INT2FIX(GDK_FUNC_MAXIMIZE));
    rb_define_const(mGdk, "FUNC_CLOSE", INT2FIX(GDK_FUNC_CLOSE));

    /* GdkOverlapType */
    rb_define_const(mGdk, "OVERLAP_RACTANGLE_IN", INT2FIX(GDK_OVERLAP_RECTANGLE_IN));
    rb_define_const(mGdk, "OVERLAP_RACTANGLE_OUT", INT2FIX(GDK_OVERLAP_RECTANGLE_OUT));
    rb_define_const(mGdk, "OVERLAP_RACTANGLE_PART", INT2FIX(GDK_OVERLAP_RECTANGLE_PART));

  /* GdkRgbDither */
  rb_define_const(mGdk, "RGB_DITHER_NONE", INT2FIX(GDK_RGB_DITHER_NONE));
  rb_define_const(mGdk, "RGB_DITHER_NORMAL", INT2FIX(GDK_RGB_DITHER_NORMAL));
  rb_define_const(mGdk, "RGB_DITHER_MAX", INT2FIX(GDK_RGB_DITHER_MAX));
}

