/*****************************************************************************
 *
 * gnomevfs.h: GnomeVFS interface
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
 * Latest Revision: 2003-07-26
 *
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef GNOMEVFS_H
#define GNOMEVFS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ******************************************************************/

#include <glib.h>
#include <ruby.h>
#include <libgnomevfs/gnome-vfs.h>

#include <rbglib.h>
#include <rbgobject.h>

#include "gnomevfs-result.h"
#include "gnomevfs-uri.h"
#include "gnomevfs-fileinfo.h"
#include "gnomevfs-directory.h"
#include "gnomevfs-file.h"
#include "gnomevfs-monitor.h"

/* Defines *******************************************************************/

#ifndef ULL2NUM
# define ULL2NUM ULONG2NUM
#endif

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

VALUE g_id_call;

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
