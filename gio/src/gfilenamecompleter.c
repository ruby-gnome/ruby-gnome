#include "gio2.h"

#define _SELF(value) \
        G_FILENAME_COMPLETER(RVAL2GOBJ(value))

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_filename_completer_new());

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE initial_text) \
        { \
                return type(g_filename_completer_##name(_SELF(self), \
                                                        RVAL2CSTR(initial_text))); \
        }

DEFGETMETHOD(get_completion_suffix, CSTR2RVAL_FREE)
DEFGETMETHOD(get_completions, str_vector_to_ary)

DECLARE(set_dirs_only)(VALUE self, VALUE dirs_only)
{
        g_filename_completer_set_dirs_only(_SELF(self), RVAL2CBOOL(dirs_only));

        return self;
}

void
Init_gfilenamecompleter(VALUE glib)
{
        VALUE filenamecompleter = G_DEF_INTERFACE(G_TYPE_FILENAME_COMPLETER,
                                                  "FilenameCompleter",
                                                  glib);

        DEF_METHOD(filenamecompleter, initialize, 0);
        DEF_METHOD(filenamecompleter, get_completion_suffix, 1);
        DEF_METHOD(filenamecompleter, get_completions, 1);
        DEF_SET_METHOD(filenamecompleter, dirs_only);
}
