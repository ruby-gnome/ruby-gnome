/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-action.c -

  $Author: ktou $
  $Date: 2006/05/17 12:51:19 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

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
    VALUE cAction, cActionGotoDest, cActionGotoRemote, cActionLaunch;
    VALUE cActionUri, cActionNamed, cActionMovie, cDest;
    VALUE cActionType, cDestType;

    cAction = G_DEF_CLASS(POPPLER_TYPE_ACTION, "Action", mPoppler);
    cActionGotoDest = G_DEF_CLASS(POPPLER_TYPE_ACTION_GOTO_DEST,
                                  "ActionGotoDest", mPoppler);
    cActionGotoRemote = G_DEF_CLASS(POPPLER_TYPE_ACTION_GOTO_REMOTE,
                                    "ActionGotoRemote", mPoppler);
    cActionLaunch = G_DEF_CLASS(POPPLER_TYPE_ACTION_LAUNCH, "ActionLaunch",
                                mPoppler);
    cActionUri = G_DEF_CLASS(POPPLER_TYPE_ACTION_URI, "ActionUri", mPoppler);
    cActionNamed = G_DEF_CLASS(POPPLER_TYPE_ACTION_NAMED, "ActionNamed",
                               mPoppler);
    cActionMovie = G_DEF_CLASS(POPPLER_TYPE_ACTION_MOVIE, "ActionMovie",
                               mPoppler);
    cActionType = G_DEF_CLASS(POPPLER_TYPE_ACTION_TYPE, "ActionType", mPoppler);
    cDest = G_DEF_CLASS(POPPLER_TYPE_DEST, "Dest", mPoppler);
    cDestType = G_DEF_CLASS(POPPLER_TYPE_DEST_TYPE, "DestType", mPoppler);

    G_DEF_SETTERS(cAction);
    G_DEF_SETTERS(cActionGotoDest);
    G_DEF_SETTERS(cActionGotoRemote);
    G_DEF_SETTERS(cActionLaunch);
    G_DEF_SETTERS(cActionUri);
    G_DEF_SETTERS(cActionNamed);
    G_DEF_SETTERS(cActionMovie);

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
