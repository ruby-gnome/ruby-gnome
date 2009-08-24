#include "gio2.h"

void
value_to_gtimeval(VALUE value, GTimeVal *time)
{
        if (rb_respond_to(value, rb_intern("tv_sec"))) {
                time->tv_sec = NUM2LONG(rb_funcall(value, rb_intern("tv_sec"), 0));
                time->tv_usec = rb_respond_to(value, rb_intern("tv_usec")) ?
                        rb_funcall(value, rb_intern("tv_usec"), 0) :
                        0;
        } else if (rb_respond_to(value, rb_intern("to_ary"))) {
                /* TODO: How do we convert to ary? */
                VALUE ary = rb_funcall(value, rb_intern("to_ary"), 0);
                if (RARRAY_LEN(ary) < 1 || RARRAY_LEN(ary) > 2)
                        rb_raise(rb_eArgError, "Array of length 1 or 2 expected");
                time->tv_sec = RARRAY_PTR(ary)[0];
                time->tv_usec = RARRAY_LEN(ary) > 1 ? RARRAY_PTR(ary)[1] : 0;
        } else {
                time->tv_sec = NUM2LONG(value);
                time->tv_usec = 0;
        }
}

GList *
g_file_ary_to_glist(VALUE ary)
{
        GList *list = NULL;
        int i,
            n;

        if (NIL_P(ary))
                return list;

        Check_Type(ary, T_ARRAY);

        n = RARRAY_LEN(ary);

        for (i = 0; i < n; i++)
                RVAL2GFILE(RARRAY_PTR(ary)[i]);

        for (i = 0; i < n; i++)
                list = g_list_append(list, RVAL2GFILE(RARRAY_PTR(ary)[i]));

        return list;
}

int
string_ary_to_char_p_ary(VALUE ary, char ***strings_out)
{
        char **strings;
        int i,
            n;

        Check_Type(ary, T_ARRAY);

        n = RARRAY_LEN(ary);

        for (i = 0; i < n; i++)
                StringValue(RARRAY_PTR(ary)[i]);

        strings = g_new(char *, n);
        for (i = 0; i < n; i++)
                strings[i] = RVAL2CSTR(RARRAY_PTR(ary)[i]);

        *strings_out = strings;

        return n;
}

VALUE
str_vector_to_ary(gchar **vector)
{
        int i;
        VALUE ary = rb_ary_new();

        for (i = 0; vector[i] != NULL; i++)
                rb_ary_push(ary, CSTR2RVAL(vector[i]));

        g_strfreev(vector);

        return ary;
}

void
rbgio_async_ready_callback(UNUSED(GObject *source),
                           GAsyncResult *result,
                           gpointer data)
{
        static VALUE s_id_call;

        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        VALUE block = USE_BLOCK(data);

        if (!NIL_P(block))
                rb_funcall(block, s_id_call, 1, GOBJ2RVAL(result));
}

/* TODO: This is a bit of a hack, but I see no other way of doing it. */
/* TODO: Need to verify that LENGTH is always correct. */
VALUE
cstr_to_rval_own(char *string, gsize length)
{
        NEWOBJ(str, struct RString);
        OBJSETUP(str, rb_cString, T_STRING);

        str->ptr = string;
        str->len = length;
        str->aux.capa = length;

        return (VALUE)str;
}

VALUE
cstr_to_rval_own_frozen(const char *string, gsize length)
{
        VALUE value = cstr_to_rval_own((char *)string, length);

        rb_str_freeze(value);

        return value;
}

VALUE
rbgio_child_remove_and_return(VALUE parent, VALUE child)
{
        G_CHILD_REMOVE(parent, child);

        return child;
}

static VALUE codes2errors[G_IO_ERROR_TOO_MANY_OPEN_FILES];

void
Init_gioerror(VALUE glib)
{
        VALUE io = rb_define_module_under(glib, "IO");

        VALUE error = rb_define_class_under(io, "Error", rb_eRuntimeError);

        codes2errors[G_IO_ERROR_FAILED] =
                rb_define_class_under(io, "FailedError", error);
        codes2errors[G_IO_ERROR_NOT_FOUND] =
                rb_define_class_under(io, "NotFoundError", error);
        codes2errors[G_IO_ERROR_EXISTS] =
                rb_define_class_under(io, "ExistsError", error);
        codes2errors[G_IO_ERROR_IS_DIRECTORY] =
                rb_define_class_under(io, "IsDirectoryError", error);
        codes2errors[G_IO_ERROR_NOT_DIRECTORY] =
                rb_define_class_under(io, "NotDirectoryError", error);
        codes2errors[G_IO_ERROR_NOT_EMPTY] =
                rb_define_class_under(io, "NotEmptyError", error);
        codes2errors[G_IO_ERROR_NOT_REGULAR_FILE] =
                rb_define_class_under(io, "NotRegularFileError", error);
        codes2errors[G_IO_ERROR_NOT_SYMBOLIC_LINK] =
                rb_define_class_under(io, "NotSymbolicLinkError", error);
        codes2errors[G_IO_ERROR_NOT_MOUNTABLE_FILE] =
                rb_define_class_under(io, "NotMountableFileError", error);
        codes2errors[G_IO_ERROR_FILENAME_TOO_LONG] =
                rb_define_class_under(io, "FilenameTooLongError", error);
        codes2errors[G_IO_ERROR_INVALID_FILENAME] =
                rb_define_class_under(io, "InvalidFilenameError", error);
        codes2errors[G_IO_ERROR_TOO_MANY_LINKS] =
                rb_define_class_under(io, "TooManyLinksError", error);
        codes2errors[G_IO_ERROR_NO_SPACE] =
                rb_define_class_under(io, "NoSpaceError", error);
        codes2errors[G_IO_ERROR_INVALID_ARGUMENT] =
                rb_define_class_under(io, "InvalidArgumentError", error);
        codes2errors[G_IO_ERROR_PERMISSION_DENIED] =
                rb_define_class_under(io, "PermissionDeniedError", error);
        codes2errors[G_IO_ERROR_NOT_SUPPORTED] =
                rb_define_class_under(io, "NotSupportedError", error);
        codes2errors[G_IO_ERROR_NOT_MOUNTED] =
                rb_define_class_under(io, "NotMountedError", error);
        codes2errors[G_IO_ERROR_ALREADY_MOUNTED] =
                rb_define_class_under(io, "AlreadyMountedError", error);
        codes2errors[G_IO_ERROR_CLOSED] =
                rb_define_class_under(io, "ClosedError", error);
        codes2errors[G_IO_ERROR_CANCELLED] =
                rb_define_class_under(io, "CancelledError", error);
        codes2errors[G_IO_ERROR_PENDING] =
                rb_define_class_under(io, "PendingError", error);
        codes2errors[G_IO_ERROR_READ_ONLY] =
                rb_define_class_under(io, "ReadOnlyError", error);
        codes2errors[G_IO_ERROR_CANT_CREATE_BACKUP] =
                rb_define_class_under(io, "CantCreateBackupError", error);
        codes2errors[G_IO_ERROR_WRONG_ETAG] =
                rb_define_class_under(io, "WrongEtagError", error);
        codes2errors[G_IO_ERROR_TIMED_OUT] =
                rb_define_class_under(io, "TimedOutError", error);
        codes2errors[G_IO_ERROR_WOULD_RECURSE] =
                rb_define_class_under(io, "WouldRecurseError", error);
        codes2errors[G_IO_ERROR_BUSY] =
                rb_define_class_under(io, "BusyError", error);
        codes2errors[G_IO_ERROR_WOULD_BLOCK] =
                rb_define_class_under(io, "WouldBlockError", error);
        codes2errors[G_IO_ERROR_HOST_NOT_FOUND] =
                rb_define_class_under(io, "HostNotFoundError", error);
        codes2errors[G_IO_ERROR_WOULD_MERGE] =
                rb_define_class_under(io, "WouldMergeError", error);
        codes2errors[G_IO_ERROR_FAILED_HANDLED] =
                rb_define_class_under(io, "FailedHandledError", error);
        codes2errors[G_IO_ERROR_TOO_MANY_OPEN_FILES] =
                rb_define_class_under(io, "TooManyOpenFilesError", error);
}

void
rbgio_raise_io_error(GError *error)
{
        if (error->code < 0 ||
            (unsigned)error->code >= (sizeof(codes2errors) / sizeof(codes2errors[0])))
                rb_raise(rb_eNotImpError,
                         "GIOErrorEnum contains items not implemented: %d",
                         error->code);

        rb_raise(codes2errors[error->code], error->message);
}
