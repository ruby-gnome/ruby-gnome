/*****************************************************************************
 *
 * rbgconf.c: Ruby GConf initialization.
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
 * $Date: 2003/02/04 21:28:49 $
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
Init_gconf2(void)
{
	VALUE m_gconf = rb_define_module("GConf");

	g_id_call = rb_intern("call");

	Init_gconf_error(m_gconf);
	Init_gconf_entry(m_gconf);
	Init_gconf_changeset(m_gconf);
	Init_gconf_schema(m_gconf);
	Init_gconf_engine(m_gconf);
	Init_gconf_client(m_gconf);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
