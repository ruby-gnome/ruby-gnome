/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkconst.c -

  $Author: mutoh $
  $Date: 2003/11/02 18:29:28 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
    /* GdkStatus */
    G_DEF_CLASS(GDK_TYPE_STATUS, "Status", mGdk);
    G_DEF_CONSTANTS(mGdk, GDK_TYPE_STATUS, "GDK_");

}


