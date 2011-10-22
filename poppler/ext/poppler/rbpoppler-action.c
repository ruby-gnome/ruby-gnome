/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbpoppler-private.h"

#define RVAL2DEST(obj) ((PopplerDest *)RVAL2BOXED(obj, POPPLER_TYPE_DEST))
#define DEST2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_DEST))

#define DEST_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_DEST_TYPE))
#define ACTION_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ACTION_TYPE))
#define RVAL2ACTION_TYPE(obj) (RVAL2GENUM(obj, POPPLER_TYPE_ACTION_TYPE))


/* PopplerAction */
static VALUE actions[POPPLER_ACTION_MOVIE + 1];

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

#define ACTION_ATTR_STR(type, name)				\
static VALUE							\
action_ ## type ## _ ## name (VALUE self)			\
{								\
    return CSTR2RVAL(RVAL2POPPLER_ACTION(self)->type.name);     \
}

#define ACTION_ATTR_DEST(type, name)				\
static VALUE							\
action_ ## type ## _ ## name (VALUE self)			\
{								\
    return DEST2RVAL(RVAL2POPPLER_ACTION(self)->type.name);     \
}

#define DEFINE_ACCESSOR(prefix, target, name) \
    rb_define_method(target, G_STRINGIFY(name), prefix ## _ ## name, 0);

#define DEFINE_ACTION_ACCESSOR(target, type, name) \
    DEFINE_ACCESSOR(action_ ## type, target, name)

/* PopplerActionAny */
static VALUE
action_any_type(VALUE self)
{
    return ACTION_TYPE2RVAL(RVAL2POPPLER_ACTION(self)->type);
}
ACTION_ATTR_STR(any, title);

/* PopplerActionGotoDest */
ACTION_ATTR_DEST(goto_dest, dest);

/* PopplerActionGotoRemote */
ACTION_ATTR_STR(goto_remote, file_name);
ACTION_ATTR_DEST(goto_remote, dest);

/* PopplerActionLaunch */
ACTION_ATTR_STR(launch, file_name);
ACTION_ATTR_STR(launch, params);

/* PopplerActionUri */
ACTION_ATTR_STR(uri, uri);

/* PopplerActionNamed */
ACTION_ATTR_STR(named, named_dest);

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
    return DEST_TYPE2RVAL(RVAL2DEST(self)->type);
}

#define DEST_ATTR_INT(name)                     \
static VALUE                                    \
dest_ ## name (VALUE self)                      \
{                                               \
    return INT2NUM(RVAL2DEST(self)->name);      \
}

#define DEST_ATTR_UINT(name)                    \
static VALUE                                    \
dest_ ## name (VALUE self)                      \
{                                               \
    return UINT2NUM(RVAL2DEST(self)->name);     \
}

#define DEST_ATTR_DOUBLE(name)                  \
static VALUE                                    \
dest_ ## name (VALUE self)                      \
{                                               \
    return rb_float_new(RVAL2DEST(self)->name); \
}

#define DEST_ATTR_STR(name)                     \
static VALUE                                    \
dest_ ## name (VALUE self)                      \
{                                               \
    return CSTR2RVAL(RVAL2DEST(self)->name);    \
}

#define DEFINE_DEST_ACCESSOR(target, name) \
    DEFINE_ACCESSOR(dest, target, name)


DEST_ATTR_INT(page_num)
DEST_ATTR_DOUBLE(left)
DEST_ATTR_DOUBLE(bottom)
DEST_ATTR_DOUBLE(right)
DEST_ATTR_DOUBLE(top)
DEST_ATTR_DOUBLE(zoom)
DEST_ATTR_STR(named_dest)
DEST_ATTR_UINT(change_left)
DEST_ATTR_UINT(change_top)
DEST_ATTR_UINT(change_zoom)

void
Init_poppler_action(VALUE mPoppler)
{
    VALUE cDest,  cAction, cActionAny, cActionGotoDest, cActionGotoRemote;
    VALUE cActionLaunch, cActionUri, cActionNamed, cActionMovie;

    cAction = G_DEF_CLASS(POPPLER_TYPE_ACTION, "Action", mPoppler);

    cActionAny = rb_define_class_under(mPoppler, "ActionAny", cAction);
    rb_define_method(cActionAny, "type", action_any_type, 0);
    DEFINE_ACTION_ACCESSOR(cActionAny, any, title);

    cActionGotoDest = rb_define_class_under(mPoppler, "ActionGotoDest",
                                            cActionAny);
    DEFINE_ACTION_ACCESSOR(cActionGotoDest, goto_dest, dest);

    cActionGotoRemote = rb_define_class_under(mPoppler, "ActionGotoRemote",
                                              cActionAny);
    DEFINE_ACTION_ACCESSOR(cActionGotoRemote, goto_remote, file_name);
    DEFINE_ACTION_ACCESSOR(cActionGotoRemote, goto_remote, dest);

    cActionLaunch = rb_define_class_under(mPoppler, "ActionLaunch", cActionAny);
    DEFINE_ACTION_ACCESSOR(cActionLaunch, launch, file_name);
    DEFINE_ACTION_ACCESSOR(cActionLaunch, launch, params);

    cActionUri = rb_define_class_under(mPoppler, "ActionUri", cActionAny);
    DEFINE_ACTION_ACCESSOR(cActionUri, uri, uri);

    cActionNamed = rb_define_class_under(mPoppler, "ActionNamed", cActionAny);
    DEFINE_ACTION_ACCESSOR(cActionNamed, named, named_dest);

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

    G_DEF_CLASS(POPPLER_TYPE_ACTION_TYPE, "ActionType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_DEST_TYPE, "DestType", mPoppler);

    cDest = G_DEF_CLASS(POPPLER_TYPE_DEST, "Dest", mPoppler);

    rb_define_method(cDest, "type", dest_get_type, 0);
    DEFINE_DEST_ACCESSOR(cDest, page_num);
    DEFINE_DEST_ACCESSOR(cDest, left);
    DEFINE_DEST_ACCESSOR(cDest, bottom);
    DEFINE_DEST_ACCESSOR(cDest, right);
    DEFINE_DEST_ACCESSOR(cDest, top);
    DEFINE_DEST_ACCESSOR(cDest, zoom);
    DEFINE_DEST_ACCESSOR(cDest, named_dest);
    DEFINE_DEST_ACCESSOR(cDest, change_left);
    DEFINE_DEST_ACCESSOR(cDest, change_top);
    DEFINE_DEST_ACCESSOR(cDest, change_zoom);
    G_DEF_SETTERS(cDest);
}
