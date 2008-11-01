/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbpopt.c,v 1.4 2007/07/13 16:07:30 ggc Exp $ */

/* Part of Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

union popt_data {
    int u_int;
    long u_long;
    float u_float;
    double u_double;
    char * u_string;
    struct poptOption *u_options;
};

static VALUE
poptoption_arginfo_to_value(option)
    struct poptOption *option;
{
    switch (option->argInfo & POPT_ARG_MASK) {
      case POPT_ARG_NONE:
        return option->arg ? CBOOL2RVAL(*((int *)option->arg)) : Qnil;
      case POPT_ARG_STRING:
        return (option->arg && *((char **)option->arg)) ? rb_str_new2(*((char **)option->arg)) : Qnil;
      case POPT_ARG_INT:
        return option->arg ? INT2NUM(*((int *)option->arg)) : Qnil;
      case POPT_ARG_LONG:
        return option->arg ? LONG2NUM(*((long *)option->arg)) : Qnil;
      case POPT_ARG_INCLUDE_TABLE:
        return rbgno_poptoption_raw_to_arary(option->arg);
      case POPT_ARG_CALLBACK:
        return Qnil;
      case POPT_ARG_INTL_DOMAIN:
        return rb_str_new2(option->arg);
      case POPT_ARG_VAL:
        return option->arg ? INT2NUM(*((int *)option->arg)) : Qnil;
      case POPT_ARG_FLOAT:
        return option->arg ? rb_float_new(*((float *)option->arg)) : Qnil;
      case POPT_ARG_DOUBLE:
        return option->arg ? rb_float_new(*((double *)option->arg)) : Qnil;
      default:
        rb_raise(rb_eRuntimeError, "Unknown arginfo %d", option->argInfo & POPT_ARG_MASK);
    }
}

VALUE
rbgno_poptoption_raw_to_arary(options)
    struct poptOption *options;
{
    VALUE obj;
    VALUE entry;
    int i;

    obj = rb_ary_new();
    for (i = 0;; i++) {
        if (options[i].longName == NULL
            && options[i].shortName == '\0'
            && options[i].argInfo == 0
            && options[i].arg == NULL)
            break;
        entry = rb_ary_new();
        rb_ary_push(entry, options[i].longName ? rb_str_new2(options[i].longName) : Qnil);
        rb_ary_push(entry, options[i].shortName ? rb_str_new(&(options[i].shortName), 1) : Qnil);
        rb_ary_push(entry, UINT2NUM(options[i].argInfo));
        rb_ary_push(entry, poptoption_arginfo_to_value(&(options[i])));
        rb_ary_push(entry, options[i].descrip ? rb_str_new2(options[i].descrip) : Qnil);
        rb_ary_push(entry, options[i].argDescrip ? rb_str_new2(options[i].argDescrip) : Qnil);
        rb_ary_push(obj, entry);
    }
    return obj;
}

void
rbgno_poptoption_arg_to_hash(hash, options)
    VALUE hash;
    struct poptOption *options;
{
    int i;

    for (i = 0;; i++) {
        if (options[i].longName == NULL
            && options[i].shortName == '\0'
            && options[i].argInfo == 0
            && options[i].arg == NULL)
            break;
        if ((options[i].argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
            rbgno_poptoption_arg_to_hash(hash, options[i].arg);
            continue;
        } else if (options[i].longName != NULL) {
            rb_hash_aset(hash, rb_str_new2(options[i].longName),
                         poptoption_arginfo_to_value(&(options[i])));
        }
    }
}

VALUE
rbgno_poptoption_array_to_obj(from)
    VALUE from;
{
    VALUE entry;
    VALUE obj;
    VALUE sub;
    struct poptOption *options;
    union popt_data *data;
    char *string;
    size_t sopt;
    size_t sdata;
    size_t sstring;
    int len;
    int i;

    Check_Type(from, T_ARRAY);
    len = RARRAY_LEN(from);
    sopt = sizeof(struct poptOption) * (len + 1);
    sdata = sizeof(union popt_data) * len;
    sstring = 0;
    for (i = 0; i < len; i++) {
        entry = RARRAY_PTR(from)[i];
        Check_Type(entry, T_ARRAY);

        if (RARRAY_LEN(entry) < 4 || 6 < RARRAY_LEN(entry)) {
            rb_raise(rb_eArgError,
                     "wrong # of popt option (%ld for 4 - 6)",
                     RARRAY_LEN(entry));
        }
        sstring += strlen(RVAL2CSTR(RARRAY_PTR(entry)[0])) + 1;
        switch (NUM2INT(RARRAY_PTR(entry)[2])) {
          case POPT_ARG_STRING:
            sstring += strlen(RVAL2CSTR(RARRAY_PTR(entry)[3])) + 1;
        }
        if (RARRAY_LEN(entry) > 4 && !NIL_P(RARRAY_PTR(entry)[4])) {
            sstring += strlen(RVAL2CSTR(RARRAY_PTR(entry)[4])) + 1;
        }
        if (RARRAY_LEN(entry) > 5 && !NIL_P(RARRAY_PTR(entry)[5])) {
            sstring += strlen(RVAL2CSTR(RARRAY_PTR(entry)[5])) + 1;
        }
    }

    options = (struct poptOption *)g_malloc(sopt + sdata + sstring);
    data = (union popt_data *)(((char *)options) + sopt);
    string = (char *)(((char *)options) + sopt + sdata);
    obj = Data_Wrap_Struct(rb_cData, NULL, g_free, options);

    for (i = 0; i < len; i++) {
        entry = RARRAY_PTR(from)[i];

        /* set longName */
        strcpy(string, RVAL2CSTR(RARRAY_PTR(entry)[0]));
        options[i].longName = string;
        string += strlen(string) + 1;

        /* set shortName */
        if (NIL_P(RARRAY_PTR(entry)[1])) {
            options[i].shortName = '\0';
        } else {
            options[i].shortName = *(RVAL2CSTR(RARRAY_PTR(entry)[1]));
        }

        /* set argInfo */
        options[i].argInfo = NUM2INT(RARRAY_PTR(entry)[2]);

        /* set arg */
        switch (options[i].argInfo & POPT_ARG_MASK) {
          case POPT_ARG_NONE:
            data[i].u_int = RVAL2CBOOL(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_STRING:
            strcpy(string, RVAL2CSTR(RARRAY_PTR(entry)[3]));
            data[i].u_string = string;
            string += strlen(string) + 1;
            break;
          case POPT_ARG_INT:
            data[i].u_int = NUM2INT(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_LONG:
            data[i].u_long = NUM2LONG(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_VAL:
            data[i].u_int = NUM2INT(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_FLOAT:
            data[i].u_float = NUM2INT(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_DOUBLE:
            data[i].u_double = NUM2INT(RARRAY_PTR(entry)[3]);
            break;
          case POPT_ARG_INCLUDE_TABLE:
            sub = rbgno_poptoption_array_to_obj(RARRAY_PTR(entry)[3]);
            data[i].u_options = DATA_PTR(sub);
            G_RELATIVE(obj, sub);
            break;
          case POPT_ARG_CALLBACK:
          case POPT_ARG_INTL_DOMAIN:
            rb_raise(rb_eRuntimeError, "Unknown arginfo %d", options[i].argInfo & POPT_ARG_MASK);
            break;
          default:
            rb_raise(rb_eRuntimeError, "Unknown arginfo %d", options[i].argInfo & POPT_ARG_MASK);

            break;
        }
        options[i].arg = &(data[i]);

        /* set val */
        options[i].val = 0;

        /* set descrip */
        if (RARRAY_LEN(entry) > 4 && !NIL_P(RARRAY_PTR(entry)[4])) {
            strcpy(string, RVAL2CSTR(RARRAY_PTR(entry)[4]));
            options[i].descrip = string;
            string += strlen(string) + 1;
        } else {
            options[i].descrip = NULL;
        }

        /* set argDescrip */
        if (RARRAY_LEN(entry) > 5 && !NIL_P(RARRAY_PTR(entry)[5])) {
            strcpy(string, RVAL2CSTR(RARRAY_PTR(entry)[5]));
            options[i].argDescrip = string;
            string += strlen(string) + 1;
        } else {
            options[i].argDescrip = NULL;
        }
    }

    options[i].longName = NULL;
    options[i].shortName = '\0';
    options[i].argInfo = 0;
    options[i].arg = NULL;
    options[i].val = 0;

    return obj;
}

void
Init_gnome_popt(mGnome)
    VALUE mGnome;
{
    rb_define_const(mGnome, "POPT_ARG_NONE", INT2FIX(POPT_ARG_NONE));
    rb_define_const(mGnome, "POPT_ARG_STRING", INT2FIX(POPT_ARG_STRING));
    rb_define_const(mGnome, "POPT_ARG_INT", INT2FIX(POPT_ARG_INT));
    rb_define_const(mGnome, "POPT_ARG_LONG", INT2FIX(POPT_ARG_LONG));
    rb_define_const(mGnome, "POPT_ARG_INCLUDE_TABLE", INT2FIX(POPT_ARG_INCLUDE_TABLE));
    rb_define_const(mGnome, "POPT_ARG_CALLBACK", INT2FIX(POPT_ARG_CALLBACK));
    rb_define_const(mGnome, "POPT_ARG_INTL_DOMAIN", INT2FIX(POPT_ARG_INTL_DOMAIN));
    rb_define_const(mGnome, "POPT_ARG_FLOAT", INT2FIX(POPT_ARG_FLOAT));
    rb_define_const(mGnome, "POPT_ARG_DOUBLE", INT2FIX(POPT_ARG_DOUBLE));
    rb_define_const(mGnome, "POPT_ARG_MASK", INT2FIX(POPT_ARG_MASK));

    rb_define_const(mGnome, "POPT_ARGFLAG_DOC_HIDDEN", UINT2NUM(POPT_ARGFLAG_DOC_HIDDEN));
}
