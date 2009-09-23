/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

void Init_gio2(void);

void
Init_gio2(void)
{
	VALUE glib = rb_define_module("GLib");

        DEF_CONSTANTS_CLASS(G_TYPE_FILESYSTEM_PREVIEW_TYPE,
                            "FilesystemPreviewType",
                            "G_",
                            glib);

	Init_gappinfo(glib);
	Init_gapplaunchcontext(glib);
	Init_gasyncresult(glib);
	Init_gbufferedinputstream(glib);
	Init_gbufferedoutputstream(glib);
	Init_gcancellable(glib);
	Init_gcontenttype(glib);
	Init_gdatainputstream(glib);
	Init_gfileattributematcher(glib);
	Init_gdataoutputstream(glib);
	Init_gdrive(glib);
	Init_gemblem(glib);
	Init_gemblemedicon(glib);
	Init_gfile(glib);
	Init_gfileattribute(glib);
	Init_gfileenumerator(glib);
	Init_gfileicon(glib);
	Init_gfileinfo(glib);
	Init_gfileinputstream(glib);
	Init_gfilemonitor(glib);
	Init_gfilenamecompleter(glib);
	Init_gfileoutputstream(glib);
	Init_gfilterinputstream(glib);
	Init_gfilteroutputstream(glib);
	Init_gicon(glib);
	Init_ginputstream(glib);
        Init_gioerror(glib);
	Init_gloadableicon(glib);
	Init_gmemoryinputstream(glib);
	Init_gmemoryoutputstream(glib);
	Init_gmount(glib);
	Init_gmountoperation(glib);
        Init_goutputstream(glib);
	Init_gseekable(glib);
	Init_gthemedicon(glib);
	Init_gvolume(glib);
	Init_gvolumemonitor(glib);
}
