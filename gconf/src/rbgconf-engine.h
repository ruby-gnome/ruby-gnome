/*****************************************************************************
 * 
 * rbgconf-engine.h: GConf::Engine class.
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
 * Latest Revision: 2002-11-10
 * 
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef RBGCONF_ENGINE_H
#define RBGCONF_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
/* Includes ******************************************************************/

/* Defines *******************************************************************/

#define GCONF_TYPE_ENGINE 	(gconf_engine_get_type())
#define RVAL2GCENGINE(en)	\
	((GConfEngine *)RVAL2BOXED(en, GCONF_TYPE_ENGINE))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

GType gconf_engine_get_type(void);
void Init_gconf_engine(VALUE m_gconf);

/* Global Variables **********************************************************/

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RBGCONF_ENGINE_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
