/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-action.c -

  $Author: ktou $
  $Date: 2006/09/06 02:25:35 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"


/* PopplerAction */
static VALUE actions[POPPLER_ACTION_MOVIE];

VALUE
rb_poppler_ruby_object_from_action(PopplerAction *action)
{
    VALUE obj;

    if (!action)
        return Qnil;

    obj = BOXED2RVAL(action, POPPLER_TYPE_ACTION);
    RBASIC(obj)->klass = actions[action->type];
    return obj;
}

PopplerAction *
rb_poppler_action_from_ruby_object(VALUE action)
{
    return NIL_P(action) ? NULL : RVAL2BOXED(action, POPPLER_TYPE_ACTION);
}

#define ATTR_STR(type, name)                                    \
static VALUE                                                    \
action_ ## type ## _ ## name (VALUE self)                       \
{                                                               \
    return CSTR2RVAL(RVAL2ACTION(self)->type.name);             \
}                                                               \
static VALUE                                                    \
action_ ## type ## _set_ ## name (VALUE self, VALUE val)        \
{                                                               \
    RVAL2ACTION(self)->type.name = RVAL2CSTR(val);              \
    return self;                                                \
}

#define ATTR_DEST(type, name)                                   \
static VALUE                                                    \
action_ ## type ## _ ## name (VALUE self)                       \
{                                                               \
    return DEST2RVAL(RVAL2ACTION(self)->type.name);             \
}                                                               \
static VALUE                                                    \
action_ ## type ## _set_ ## name (VALUE self, VALUE val)        \
{                                                               \
    RVAL2ACTION(self)->type.name = RVAL2DEST(val);              \
    return self;                                                \
}

#define DEFINE_ACCESSOR(action, type, name)                     \
    rb_define_method(action, G_STRINGIFY(name),                 \
                     action_ ## type ## _## name, 0);           \
    rb_define_method(action, G_STRINGIFY(set_ ## name),         \
                     action_ ## type ## _set_## name, 1);

/* PopplerActionAny */
static VALUE
action_any_type(VALUE self)
{
    return ACTIONTYPE2RVAL(RVAL2ACTION(self)->type);
}
ATTR_STR(any, title);

/* PopplerActionGotoDest */
ATTR_DEST(goto_dest, dest);

/* PopplerActionGotoRemote */
ATTR_STR(goto_remote, file_name);
ATTR_DEST(goto_remote, dest);

/* PopplerActionLaunch */
ATTR_STR(launch, file_name);
ATTR_STR(launch, params);

/* PopplerActionUri */
ATTR_STR(uri, uri);

/* PopplerActionNamed */
ATTR_STR(named, named_dest);

/* PopplerActionMovie */


/* PopplerDest */
#ifdef RB_POPPLER_TYPE_DEST_NOT_DEFINED
GType
poppler_dest_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("PopplerDest",
					     (GBoxedCopyFunc) poppler_dest_copy,
					     (GBoxedFreeFunc) poppler_dest_free);

  return our_type;
}
#endif

static VALUE
dest_get_type(VALUE self)
{
    return DESTTYPE2RVAL(RVAL2DEST(self)->type);
}

static VALUE
dest_get_page_num(VALUE self)
{
    return INT2NUM(RVAL2DEST(self)->page_num);
}

static VALUE
dest_get_left(VALUE self)
{
    return rb_float_new(RVAL2DEST(self)->left);
}

static VALUE
dest_get_bottom(VALUE self)
{
    return rb_float_new(RVAL2DEST(self)->bottom);
}

static VALUE
dest_get_right(VALUE self)
{
    return rb_float_new(RVAL2DEST(self)->right);
}

static VALUE
dest_get_top(VALUE self)
{
    return rb_float_new(RVAL2DEST(self)->top);
}

static VALUE
dest_get_zoom(VALUE self)
{
    return rb_float_new(RVAL2DEST(self)->zoom);
}

static VALUE
dest_get_named_dest(VALUE self)
{
    return CSTR2RVAL(RVAL2DEST(self)->named_dest);
}

static VALUE
dest_get_change_left(VALUE self)
{
    return UINT2NUM(RVAL2DEST(self)->change_left);
}

static VALUE
dest_get_change_top(VALUE self)
{
    return UINT2NUM(RVAL2DEST(self)->change_top);
}

static VALUE
dest_get_change_zoom(VALUE self)
{
    return UINT2NUM(RVAL2DEST(self)->change_zoom);
}


void
Init_poppler_action(VALUE mPoppler)
{
    VALUE cActionType, cDestType, cDest;
    VALUE cAction, cActionAny, cActionGotoDest, cActionGotoRemote;
    VALUE cActionLaunch, cActionUri, cActionNamed, cActionMovie;

    cAction = G_DEF_CLASS(POPPLER_TYPE_ACTION, "Action", mPoppler);

    cActionAny = rb_define_class_under(mPoppler, "ActionAny", cAction);
    rb_define_method(cActionAny, "type", action_any_type, 0);
    DEFINE_ACCESSOR(cActionAny, any, title);

    cActionGotoDest = rb_define_class_under(mPoppler, "ActionGotoDest",
                                            cActionAny);
    DEFINE_ACCESSOR(cActionGotoDest, goto_dest, dest);

    cActionGotoRemote = rb_define_class_under(mPoppler, "ActionGotoRemote",
                                              cActionAny);
    DEFINE_ACCESSOR(cActionGotoRemote, goto_remote, file_name);
    DEFINE_ACCESSOR(cActionGotoRemote, goto_remote, dest);

    cActionLaunch = rb_define_class_under(mPoppler, "ActionLaunch", cActionAny);
    DEFINE_ACCESSOR(cActionLaunch, launch, file_name);
    DEFINE_ACCESSOR(cActionLaunch, launch, params);

    cActionUri = rb_define_class_under(mPoppler, "ActionUri", cActionAny);
    DEFINE_ACCESSOR(cActionUri, uri, uri);

    cActionNamed = rb_define_class_under(mPoppler, "ActionNamed", cActionAny);
    DEFINE_ACCESSOR(cActionNamed, named, named_dest);

    cActionMovie = rb_define_class_under(mPoppler, "ActionMovie", cActionAny);

    actions[POPPLER_ACTION_UNKNOWN] = cActionAny;
    actions[POPPLER_ACTION_GOTO_DEST] = cActionGotoDest;
    actions[POPPLER_ACTION_GOTO_REMOTE] = cActionGotoRemote;
    actions[POPPLER_ACTION_LAUNCH] = cActionLaunch;
    actions[POPPLER_ACTION_URI] = cActionUri;
    actions[POPPLER_ACTION_NAMED] = cActionNamed;
    actions[POPPLER_ACTION_MOVIE] = cActionMovie;

    G_DEF_SETTERS(cAction);
    G_DEF_SETTERS(cActionAny);
    G_DEF_SETTERS(cActionGotoDest);
    G_DEF_SETTERS(cActionGotoRemote);
    G_DEF_SETTERS(cActionLaunch);
    G_DEF_SETTERS(cActionUri);
    G_DEF_SETTERS(cActionNamed);
    G_DEF_SETTERS(cActionMovie);

    cActionType = G_DEF_CLASS(POPPLER_TYPE_ACTION_TYPE, "ActionType", mPoppler);
    cDestType = G_DEF_CLASS(POPPLER_TYPE_DEST_TYPE, "DestType", mPoppler);

    cDest = G_DEF_CLASS(POPPLER_TYPE_DEST, "Dest", mPoppler);

    rb_define_method(cDest, "type", dest_get_type, 0);
    rb_define_method(cDest, "page_num", dest_get_page_num, 0);
    rb_define_method(cDest, "left", dest_get_left, 0);
    rb_define_method(cDest, "bottom", dest_get_bottom, 0);
    rb_define_method(cDest, "right", dest_get_right, 0);
    rb_define_method(cDest, "top", dest_get_top, 0);
    rb_define_method(cDest, "zoom", dest_get_zoom, 0);
    rb_define_method(cDest, "named_dest", dest_get_named_dest, 0);
    rb_define_method(cDest, "change_left", dest_get_change_left, 0);
    rb_define_method(cDest, "change_top", dest_get_change_top, 0);
    rb_define_method(cDest, "change_zoom", dest_get_change_zoom, 0);
    G_DEF_SETTERS(cDest);
}
