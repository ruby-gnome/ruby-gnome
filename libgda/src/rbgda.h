
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
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

/* Gda classes */
extern VALUE cGdaClient;
extern VALUE cGdaCommand;
extern VALUE cGdaConnection;
extern VALUE cGdaDataModel;
extern VALUE cGdaDataModelArray;
extern VALUE cGdaDataModelHash;
extern VALUE cGdaDataModelList;
extern VALUE cGdaDataSource;
extern VALUE cGdaError;
extern VALUE cGdaExport;
extern VALUE cGdaFieldAttributes;
extern VALUE cGdaGeometricPoint;
extern VALUE cGdaMoney;
extern VALUE cGdaParameter;
extern VALUE cGdaParameterList;
extern VALUE cGdaProvider;
extern VALUE cGdaQuarkList;
extern VALUE cGdaRow;
extern VALUE cGdaSelect;
extern VALUE cGdaTable;
extern VALUE cGdaTime;
extern VALUE cGdaTransaction;

#define GDA_COMMAND(o)          ((GdaCommand *)o)
#define GDA_DATASOURCE(o)       ((GdaDataSourceInfo *)o)
#define GDA_FIELD_ATTRIBUTES(o) ((GdaFieldAttributes *)o)
#define GDA_PARAMETER(o)        ((GdaParameter *)o)
#define GDA_PARAMETER_LIST(o)   ((GdaParameterList *)o)
#define GDA_PROVIDER(o)         ((GdaProviderInfo *)o)
#define GDA_QUARK_LIST(o)       ((GdaQuarkList *)o)
#define GDA_ROW(o)              ((GdaRow *)o)
#define GDA_VALUE(o)            ((GdaValue *)o)

#define RGDA_CLIENT(o)           (GDA_CLIENT(RVAL2GOBJ(o)))
#define RGDA_COMMAND(o)          (GDA_COMMAND(RVAL2BOXED(o, GDA_TYPE_COMMAND)))
#define RGDA_CONNECTION(o)       (GDA_CONNECTION(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL(o)        (GDA_DATA_MODEL(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL_ARRAY(o)  (GDA_DATA_MODEL_ARRAY(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL_HASH(o)   (GDA_DATA_MODEL_HASH(RVAL2GOBJ(o)))
#define RGDA_DATAMODEL_LIST(o)   (GDA_DATA_MODEL_LIST(RVAL2GOBJ(o)))
#define RGDA_DATASOURCE(o)       (GDA_DATASOURCE(RVAL2BOXED(o, GDA_TYPE_DATA_SOURCE_INFO)))
#define RGDA_ERROR(o)            (GDA_ERROR(RVAL2GOBJ(o)))
#define RGDA_EXPORT(o)           (GDA_EXPORT(RVAL2GOBJ(o)))
#define RGDA_FIELD_ATTRIBUTES(o) (GDA_FIELD_ATTRIBUTES(RVAL2BOXED(o, GDA_TYPE_FIELD_ATTRIBUTES)))
#define RGDA_PARAMETER(o)        (GDA_PARAMETER(RVAL2BOXED(o, GDA_TYPE_PARAMETER)))
#define RGDA_PARAMETER_LIST(o)   (GDA_PARAMETER_LIST(RVAL2BOXED(o, GDA_TYPE_PARAMETER_LIST)))
#define RGDA_PROVIDER(o)         (GDA_PROVIDER(RVAL2BOXED(o, GDA_TYPE_PROVIDER_INFO)))
#define RGDA_QUARKLIST(o)        (GDA_QUARK_LIST(RVAL2BOXED(o, GDA_TYPE_QUARK_LIST)))
#define RGDA_ROW(o)              (GDA_ROW(RVAL2BOXED(o, GDA_TYPE_ROW)))
#define RGDA_SELECT(o)           (GDA_SELECT(RVAL2GOBJ(o)))
#define RGDA_TABLE(o)            (GDA_TABLE(RVAL2GOBJ(o)))
#define RGDA_TRANSACTION(o)      (GDA_TRANSACTION(RVAL2GOBJ(o)))
#define RGDA_VALUE(o,t)          (GDA_VALUE(rb_rb_value_to_gda_value(o, t)))

#define RGDA_CLIENT_NEW(o)           (GOBJ2RVAL(GDA_CLIENT(o)))
#define RGDA_COMMAND_NEW(o)          (BOXED2RVAL(GDA_COMMAND(o), GDA_TYPE_COMMAND))
#define RGDA_CONNECTION_NEW(o)       (GOBJ2RVAL(GDA_CONNECTION(o)))
#define RGDA_DATAMODEL_ARRAY_NEW(o)  (GOBJ2RVAL(GDA_DATA_MODEL_ARRAY(o)))
#define RGDA_DATAMODEL_HASH_NEW(o)   (GOBJ2RVAL(GDA_DATA_MODEL_HASH(o)))
#define RGDA_DATAMODEL_LIST_NEW(o)   (GOBJ2RVAL(GDA_DATA_MODEL_LIST(o)))
#define RGDA_DATAMODEL_NEW(o)        (GOBJ2RVAL(GDA_DATA_MODEL(o)))
#define RGDA_DATASOURCE_NEW(o)       (BOXED2RVAL(GDA_DATASOURCE(o), GDA_TYPE_DATA_SOURCE_INFO))
#define RGDA_ERROR_NEW(o)            (GOBJ2RVAL(GDA_ERROR(o)))
#define RGDA_EXPORT_NEW(o)           (GOBJ2RVAL(GDA_EXPORT(o)))
#define RGDA_FIELD_ATTRIBUTES_NEW(o) (BOXED2RVAL(GDA_FIELD_ATTRIBUTES(o), GDA_TYPE_FIELD_ATTRIBUTES))
#define RGDA_PARAMETER_NEW(o)        (BOXED2RVAL(GDA_PARAMETER(o), GDA_TYPE_PARAMETER))
#define RGDA_PARAMETER_LIST_NEW(o)   (BOXED2RVAL(GDA_PARAMETER_LIST(o), GDA_TYPE_PARAMETER_LIST))
#define RGDA_PROVIDER_NEW(o)         (BOXED2RVAL(GDA_PROVIDER(o), GDA_TYPE_PROVIDER_INFO))
#define RGDA_QUARKLIST_NEW(o)        (BOXED2RVAL(GDA_QUARK_LIST(o), GDA_TYPE_QUARK_LIST))
#define RGDA_ROW_NEW(o)              (BOXED2RVAL(GDA_ROW(o), GDA_TYPE_ROW))
#define RGDA_SELECT_NEW(o)           (GOBJ2RVAL(GDA_SELECT(o)))
#define RGDA_TABLE_NEW(o)            (GOBJ2RVAL(GDA_TABLE(o)))
#define RGDA_TRANSACTION_NEW(o)      (GOBJ2RVAL(GDA_TRANSACTION(o)))
#define RGDA_VALUE_NEW(o)            (rb_gda_value_to_rb_value(GDA_VALUE(o)))

/* rbgdaconnection.c */
VALUE rb_gda_connection_close(VALUE self);

/* rbgdavalue.c */
VALUE rb_gda_value_to_rb_value(GdaValue *value);
GdaValue *rb_rb_value_to_gda_value(VALUE value, GdaValueType type);

#endif /* __RBGDA_H_ */

