/*****************************************************************************
 *
 * rbgconf-util.c: Ruby GConf interface utility functions.
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
 * $Date: 2003/02/04 21:28:48 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

/*
 * convert a Ruby array to a GSList of GConfValues.
 */
static GSList *
rb_ary_to_gconfval_list(ary_val, list_type)
	VALUE ary_val;
	GConfValueType *list_type;
{
	GSList *list = NULL;
	GConfValueType type = GCONF_VALUE_INVALID;
	GConfValue *val;
	int i, n;

	/* loop over the array and convert to GConfValues and store in list */
	Check_Type(ary_val, T_ARRAY);
	n = RARRAY(ary_val)->len;
	for (i = 0; i < n; i++) {
		val = rb_value_to_gconf_value(rb_ary_entry(ary_val, i));

		/* uh, oh. not all elements were of the same type. */
		if (type != GCONF_VALUE_INVALID && type != val->type) {
			*list_type = GCONF_VALUE_INVALID;
			return NULL;
		}

		list = g_slist_append(list, val);
	}

	return list;
}

/*
 * convert a Ruby VALUE to a GConfValue.
 */
GConfValue *
rb_value_to_gconf_value(val)
	VALUE val;
{
	GConfValue *gval;

	switch (TYPE(val)) {
	case T_STRING:
		gval = gconf_value_new(GCONF_VALUE_STRING);
		gconf_value_set_string(gval, RVAL2CSTR(val));
		break;
	case T_FIXNUM:
	case T_BIGNUM:
		gval = gconf_value_new(GCONF_VALUE_INT);
		gconf_value_set_int(gval, NUM2INT(val));
		break;
	case T_FLOAT:
		gval = gconf_value_new(GCONF_VALUE_FLOAT);
		gconf_value_set_float(gval, NUM2DBL(val));
		break;
	case T_TRUE:
	case T_FALSE:
		gval = gconf_value_new(GCONF_VALUE_BOOL);
		gconf_value_set_bool(gval, RTEST(val));
		break;
	case T_ARRAY: {
		GConfValueType type;
		GSList *list;

		list = rb_ary_to_gconfval_list(val, &type);
		if (type != GCONF_VALUE_INVALID) {
			gval = gconf_value_new(GCONF_VALUE_LIST);
			gconf_value_set_list_type(gval, type);
			gconf_value_set_list_nocopy(gval, list);
		} else if (g_slist_length(list) == 2) {
			gval = gconf_value_new(GCONF_VALUE_PAIR);
			gconf_value_set_car_nocopy(gval,
				(GConfValue *)g_slist_nth_data(list, 0));
			gconf_value_set_car_nocopy(gval,
				(GConfValue *)g_slist_nth_data(list, 1));
			g_slist_free(list);
		} else {
			GSList *i;
			for (i = list; i != NULL; i = i->next) {
				gconf_value_free((GConfValue *)i->data);
			}
			g_slist_free(list);

			rb_raise(rb_eArgError,
				 "all elements must be of same type");
		}
		break;
	}
	default:
		if (RVAL2GTYPE(val) == GCONF_TYPE_SCHEMA) {
			gval = gconf_value_new(GCONF_VALUE_SCHEMA);
			gconf_value_set_schema_nocopy(gval,
						       RVAL2GCSCHEMA(val));
		} else {
			rb_raise(rb_eTypeError, "invalid GConfValue type");
		}
		break;
	}

	return gval;
}

/*
 * convert a list of GConfValues to a Ruby array.
 */
static VALUE
gconfval_list_to_rb_ary(list)
	GSList *list;
{
	VALUE ary;

	ary = rb_ary_new2(g_slist_length(list));
	while (list != NULL) {
		rb_ary_push(ary,
			gconf_value_to_rb_value((GConfValue *)list->data));
		list = list->next;
	}

	return ary;
}

/*
 * convert a pair of GConfValues to a Ruby array.
 */
static VALUE
gconfval_pair_to_rb_ary(car, cdr)
	GConfValue *car, *cdr;
{
	VALUE ary;

	ary = rb_ary_new2(2);
	rb_ary_push(ary, gconf_value_to_rb_value(car));
	rb_ary_push(ary, gconf_value_to_rb_value(cdr));

	return ary;
}

/*
 * convert a GConfValue to a Ruby VALUE.
 */
VALUE
gconf_value_to_rb_value(gval)
	GConfValue *gval;
{
	VALUE val = Qnil;

	if (gval == NULL) {
		return Qnil;
	}

	if (!GCONF_VALUE_TYPE_VALID(gval->type)) {
		rb_bug("Found a bogus GConfValueType `%d'; please report.",
		       gval->type);
	}

	switch (gval->type) {
	case GCONF_VALUE_STRING:
		val = CSTR2RVAL(gconf_value_get_string(gval));
		break;
	case GCONF_VALUE_INT:
		val = INT2FIX(gconf_value_get_int(gval));
		break;
	case GCONF_VALUE_FLOAT:
		val = rb_float_new(gconf_value_get_float(gval));
		break;
	case GCONF_VALUE_BOOL:
		val = CBOOL2RVAL(gconf_value_get_bool(gval));
		break;
	case GCONF_VALUE_SCHEMA:
		val = GCSCHEMA2RVAL(gconf_value_get_schema(gval));
		break;
	case GCONF_VALUE_LIST:
		val = gconfval_list_to_rb_ary(gconf_value_get_list(gval));
		break;
	case GCONF_VALUE_PAIR:
		val = gconfval_pair_to_rb_ary(gconf_value_get_car(gval),
					      gconf_value_get_cdr(gval));
		break;
	default:
		/* in case the bastards add yet another useless type */
		rb_notimplement();
		break;
	}

	return val;
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
