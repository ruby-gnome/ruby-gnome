
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

#include "rbgda.h"

/*
 * Class: Gda::Value
 *
 * This class has been deprecated.
 *
 * You must use native Ruby classes like Fixnum/Bignum/Float,
 * String, TrueClass/FalseClass, NilClass, Array or Time instead.
 *
 * In addition, you can use Gda::GeometricPoint, Gda::Money and
 * Gda::Time classes.
 */

VALUE rb_gda_value_to_rb_value(value)
    GdaValue *value;
{
    gpointer binary;
    glong size;
    const GdaDate *dt;
    const GdaGeometricPoint *pt;
    const GdaValueList *ls;
    VALUE ary;
    const GdaMoney *mn;
    const GdaNumeric *gn;
    gint64 num;
    gdouble dnum;
    const GdaTime *gt;
    const GdaTimestamp *st;
    struct tm tm;

    switch (value->type)
    {
    case GDA_VALUE_TYPE_NULL:
        return Qnil;
        
    case GDA_VALUE_TYPE_BIGINT:
        return LL2NUM(gda_value_get_bigint(value));
        
    case GDA_VALUE_TYPE_BIGUINT:
        return ULL2NUM(gda_value_get_biguint(value));
        
    case GDA_VALUE_TYPE_BINARY:
        binary = gda_value_get_binary(value, &size);
        return rb_str_new(binary, size);
        
    case GDA_VALUE_TYPE_BLOB:
        /* TODO */
        return Qnil;
        
    case GDA_VALUE_TYPE_BOOLEAN:
        return CBOOL2RVAL(gda_value_get_boolean(value));
        
    case GDA_VALUE_TYPE_DATE:
        dt = gda_value_get_date(value);
        return rb_funcall(rb_path2class("Date"), rb_intern("new"), 3,
                  INT2FIX(dt->year),
                  INT2FIX(dt->month),
                  INT2FIX(dt->day));
                  
    case GDA_VALUE_TYPE_DOUBLE:
        return rb_float_new(gda_value_get_double(value));
        
    case GDA_VALUE_TYPE_GEOMETRIC_POINT:
        pt = gda_value_get_geometric_point(value);
        return rb_struct_new(cGdaGeometricPoint,
                     rb_float_new(pt->x),
                     rb_float_new(pt->y));
                     
    case GDA_VALUE_TYPE_GOBJECT:
        /* TODO: register unregistered GObjects with Ruby-GLib */
        return GOBJ2RVAL(G_OBJECT(gda_value_get_gobject(value)));
        
    case GDA_VALUE_TYPE_INTEGER:
        return INT2NUM(gda_value_get_integer(value));
        
    case GDA_VALUE_TYPE_LIST:
        ls = gda_value_get_list(value);
        ary = rb_ary_new2(ls == NULL ? 0 : g_list_length((GList *)ls));
        for ( ; ls != NULL; ls = ls->next) {
            rb_ary_push(ary,
                rb_gda_value_to_rb_value((GdaValue *)ls->data));
        }
        return ary;
        
    case GDA_VALUE_TYPE_MONEY:
        mn = gda_value_get_money(value);
        return rb_struct_new(cGdaMoney,
                     CSTR2RVAL(mn->currency),
                     INT2NUM(mn->amount));
                     
    case GDA_VALUE_TYPE_NUMERIC:
        /* XXX: is this sufficient? */
        gn = gda_value_get_numeric(value);
        if (gn->precision == 0) {
            return sscanf(gn->number, "%Ld", &num) == 1 ?
                LL2NUM(num) : CSTR2RVAL(gn->number);
        } else {
            return sscanf(gn->number, "%lf", &dnum) == 1 ?
                rb_float_new(dnum) : CSTR2RVAL(gn->number);
        }
        
    case GDA_VALUE_TYPE_SINGLE:
        return rb_float_new(gda_value_get_single(value));
        
    case GDA_VALUE_TYPE_SMALLINT:
        return FIX2INT(gda_value_get_smallint(value));
        
    case GDA_VALUE_TYPE_SMALLUINT:
        return UINT2NUM(gda_value_get_smalluint(value));
        
    case GDA_VALUE_TYPE_STRING:
        return CSTR2RVAL(gda_value_get_string(value));
        
    case GDA_VALUE_TYPE_TIME:
        gt = gda_value_get_time(value);
        return rb_struct_new(cGdaTime,
                     UINT2NUM(gt->hour),
                     UINT2NUM(gt->minute),
                     UINT2NUM(gt->second),
                     LONG2NUM(gt->timezone));
                     
    case GDA_VALUE_TYPE_TIMESTAMP:
        st = gda_value_get_timestamp(value);
        memset(&tm, 0, sizeof tm);
        tm.tm_year = st->year - 1900;
        tm.tm_mon = st->month - 1;
        tm.tm_mday = st->day;
        tm.tm_hour = st->hour;
        tm.tm_min = st->minute;
        tm.tm_sec = st->second;
        /* XXX: this is glibc special, perhaps make conversion here */
        tm.tm_gmtoff = st->timezone;
        return rb_time_new(mktime(&tm), st->fraction);
        
    case GDA_VALUE_TYPE_TINYINT:
        return FIX2INT(gda_value_get_tinyint(value));
        
    case GDA_VALUE_TYPE_TINYUINT:
        return UINT2NUM(gda_value_get_tinyuint(value));
        
    case GDA_VALUE_TYPE_TYPE:
        /* XXX: TODO */
        return Qnil;
        
    case GDA_VALUE_TYPE_UINTEGER:
        return UINT2NUM(gda_value_get_uinteger(value));
        
    case GDA_VALUE_TYPE_UNKNOWN:
        /* XXX: this needs better error message */
        rb_raise(rb_eTypeError, "Unknown column type");

    default:
        /* XXX: is this really an exception, perhaps better as
         * rb_notimplement */
        rb_raise(rb_eTypeError, "Unsupported GdaValueType (%d)",
             value->type);
    }
}

static GdaValue *rb_rb_value_to_gda_value_try_as_string(value, type, message)
    VALUE value;
    GdaValueType type;
    const char *message;
{
    GdaValue *gv;
    
    gv = gda_value_new_from_string(RVAL2CSTR(value), type);
    if (gv == NULL) {
        rb_raise(rb_eArgError, message);
    }

    return gv;
}

GdaValue *rb_rb_value_to_gda_value(value, type)
    VALUE value;
    GdaValueType type;
{
    GList *list;
    int i;
    char *str;
    GdaDate dt;
    GdaGeometricPoint pt;
    GdaMoney mn;
    VALUE v;
    GdaTime gt;
    GdaTimestamp ts;

    str = NULL;

    if (type == GDA_VALUE_TYPE_NULL) {
        switch (TYPE(value)) {
        case T_NIL:
            /* already typed as NULL */
            break;

        case T_TRUE:
        case T_FALSE:
            type = GDA_VALUE_TYPE_BOOLEAN;
            break;

        case T_STRING:
            type = GDA_VALUE_TYPE_STRING;
            break;

        case T_ARRAY:
            type = GDA_VALUE_TYPE_LIST;
            break;

        case T_FIXNUM:
            type = GDA_VALUE_TYPE_INTEGER;
            break;

        case T_FLOAT:
            type = GDA_VALUE_TYPE_DOUBLE;
            break;

        default:
            if (CLASS_OF(value) == rb_cTime) {
                type = GDA_VALUE_TYPE_TIMESTAMP; 
            } else if (CLASS_OF(value) == cGdaGeometricPoint) {
                type = GDA_VALUE_TYPE_GEOMETRIC_POINT;
            } else if (CLASS_OF(value) == cGdaMoney) {
                type = GDA_VALUE_TYPE_MONEY;
            } else if (CLASS_OF(value) == cGdaTime) {
                type = GDA_VALUE_TYPE_TIME;
            } else {
                /* XXX: fix this error */
                rb_raise(rb_eArgError,
                         "cannot auto-convert value without type information");
            }
        }
    }

    /*
     * XXX: we can extend this by allowing many different types here, more than
     * just the exact conversion, example is TYPE_TIME, which could be
     * Gda::Time or String, right?
     */
    switch (type)
    {
    case GDA_VALUE_TYPE_NULL:
        return gda_value_new_null();

    case GDA_VALUE_TYPE_BIGINT:
        return gda_value_new_bigint(NUM2LL(value));

    case GDA_VALUE_TYPE_BIGUINT:
        return gda_value_new_biguint(rb_num2ull(value));

    case GDA_VALUE_TYPE_BINARY:
        if (TYPE(value) == T_STRING) {
            return gda_value_new_binary(RSTRING(str)->ptr, RSTRING(str)->len);
        } else {
            str = RVAL2CSTR(value);
            return gda_value_new_binary(str, strlen(str));
        }

    case GDA_VALUE_TYPE_BLOB:
        /* TODO */
        return NULL;

    case GDA_VALUE_TYPE_BOOLEAN:
        return gda_value_new_boolean(RVAL2CBOOL(value));

    case GDA_VALUE_TYPE_DATE:
        if (CLASS_OF(value) == rb_path2class("Date")) {
            dt.year = NUM2INT(rb_funcall(value, rb_intern("year"), 0));
            dt.month = NUM2INT(rb_funcall(value, rb_intern("month"), 0));
            dt.day = NUM2INT(rb_funcall(value, rb_intern("day"), 0));
            return gda_value_new_date(&dt);
        } else {
            /* XXX: better error message needed */
            return rb_rb_value_to_gda_value_try_as_string(value, type,
                                                    "Date instance expected");
        }

    case GDA_VALUE_TYPE_DOUBLE:
        return gda_value_new_double(NUM2DBL(value));
        
    case GDA_VALUE_TYPE_GEOMETRIC_POINT:
        if (CLASS_OF(value) == cGdaGeometricPoint) {
            pt.x = NUM2DBL(rb_struct_getmember(value, rb_intern("x")));
            pt.y = NUM2DBL(rb_struct_getmember(value, rb_intern("y")));
            return gda_value_new_geometric_point(&pt);
        } else {
            return rb_rb_value_to_gda_value_try_as_string(value, type,
                                    "Gda::GeometricPoint instance expected");
        }

    case GDA_VALUE_TYPE_GOBJECT:
        return gda_value_new_gobject(RVAL2GOBJ(value));
        
    case GDA_VALUE_TYPE_INTEGER:
        return gda_value_new_integer(NUM2INT(value));
        
    case GDA_VALUE_TYPE_LIST:
        list = NULL;
        /* XXX: is this more safe for multi-threading perhaps? */
        for (i = 0; i < RARRAY(value)->len; i++) {
            /* XXX: type here must be a list then... */
		list = g_list_append(list,
				     rb_rb_value_to_gda_value(
						rb_ary_entry(value, i),
						GDA_VALUE_TYPE_UNKNOWN));
        }
        return gda_value_new_list((GdaValueList *)list);
        
    case GDA_VALUE_TYPE_MONEY:
        if (CLASS_OF(value) == cGdaMoney) {
	    v = rb_struct_getmember(value, rb_intern("currency"));
            mn.currency = RVAL2CSTR(v);
            mn.amount = NUM2DBL(rb_struct_getmember(value,
                                                    rb_intern("amount")));
            return gda_value_new_money(&mn);
        } else {
            return rb_rb_value_to_gda_value_try_as_string(value, type,
                                            "Gda::Money instance expected");
        }
                    
    case GDA_VALUE_TYPE_NUMERIC:
        return rb_rb_value_to_gda_value_try_as_string(value, type,
                            "don't know how to treat GDA_VALUE_TYPE_NUMERIC");
        
    case GDA_VALUE_TYPE_SINGLE:
        return gda_value_new_single(NUM2DBL(value));
        
    case GDA_VALUE_TYPE_SMALLINT:
        return gda_value_new_smallint(NUM2INT(value));
        
    case GDA_VALUE_TYPE_SMALLUINT:
        return gda_value_new_smalluint(NUM2UINT(value));
        
    case GDA_VALUE_TYPE_STRING:
        return gda_value_new_string(RVAL2CSTR(value));
        
    case GDA_VALUE_TYPE_TIME:
        if (CLASS_OF(value) == cGdaTime) {
            gt.hour = NUM2UINT(rb_struct_getmember(value,
                                                    rb_intern("hour")));
            gt.minute = NUM2UINT(rb_struct_getmember(value,
                                                        rb_intern("minute")));
            gt.second = NUM2UINT(rb_struct_getmember(value,
                                                        rb_intern("second")));
            gt.timezone = NUM2LONG(rb_struct_getmember(value,
                                                rb_intern("timezone")));
            return gda_value_new_time(&gt);
        } else {
            return rb_rb_value_to_gda_value_try_as_string(value, type,
                                                "Gda::Time instance expected");
        }
                    
    case GDA_VALUE_TYPE_TIMESTAMP:
        if (FIXNUM_P(value) || TYPE(value) == T_BIGNUM) {
            return gda_value_new_timestamp_from_timet(NUM2DBL(value));
        } else if (CLASS_OF(value) == rb_cTime) {
            ts.year = NUM2INT(rb_funcall(value, rb_intern("year"), 0));
            ts.month = NUM2INT(rb_funcall(value, rb_intern("month"), 0));
            ts.day = NUM2INT(rb_funcall(value, rb_intern("day"), 0));
            ts.hour = NUM2INT(rb_funcall(value, rb_intern("hour"), 0));
            ts.minute = NUM2INT(rb_funcall(value, rb_intern("min"), 0));
            ts.second = NUM2INT(rb_funcall(value, rb_intern("sec"), 0));
            ts.fraction = NUM2ULONG(rb_funcall(value, rb_intern("usec"), 0));
            ts.timezone = NUM2LONG(rb_funcall(value, rb_intern("gmtoff"), 0));
            return gda_value_new_timestamp(&ts);
        } else {
            return rb_rb_value_to_gda_value_try_as_string(value,
                                "cannot convert value to timestamp");
        }
        
    case GDA_VALUE_TYPE_TINYINT:
        return gda_value_new_tinyint(NUM2INT(value));
        
    case GDA_VALUE_TYPE_TINYUINT:
        return gda_value_new_tinyint(NUM2UINT(value));
        
    case GDA_VALUE_TYPE_TYPE:
        /* TODO */
        return NULL;
        
    case GDA_VALUE_TYPE_UINTEGER:
        return gda_value_new_uinteger(NUM2UINT(value));
        
    case GDA_VALUE_TYPE_UNKNOWN:
    default:
        /* XXX: is this really an exception, perhaps better as
        * rb_notimplement */
        rb_raise(rb_eTypeError, "Unsupported GdaValueType (%d)", type);
    }

    return NULL;
}
