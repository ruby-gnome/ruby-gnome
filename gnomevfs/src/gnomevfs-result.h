/*****************************************************************************
 *
 * gnomevfs-result.h: GnomeVFSResult wrapper.
 *
 * Copyright (C) 2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
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
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2003-07-24
 *
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef GNOMEVFS_RESULT_H
#define GNOMEVFS_RESULT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ******************************************************************/

/* Defines *******************************************************************/

#define GVFSRESULT2RVAL(res)	(gnomevfs_result_to_rval(res))
#define CHECK_RESULT(res, ret)	\
	((res == GNOME_VFS_OK) ? ret : GVFSRESULT2RVAL(res))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

VALUE gnomevfs_result_to_rval(GnomeVFSResult result);
void Init_gnomevfs_result(VALUE m_gvfs);

/* Global Variables **********************************************************/

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_RESULT_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
