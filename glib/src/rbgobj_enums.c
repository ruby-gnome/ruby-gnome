/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_enums.c -

  $Author: ktou $
  $Date: 2007/08/08 11:53:09 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2004-2006  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  Masahiro Sakai
**********************************************************************/

#include "rbgprivate.h"
#include <ctype.h>

static ID id_new;
static ID id_module_eval;
static ID id_to_s;
static ID id_or;

/**********************************************************************/

typedef struct {
	char *original;
	char *replacement;
} constant_map;

static GSList *rbgobj_cmap = NULL;

static gchar *
nick_to_const_name(const gchar *nick)
{
    gchar *const_name;
    gchar *p;

    if (!nick)
        return NULL;

    const_name = g_strdup(nick);
    for (p = const_name; *p; p++) {
        if (*p == '-' || *p == ' ')
            *p = '_';
        else
            *p = g_ascii_toupper(*p);
    }
    return const_name;
}

static VALUE
resolve_enum_value(VALUE klass, VALUE nick)
{
    VALUE value = Qnil;
    gchar *const_nick;
    ID const_nick_id;

    if (RVAL2CBOOL(rb_obj_is_kind_of(nick, klass)))
        return nick;

    nick = rb_funcall(nick, id_to_s, 0);
    const_nick = nick_to_const_name(RVAL2CSTR(nick));
    const_nick_id = rb_intern(const_nick);
    if (rb_const_defined(klass, const_nick_id)) {
        value = rb_const_get(klass, const_nick_id);
    }
    g_free(const_nick);

    return value;
}

static VALUE
resolve_flags_value(VALUE klass, VALUE nick_or_nicks)
{
    int i, len;
    VALUE flags_value = Qnil;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(nick_or_nicks, rb_cArray)))
        return resolve_enum_value(klass, nick_or_nicks);

    len = RARRAY(nick_or_nicks)->len;
    for (i = 0; i < len; i++) {
        VALUE value;

        value = resolve_enum_value(klass, RARRAY(nick_or_nicks)->ptr[i]);
        if (NIL_P(value))
            return Qnil;

        if (NIL_P(flags_value))
            flags_value = value;
        else
            flags_value = rb_funcall(flags_value, id_or, 1, value);
    }

    return flags_value;
}

static gint
rbgobj_constant_find(constant_map *a, char *name)
{
      return strcmp(a->original, name);
}

void 
rbgobj_constant_remap(const char *original, 
			const char *replacement)
{
      constant_map *map = g_new(constant_map,1);
      
      map -> original     = g_strdup(original);
      map -> replacement  = g_strdup(replacement);
      
      rbgobj_cmap = g_slist_append(rbgobj_cmap, map);
}

static char *
rbgobj_constant_lookup(const char *name)
{
      GSList *p = rbgobj_cmap;
      
      p = g_slist_find_custom(rbgobj_cmap, name, 
      			(GCompareFunc)rbgobj_constant_find);
      if (p)
      {
          char *replacement;
	  constant_map *map;
	  
	  map         = (constant_map*) p -> data;
	  rbgobj_cmap = g_slist_delete_link(rbgobj_cmap, p);
	  replacement = map -> replacement;
	  
	  g_free(map -> original);
	  g_free(map);
	  
	  return replacement;
      }
      return NULL;
}

void 
rbgobj_define_const(VALUE mod, const char *name,
			VALUE value)
{
     if (name[0] >= 'A' && name[0] <= 'Z')
     {
         rb_define_const(mod, name, value);
     }
     else
     {
         char *new_name = rbgobj_constant_lookup(name);
	 
	 if (new_name)
	 {
	     rb_define_const(mod, new_name, value);
	     g_free(new_name);
	 }
	 else
	 {
	     rb_warn("Invalid constant name '%s' - skipped", name);
	 }
     }
}

static void
rbgobj_enum_add_constants(VALUE mod, GType enum_type,
                          const gchar *strip_prefix)
{
    GEnumClass *gclass;
    int i;
    int prefix_len = strlen(strip_prefix);

    gclass = G_ENUM_CLASS(g_type_class_ref(enum_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GEnumValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\"%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rbgobj_define_const(mod, name,
                                rbgobj_make_enum(value->value, enum_type));
        }
    }

    g_type_class_unref(gclass);
}

static void
rbgobj_flags_add_constants(VALUE mod, GType flags_type,
                           const gchar *strip_prefix)
{
    GFlagsClass *gclass;
    int i;
    int prefix_len = strlen(strip_prefix);

    gclass = G_FLAGS_CLASS(g_type_class_ref(flags_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GFlagsValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\"%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rbgobj_define_const(mod, name,
                                rbgobj_make_flags(value->value, flags_type));

        }
    }

    g_type_class_unref(gclass);
}

void
rbgobj_add_constants(VALUE mod, GType type, const gchar *strip_prefix)
{
    if (G_TYPE_IS_ENUM(type)) {
        rbgobj_enum_add_constants(mod, type, strip_prefix);
    } else if (G_TYPE_IS_FLAGS(type)) {
        rbgobj_flags_add_constants(mod, type, strip_prefix);
    } else {
        g_warning("`%s' is not an enum/flags type", g_type_name(type));
    }
}

/**********************************************************************/

typedef struct {
    GEnumClass* gclass;
    gint value;
    GEnumValue* info;
} enum_holder;

static void
enum_free(enum_holder* p)
{
    g_type_class_unref(p->gclass);
    free(p);
}

static enum_holder*
enum_get_holder(VALUE obj)
{
    enum_holder* p;
    Data_Get_Struct(obj, enum_holder, p);
    return p;
}

VALUE
rbgobj_make_enum(gint n, GType gtype)
{
    return rb_funcall(GTYPE2CLASS(gtype), id_new, 1, INT2NUM(n));
}

gint
rbgobj_get_enum(VALUE obj, GType gtype)
{
    VALUE klass;

    if (!g_type_is_a(gtype, G_TYPE_ENUM))
        rb_raise(rb_eTypeError, "%s is not a %s",
                 g_type_name(gtype), g_type_name(G_TYPE_ENUM));

    /* for compatibility */
    if (rb_obj_is_kind_of(obj, rb_cInteger))
        obj = rbgobj_make_enum(NUM2INT(obj), gtype);

    klass = GTYPE2CLASS(gtype);

    if (!rb_obj_is_kind_of(obj, klass)) {
        VALUE enum_value;

        enum_value = resolve_enum_value(klass, obj);
        if (!NIL_P(enum_value))
            obj = enum_value;
    }

    if (rb_obj_is_kind_of(obj, klass))
        return enum_get_holder(obj)->value;
    else
        rb_raise(rb_eTypeError, "not a %s", rb_class2name(klass));
}

/**********************************************************************/

void
rbgobj_init_enum_class(VALUE klass)
{
    GEnumClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    int i;

    for (i = 0; i < gclass->n_values; i++) {
        GEnumValue* entry = &(gclass->values[i]);
        gchar *const_nick_name;

        const_nick_name = nick_to_const_name(entry->value_nick);

#if 0
        {
            ID id = rb_intern(const_nick_name);
            if (rb_is_const_id(id)) {
                VALUE value = rbgobj_make_enum(entry->value, CLASS2GTYPE(klass));
                rb_define_const(klass, const_nick_name, value);
            }
        }
#else
        {
            if (const_nick_name) {
                VALUE value = rbgobj_make_enum(entry->value, CLASS2GTYPE(klass));
                rbgobj_define_const(klass, const_nick_name, value);
            }
        }
#endif

        g_free(const_nick_name);
    }

    g_type_class_unref(gclass);
}

static VALUE
enum_s_range(self)
    VALUE self;
{
    GEnumClass* gclass = g_type_class_ref(CLASS2GTYPE(self));
    VALUE result = rb_range_new(INT2NUM(gclass->minimum),
                                INT2NUM(gclass->maximum),
                                FALSE);
    g_type_class_unref(gclass);
    return result;
}

static VALUE
enum_s_values(self)
    VALUE self;
{
    GEnumClass* gclass = g_type_class_ref(CLASS2GTYPE(self));
    GType gtype = G_TYPE_FROM_CLASS(gclass);
    VALUE result = rb_ary_new();
    int i;

    for (i = 0; i < gclass->n_values; i++) {
        GEnumValue* p = &(gclass->values[i]);
        VALUE obj = rbgobj_make_enum(p->value, gtype);
        rb_ary_push(result, obj);
    }

    g_type_class_unref(gclass);
    return result;
}

static VALUE
enum_s_allocate(VALUE self)
{
    GType gtype = CLASS2GTYPE(self);

    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError, "abstract class");
    } else {
        enum_holder* p;
        VALUE result = Data_Make_Struct(self, enum_holder, NULL, enum_free, p);
        p->gclass = g_type_class_ref(gtype);
        p->info   = NULL;
        return result;
    }
}

static VALUE
enum_initialize(VALUE self, VALUE arg)
{
    enum_holder* p = enum_get_holder(self);

    if (rb_respond_to(arg, rb_intern("to_str"))) {
        const char* str = StringValuePtr(arg);
        p->info = g_enum_get_value_by_name(p->gclass, str);
        if (! p->info)
            p->info = g_enum_get_value_by_nick(p->gclass, str);
        if (! p->info)
            rb_raise(rb_eArgError, "invalid argument");
    } else {
        p->value = NUM2INT(arg);
        p->info  = g_enum_get_value(p->gclass, p->value);
    }

    return Qnil;
}

static VALUE
enum_to_i(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return INT2NUM(p->value);
}

static VALUE
enum_name(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_name) : Qnil;
}

static VALUE
enum_nick(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_nick) : Qnil;
}

static VALUE
enum_inspect(VALUE self)
{
    const char* cname = rb_class2name(CLASS_OF(self));
    enum_holder* p = enum_get_holder(self);
    gchar* str;
    VALUE result;

    if (p->info)
        str = g_strdup_printf("#<%s %s>",
                              cname, p->info->value_nick);
    else
        str = g_strdup_printf("#<%s %d>",
                              cname, p->value);
    result = rb_str_new2(str);
    g_free(str);

    return result;
}

static VALUE
enum_eqv(VALUE self, VALUE rhs)
{
    enum_holder* p = enum_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;
    return CBOOL2RVAL(rbgobj_get_enum(self, gtype) == rbgobj_get_enum(rhs, gtype));
}

static VALUE
enum_hash(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return UINT2NUM(p->value ^ G_TYPE_FROM_CLASS(p->gclass));
}

static VALUE
enum_coerce(VALUE self, VALUE other)
{
    if (rb_obj_is_kind_of(other, rb_cInteger)){
        enum_holder* p = enum_get_holder(self);
        GType gtype = G_TYPE_FROM_CLASS(p->gclass);
        other = rbgobj_make_enum(NUM2INT(other), gtype);
        return rb_ary_new3(2, other, self);
    } else
        rb_raise(rb_eTypeError, "can't coerce");
}

static void
Init_enum()
{
    VALUE cEnum  = G_DEF_CLASS(G_TYPE_ENUM, "Enum", mGLib);
    rb_define_singleton_method(cEnum, "gtype", generic_s_gtype, 0);
    rb_define_method(cEnum, "gtype", generic_gtype, 0);

    rb_define_singleton_method(cEnum, "range", enum_s_range, 0);
    rb_define_singleton_method(cEnum, "values", enum_s_values, 0);

    rb_define_alloc_func(cEnum, enum_s_allocate);

    rb_define_method(cEnum, "initialize", enum_initialize, 1);
    rb_define_method(cEnum, "to_i", enum_to_i, 0);
    rb_define_method(cEnum, "name", enum_name, 0);
    rb_define_method(cEnum, "nick", enum_nick, 0);

    rb_define_method(cEnum, "inspect", enum_inspect, 0);
    rb_define_method(cEnum, "==", enum_eqv, 1);
    rb_define_method(cEnum, "hash", enum_hash, 0);
    rb_define_method(cEnum, "eql?", enum_eqv, 1);

    /* for compatibility */
    rb_define_method(cEnum, "coerce", enum_coerce, 1);
    rb_define_alias(cEnum, "to_int", "to_i");
}

/**********************************************************************/

typedef struct {
    GFlagsClass* gclass;
    guint value;
    GFlagsValue* info;
} flags_holder;

static void
flags_free(enum_holder* p)
{
    g_type_class_unref(p->gclass);
    free(p);
}

static flags_holder*
flags_get_holder(VALUE obj)
{
    flags_holder* p;
    Data_Get_Struct(obj, flags_holder, p);
    return p;
}

VALUE
rbgobj_make_flags(guint n, GType gtype)
{
    return rb_funcall(GTYPE2CLASS(gtype), id_new, 1, UINT2NUM(n));
}

guint
rbgobj_get_flags(VALUE obj, GType gtype)
{
    VALUE klass;

    if (!g_type_is_a(gtype, G_TYPE_FLAGS))
        rb_raise(rb_eTypeError, "%s is not a %s",
                 g_type_name(gtype), g_type_name(G_TYPE_FLAGS));

    /* for compatibility */
    if (rb_obj_is_kind_of(obj, rb_cInteger))
        obj = rbgobj_make_flags(NUM2UINT(obj), gtype);

    klass = GTYPE2CLASS(gtype);

    if (!rb_obj_is_kind_of(obj, klass)) {
        VALUE flags_value = Qnil;

        flags_value = resolve_flags_value(klass, obj);
        if (!NIL_P(flags_value))
            obj = flags_value;
    }

    if (rb_obj_is_kind_of(obj, klass))
        return flags_get_holder(obj)->value;
    else
        rb_raise(rb_eTypeError, "not a %s", rb_class2name(klass));
}

/**********************************************************************/

void
rbgobj_init_flags_class(VALUE klass)
{
    GFlagsClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    GString* source = g_string_new(NULL);
    int i;

    for (i = 0; i < gclass->n_values; i++) {
        GFlagsValue* entry = &(gclass->values[i]);
        gchar* nick;
        gchar* p;
        gchar* replace_nick;

        replace_nick = rbgobj_constant_lookup(entry->value_nick);
        if (replace_nick){
            nick = g_strdup(replace_nick);
        } else {
            nick = g_strdup(entry->value_nick);
        }

        for (p = nick; *p; p++)
            if (*p == '-' || *p == ' ')
                *p = '_';
            else
                *p = tolower(*p);

        g_string_append_printf(
            source,
            "def %s%s?; self >= self.class.new(%d); end\n",
            g_ascii_isdigit(nick[0]) ? "_" : "",
            nick, entry->value);

        for (p = nick; *p; p++)
            *p = g_ascii_toupper(*p);

#if 0
        {
            ID id = rb_intern(nick);
            if (rb_is_const_id(id)) {
                VALUE value = rbgobj_make_flags(entry->value, CLASS2GTYPE(klass));
                rb_define_const(klass, nick, value);
            }
        }
#else
        {
            VALUE value = rbgobj_make_flags(entry->value, CLASS2GTYPE(klass));
            rbgobj_define_const(klass, nick, value);
        }
#endif

        g_free(nick);
    }

    rb_funcall(klass, id_module_eval, 1, rb_str_new2(source->str));
    g_string_free(source, TRUE);

    g_type_class_unref(gclass);
}

static VALUE
flags_s_mask(VALUE klass)
{
    GFlagsClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    VALUE result = UINT2NUM(gclass->mask);
    g_type_class_unref(gclass);
    return result;
}

static VALUE
flags_s_values(VALUE klass)
{
    GFlagsClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    GType gtype = G_TYPE_FROM_CLASS(gclass);
    VALUE result = rb_ary_new();
    int i;

    for (i = 0; i < gclass->n_values; i++) {
        GFlagsValue* p = &(gclass->values[i]);
        VALUE obj = rbgobj_make_flags(p->value, gtype);
        rb_ary_push(result, obj);
    }

    g_type_class_unref(gclass);

    return result;
}

static VALUE
flags_s_allocate(VALUE self)
{
    GType gtype = CLASS2GTYPE(self);

    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError, "abstract class");
    } else {
        flags_holder* p;
        VALUE result = Data_Make_Struct(self, flags_holder, NULL, flags_free, p);
        p->gclass = g_type_class_ref(gtype);
        p->value  = 0;
        p->info   = NULL;
        return result;
    }
}

static VALUE
flags_initialize(int argc, VALUE* argv, VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    VALUE arg;

    rb_scan_args(argc, argv, "01", &arg);

    if (argc == 0) {
        p->value = 0;
    } else {
        if (rb_respond_to(arg, rb_intern("to_str"))) {
            const char* str = StringValuePtr(arg);
            p->info = g_flags_get_value_by_name(p->gclass, str);
            if (!p->info)
                p->info = g_flags_get_value_by_nick(p->gclass, str);
            if (!p->info)
                rb_raise(rb_eArgError, "invalid argument");
            p->value = p->info->value;
        } else {
            p->value = NUM2UINT(arg);
        }
    }

    if (!p->info) {
        int i;
        for (i = 0; i < p->gclass->n_values; i++){
            GFlagsValue* val = &(p->gclass->values[i]);
            if (val->value == p->value){
                p->info = val;
                break;
            }
        }
    }

    return Qnil;
}

static VALUE
flags_to_i(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return UINT2NUM(p->value);
}

static VALUE
flags_name(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_name) : Qnil;
}

static VALUE
flags_nick(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_nick) : Qnil;
}

static VALUE
flags_compare(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);

    if (p->value == rhs_val)
        return INT2FIX(0);
    else if ((p->value & rhs_val) == rhs_val)
        return INT2FIX(1);
    else if ((p->value & rhs_val) == p->value)
        return INT2FIX(-1);
    else
        return Qnil;
}

static VALUE
flags_eqv(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);
    return CBOOL2RVAL(p->value == rhs_val);
}

static VALUE
flags_gt_eq(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);
    return CBOOL2RVAL((p->value & rhs_val) == rhs_val);
}

static VALUE
flags_lt_eq(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);
    return CBOOL2RVAL((p->value & rhs_val) == p->value);
}

static VALUE
flags_gt(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);
    return CBOOL2RVAL((p->value & rhs_val) == rhs_val &&
                      p->value != rhs_val);
}

static VALUE
flags_lt(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    guint rhs_val;

    if (CLASS_OF(rhs) != CLASS_OF(self) &&
        !rb_obj_is_kind_of(rhs, rb_cInteger))
        return Qnil;

    rhs_val = rbgobj_get_flags(rhs, gtype);
    return CBOOL2RVAL((p->value & rhs_val) == p->value &&
                      p->value != rhs_val);
}

static VALUE
flags_not(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    return rbgobj_make_flags((~ p->value) & p->gclass->mask,
                             G_TYPE_FROM_CLASS(p->gclass));
}

#define LIFT_BINARY_OP(funcname, op) \
    static VALUE \
    funcname(VALUE self, VALUE rhs) \
    { \
        flags_holder* p = flags_get_holder(self); \
        GType gtype = G_TYPE_FROM_CLASS(p->gclass); \
        return rbgobj_make_flags(p->value op rbgobj_get_flags(rhs, gtype), \
                                 gtype); \
    }

LIFT_BINARY_OP(flags_and, &)
LIFT_BINARY_OP(flags_or, |)
LIFT_BINARY_OP(flags_xor, ^)

static VALUE
flags_minus(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    return rbgobj_make_flags(p->value & ~rbgobj_get_flags(rhs, gtype),
                             gtype);
}

static VALUE
flags_empty_p(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return CBOOL2RVAL(p->value == 0);
}

static VALUE
flags_hash(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return UINT2NUM(p->value ^ G_TYPE_FROM_CLASS(p->gclass));
}

static VALUE
flags_coerce(VALUE self, VALUE other)
{
    if (rb_obj_is_kind_of(other, rb_cInteger)){
        flags_holder* p = flags_get_holder(self);
        GType gtype = G_TYPE_FROM_CLASS(p->gclass);
        other = rbgobj_make_flags(NUM2UINT(other), gtype);
        return rb_ary_new3(2, other, self);
    } else
        rb_raise(rb_eTypeError, "can't coerce");
}

static VALUE
flags_nonzero_p(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return CBOOL2RVAL(p->value != 0);
}

static void
Init_flags()
{
    VALUE cFlags = G_DEF_CLASS(G_TYPE_FLAGS, "Flags", mGLib);
    rb_define_singleton_method(cFlags, "gtype", generic_s_gtype, 0);
    rb_define_method(cFlags, "gtype", generic_gtype, 0);

    rb_define_singleton_method(cFlags, "mask", flags_s_mask, 0);
    rb_define_singleton_method(cFlags, "values", flags_s_values, 0);

    rb_define_alloc_func(cFlags, flags_s_allocate);

    rb_define_method(cFlags, "initialize", flags_initialize, -1);

    rb_define_method(cFlags, "to_i", flags_to_i, 0);
    rb_define_alias(cFlags, "to_int", "to_i");
    rb_define_method(cFlags, "name", flags_name, 0);
    rb_define_method(cFlags, "nick", flags_nick, 0);

    /*
    rb_define_method(cFlags, "inspect", flags_inspect, 0);
    */

    rb_define_method(cFlags, "<=>", flags_compare, 1);
    rb_define_method(cFlags, "==", flags_eqv, 1);
    rb_define_method(cFlags, ">=", flags_gt_eq, 1);
    rb_define_method(cFlags, "<=", flags_lt_eq, 1);
    rb_define_method(cFlags, ">", flags_gt, 1);
    rb_define_method(cFlags, "<", flags_lt, 1);
    rb_define_method(cFlags, "~", flags_not, 0);
    rb_define_method(cFlags, "&", flags_and, 1);
    rb_define_method(cFlags, "|", flags_or, 1);
    rb_define_method(cFlags, "^", flags_xor, 1);
    rb_define_method(cFlags, "-", flags_minus, 1);

    rb_define_method(cFlags, "empty?", flags_empty_p, 0);

    rb_define_method(cFlags, "hash", flags_hash, 0);
    rb_define_method(cFlags, "eql?", flags_eqv, 1);

    /* for compatibility */
    rb_define_method(cFlags, "coerce", flags_coerce, 1);
    rb_define_method(cFlags, "zero?", flags_empty_p, 0);
    rb_define_method(cFlags, "nonzero?", flags_nonzero_p, 0);
}

/**********************************************************************/

void
Init_gobject_genums()
{
    id_module_eval = rb_intern("module_eval");
    id_new = rb_intern("new");
    id_to_s = rb_intern("to_s");
    id_or = rb_intern("|");

    Init_enum();
    Init_flags();
} 
