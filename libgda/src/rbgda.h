
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/Gda.
 *
 * Ruby/Gda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Gda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Gda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __RBGDA_H_
#define __RBGDA_H_

#include <ruby.h>
#include <libgda/libgda.h>

#include <rbglib.h>
#include <rbgobject.h>

/* Gda module */
extern VALUE mGda;

#define GDA_COMMAND(o)          ((GdaCommand *)o)
#define GDA_DATASOURCE(o)       ((GdaDataSourceInfo *)o)
#define GDA_PROVIDER(o)         ((GdaProviderInfo *)o)
#define GDA_ROW(o)              ((GdaRow *)o)
#define GDA_VALUE(o)            ((GdaValue *)o)

#define GDA_TYPE_COMMAND        (gda_command_get_type())
#define GDA_TYPE_DATASOURCE     (gda_datasource_get_type())
#define GDA_TYPE_PROVIDER       (gda_provider_get_type())
#define GDA_TYPE_ROW            (gda_row_get_type())
#define GDA_TYPE_VALUE          (gda_value_get_type_bis())

#define RGDA_CLIENT(o)          (GDA_CLIENT(RVAL2GOBJ(o)))
#define RGDA_COMMAND(o)         (GDA_COMMAND(RVAL2BOXED(o, GDA_TYPE_COMMAND)))
#define RGDA_CONNECTION(o)      (GDA_CONNECTION(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL(o)       (GDA_DATA_MODEL(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL_ARRAY(o) (GDA_DATA_MODEL_ARRAY(RVAL2GOBJ(o)))
#define RGDA_DATASOURCE(o)      (GDA_DATASOURCE(RVAL2BOXED(o, GDA_TYPE_DATASOURCE)))
#define RGDA_ERROR(o)           (GDA_ERROR(RVAL2GOBJ(o)))
#define RGDA_PROVIDER(o)        (GDA_PROVIDER(RVAL2BOXED(o, GDA_TYPE_PROVIDER)))
#define RGDA_ROW(o)             (GDA_ROW(RVAL2BOXED(o, GDA_TYPE_ROW)))
#define RGDA_TRANSACTION(o)     (GDA_TRANSACTION(RVAL2GOBJ(o)))
#define RGDA_VALUE(o)           (GDA_VALUE(RVAL2BOXED(o, GDA_TYPE_VALUE)))

#define RGDA_CLIENT_NEW(o)          (GOBJ2RVAL(GDA_CLIENT(o)))
#define RGDA_COMMAND_NEW(o)         (BOXED2RVAL(GDA_COMMAND(o), GDA_TYPE_COMMAND))
#define RGDA_CONNECTION_NEW(o)      (GOBJ2RVAL(GDA_CONNECTION(o)))
#define RGDA_DATAMODEL_ARRAY_NEW(o) (GOBJ2RVAL(GDA_DATA_MODEL_ARRAY(o)))
#define RGDA_DATAMODEL_NEW(o)       (GOBJ2RVAL(GDA_DATA_MODEL(o)))
#define RGDA_DATASOURCE_NEW(o)      (BOXED2RVAL(GDA_DATASOURCE(o), GDA_TYPE_DATASOURCE))
#define RGDA_ERROR_NEW(o)           (GOBJ2RVAL(GDA_ERROR(o)))
#define RGDA_PROVIDER_NEW(o)        (BOXED2RVAL(GDA_PROVIDER(o), GDA_TYPE_PROVIDER))
#define RGDA_ROW_NEW(o)             (BOXED2RVAL(GDA_ROW(o), GDA_TYPE_ROW))
#define RGDA_TRANSACTION_NEW(o)     (GOBJ2RVAL(GDA_TRANSACTION(o)))
#define RGDA_VALUE_NEW(o)           (BOXED2RVAL(GDA_VALUE(o), GDA_TYPE_VALUE))

GType gda_command_get_type();
GType gda_datasource_get_type();
GType gda_provider_get_type();
GType gda_row_get_type();
GType gda_value_get_type_bis();   /* gda_value_get_type() exists already :| */

/* rbgdaconnection.c */
VALUE rb_gda_connection_close(VALUE self);

#endif /* __RBGDA_H_ */

