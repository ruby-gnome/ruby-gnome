/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkconversions.h -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#ifndef __RB_GDK_CONVERSIONS_H__
#define __RB_GDK_CONVERSIONS_H__

#include <rbgobject.h>
#include <gdk/gdk.h>

#define RVAL2GDKCOLOR(color) ((GdkColor *)RVAL2BOXED(color, GDK_TYPE_COLOR))
#define GDKCOLOR2RVAL(color) (BOXED2RVAL(color, GDK_TYPE_COLOR))

#endif /* __RB_GDK_CONVERSIONS_H__ */
