/*****************************************************************************
 * 
 * rbgconf-client.h: GConf::Client class.
 * 
 * Copyright (C) 2002 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
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
 * $Date: 2002/11/18 17:44:45 $
 * 
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef RBGCONF_CLIENT_H
#define RBGCONF_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
/* Includes ******************************************************************/

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

void Init_gconf_client(VALUE m_gconf);

/* Global Variables **********************************************************/

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RBGCONF_CLIENT_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
