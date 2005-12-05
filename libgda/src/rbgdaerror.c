
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

#include "rbgda.h"

/*
 * Class: Gda::Error
 * Management of errors.
 */
VALUE cGdaError;

#if defined(GDA_AT_LEAST_1_3)
  #define gda_error_get_description     gda_connection_event_get_description
  #define gda_error_set_description     gda_connection_event_set_description
  #define gda_error_get_source          gda_connection_event_get_source
  #define gda_error_set_source          gda_connection_event_set_source
  #define gda_error_get_sqlstate        gda_connection_event_get_sqlstate
  #define gda_error_set_sqlstate        gda_connection_event_set_sqlstate
#endif

/*
 * Method: description
 *
 * Returns: the description on this error.
 */
static VALUE rb_gda_error_get_description(self)
    VALUE self;
{
    return CSTR2RVAL(gda_error_get_description(RGDA_ERROR(self)));
}

/*
 * Method: source
 *
 * Returns: the name of the source (usually provider's name).
 */
static VALUE rb_gda_error_get_source(self)
    VALUE self;
{
    return CSTR2RVAL(gda_error_get_source(RGDA_ERROR(self)));
}

/*
 * Method: sqlstate
 *
 * Returns: the SQL state of this error.
 */
static VALUE rb_gda_error_get_sqlstate(self)
    VALUE self;
{
    return CSTR2RVAL(gda_error_get_sqlstate(RGDA_ERROR(self)));
}

/*
 * Method: set_description(description)
 * description: a textual description of the error.
 *
 * Sets the description of the error.
 *
 * Returns: self.
 */
static VALUE rb_gda_error_set_description(self, description)
    VALUE self, description;
{
    gda_error_set_description(RGDA_ERROR(self),
                              RVAL2CSTR(description));
    return self;
}

/*
 * Method: set_source(source)
 * source: the source of the error.
 *
 * Sets the source of the error.
 *
 * Returns: self.
 */
static VALUE rb_gda_error_set_source(self, source)
    VALUE self, source;
{
    gda_error_set_source(RGDA_ERROR(self),
                         RVAL2CSTR(source));
    return self;
}

/*
 * Method: set_sqlstate(sqlstate)
 * sqlstate: SQL state for the error.
 *
 * Sets the SQL state of the error.
 *
 * Returns: self.
 */
static VALUE rb_gda_error_set_sqlstate(self, sqlstate)
    VALUE self, sqlstate;
{
    gda_error_set_sqlstate(RGDA_ERROR(self),
                           RVAL2CSTR(sqlstate));
    return self;
}

#if defined(GDA_AT_LEAST_1_3)

#undef gda_error_get_description 
#undef gda_error_set_description
#undef gda_error_get_source
#undef gda_error_set_source
#undef gda_error_get_sqlstate 
#undef gda_error_set_sqlstate

/* TODO: document me */

static VALUE rb_gda_error_get_code(self)
    VALUE self;
{
    return INT2FIX(gda_connection_event_get_code(RGDA_ERROR(self)));
}

static VALUE rb_gda_error_set_code(self, code)
    VALUE self, code;
{
    gda_connection_event_set_code(RGDA_ERROR(self), FIX2INT(code));
    return self;
}

static VALUE rb_gda_error_get_gda_code(self)
    VALUE self;
{
    return GENUM2RVAL(gda_connection_event_get_gda_code(RGDA_ERROR(self)),
                      GDA_TYPE_CONNECTION_EVENT_CODE);
}

static VALUE rb_gda_error_set_gda_code(self, code)
    VALUE self, code;
{
    gda_connection_event_set_gda_code(RGDA_ERROR(self),
                                      RVAL2GENUM(code, GDA_TYPE_CONNECTION_EVENT_CODE));
    return self;
}

static VALUE rb_gda_error_get_event_type(self)
    VALUE self;
{
    return GENUM2RVAL(gda_connection_event_get_event_type(RGDA_ERROR(self)),
                      GDA_TYPE_CONNECTION_EVENT_TYPE);
}

static VALUE rb_gda_error_set_event_type(self, type)
    VALUE self, type;
{
    gda_connection_event_set_event_type(RGDA_ERROR(self),
                                        RVAL2GENUM(type, GDA_TYPE_CONNECTION_EVENT_TYPE));
    return self;
}

#else
/*
 * Method: set_number(number)
 * number: the number of the error.
 *
 * Sets the number of the error.
 *
 * Returns: self.
 */
static VALUE rb_gda_error_set_number(self, number)
    VALUE self, number;
{
    gda_error_set_number(RGDA_ERROR(self),
                         FIX2INT(number));
    return self;
}

/*
 * Method: number
 *
 * Returns: the number of this error.
 */

static VALUE rb_gda_error_get_number(self)
    VALUE self;
{
    return INT2FIX(gda_error_get_number(RGDA_ERROR(self)));
}
#endif

/*
 * Class method: new(description=nil, number=nil, source=nil, sqlstate=nil)
 *
 * Returns: a newly created Gda::Error object.
 */
static VALUE rb_gda_error_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
#if defined(GDA_AT_LEAST_1_3)
    GdaConnectionEvent *event;
    VALUE event_type;

    rb_scan_args(argc, argv, "10", &event_type);

    event = gda_connection_event_new(RVAL2GENUM(event_type,
                                                GDA_TYPE_CONNECTION_EVENT_TYPE));
    
    if (event != NULL)
        G_INITIALIZE(self, event);
#else
    VALUE description, number, source, sqlstate;
    GdaError *error;

    rb_scan_args(argc, argv, "04", &description, &number, &source, &sqlstate);

    error = gda_error_new();
    if (error != NULL) {
        G_INITIALIZE(self, error);
    }

    if (!NIL_P(description)) {
        rb_gda_error_set_description(self, description);
    }
    if (!NIL_P(number)) {
        rb_gda_error_set_number(self, number);
    }
    if (!NIL_P(source)) {
        rb_gda_error_set_source(self, source);
    }
    if (!NIL_P(sqlstate)) {
        rb_gda_error_set_sqlstate(self, sqlstate);
    }
#endif
    
    return Qnil;
}

void Init_gda_error(void) {
#if GDA_AT_LEAST_1_3
    VALUE c = G_DEF_CLASS(GDA_TYPE_CONNECTION_EVENT, "ConnectionEvent", mGda);
#else
    VALUE c = G_DEF_CLASS(GDA_TYPE_ERROR, "Error", mGda);
#endif
    
    rb_define_method(c, "initialize", rb_gda_error_new, -1);

    rb_define_method(c, "description",     rb_gda_error_get_description, 0);
    rb_define_method(c, "set_description", rb_gda_error_set_description, 1);
    rb_define_method(c, "source",          rb_gda_error_get_source,      0);
    rb_define_method(c, "set_source",      rb_gda_error_set_source,      1);
    rb_define_method(c, "sqlstate",        rb_gda_error_get_sqlstate,    0);
    rb_define_method(c, "set_sqlstate",    rb_gda_error_set_sqlstate,    1);

#if GDA_AT_LEAST_1_3
    rb_define_method(c, "code",           rb_gda_error_get_code,        0);
    rb_define_method(c, "set_code",       rb_gda_error_set_code,        1);
    rb_define_method(c, "gda_code",       rb_gda_error_get_gda_code,    0);
    rb_define_method(c, "set_gda_code",   rb_gda_error_set_gda_code,    1);
    rb_define_method(c, "event_type",     rb_gda_error_get_event_type,  0);
    rb_define_method(c, "set_event_type", rb_gda_error_set_event_type,  1);
    
    G_DEF_CLASS(GDA_TYPE_CONNECTION_EVENT_TYPE, "Type", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CONNECTION_EVENT_TYPE, "GDA_CONNECTION_EVENT_");
    G_DEF_CLASS(GDA_TYPE_CONNECTION_EVENT_CODE, "Code", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CONNECTION_EVENT_CODE, "GDA_CONNECTION_EVENT_CODE_");
#else
    rb_define_method(c, "number",      rb_gda_error_get_number,      0);
    rb_define_method(c, "set_number",  rb_gda_error_set_number,      1);
#endif
    
    G_DEF_SETTERS(c);

    cGdaError = c;
}

