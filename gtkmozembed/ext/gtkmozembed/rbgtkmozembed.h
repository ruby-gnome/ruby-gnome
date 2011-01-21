/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmozembed.h -

    Copyright (C) 2009 Ruby-GNOME2 Project Team.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
************************************************/

#ifndef __RB_GTK_MOZ_EMBED_H__
#define __RB_GTK_MOZ_EMBED_H__

#include "rbgtk.h"
#include "rbgtkmozembedversion.h"

#define MOZILLA_CLIENT
#include <mozilla-config.h>
#include <gtkmozembed.h>

G_BEGIN_DECLS

void Init_gtk_moz_embed(void);

G_END_DECLS

#endif
