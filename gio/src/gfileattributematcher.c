#include "gio2.h"

GType
g_file_attribute_matcher_get_type(void)
{
        static GType our_type = 0;
        if (our_type == 0)
                our_type = g_boxed_type_register_static("GFileAttributeMatcher",
                                                        (GBoxedCopyFunc)g_file_attribute_matcher_ref,
                                                        (GBoxedFreeFunc)g_file_attribute_matcher_unref);
        return our_type;
}

#define _SELF(value) \
        RVAL2GFILEATTRIBUTEMATCHER(value)

DECLARE(initialize)(VALUE self, VALUE attributes)
{
        G_INITIALIZE(self, g_file_attribute_matcher_new(RVAL2CSTR(attributes)));

        return Qnil;
}

#define DEFMATCHESMETHOD(name) \
        DECLARE(name)(VALUE self, VALUE attribute) \
        { \
                return CBOOL2RVAL(g_file_attribute_matcher_##name(_SELF(self), \
                                                                  RVAL2CSTR(attribute))); \
        }

DEFMATCHESMETHOD(matches)
DEFMATCHESMETHOD(matches_only)

DECLARE(enumerate_namespace)(VALUE self, VALUE ns)
{
        const char *match;
        gboolean matches_all = g_file_attribute_matcher_enumerate_namespace(_SELF(self),
                                                                            RVAL2CSTR(ns));
        if (!rb_block_given_p())
                return CBOOL2RVAL(matches_all);

        while ((match = g_file_attribute_matcher_enumerate_next(_SELF(self))) != NULL)
                rb_yield(CSTR2RVAL(match));

        return self;
}

DECLARE(enumerate_next)(VALUE self)
{
        return CSTR2RVAL(g_file_attribute_matcher_enumerate_next(_SELF(self)));
}

void
Init_gfileattributematcher(VALUE glib)
{
        /* TODO: Should be defined under G::FileAttribute as Matcher? */
        VALUE fileattributematcher = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_MATCHER,
                                                 "FileAttributeMatcher",
                                                 glib);

        DEF_METHOD(fileattributematcher, initialize, 1);
        DEF_BOOL_METHOD(fileattributematcher, matches, 1);
        /* TODO: Is this confusing when we have both #matches and
         * #matches_only?  What does #=~ call? */
        rb_define_alias(fileattributematcher, "=~", "matches?");
        DEF_BOOL_METHOD(fileattributematcher, matches_only, 1);
        DEF_METHOD(fileattributematcher, enumerate_namespace, 1);
        DEF_METHOD(fileattributematcher, enumerate_next, 0);
}
