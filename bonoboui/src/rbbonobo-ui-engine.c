/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo-ui-engine.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonoboui.h"

#define _SELF(self) (BONOBO_UI_ENGINE(RVAL2GOBJ(self)))

static VALUE
rbonobo_ue_deregister_dead_components(self)
    VALUE self;
{
    bonobo_ui_engine_deregister_dead_components(_SELF(self));
    return self;
}
/*
void bonobo_ui_engine_deregister_component_by_ref    (BonoboUIEngine *engine,
						      Bonobo_Unknown  ref);
*/

static VALUE
rbonobo_ue_deregister_component(self, name)
    VALUE self, name;
{
    bonobo_ui_engine_deregister_component(_SELF(self), RVAL2CSTR(name));
    return self;
}

/*
void bonobo_ui_engine_register_component             (BonoboUIEngine *engine,
						      const char     *name,
						      Bonobo_Unknown  component);
GList         *bonobo_ui_engine_get_component_names  (BonoboUIEngine *engine);
Bonobo_Unknown bonobo_ui_engine_get_component        (BonoboUIEngine *engine,
						      const char     *name);
*/

static VALUE
rbonobo_ue_config_set_path(self, path)
    VALUE self, path;
{
    bonobo_ui_engine_config_set_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

static VALUE
rbonobo_ue_config_get_path(self)
    VALUE self;
{
    return CSTR2RVAL(bonobo_ui_engine_config_get_path(_SELF(self)));
}

static VALUE
rbonobo_ue_set_ui_container(self, ui_container)
    VALUE self, ui_container;
{
    bonobo_ui_engine_set_ui_container(_SELF(self), BONOBO_UI_CONTAINER(RVAL2GOBJ(ui_container)));
    return self;
}

static VALUE
rbonobo_ue_get_ui_container(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_ui_engine_get_ui_container(_SELF(self)));
}

void
Init_bonobo_ui_engine()
{
    VALUE uiEngine = G_DEF_CLASS(BONOBO_TYPE_WINDOW, "Engine", mBonoboUI);

    rb_define_method(uiEngine, "deregister_dead_components", rbonobo_ue_deregister_dead_components, 0);
    rb_define_method(uiEngine, "deregister_component", rbonobo_ue_deregister_component, 1);
    rb_define_method(uiEngine, "config_set_path", rbonobo_ue_config_set_path, 1);
    rb_define_method(uiEngine, "config_get_path", rbonobo_ue_config_get_path, 0);
    rb_define_method(uiEngine, "set_ui_container", rbonobo_ue_set_ui_container, 1);
    rb_define_method(uiEngine, "ui_container", rbonobo_ue_get_ui_container, 0);

    G_DEF_SETTERS(uiEngine);
}
