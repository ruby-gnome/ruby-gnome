/*****************************************************************************
 * 
 * rbgconf-schema.h: GConf::Schema class.
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
 * $Author: pcppopper $
 * 
 * $Date: 2002/11/17 16:29:12 $
 * 
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef RBGCONF_SCHEMA_H
#define RBGCONF_SCHEMA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
/* Includes ******************************************************************/

/* Defines *******************************************************************/

#define GCONF_TYPE_SCHEMA 	(gconf_schema_get_gobject_type())
#define GCSCHEMA2RVAL(schema)	(BOXED2RVAL(schema, GCONF_TYPE_SCHEMA))
#define RVAL2GCSCHEMA(val)	\
	((GConfSchema *)RVAL2BOXED(val, GCONF_TYPE_SCHEMA))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

GType gconf_schema_get_gobject_type(void);
void Init_gconf_schema(VALUE m_gconf);

/* Global Variables **********************************************************/

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RBGCONF_SCHEMA_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
