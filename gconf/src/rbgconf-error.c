/*****************************************************************************
 *
 * rbgconf-error.c: GConf error handling.
 *
 * Copyright (C) 2002 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2002-11-12
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

#define ERRORS_COUNT	16

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

static int s_errors[ERRORS_COUNT];
static struct GCToMy {
	int gc;
	int my;
} s_gc_to_my[] = {
	{ GCONF_ERROR_FAILED, 0 },
	{ GCONF_ERROR_NO_SERVER, 1 },
	{ GCONF_ERROR_NO_PERMISSION, 2 },
	{ GCONF_ERROR_BAD_ADDRESS, 3 },
	{ GCONF_ERROR_BAD_KEY, 4 },
	{ GCONF_ERROR_PARSE_ERROR, 5 },
	{ GCONF_ERROR_CORRUPT, 6 },
	{ GCONF_ERROR_TYPE_MISMATCH, 7 },
	{ GCONF_ERROR_IS_DIR, 8 },
	{ GCONF_ERROR_IS_KEY, 9 },
	{ GCONF_ERROR_OVERRIDDEN, 10 },
	{ GCONF_ERROR_OAF_ERROR, 11 },
	{ GCONF_ERROR_LOCAL_ENGINE, 12 },
	{ GCONF_ERROR_LOCK_FAILED, 13 },
	{ GCONF_ERROR_NO_WRITABLE_DATABASE, 14 },
	{ GCONF_ERROR_IN_SHUTDOWN, 15 }
};


/* Function Implementations **************************************************/

void
error_default_error_handler(client, error)
	GConfClient *client;
	GError *error;
{
	int i;
	VALUE err = Qnil;

	for (i = 0; i < G_N_ELEMENTS(s_errors); i++) {
		if (error->code == s_gc_to_my[i].gc) {
			err = s_errors[s_gc_to_my[i].my];
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

	gconf_error = rb_define_class_under(m_gconf, "Error",
					    rb_eStandardError);
	s_errors[s_gc_to_my[GCONF_ERROR_FAILED].my] =
		rb_define_class_under(m_gconf, "FailedError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_NO_SERVER].my] =
		rb_define_class_under(m_gconf, "NoServerError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_NO_PERMISSION].my] =
	      rb_define_class_under(m_gconf, "NoPermissionError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_BAD_ADDRESS].my] =
		rb_define_class_under(m_gconf, "BadAddressError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_BAD_KEY].my] =
		rb_define_class_under(m_gconf, "BadKeyError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_PARSE_ERROR].my] =
		rb_define_class_under(m_gconf, "ParseError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_CORRUPT].my] =
		rb_define_class_under(m_gconf, "CorruptError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_TYPE_MISMATCH].my] =
	      rb_define_class_under(m_gconf, "TypeMismatchError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_IS_DIR].my] =
		rb_define_class_under(m_gconf, "IsDirError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_IS_KEY].my] =
		rb_define_class_under(m_gconf, "IsKeyError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_OVERRIDDEN].my] =
		rb_define_class_under(m_gconf, "OverriddenError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_OAF_ERROR].my] =
		rb_define_class_under(m_gconf, "OafError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_LOCAL_ENGINE].my] =
	       rb_define_class_under(m_gconf, "LocalEngineError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_LOCK_FAILED].my] =
		rb_define_class_under(m_gconf, "LockFailedError", gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_NO_WRITABLE_DATABASE].my] =
		rb_define_class_under(m_gconf, "NoWritableDatabaseError",
				      gconf_error);
	s_errors[s_gc_to_my[GCONF_ERROR_IN_SHUTDOWN].my] =
		rb_define_class_under(m_gconf, "InShutdownError", gconf_error);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
