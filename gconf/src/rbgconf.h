/*****************************************************************************
 * 
 * rbgconf.h: Ruby GConf Module.
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

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef RBGCONF_H
#define RBGCONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
/* Includes ******************************************************************/

#include <glib.h>
#include <ruby.h>
#include <gconf/gconf-client.h>

#include <rbglib.h>
#include <rbgobject.h>

#include "rbgconf-util.h"
#include "rbgconf-error.h"
#include "rbgconf-entry.h"
#include "rbgconf-client.h"
#include "rbgconf-engine.h"
#include "rbgconf-schema.h"
#include "rbgconf-changeset.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

ID g_id_call;

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RBGCONF_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
