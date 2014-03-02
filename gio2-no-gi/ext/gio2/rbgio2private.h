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

#ifndef __RBGIO2PRIVATE_H__
#define __RBGIO2PRIVATE_H__

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>
#include "rbgio2.h"

#define CSTR2RVAL_TAINTED(string, length) \
        rbgio_cstr_to_rval_tainted((string), (length))
#define CSTR2RVAL_TAINTED_FREE(string, length) \
        rbgio_cstr_to_rval_tainted_free((string), (length))

#define GLIST2ARY_UNREF_FREE(list) rbgio_glist_to_ary_unref_free(list)

#define GFDS2ARY(fds) rbgio_fds_to_ary(fds)
#define GFDS2ARY_FREE(fds) rbgio_fds_to_ary_free(fds)

#define GFILEARY2GLIST(ary) rbgio_gfile_ary_to_glist(ary)
#define GFILEARY2GLIST_ACCEPT_NIL(ary) rbgio_gfile_ary_to_glist_accept_nil(ary)

G_GNUC_INTERNAL VALUE rbgio_cstr_to_rval_tainted(const char *string,
                                                 gsize length);
G_GNUC_INTERNAL VALUE rbgio_cstr_to_rval_tainted_free(char *string,
                                                      gsize length);
G_GNUC_INTERNAL VALUE rbgio_glist_to_ary_unref_free(GList *list);
G_GNUC_INTERNAL VALUE rbgio_fds_to_ary(const gint *fds);
G_GNUC_INTERNAL VALUE rbgio_fds_to_ary_free(gint *fds);
G_GNUC_INTERNAL GList *rbgio_gfile_ary_to_glist(VALUE ary);
G_GNUC_INTERNAL GList *rbgio_gfile_ary_to_glist_accept_nil(VALUE ary);
G_GNUC_INTERNAL void rbgio_rval_to_gtimeval(VALUE value, GTimeVal *time);
G_GNUC_INTERNAL void rbgio_async_ready_callback(GObject *source,
                                       GAsyncResult *result,
                                       gpointer data);
G_GNUC_INTERNAL VALUE rbgio_child_remove_and_return(VALUE parent, VALUE child);

G_GNUC_INTERNAL void Init_gio(void);
G_GNUC_INTERNAL void Init_gappinfo(VALUE mGio);
G_GNUC_INTERNAL void Init_gapplaunchcontext(VALUE mGio);
G_GNUC_INTERNAL void Init_gasyncinitable(VALUE mGio);
G_GNUC_INTERNAL void Init_gasyncresult(VALUE mGio);
G_GNUC_INTERNAL void Init_gbufferedinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gbufferedoutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gcancellable(VALUE mGio);
G_GNUC_INTERNAL void Init_gcharsetconverter(VALUE mGio);
G_GNUC_INTERNAL void Init_gcontenttype(VALUE mGio);
G_GNUC_INTERNAL void Init_gconverter(VALUE mGio);
G_GNUC_INTERNAL void Init_gconverterinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gconverteroutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gdatainputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gdataoutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gdesktopappinfo(VALUE mGio);
G_GNUC_INTERNAL void Init_gdrive(VALUE mGio);
G_GNUC_INTERNAL void Init_gemblem(VALUE mGio);
G_GNUC_INTERNAL void Init_gemblemedicon(VALUE mGio);
G_GNUC_INTERNAL void Init_gfile(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileattribute(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileattributeinfo(VALUE mGio);
G_GNUC_INTERNAL void Init_fileattributeinfolist(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileattributematcher(VALUE mGio);
G_GNUC_INTERNAL void Init_gfiledescriptorbased(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileenumerator(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileicon(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileinfo(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileiostream(VALUE mGio);
G_GNUC_INTERNAL void Init_gfilemonitor(VALUE mGio);
G_GNUC_INTERNAL void Init_gfilenamecompleter(VALUE mGio);
G_GNUC_INTERNAL void Init_gfileoutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gfilterinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gfilteroutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gicon(VALUE mGio);
G_GNUC_INTERNAL void Init_ginetaddress(VALUE mGio);
G_GNUC_INTERNAL void Init_ginetsocketaddress(VALUE mGio);
G_GNUC_INTERNAL void Init_ginitable(VALUE mGio);
G_GNUC_INTERNAL void Init_ginputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gioerror(VALUE mGio);
G_GNUC_INTERNAL void Init_giomodule(VALUE mGio);
G_GNUC_INTERNAL void Init_giomodules(VALUE mGio);
G_GNUC_INTERNAL void Init_gioscheduler(VALUE mGio);
G_GNUC_INTERNAL void Init_gioschedulerjob(VALUE mGio);
G_GNUC_INTERNAL void Init_giostream(VALUE mGio);
G_GNUC_INTERNAL void Init_gloadableicon(VALUE mGio);
G_GNUC_INTERNAL void Init_gmemoryinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gmemoryoutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gmount(VALUE mGio);
G_GNUC_INTERNAL void Init_gmountoperation(VALUE mGio);
G_GNUC_INTERNAL void Init_gnetworkaddress(VALUE mGio);
G_GNUC_INTERNAL void Init_gnetworkservice(VALUE mGio);
G_GNUC_INTERNAL void Init_goutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gresolver(VALUE mGio);
G_GNUC_INTERNAL void Init_gseekable(VALUE mGio);
G_GNUC_INTERNAL void Init_gsimpleasyncresult(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocket(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketaddress(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketclient(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketconnectable(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketaddressenumerator(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketconnection(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketconnectionfactory(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketcontrolmessage(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketlistener(VALUE mGio);
G_GNUC_INTERNAL void Init_gsocketservice(VALUE mGio);
G_GNUC_INTERNAL void Init_gsrvtarget(VALUE mGio);
G_GNUC_INTERNAL void Init_gtcpconnection(VALUE mGio);
G_GNUC_INTERNAL void Init_gthreadedsocketservice(VALUE mGio);
G_GNUC_INTERNAL void Init_gthemedicon(VALUE mGio);
G_GNUC_INTERNAL void Init_gtlscertificate(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixconnection(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixfdlist(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixfdmessage(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixinputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixmount(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixmounts(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixmountpoint(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixmountpoints(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixmountmonitor(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixoutputstream(VALUE mGio);
G_GNUC_INTERNAL void Init_gunixsocketaddress(VALUE mGio);
G_GNUC_INTERNAL void Init_gvfs(VALUE mGio);
G_GNUC_INTERNAL void Init_gvolume(VALUE mGio);
G_GNUC_INTERNAL void Init_gvolumemonitor(VALUE mGio);
G_GNUC_INTERNAL void Init_gzlibcompressor(VALUE mGio);
G_GNUC_INTERNAL void Init_gzlibdecompressor(VALUE mGio);
G_GNUC_INTERNAL void Init_util(void);

#endif /* __RBGIO2PRIVATE_H__ */
