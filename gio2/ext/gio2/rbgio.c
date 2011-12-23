/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE mGio

static VALUE
rg_m_has_unix_p(G_GNUC_UNUSED VALUE self)
{
#ifdef HAVE_GIO_UNIX
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_gio(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module("Gio");
    G_DEF_CLASS(G_TYPE_FILESYSTEM_PREVIEW_TYPE, "FilesystemPreviewType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(G_TYPE_DATA_STREAM_BYTE_ORDER, "DataStreamByteOrder", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(G_TYPE_DATA_STREAM_NEWLINE_TYPE, "DataStreamNewlineType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(G_TYPE_ASK_PASSWORD_FLAGS, "AskPasswordFlags", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(G_TYPE_PASSWORD_SAVE, "PasswordSave", RG_TARGET_NAMESPACE);

    RG_DEF_MODFUNC_P(has_unix, 0);

    Init_gappinfo(RG_TARGET_NAMESPACE);
    Init_gapplaunchcontext(RG_TARGET_NAMESPACE);
    Init_gasyncinitable(RG_TARGET_NAMESPACE);
    Init_gasyncresult(RG_TARGET_NAMESPACE);
    Init_gbufferedinputstream(RG_TARGET_NAMESPACE);
    Init_gbufferedoutputstream(RG_TARGET_NAMESPACE);
    Init_gcancellable(RG_TARGET_NAMESPACE);
    Init_gcharsetconverter(RG_TARGET_NAMESPACE);
    Init_gcontenttype(RG_TARGET_NAMESPACE);
    Init_gconverter(RG_TARGET_NAMESPACE);
    Init_gconverterinputstream(RG_TARGET_NAMESPACE);
    Init_gconverteroutputstream(RG_TARGET_NAMESPACE);
    Init_gdatainputstream(RG_TARGET_NAMESPACE);
    Init_gdesktopappinfo(RG_TARGET_NAMESPACE);
    Init_gdataoutputstream(RG_TARGET_NAMESPACE);
    Init_gdrive(RG_TARGET_NAMESPACE);
    Init_gemblem(RG_TARGET_NAMESPACE);
    Init_gemblemedicon(RG_TARGET_NAMESPACE);
    Init_gfile(RG_TARGET_NAMESPACE);
    Init_gfileattribute(RG_TARGET_NAMESPACE);
    Init_gfileattributeinfo(RG_TARGET_NAMESPACE);
    Init_gfileattributematcher(RG_TARGET_NAMESPACE);
    Init_gfiledescriptorbased(RG_TARGET_NAMESPACE);
    Init_gfileenumerator(RG_TARGET_NAMESPACE);
    Init_gfileicon(RG_TARGET_NAMESPACE);
    Init_gfileinfo(RG_TARGET_NAMESPACE);
    Init_gfileinputstream(RG_TARGET_NAMESPACE);
    Init_gfileiostream(RG_TARGET_NAMESPACE);
    Init_gfilemonitor(RG_TARGET_NAMESPACE);
    Init_gfilenamecompleter(RG_TARGET_NAMESPACE);
    Init_gfileoutputstream(RG_TARGET_NAMESPACE);
    Init_gfilterinputstream(RG_TARGET_NAMESPACE);
    Init_gfilteroutputstream(RG_TARGET_NAMESPACE);
    Init_gicon(RG_TARGET_NAMESPACE);
    Init_ginetaddress(RG_TARGET_NAMESPACE);
    Init_ginetsocketaddress(RG_TARGET_NAMESPACE);
    Init_ginitable(RG_TARGET_NAMESPACE);
    Init_ginputstream(RG_TARGET_NAMESPACE);
    Init_gioerror(RG_TARGET_NAMESPACE);
    Init_giomodule(RG_TARGET_NAMESPACE);
    Init_giomodules(RG_TARGET_NAMESPACE);
    Init_gioscheduler(RG_TARGET_NAMESPACE);
    Init_gioschedulerjob(RG_TARGET_NAMESPACE);
    Init_giostream(RG_TARGET_NAMESPACE);
    Init_gloadableicon(RG_TARGET_NAMESPACE);
    Init_gmemoryinputstream(RG_TARGET_NAMESPACE);
    Init_gmemoryoutputstream(RG_TARGET_NAMESPACE);
    Init_gmount(RG_TARGET_NAMESPACE);
    Init_gmountoperation(RG_TARGET_NAMESPACE);
    Init_gnetworkaddress(RG_TARGET_NAMESPACE);
    Init_gnetworkservice(RG_TARGET_NAMESPACE);
    Init_goutputstream(RG_TARGET_NAMESPACE);
    Init_gresolver(RG_TARGET_NAMESPACE);
    Init_gseekable(RG_TARGET_NAMESPACE);
    Init_gsimpleasyncresult(RG_TARGET_NAMESPACE);
    Init_gsocket(RG_TARGET_NAMESPACE);
    Init_gsocketaddress(RG_TARGET_NAMESPACE);
    Init_gsocketclient(RG_TARGET_NAMESPACE);
    Init_gsocketconnectable(RG_TARGET_NAMESPACE);
    Init_gsocketaddressenumerator(RG_TARGET_NAMESPACE);
    Init_gsocketconnection(RG_TARGET_NAMESPACE);
    Init_gsocketconnectionfactory(RG_TARGET_NAMESPACE);
    Init_gsocketcontrolmessage(RG_TARGET_NAMESPACE);
    Init_gsocketlistener(RG_TARGET_NAMESPACE);
    Init_gsocketservice(RG_TARGET_NAMESPACE);
    Init_gsrvtarget(RG_TARGET_NAMESPACE);
    Init_gtcpconnection(RG_TARGET_NAMESPACE);
    Init_gthemedicon(RG_TARGET_NAMESPACE);
    Init_gthreadedsocketservice(RG_TARGET_NAMESPACE);
    Init_gunixconnection(RG_TARGET_NAMESPACE);
    Init_gunixfdlist(RG_TARGET_NAMESPACE);
    Init_gunixfdmessage(RG_TARGET_NAMESPACE);
    Init_gunixinputstream(RG_TARGET_NAMESPACE);
    Init_gunixmount(RG_TARGET_NAMESPACE);
    Init_gunixmounts(RG_TARGET_NAMESPACE);
    Init_gunixmountpoint(RG_TARGET_NAMESPACE);
    Init_gunixmountpoints(RG_TARGET_NAMESPACE);
    Init_gunixmountmonitor(RG_TARGET_NAMESPACE);
    Init_gunixoutputstream(RG_TARGET_NAMESPACE);
    Init_gunixsocketaddress(RG_TARGET_NAMESPACE);
    Init_gvfs(RG_TARGET_NAMESPACE);
    Init_gvolume(RG_TARGET_NAMESPACE);
    Init_gvolumemonitor(RG_TARGET_NAMESPACE);
    Init_gzlibcompressor(RG_TARGET_NAMESPACE);
    Init_gzlibdecompressor(RG_TARGET_NAMESPACE);
}
