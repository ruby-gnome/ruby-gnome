/*****************************************************************************
 *
 * gnomevfs-monitor.c: GnomeVFS::Monitor class.
 *
 * Copyright (C) 2003 Nikolai Weibull <lone-star@home.se>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Nikolai Weibull <lone-star@home.se>
 *
 * Latest Revision: 2003-07-25
 *
 *****************************************************************************/

/* Includes ******************************************************************/

/* Defines *******************************************************************/

#define _SELF(s)	\
	((GnomeVFSMonitorHandle *)RVAL2BOXED(s, GNOMEVFS_TYPE_MONITOR))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static GnomeVFSMonitorHandle *
gnome_vfs_monitor_copy(gpointer boxed)
{
	return (GnomeVFSMonitorHandle *)boxed;
}

void
gnome_vfs_monitor_free(gpointer boxed)
{
}

GType
gnome_vfs_monitor_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GnomeVFSMonitor",
				(GBoxedCopyFunc)gnome_vfs_monitor_copy,
				(GBoxedFreeFunc)gnome_vfs_monitor_free);
	}

	return our_type;
}

/* TODO: this feels very, very C'ish.  There should be a way of monitoring only
 * certain events, such as changes or deletions, and perhaps some kind of
 * visitor interface should be created?  so that there's not a big
 * case-statement, but rather a dispatch in the proc.
 */
static void
monitor_callback(handle, monitor_uri, info_uri, event_type, data)
	GnomeVFSMonitorHandle *handle;
	const gchar *monitor_uri;
	const gchar *info_uri;
	GnomeVFSMonitorEventType event_type;
	gpointer data;
{
	rb_funcall((VALUE)data, g_id_call, CSTR2RVAL(monitor_uri),
		   CSTR2RVAL(info_uri), INT2FIX(event_type));
}

static VALUE
monitor_initialize(argc, argv, self);
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri, type, func;
	gchar *text_uri;
	gboolean free_uri;
	GnomeVFSMonitorHandle *handle;
	GnomeVFSResult result;

	/* XXX: is this the way to do it? */
	rb_scan_args(argc, argv, "2&", &uri, &type, &func);

	if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		text_uri = gnome_vfs_uri_to_string(RVAL2GVFSURI(uri),
						   GNOME_VFS_URI_HIDE_NONE);
		free_uri = TRUE;
	} else {
		text_uri = RVAL2CSTR(uri);
		free_uri = FALSE;
	}

	if (NIL_P(func)) {
		func = G_BLOCK_PROC();
	}
	G_RELATIVE(self, func);


	result = gnome_vfs_monitor_add(&handle, text_uri, FIX2INT(type),
				(GnomeVFSMonitorCallback)monitor_callback,
				(gpointer)func);

	RAISE_IF_ERROR(result);
	G_INITIALIZE(self, handle);
	if (free_uri) {
		g_free(text_uri);
	}
	return Qnil;
}

static VALUE
monitor_add(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE monitor;

	monitor = rb_funcall(self, rb_intern("new"), argc, argv);
	return monitor;
}

static VALUE
monitor_cancel(self)
	VALUE self;
{
	return GVFSRESULT2RVAL(gnome_vfs_monitor_cancel(_SELF(self)));
}

void
Init_gnomevfs_monitor(m_gvfs)
	VALUE m_gvfs;
{
	VALUE gvfs_monitor = G_DEF_CLASS(GNOMEVFS_TYPE_MONITOR, "Monitor",
					 m_gvfs);

	rb_define_singleton_method(gvfs_monitor, "initialize",
				   monitor_initialize, -1);
	rb_define_singelton_method(gvfs_monitor, "add", monitor_add, -1);
	rb_define_singelton_method(gvfs_monitor, "cancel", monitor_cancel, 0);

	rb_define_method(gvfs_monitor, "remove", monitor_cancel, 0);

	rb_define_const(gvfs_monitor, "FILE",
			INT2FIX(GNOME_VFS_MONITOR_FILE));
	rb_define_const(gvfs_monitor, "DIRECTORY",
			INT2FIX(GNOME_VFS_MONITOR_DIRECTORY));

	rb_define_const(gvfs_monitor, "EVENT_CHANGED",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_CHANGED));
	rb_define_const(gvfs_monitor, "EVENT_DELETED",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_DELETED));
	rb_define_const(gvfs_monitor, "EVENT_STARTEXECUTING",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_STARTEXECUTING));
	rb_define_const(gvfs_monitor, "EVENT_STOPTEXECUTING",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_STOPTEXECUTING));
	rb_define_const(gvfs_monitor, "EVENT_CREATED",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_CREATED));
	rb_define_const(gvfs_monitor, "EVENT_METADATA_CHANGED",
			INT2FIX(GNOME_VFS_MONITOR_EVENT_METADATA_CHANGED));
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
