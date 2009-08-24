#include "gio2.h"

#define _SELF(value) \
        G_DATA_OUTPUT_STREAM(RVAL2GOBJ(value))

DECLARE_VARG(initialize)
{
        VALUE base_stream,
              byte_order;

        rb_scan_args(argc, argv, "11", &base_stream, &byte_order);

        G_INITIALIZE(self,
                     g_data_output_stream_new(RVAL2GOUTPUTSTREAM(base_stream)));

        if (!NIL_P(byte_order))
                g_data_output_stream_set_byte_order(_SELF(self),
                                                    RVAL2GDATASTREAMBYTEORDER(byte_order));

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_data_output_stream_get_##name(_SELF(self))); \
        }

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_data_output_stream_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFGETMETHOD(byte_order, GDATASTREAMBYTEORDER2RVAL)
DEFSETMETHOD(set_byte_order, RVAL2GDATASTREAMBYTEORDER)

#define DEFPUTMETHOD(name, c_type, type) \
        DECLARE_VARG(put_##name) \
        { \
                c_type value; \
                GCancellable *cancellable; \
                GError *error = NULL; \
                SCAN_2_ARGS("11", \
                            value, type, \
                            cancellable, RVAL2GCANCELLABLE); \
                if (!g_data_output_stream_put_##name(_SELF(self), \
                                                     value, \
                                                     cancellable, \
                                                     &error)) \
                        rbgio_raise_io_error(error); \
                return self; \
        }

DEFPUTMETHOD(byte, guchar, FIX2UINT)
DEFPUTMETHOD(int16, gint16, FIX2UINT)
DEFPUTMETHOD(uint16, guint16, FIX2UINT)
DEFPUTMETHOD(int32, gint32, NUM2UINT)
DEFPUTMETHOD(uint32, guint32, NUM2UINT)
DEFPUTMETHOD(int64, gint64, rbglib_num_to_int64)
DEFPUTMETHOD(uint64, guint64, rbglib_num_to_uint64)
DEFPUTMETHOD(string, const char *, RVAL2CSTR)

#if 0
static VALUE
put_value(int argc, VALUE *argv, VALUE self)
{
        VALUE value,
              cancellable;
        gboolean success;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);

        /* TODO: How do we convert from a Number to uint64 through byte in a
         * reliable manner?  Is it even sane to do so? */
        switch (TYPE(value)) {
        case T_STRING:
                success = g_data_output_stream_put_string(_SELF(self),
                                                          RVAL2CSTR(value),
                                                          cancellable,
                                                          &error);
                break;

        if (!success)
                rbgio_raise_io_error(error);

        return self;
}
#endif

void
Init_gdataoutputstream(VALUE glib)
{
        VALUE dataoutputstream = G_DEF_CLASS(G_TYPE_DATA_OUTPUT_STREAM,
                                             "DataOutputStream",
                                             glib);

        DEF_METHOD(dataoutputstream, initialize, -1);
        DEF_ATTRIBUTE(dataoutputstream, byte_order);
        DEF_METHOD(dataoutputstream, put_byte, -1);
        DEF_METHOD(dataoutputstream, put_int16, -1);
        DEF_METHOD(dataoutputstream, put_uint16, -1);
        DEF_METHOD(dataoutputstream, put_int32, -1);
        DEF_METHOD(dataoutputstream, put_uint32, -1);
        DEF_METHOD(dataoutputstream, put_int64, -1);
        DEF_METHOD(dataoutputstream, put_uint64, -1);
        DEF_METHOD(dataoutputstream, put_string, -1);
        rb_define_alias(dataoutputstream, "<<", "put_string");
        rb_define_alias(dataoutputstream, "write", "put_string");

        /* TODO: Should we add #put_value/#put? */
}
