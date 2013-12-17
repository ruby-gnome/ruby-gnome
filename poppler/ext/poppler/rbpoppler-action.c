/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2013 Ruby-GNOME2 Project Team
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

/* PopplerAction */
static VALUE rb_cPopplerAction;
static VALUE rb_cPopplerActionDest;
static VALUE rb_cPopplerActionAny;
static VALUE rb_cPopplerActionUnknown;
static VALUE rb_cPopplerActionGotoDest;
static VALUE rb_cPopplerActionGotoRemote;
static VALUE rb_cPopplerActionLaunch;
static VALUE rb_cPopplerActionURI;
static VALUE rb_cPopplerActionNamed;
static VALUE rb_cPopplerActionMovie;
static VALUE rb_cPopplerActionRendition;
static VALUE rb_cPopplerActionOCGState;
static VALUE rb_cPopplerActionJavascript;

#define DEFINE_ACTION_TYPE(type_lower_case, type_upper_case)            \
static GType                                                            \
rb_poppler_action_ ## type_lower_case ## _get_type(void)                \
{                                                                       \
    static GType type = 0;                                              \
    if (type == 0) {                                                    \
        type = g_boxed_type_register_static("PopplerAction" # type_upper_case, \
                                            (GBoxedCopyFunc)poppler_action_copy, \
                                            (GBoxedFreeFunc)poppler_action_free); \
    }                                                                   \
    return type;                                                        \
}

DEFINE_ACTION_TYPE(any, Any)
DEFINE_ACTION_TYPE(unknown, Unknown)
DEFINE_ACTION_TYPE(goto_dest, GotoDest)
DEFINE_ACTION_TYPE(goto_remote, GotoRemote)
DEFINE_ACTION_TYPE(launch, Launch)
DEFINE_ACTION_TYPE(uri, URI)
DEFINE_ACTION_TYPE(named, Named)
DEFINE_ACTION_TYPE(movie, Movie)
DEFINE_ACTION_TYPE(rendition, Rendition)
DEFINE_ACTION_TYPE(ocg_state, OCGState)
DEFINE_ACTION_TYPE(javascript, Javascript)

#define POPPLER_ACTION_TYPE_ANY         (rb_poppler_action_any_get_type())
#define POPPLER_ACTION_TYPE_UNKNOWN     (rb_poppler_action_unknown_get_type())
#define POPPLER_ACTION_TYPE_GOTO_DEST   (rb_poppler_action_goto_dest_get_type())
#define POPPLER_ACTION_TYPE_GOTO_REMOTE (rb_poppler_action_goto_remote_get_type())
#define POPPLER_ACTION_TYPE_LAUNCH      (rb_poppler_action_launch_get_type())
#define POPPLER_ACTION_TYPE_URI         (rb_poppler_action_uri_get_type())
#define POPPLER_ACTION_TYPE_NAMED       (rb_poppler_action_named_get_type())
#define POPPLER_ACTION_TYPE_MOVIE       (rb_poppler_action_movie_get_type())
#define POPPLER_ACTION_TYPE_RENDITION   (rb_poppler_action_rendition_get_type())
#define POPPLER_ACTION_TYPE_OCG_STATE   (rb_poppler_action_ocg_state_get_type())
#define POPPLER_ACTION_TYPE_JAVASCRIPT  (rb_poppler_action_javascript_get_type())

static GType
rb_poppler_action_type_to_gtype(PopplerActionType action_type)
{
    GType gtype = POPPLER_ACTION_TYPE_ANY;

    switch (action_type) {
      case POPPLER_ACTION_NONE:
        break;
      case POPPLER_ACTION_UNKNOWN:
        gtype = POPPLER_ACTION_TYPE_UNKNOWN;
        break;
      case POPPLER_ACTION_GOTO_DEST:
        gtype = POPPLER_ACTION_TYPE_GOTO_DEST;
        break;
      case POPPLER_ACTION_GOTO_REMOTE:
        gtype = POPPLER_ACTION_TYPE_GOTO_REMOTE;
        break;
      case POPPLER_ACTION_LAUNCH:
        gtype = POPPLER_ACTION_TYPE_LAUNCH;
        break;
      case POPPLER_ACTION_URI:
        gtype = POPPLER_ACTION_TYPE_URI;
        break;
      case POPPLER_ACTION_NAMED:
        gtype = POPPLER_ACTION_TYPE_NAMED;
        break;
      case POPPLER_ACTION_MOVIE:
        gtype = POPPLER_ACTION_TYPE_MOVIE;
        break;
      case POPPLER_ACTION_RENDITION:
        gtype = POPPLER_ACTION_TYPE_RENDITION;
        break;
      case POPPLER_ACTION_OCG_STATE:
        gtype = POPPLER_ACTION_TYPE_OCG_STATE;
        break;
      case POPPLER_ACTION_JAVASCRIPT:
        gtype = POPPLER_ACTION_TYPE_JAVASCRIPT;
        break;
      default:
        break;
    }

    return gtype;
}

static GType
rb_poppler_action_to_gtype(VALUE action)
{
    VALUE klass;
    GType type = POPPLER_ACTION_TYPE_ANY;

    klass = rb_obj_class(action);
    if (klass == rb_cPopplerActionAny) {
        type = POPPLER_ACTION_TYPE_ANY;
    } else if (klass == rb_cPopplerActionUnknown) {
        type = POPPLER_ACTION_TYPE_UNKNOWN;
    } else if (klass == rb_cPopplerActionGotoDest) {
        type = POPPLER_ACTION_TYPE_GOTO_DEST;
    } else if (klass == rb_cPopplerActionGotoRemote) {
        type = POPPLER_ACTION_TYPE_GOTO_REMOTE;
    } else if (klass == rb_cPopplerActionLaunch) {
        type = POPPLER_ACTION_TYPE_LAUNCH;
    } else if (klass == rb_cPopplerActionURI) {
        type = POPPLER_ACTION_TYPE_URI;
    } else if (klass == rb_cPopplerActionNamed) {
        type = POPPLER_ACTION_TYPE_NAMED;
    } else if (klass == rb_cPopplerActionMovie) {
        type = POPPLER_ACTION_TYPE_MOVIE;
    } else if (klass == rb_cPopplerActionRendition) {
        type = POPPLER_ACTION_TYPE_RENDITION;
    } else if (klass == rb_cPopplerActionOCGState) {
        type = POPPLER_ACTION_TYPE_OCG_STATE;
    } else if (klass == rb_cPopplerActionJavascript) {
        type = POPPLER_ACTION_TYPE_JAVASCRIPT;
    } else {
        rb_raise(rb_eArgError, "Not action object: %s", RBG_INSPECT(action));
    }

    return type;
}

VALUE
rb_poppler_ruby_object_from_action(PopplerAction *action)
{
    return BOXED2RVAL(action, rb_poppler_action_type_to_gtype(action->any.type));
}

PopplerAction *
rb_poppler_action_from_ruby_object(VALUE action)
{
    return RVAL2BOXED(action, rb_poppler_action_to_gtype(action));
}

#define ACTION_ATTR_STR(type, name)				\
static VALUE							\
action_ ## type ## _ ## name (VALUE self)			\
{								\
    return CSTR2RVAL(RVAL2POPPLERACTION(self)->type.name);      \
}

#define ACTION_ATTR_DEST(type, name)                                    \
static VALUE                                                            \
action_ ## type ## _ ## name (VALUE self)                               \
{                                                                       \
    return POPPLERDEST2RVAL(RVAL2POPPLERACTION(self)->type.name);       \
}

#define DEFINE_ACCESSOR(prefix, target, name) \
    rbg_define_method(target, G_STRINGIFY(name), prefix ## _ ## name, 0);

#define DEFINE_ACTION_ACCESSOR(target, type, name) \
    DEFINE_ACCESSOR(action_ ## type, target, name)

/* PopplerActionAny */
static VALUE
action_any_type(VALUE self)
{
    return POPPLERACTIONTYPE2RVAL(RVAL2POPPLERACTION(self)->type);
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

/* PopplerActionURI */
ACTION_ATTR_STR(uri, uri);

/* PopplerActionNamed */
ACTION_ATTR_STR(named, named_dest);

/* PopplerActionMovie */
ACTION_ATTR_DEST(movie, movie);

/* PopplerActionRendition */
ACTION_ATTR_DEST(rendition, media);

/* PopplerActionOCGState */
ACTION_ATTR_DEST(ocg_state, state_list);

/* PopplerDest */
#ifdef RB_POPPLER_TYPE_DEST_NOT_DEFINED
GType
poppler_dest_get_type (void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static("PopplerDest",
                                                (GBoxedCopyFunc)poppler_dest_copy,
                                                (GBoxedFreeFunc)poppler_dest_free);

    return our_type;
}
#endif

static VALUE
dest_get_type(VALUE self)
{
    return POPPLERDESTTYPE2RVAL(RVAL2POPPLERDEST(self)->type);
}

#define DEST_ATTR_INT(name)                             \
static VALUE                                            \
dest_ ## name (VALUE self)                              \
{                                                       \
    return INT2NUM(RVAL2POPPLERDEST(self)->name);       \
}

#define DEST_ATTR_UINT(name)                            \
static VALUE                                            \
dest_ ## name (VALUE self)                              \
{                                                       \
    return UINT2NUM(RVAL2POPPLERDEST(self)->name);      \
}

#define DEST_ATTR_DOUBLE(name)                          \
static VALUE                                            \
dest_ ## name (VALUE self)                              \
{                                                       \
    return rb_float_new(RVAL2POPPLERDEST(self)->name);  \
}

#define DEST_ATTR_STR(name)                             \
static VALUE                                            \
dest_ ## name (VALUE self)                              \
{                                                       \
    return CSTR2RVAL(RVAL2POPPLERDEST(self)->name);     \
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
    rb_cPopplerAction = G_DEF_CLASS(POPPLER_TYPE_ACTION, "Action", mPoppler);

    rb_cPopplerActionAny = rb_define_class_under(mPoppler, "ActionAny", rb_cPopplerAction);
    rbg_define_method(rb_cPopplerActionAny, "type", action_any_type, 0);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionAny, any, title);

    rb_cPopplerActionUnknown = rb_define_class_under(mPoppler, "ActionUnknown",
                                                     rb_cPopplerAction);

    rb_cPopplerActionGotoDest = rb_define_class_under(mPoppler, "ActionGotoDest",
                                                      rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionGotoDest, goto_dest, dest);

    rb_cPopplerActionGotoRemote = rb_define_class_under(mPoppler, "ActionGotoRemote",
                                                        rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionGotoRemote, goto_remote, file_name);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionGotoRemote, goto_remote, dest);

    rb_cPopplerActionLaunch = rb_define_class_under(mPoppler, "ActionLaunch",
                                                    rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionLaunch, launch, file_name);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionLaunch, launch, params);

    rb_cPopplerActionURI = rb_define_class_under(mPoppler, "ActionURI", rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionURI, uri, uri);

    rb_cPopplerActionNamed = rb_define_class_under(mPoppler, "ActionNamed",
                                                   rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionNamed, named, named_dest);

    rb_cPopplerActionMovie = rb_define_class_under(mPoppler, "ActionMovie",
                                                   rb_cPopplerActionAny);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionMovie, movie, movie);

    rb_cPopplerActionRendition = rb_define_class_under(mPoppler, "ActionRendtion",
                                                       rb_cPopplerActionRendition);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionRendition, rendition, media);

    rb_cPopplerActionOCGState = rb_define_class_under(mPoppler, "ActionOCGState",
                                                      rb_cPopplerActionOCGState);
    DEFINE_ACTION_ACCESSOR(rb_cPopplerActionOCGState, ocg_state, state_list);

    G_DEF_CLASS(POPPLER_TYPE_ACTION_TYPE, "ActionType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_DEST_TYPE, "DestType", mPoppler);

    rb_cPopplerActionDest = G_DEF_CLASS(POPPLER_TYPE_DEST, "Dest", mPoppler);

    rbg_define_method(rb_cPopplerActionDest, "type", dest_get_type, 0);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, page_num);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, left);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, bottom);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, right);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, top);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, zoom);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, named_dest);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, change_left);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, change_top);
    DEFINE_DEST_ACCESSOR(rb_cPopplerActionDest, change_zoom);
}
