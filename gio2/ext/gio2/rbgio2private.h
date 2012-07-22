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
G_GNUC_INTERNAL void Init_gappinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gapplaunchcontext(VALUE glib);
G_GNUC_INTERNAL void Init_gasyncinitable(VALUE glib);
G_GNUC_INTERNAL void Init_gasyncresult(VALUE glib);
G_GNUC_INTERNAL void Init_gbufferedinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gbufferedoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gcancellable(VALUE glib);
G_GNUC_INTERNAL void Init_gcharsetconverter(VALUE glib);
G_GNUC_INTERNAL void Init_gcontenttype(VALUE glib);
G_GNUC_INTERNAL void Init_gconverter(VALUE glib);
G_GNUC_INTERNAL void Init_gconverterinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gconverteroutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdatainputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdataoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdesktopappinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gdrive(VALUE glib);
G_GNUC_INTERNAL void Init_gemblem(VALUE glib);
G_GNUC_INTERNAL void Init_gemblemedicon(VALUE glib);
G_GNUC_INTERNAL void Init_gfile(VALUE glib);
G_GNUC_INTERNAL void Init_gfileattribute(VALUE glib);
G_GNUC_INTERNAL void Init_gfileattributeinfo(VALUE glib);
G_GNUC_INTERNAL void Init_fileattributeinfolist(VALUE glib);
G_GNUC_INTERNAL void Init_gfileattributematcher(VALUE glib);
G_GNUC_INTERNAL void Init_gfiledescriptorbased(VALUE glib);
G_GNUC_INTERNAL void Init_gfileenumerator(VALUE glib);
G_GNUC_INTERNAL void Init_gfileicon(VALUE glib);
G_GNUC_INTERNAL void Init_gfileinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gfileinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfileiostream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilemonitor(VALUE glib);
G_GNUC_INTERNAL void Init_gfilenamecompleter(VALUE glib);
G_GNUC_INTERNAL void Init_gfileoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilterinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilteroutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gicon(VALUE glib);
G_GNUC_INTERNAL void Init_ginetaddress(VALUE glib);
G_GNUC_INTERNAL void Init_ginetsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_ginitable(VALUE glib);
G_GNUC_INTERNAL void Init_ginputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gioerror(VALUE glib);
G_GNUC_INTERNAL void Init_giomodule(VALUE glib);
G_GNUC_INTERNAL void Init_giomodules(VALUE glib);
G_GNUC_INTERNAL void Init_gioscheduler(VALUE glib);
G_GNUC_INTERNAL void Init_gioschedulerjob(VALUE glib);
G_GNUC_INTERNAL void Init_giostream(VALUE glib);
G_GNUC_INTERNAL void Init_gloadableicon(VALUE glib);
G_GNUC_INTERNAL void Init_gmemoryinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gmemoryoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gmount(VALUE glib);
G_GNUC_INTERNAL void Init_gmountoperation(VALUE glib);
G_GNUC_INTERNAL void Init_gnetworkaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gnetworkservice(VALUE glib);
G_GNUC_INTERNAL void Init_goutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gresolver(VALUE glib);
G_GNUC_INTERNAL void Init_gseekable(VALUE glib);
G_GNUC_INTERNAL void Init_gsimpleasyncresult(VALUE glib);
G_GNUC_INTERNAL void Init_gsocket(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketclient(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketconnectable(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketaddressenumerator(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketconnectionfactory(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketcontrolmessage(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketlistener(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketservice(VALUE glib);
G_GNUC_INTERNAL void Init_gsrvtarget(VALUE glib);
G_GNUC_INTERNAL void Init_gtcpconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gthreadedsocketservice(VALUE glib);
G_GNUC_INTERNAL void Init_gthemedicon(VALUE glib);
G_GNUC_INTERNAL void Init_gtlscertificate(VALUE glib);
G_GNUC_INTERNAL void Init_gunixconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gunixfdlist(VALUE glib);
G_GNUC_INTERNAL void Init_gunixfdmessage(VALUE glib);
G_GNUC_INTERNAL void Init_gunixinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmount(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmounts(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmountpoint(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmountpoints(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmountmonitor(VALUE glib);
G_GNUC_INTERNAL void Init_gunixoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gunixsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gvfs(VALUE glib);
G_GNUC_INTERNAL void Init_gvolume(VALUE glib);
G_GNUC_INTERNAL void Init_gvolumemonitor(VALUE glib);
G_GNUC_INTERNAL void Init_gzlibcompressor(VALUE glib);
G_GNUC_INTERNAL void Init_gzlibdecompressor(VALUE glib);
G_GNUC_INTERNAL void Init_util(void);

#endif /* __RBGIO2PRIVATE_H__ */
