/*****************************************************************************
 *
 * rbgconf-error.c: GConf error handling.
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
 * $Author: pcppopper $
 *
 * $Date: 2003/04/13 20:31:43 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

static struct GCToMy {
	int gconf_code;
	char *rb_class_name;
	VALUE rb_class;
} s_gc_to_my[] = {
	{ GCONF_ERROR_FAILED, 			"FailedError" },
	{ GCONF_ERROR_NO_SERVER, 	 	"NoServerError" },
	{ GCONF_ERROR_NO_PERMISSION, 		"NoPermissionError" },
	{ GCONF_ERROR_BAD_ADDRESS, 		"BadAddressError" },
	{ GCONF_ERROR_BAD_KEY, 			"BadKeyError" },
	{ GCONF_ERROR_PARSE_ERROR, 		"ParseError" },
	{ GCONF_ERROR_CORRUPT, 			"CorruptError" },
	{ GCONF_ERROR_TYPE_MISMATCH, 		"TypeMismatchError" },
	{ GCONF_ERROR_IS_DIR, 			"IsDirError" },
	{ GCONF_ERROR_IS_KEY, 			"IsKeyError" },
	{ GCONF_ERROR_OVERRIDDEN, 		"OverriddenError" },
	{ GCONF_ERROR_OAF_ERROR, 		"OafError" },
	{ GCONF_ERROR_LOCAL_ENGINE, 		"LocalEngineError" },
	{ GCONF_ERROR_LOCK_FAILED, 		"LockFailedError" },
	{ GCONF_ERROR_NO_WRITABLE_DATABASE,	"NoWritableDatabaseError" },
	{ GCONF_ERROR_IN_SHUTDOWN, 		"InShutdownError" }
};


/* Function Implementations **************************************************/

void
error_default_error_handler(client, error)
	GConfClient *client;
	GError *error;
{
	VALUE err = Qnil;
	int i, n;

	for (i = 0, n = G_N_ELEMENTS(s_gc_to_my); i < n; i++) {
		if (error->code == s_gc_to_my[i].gconf_code) {
			err = s_gc_to_my[i].rb_class;
			break;
		}
	}

	if (err == Qnil) {
		rb_warn("Unrecognized GConf error ignored; please report."
			" code: %d, message: %s", error->code, error->message);
		return;
	}

	rb_raise(err, "GConf Error: %s", error->message);
}

void
Init_gconf_error(m_gconf)
	VALUE m_gconf;
{
	VALUE gconf_error;
	int i, n;

	gconf_error = rb_define_class_under(m_gconf, "Error",
					    rb_eStandardError);
	for (i = 0, n = G_N_ELEMENTS(s_gc_to_my); i <  n; i++) {
		s_gc_to_my[i].rb_class = rb_define_class_under(m_gconf,
						s_gc_to_my[i].rb_class_name,
						gconf_error);
	}
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
