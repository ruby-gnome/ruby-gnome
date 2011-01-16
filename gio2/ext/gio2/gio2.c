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

static VALUE
gio_has_unix(G_GNUC_UNUSED VALUE self)
{
#ifdef HAVE_GIO_UNIX
        return Qtrue;
#else
        return Qfalse;
#endif
}

void
Init_gio2(void)
{
        VALUE glib = mGLib;

        G_DEF_CLASS(G_TYPE_FILESYSTEM_PREVIEW_TYPE, "FilesystemPreviewType", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_FILESYSTEM_PREVIEW_TYPE, "G_");

        G_DEF_CLASS(G_TYPE_DATA_STREAM_BYTE_ORDER, "DataStreamByteOrder", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_DATA_STREAM_BYTE_ORDER, "G_");

        G_DEF_CLASS(G_TYPE_DATA_STREAM_NEWLINE_TYPE, "DataStreamNewlineType", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_DATA_STREAM_NEWLINE_TYPE, "G_");

        rb_define_module_function(glib, "gio_has_unix?", gio_has_unix, 0);

        Init_util();

        Init_gappinfo(glib);
        Init_gapplaunchcontext(glib);
        Init_gasyncinitable(glib);
        Init_gasyncresult(glib);
        Init_gbufferedinputstream(glib);
        Init_gbufferedoutputstream(glib);
        Init_gcancellable(glib);
        Init_gcharsetconverter(glib);
        Init_gcontenttype(glib);
        Init_gconverter(glib);
        Init_gconverterinputstream(glib);
        Init_gconverteroutputstream(glib);
        Init_gdatainputstream(glib);
        Init_gdesktopappinfo(glib);
        Init_gdataoutputstream(glib);
        Init_gdrive(glib);
        Init_gemblem(glib);
        Init_gemblemedicon(glib);
        Init_gfile(glib);
        Init_gfileattribute(glib);
        Init_gfileattributeinfo(glib);
        Init_gfileattributematcher(glib);
        Init_gfiledescriptorbased(glib);
        Init_gfileenumerator(glib);
        Init_gfileicon(glib);
        Init_gfileinfo(glib);
        Init_gfileinputstream(glib);
        Init_gfileiostream(glib);
        Init_gfilemonitor(glib);
        Init_gfilenamecompleter(glib);
        Init_gfileoutputstream(glib);
        Init_gfilterinputstream(glib);
        Init_gfilteroutputstream(glib);
        Init_gicon(glib);
        Init_ginetaddress(glib);
        Init_ginetsocketaddress(glib);
        Init_ginitable(glib);
        Init_ginputstream(glib);
        Init_gioerror(glib);
        Init_giomodule(glib);
        Init_gioscheduler(glib);
        Init_giostream(glib);
        Init_gloadableicon(glib);
        Init_gmemoryinputstream(glib);
        Init_gmemoryoutputstream(glib);
        Init_gmount(glib);
        Init_gmountoperation(glib);
        Init_gnetworkaddress(glib);
        Init_gnetworkservice(glib);
        Init_goutputstream(glib);
        Init_gresolver(glib);
        Init_gseekable(glib);
        Init_gsimpleasyncresult(glib);
        Init_gsocket(glib);
        Init_gsocketaddress(glib);
        Init_gsocketclient(glib);
        Init_gsocketconnectable(glib);
        Init_gsocketconnection(glib);
        Init_gsocketcontrolmessage(glib);
        Init_gsocketlistener(glib);
        Init_gsocketservice(glib);
        Init_gsrvtarget(glib);
        Init_gtcpconnection(glib);
        Init_gthemedicon(glib);
        Init_gthreadedsocketservice(glib);
        Init_gunixconnection(glib);
        Init_gunixfdlist(glib);
        Init_gunixfdmessage(glib);
        Init_gunixinputstream(glib);
        Init_gunixmounts(glib);
        Init_gunixoutputstream(glib);
        Init_gunixsocketaddress(glib);
        Init_gvfs(glib);
        Init_gvolume(glib);
        Init_gvolumemonitor(glib);
        Init_gzlibcompressor(glib);
        Init_gzlibdecompressor(glib);
}
