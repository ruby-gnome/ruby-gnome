/*****************************************************************************
 *
 * rbgconf-client.c: GConf::Client class.
 *
 * Copyright (C) 2002,2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
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
 * $Author: mutoh $
 *
 * $Date: 2003/06/26 15:17:01 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

#define _SELF(s)	(GCONF_CLIENT(RVAL2GOBJ(s)))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

/*
 * called when a notification is executed. user_data is the proc the user
 * gave us.
 */
static void
client_notify_callback(client, cnxn_id, entry, user_data)
	GConfClient *client;
	guint cnxn_id;
	GConfEntry *entry;
	gpointer user_data;
{   
	rb_funcall((VALUE)user_data, g_id_call, 2, GOBJ2RVAL(client),
		   GCENTRY2RVAL(entry));
}

static VALUE
client_initialize(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE engine;
	GConfClient *client;

	/* check if we were passed an engine to use */
	if (rb_scan_args(argc, argv, "01", &engine) == 1) {
		client = gconf_client_get_for_engine(RVAL2GCENGINE(engine));
	} else {
		client = gconf_client_get_default();
	}
	
	G_INITIALIZE(self, client);
	return Qnil;
}

static VALUE
client_add_dir(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE dir, preload;

	rb_scan_args(argc, argv, "11", &dir, &preload);

	/* check if we were passed a preloading specification */
	if (NIL_P(preload)) {
		preload = INT2FIX(GCONF_CLIENT_PRELOAD_NONE);
	}

	gconf_client_add_dir(_SELF(self), RVAL2CSTR(dir), NUM2INT(preload),
			     NULL);
	return self;
}

static VALUE
client_remove_dir(self, dir)
	VALUE self;
	VALUE dir;
{
	gconf_client_remove_dir(_SELF(self), RVAL2CSTR(dir), NULL);
	return self;
}

static VALUE
client_notify_add(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE namespace;
	VALUE func;

	/* parse the namespace and get a block to be called on notifies */
	rb_scan_args(argc, argv, "1&", &namespace, &func);
	if (NIL_P(func)) {
		func = G_BLOCK_PROC();
	}
	G_RELATIVE(self, func);

	return UINT2NUM(gconf_client_notify_add(_SELF(self),
				RVAL2CSTR(namespace),
				(GConfClientNotifyFunc)client_notify_callback,
				(gpointer)func, NULL, NULL));
}

static VALUE
client_notify_remove(self, cnxn_id)
	VALUE self;
	VALUE cnxn_id;
{
	gconf_client_notify_remove(_SELF(self), NUM2INT(cnxn_id));
	return self;
}

static VALUE
client_clear_cache(self)
	VALUE self;
{
	gconf_client_clear_cache(_SELF(self));
	return self;
}

static VALUE
client_preload(self, dir, preload)
	VALUE self;
	VALUE dir;
	VALUE preload;
{
	gconf_client_preload(_SELF(self), RVAL2CSTR(dir), NUM2INT(preload),
			     NULL);
	return self;
}

static VALUE
client_set(self, key, val)
	VALUE self;
	VALUE key;
	VALUE val;
{
	GConfValue *gcval;

	gcval = RVAL2GCVAL(val);
	gconf_client_set(_SELF(self), RVAL2CSTR(key), gcval, NULL);
	gconf_value_free(gcval);
	return self;
}

/* TODO: is the use_schema_default nice? */
static VALUE
client_get(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE key;
	VALUE use_schema_default;
	GConfValue *val;

	rb_scan_args(argc, argv, "11", &key, &use_schema_default);
	if (NIL_P(use_schema_default) || RTEST(use_schema_default)) {
		val = gconf_client_get(_SELF(self), RVAL2CSTR(key), NULL);
	} else {
		val = gconf_client_get_without_default(_SELF(self),
						       RVAL2CSTR(key), NULL);
	}
	
	return GCVAL2RVAL(val);
}

static VALUE
client_get_entry(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE key, locale, use_schema_default;
	char *clocale;

	rb_scan_args(argc, argv, "12", &key, &locale, &use_schema_default);
	if (NIL_P(use_schema_default)) {
		use_schema_default = Qtrue;
	}
	clocale = NIL_P(locale) ? NULL : RVAL2CSTR(locale);

	return GCENTRY2RVAL(gconf_client_get_entry(_SELF(self), RVAL2CSTR(key),
						   NULL,
						   RTEST(use_schema_default),
						   NULL));
}

static VALUE
client_get_default_from_schema(self, key)
	VALUE self;
	VALUE key;
{
	return GCVAL2RVAL(gconf_client_get_default_from_schema(_SELF(self),
							RVAL2CSTR(key), NULL));
}

static VALUE
client_unset(self, key)
	VALUE self;
	VALUE key;
{
	return CBOOL2RVAL(gconf_client_unset(_SELF(self), RVAL2CSTR(key),
					     NULL));
}

static VALUE
client_all_entries(self, dir)
	VALUE self, dir;
{
	GSList *entries, *i;
	VALUE entry_ary;

	i = entries = gconf_client_all_entries(_SELF(self), RVAL2CSTR(dir),
					       NULL);
	entry_ary = rb_ary_new2(g_slist_length(entries));
	while (i != NULL) {
		rb_ary_push(entry_ary, GCENTRY2RVAL(i->data));
		i = g_slist_next(i);
	}

	g_slist_free(entries);
	return entry_ary;
}


static VALUE
client_all_dirs(self, dir)
	VALUE self, dir;
{
	GSList *dirs, *i;
	VALUE dirs_ary;

	i = dirs = gconf_client_all_dirs(_SELF(self), RVAL2CSTR(dir), NULL);
	dirs_ary = rb_ary_new2(g_slist_length(dirs));
	while (i != NULL) {
		rb_ary_push(dirs_ary, CSTR2RVAL(i->data));
		g_free(i->data);
		i = g_slist_next(i);
	}

	g_slist_free(dirs);
	return dirs_ary;
}

static VALUE
client_suggest_sync(self)
	VALUE self;
{
	gconf_client_suggest_sync(_SELF(self), NULL);
	return self;
}

static VALUE
client_dir_exists(self, dir)
	VALUE self;
	VALUE dir;
{
	return CBOOL2RVAL(gconf_client_dir_exists(_SELF(self),
						  RVAL2CSTR(dir), NULL));
}

static VALUE
client_reverse_change_set(self, cs)
	VALUE self, cs;
{
	return GCCHANGESET2RVAL(gconf_client_reverse_change_set(_SELF(self),
							RVAL2GCCHANGESET(cs),
							NULL));
}

static VALUE
client_change_set_from_currentv(self, keys)
	VALUE self, keys;
{
	int i, n;
	gchar **gkeys;
	VALUE key, set;

	Check_Type(keys, T_ARRAY);
	n = RARRAY(keys)->len;
	gkeys = g_new(gchar *, n + 1);

	/* convert to NULL-terminated array of gchar *'s */
	for (i = 0; i < n; i++) {
		key = rb_ary_entry(keys, i);
		gkeys[i] = RVAL2CSTR(key);
	}
	gkeys[n] = NULL;

	set = GCCHANGESET2RVAL(gconf_client_change_set_from_currentv(
						_SELF(self),
						(const gchar **)gkeys, NULL));
	g_free(gkeys);
	return set;
}

/*
 * XXX: should the value of remove_committed be Qfalse?
 * what it does is free the keys of the GConfChangeSet being passed,
 * but as these are references to Ruby strings we shouldn't really touch them
 * right?
 */
static VALUE
client_commit_change_set(self, cs)
	VALUE self, cs;
{
	return CBOOL2RVAL(gconf_client_commit_change_set(_SELF(self),
							 RVAL2GCCHANGESET(cs),
							 FALSE, NULL));
}

void
Init_gconf_client(m_gconf)
	VALUE m_gconf;
{
	VALUE gconf_client;
	
	gconf_client = G_DEF_CLASS(GCONF_TYPE_CLIENT, "Client", m_gconf);

	rb_define_method(gconf_client, "initialize", client_initialize, -1);
	rb_define_method(gconf_client, "add_dir", client_add_dir, -1);
	rb_define_method(gconf_client, "remove_dir", client_remove_dir, 1);
	rb_define_method(gconf_client, "notify_add", client_notify_add, -1);
	rb_define_method(gconf_client, "notify_remove", client_notify_remove,
			 1);
	rb_define_method(gconf_client, "clear_cache", client_clear_cache, 0);
	rb_define_method(gconf_client, "preload", client_preload, 2);
	rb_define_method(gconf_client, "set", client_set, 2);
	rb_define_method(gconf_client, "get", client_get, -1);
	rb_define_method(gconf_client, "entry", client_get_entry, -1);
	rb_define_method(gconf_client, "default_from_schema",
			 client_get_default_from_schema, 1);
	rb_define_method(gconf_client, "unset", client_unset, 1);
	rb_define_method(gconf_client, "all_entries", client_all_entries, 1);
	rb_define_method(gconf_client, "all_dirs", client_all_dirs, 1);
	rb_define_method(gconf_client, "suggest_sync", client_suggest_sync, 0);
	rb_define_method(gconf_client, "dir_exists?", client_dir_exists, 1);
	rb_define_method(gconf_client, "reverse_change_set",
			 client_reverse_change_set, 1);
	rb_define_method(gconf_client, "change_set_from_current",
			 client_change_set_from_currentv, 1);
	rb_define_method(gconf_client, "commit_change_set",
			 client_commit_change_set, 1);

	rb_define_alias(gconf_client, "[]", "get");
	rb_define_alias(gconf_client, "[]=", "set");
	rb_define_alias(gconf_client, "reverse", "reverse_change_set");
	rb_define_alias(gconf_client, "diff", "change_set_from_current");
	rb_define_alias(gconf_client, "commit", "commit_change_set");

	rb_define_const(gconf_client, "PRELOAD_NONE",
			INT2FIX(GCONF_CLIENT_PRELOAD_NONE));
	rb_define_const(gconf_client, "PRELOAD_ONELEVEL",
			INT2FIX(GCONF_CLIENT_PRELOAD_ONELEVEL));
	rb_define_const(gconf_client, "PRELOAD_RECURSIVE",
			INT2FIX(GCONF_CLIENT_PRELOAD_RECURSIVE));

	/* setup error handling in the client */
	gconf_client_set_global_default_error_handler(
		(GConfClientErrorHandlerFunc)error_default_error_handler);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
