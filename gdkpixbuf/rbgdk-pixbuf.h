/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf.h -

  $Author: mutoh $
  $Date: 2004/11/13 17:54:33 $

  Copyright (C) 2002-2004 Masao Mutoh
************************************************/

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include "rbgobject.h"

extern void Init_gdk_pixbuf_animation(VALUE mGLib);
extern void Init_gdk_pixdata(VALUE mGLib);
extern void Init_gdk_pixbuf_loader(VALUE mGLib);
extern void Init_gdk_pixbuf_format(VALUE mGLib);
extern GType gdk_pixbuf_format_get_type(void);

#define RBGDK_PIXBUF_CHECK_VERSION(major,minor,micro)    \
    (GDK_PIXBUF_MAJOR > (major) || \
     (GDK_PIXBUF_MAJOR == (major) && GDK_PIXBUF_MINOR > (minor)) || \
     (GDK_PIXBUF_MAJOR == (major) && GDK_PIXBUF_MINOR == (minor) && \
      GDK_PIXBUF_MICRO >= (micro)))

#define GDK_TYPE_PIXBUF_FORMAT (gdk_pixbuf_format_get_type())
