#include "gio2.h"

#define _SELF(value) \
        RVAL2GFILEINFO(value)

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_file_info_new());

        return Qnil;
}

DECLARE(dup)(VALUE self)
{
        return GOBJ2RVAL(g_file_info_dup(_SELF(self)));
}

/* TODO: Does this make sense from the Ruby side? */
DECLARE(copy_into)(VALUE self, VALUE other)
{
        g_file_info_copy_into(_SELF(self), _SELF(other));

        return self;
}

#define DEFATTRMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE attribute) \
        { return type(g_file_info_##name(_SELF(self), RVAL2CSTR(attribute))); }

DEFATTRMETHOD(has_attribute, CBOOL2RVAL)

DECLARE_VARG(attributes)
{
        const char *name_space;

        SCAN_1_ARG("01",
                   name_space, RVAL2FILEINFONAMESPACEDEFAULT);

        return str_vector_to_ary(g_file_info_list_attributes(_SELF(self),
                                                             name_space));
}

DEFATTRMETHOD(get_attribute_type, GFILEATTRIBUTETYPE2RVAL)

DECLARE(remove_attribute)(VALUE self, VALUE attribute)
{
        g_file_info_remove_attribute(_SELF(self), RVAL2CSTR(attribute));

        return self;
}

DEFATTRMETHOD(get_attribute_as_string, CSTR2RVAL2)

static VALUE
file_attribute_to_value(GFileAttributeType type, gpointer value)
{
        switch (type) {
        case G_FILE_ATTRIBUTE_TYPE_STRING:
        case G_FILE_ATTRIBUTE_TYPE_BYTE_STRING:
                return CSTR2RVAL(value);
        case G_FILE_ATTRIBUTE_TYPE_BOOLEAN:
                return CBOOL2RVAL(*(gboolean *)value);
        case G_FILE_ATTRIBUTE_TYPE_UINT32:
                return UINT2NUM(*(guint32 *)value);
        case G_FILE_ATTRIBUTE_TYPE_INT32:
                return INT2NUM(*(gint32 *)value);
        case G_FILE_ATTRIBUTE_TYPE_UINT64:
                return rbglib_uint64_to_num(*(guint64 *)value);
        case G_FILE_ATTRIBUTE_TYPE_INT64:
                return rbglib_int64_to_num(*(gint64 *)value);
        case G_FILE_ATTRIBUTE_TYPE_OBJECT:
                return GOBJ2RVAL((GObject *)value);
        case G_FILE_ATTRIBUTE_TYPE_INVALID:
                rb_raise(rb_eRuntimeError, "Invalid file attribute type"); /* TODO: What class should we use? */
                break;
        default:
                rb_raise(rb_eRuntimeError, "Unknown file attribute type"); /* TODO: What class should we use? */
                break;
        }
}

DECLARE(get_attribute_data)(VALUE self, VALUE attribute)
{
        GFileAttributeType type;
        gpointer value;
        GFileAttributeStatus status;

        if (!g_file_info_get_attribute_data(_SELF(self),
                                            RVAL2CSTR(attribute),
                                            &type,
                                            &value,
                                            &status))
                return Qnil;

        return rb_ary_new3(3,
                           GFILEATTRIBUTETYPE2RVAL(type),
                           file_attribute_to_value(type, value),
                           GFILEATTRIBUTESTATUS2RVAL(status));
}

DEFATTRMETHOD(get_attribute_status, GFILEATTRIBUTESTATUS2RVAL)
DEFATTRMETHOD(get_attribute_string, CSTR2RVAL)
DEFATTRMETHOD(get_attribute_byte_string, CSTR2RVAL)
DEFATTRMETHOD(get_attribute_boolean, CBOOL2RVAL)
DEFATTRMETHOD(get_attribute_uint32, UINT2NUM)
DEFATTRMETHOD(get_attribute_int32, INT2NUM)
DEFATTRMETHOD(get_attribute_uint64, rbglib_uint64_to_num)
DEFATTRMETHOD(get_attribute_int64, rbglib_int64_to_num)
DEFATTRMETHOD(get_attribute_object, GOBJ2RVAL)

/* TODO: We can implement this in Ruby as
 * def get_attribute_value(attribute)
 *   get_attribute_data[1]
 * end */
DECLARE(get_attribute_value)(VALUE self, VALUE attribute)
{
        GFileAttributeType type;
        gpointer value;
        GFileAttributeStatus status;

        if (!g_file_info_get_attribute_data(_SELF(self),
                                            RVAL2CSTR(attribute),
                                            &type,
                                            &value,
                                            &status))
                return Qnil;

        return file_attribute_to_value(type, value);
}


/* TODO: Do we need set_attribute? */

#define DEFSETATTRMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE attribute, VALUE value) \
        { \
                g_file_info_##name(_SELF(self), \
                                   RVAL2CSTR(attribute), \
                                   type(value)); \
                return self; \
        }

DEFSETATTRMETHOD(set_attribute_string, RVAL2CSTR)
DEFSETATTRMETHOD(set_attribute_byte_string, RVAL2CSTR)
DEFSETATTRMETHOD(set_attribute_boolean, RVAL2CBOOL)
DEFSETATTRMETHOD(set_attribute_uint32, NUM2UINT)
DEFSETATTRMETHOD(set_attribute_int32, NUM2INT)
DEFSETATTRMETHOD(set_attribute_uint64, rbglib_num_to_uint64)
DEFSETATTRMETHOD(set_attribute_int64, rbglib_num_to_int64)
DEFSETATTRMETHOD(set_attribute_object, RVAL2GOBJ)

/* TODO: How do we implement set_attribute_value? */

#define DEFSIMPLEMETHOD(name) \
        DECLARE(name)(VALUE self) \
        { \
                g_file_info_##name(_SELF(self)); \
                return self; \
        }

DEFSIMPLEMETHOD(clear_status)

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_file_info_get_##name(_SELF(self))); }

DEFGETMETHOD(file_type, GFILETYPE2RVAL)
DEFGETMETHOD(is_hidden, CBOOL2RVAL)
DEFGETMETHOD(is_backup, CBOOL2RVAL)
DEFGETMETHOD(is_symlink, CBOOL2RVAL)
DEFGETMETHOD(name, CSTR2RVAL)
DEFGETMETHOD(display_name, CSTR2RVAL)
DEFGETMETHOD(edit_name, CSTR2RVAL)
DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(content_type, CSTR2RVAL)
DEFGETMETHOD(size, rbglib_int64_to_num)

DECLARE(modification_time)(VALUE self)
{
        GTimeVal time;
        g_file_info_get_modification_time(_SELF(self), &time);
        /* TODO: Perhaps GTIMEVAL2RVAL should take pointer? */
        return GTIMEVAL2RVAL(time);
}

DEFGETMETHOD(symlink_target, CSTR2RVAL)
DEFGETMETHOD(etag, CSTR2RVAL)
DEFGETMETHOD(sort_order, INT2NUM);

/* TODO: Is this the best way to do it?  Having set/unset pairs seems good, but
 * perhaps enough with having #attribute_mask = nil for unset? */
DECLARE(set_attribute_mask)(VALUE self, VALUE rbmask)
{
        gboolean should_unref;
        GFileAttributeMatcher *mask;

        if (NIL_P(rbmask)) {
                g_file_info_unset_attribute_mask(_SELF(self));
                return self;
        }

        if (TYPE(rbmask) == T_STRING) {
                mask = g_file_attribute_matcher_new(RVAL2CSTR(rbmask));
                should_unref = TRUE;
        } else {
                mask = RVAL2GFILEATTRIBUTEMATCHER(rbmask);
                should_unref = FALSE;
        }

        g_file_info_set_attribute_mask(_SELF(self), mask);

        if (should_unref)
                g_file_attribute_matcher_unref(mask);

        return self;
}

DEFSIMPLEMETHOD(unset_attribute_mask)

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_file_info_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFSETMETHOD(set_file_type, RVAL2GFILETYPE)
DEFSETMETHOD(set_is_hidden, RVAL2CBOOL)
DEFSETMETHOD(set_is_symlink, RVAL2CBOOL)
DEFSETMETHOD(set_name, RVAL2CSTR)
DEFSETMETHOD(set_display_name, RVAL2CSTR)
DEFSETMETHOD(set_edit_name, RVAL2CSTR)
DEFSETMETHOD(set_icon, RVAL2GICON)
DEFSETMETHOD(set_content_type, RVAL2CSTR)
DEFSETMETHOD(set_size, rbglib_num_to_int64)

DECLARE(set_modification_time)(VALUE self, VALUE rbtime)
{
        GTimeVal time;

        value_to_gtimeval(rbtime, &time);

        g_file_info_set_modification_time(_SELF(self), &time);

        return self;
}

DEFSETMETHOD(set_symlink_target, RVAL2CSTR)
DEFSETMETHOD(set_sort_order, NUM2INT)

void
Init_gfileinfo(VALUE glib)
{
        /* Should this be G::FileInfo or G::File::Info.  It’s File::Stat, after
         * all. */
        VALUE fileinfo = G_DEF_CLASS(G_TYPE_FILE_INFO, "FileInfo", glib);

        DEF_METHOD(fileinfo, initialize, 0);
        DEF_METHOD(fileinfo, dup, 0);
        DEF_METHOD(fileinfo, copy_into, 1);
        DEF_BOOL_METHOD(fileinfo, has_attribute, 1);
        DEF_METHOD(fileinfo, attributes, -1);
        DEF_METHOD(fileinfo, get_attribute_type, 1);
        DEF_METHOD(fileinfo, remove_attribute, 1);
        DEF_METHOD(fileinfo, get_attribute_as_string, 1);
        DEF_METHOD(fileinfo, get_attribute_data, 1);
        DEF_METHOD(fileinfo, get_attribute_status, 1);
        DEF_METHOD(fileinfo, get_attribute_string, 1);
        DEF_METHOD(fileinfo, get_attribute_byte_string, 1);
        DEF_METHOD(fileinfo, get_attribute_boolean, 1);
        DEF_METHOD(fileinfo, get_attribute_uint32, 1);
        DEF_METHOD(fileinfo, get_attribute_int32, 1);
        DEF_METHOD(fileinfo, get_attribute_uint64, 1);
        DEF_METHOD(fileinfo, get_attribute_int64, 1);
        DEF_METHOD(fileinfo, get_attribute_object, 1);
        DEF_METHOD(fileinfo, get_attribute_value, 1);
        /* TODO: Alias #[] to #get_attribute_value? */
        DEF_METHOD(fileinfo, set_attribute_string, 1);
        DEF_METHOD(fileinfo, set_attribute_byte_string, 1);
        DEF_METHOD(fileinfo, set_attribute_boolean, 1);
        DEF_METHOD(fileinfo, set_attribute_uint32, 1);
        DEF_METHOD(fileinfo, set_attribute_int32, 1);
        DEF_METHOD(fileinfo, set_attribute_uint64, 1);
        DEF_METHOD(fileinfo, set_attribute_int64, 1);
        DEF_METHOD(fileinfo, set_attribute_object, 1);
        /* TODO:
        DEF_METHOD(fileinfo, set_attribute_value, 1);
        rb_alias("#[]=", #get_attribute_value);
        */
        DEF_METHOD(fileinfo, clear_status, 0);
        DEF_METHOD(fileinfo, file_type, 0);
        DEF_IS_METHOD(fileinfo, hidden, 0);
        DEF_IS_METHOD(fileinfo, backup, 0);
        DEF_IS_METHOD(fileinfo, symlink, 0);
        DEF_METHOD(fileinfo, name, 0);
        DEF_METHOD(fileinfo, display_name, 0);
        DEF_METHOD(fileinfo, edit_name, 0);
        DEF_METHOD(fileinfo, icon, 0);
        DEF_METHOD(fileinfo, content_type, 0);
        DEF_METHOD(fileinfo, size, 0);
        DEF_METHOD(fileinfo, modification_time, 0);
        DEF_METHOD(fileinfo, symlink_target, 0);
        DEF_METHOD(fileinfo, etag, 0);
        DEF_METHOD(fileinfo, sort_order, 0);
        DEF_METHOD(fileinfo, unset_attribute_mask, 0);
        DEF_SET_METHOD(fileinfo, attribute_mask);
        DEF_SET_METHOD(fileinfo, file_type);
        /* TODO: Is this the best way to name it? */
        DEF_METHOD(fileinfo, set_is_hidden, 1);
        rb_define_alias(fileinfo, "hidden=", "set_is_hidden");
        DEF_SET_METHOD(fileinfo, is_symlink);
        DEF_SET_METHOD(fileinfo, name);
        DEF_SET_METHOD(fileinfo, display_name);
        DEF_SET_METHOD(fileinfo, edit_name);
        DEF_SET_METHOD(fileinfo, icon);
        DEF_SET_METHOD(fileinfo, content_type);
        DEF_SET_METHOD(fileinfo, size);
        DEF_SET_METHOD(fileinfo, modification_time);
        DEF_SET_METHOD(fileinfo, symlink_target);
        DEF_SET_METHOD(fileinfo, sort_order);
}
