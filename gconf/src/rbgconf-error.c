/*****************************************************************************
 *
 * rbgconf-error.c: GConf error handling.
 *
 * Copyright (C) 2004 Ruby-GNOME2 Project Team
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
 * $Date: 2004/08/22 17:47:48 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

void
error_default_error_handler(client, error)
     GConfClient *client;
     GError *error;
{
  RAISE_GERROR(error);
}

void
Init_gconf_error(m_gconf)
	VALUE m_gconf;
{
  VALUE gerror = G_DEF_ERROR2(GCONF_ERROR, "Error", m_gconf, rb_eRuntimeError);
  
  rb_define_const(gerror, "FAILED", INT2NUM(GCONF_ERROR_FAILED));
  rb_define_const(gerror, "NO_SERVER", INT2NUM(GCONF_ERROR_NO_SERVER));
  rb_define_const(gerror, "NO_PERMISSION", INT2NUM(GCONF_ERROR_NO_PERMISSION));
  rb_define_const(gerror, "BAD_ADDRESS", INT2NUM(GCONF_ERROR_BAD_ADDRESS));
  rb_define_const(gerror, "BAD_KEY", INT2NUM(GCONF_ERROR_BAD_KEY));
  rb_define_const(gerror, "PARSE_ERROR", INT2NUM(GCONF_ERROR_PARSE_ERROR));
  rb_define_const(gerror, "CORRUPT", INT2NUM(GCONF_ERROR_CORRUPT));
  rb_define_const(gerror, "TYPE_MISMATCH", INT2NUM(GCONF_ERROR_TYPE_MISMATCH));
  rb_define_const(gerror, "IS_DIR", INT2NUM(GCONF_ERROR_IS_DIR));
  rb_define_const(gerror, "IS_KEY", INT2NUM(GCONF_ERROR_IS_KEY));
  rb_define_const(gerror, "OVERRIDDEN", INT2NUM(GCONF_ERROR_OVERRIDDEN));
  rb_define_const(gerror, "OAF_ERROR", INT2NUM(GCONF_ERROR_OAF_ERROR));
  rb_define_const(gerror, "LOCAL_ENGINE", INT2NUM(GCONF_ERROR_LOCAL_ENGINE));
  rb_define_const(gerror, "LOCK_FAILED", INT2NUM(GCONF_ERROR_LOCK_FAILED));
  rb_define_const(gerror, "NO_WRITABLE_DATABASE", INT2NUM(GCONF_ERROR_NO_WRITABLE_DATABASE));
  rb_define_const(gerror, "IN_SHUTDOWN", INT2NUM(GCONF_ERROR_IN_SHUTDOWN));
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
