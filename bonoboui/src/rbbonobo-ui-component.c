/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo-ui-engine.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonoboui.h"

#define _SELF(self) (BONOBO_UI_COMPONENT(RVAL2GOBJ(self)))


static VALUE
ucomponent_construct(self, name)
    VALUE self, name;
{
    return GOBJ2RVAL(bonobo_ui_component_construct(_SELF(self), RVAL2CSTR(name)));
}

static VALUE
ucomponent_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    if (NIL_P(name)){
        RBGTK_INITIALIZE(self, bonobo_ui_compoent_new_default());
    } else {
        RBGTK_INITIALIZE(self, bonobo_ui_compoent_new(RVAL2CSTR(name)));
    }
    return Qnil;
}  

static VALUE
ucomponent_set_name(self, name)
    VALUE self, name;
{
    bonobo_ui_component_set_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
ucomponent_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(bonobo_ui_component_get_name(_SELF(self)));
}

/*
void               bonobo_ui_component_set_container   (BonoboUIComponent  *component,
							Bonobo_UIContainer  container,
							CORBA_Environment  *opt_ev);
void               bonobo_ui_component_unset_container (BonoboUIComponent  *component,
							CORBA_Environment  *opt_ev);
Bonobo_UIContainer bonobo_ui_component_get_container   (BonoboUIComponent  *component);
void               bonobo_ui_component_add_verb        (BonoboUIComponent  *component,
							const char         *cname,
							BonoboUIVerbFn      fn,
							gpointer            user_data);

void               bonobo_ui_component_add_verb_full   (BonoboUIComponent  *component,
							const char         *cname,
							GClosure           *closure);

void               bonobo_ui_component_remove_verb            (BonoboUIComponent  *component,
							       const char         *cname);

void               bonobo_ui_component_remove_verb_by_closure (BonoboUIComponent  *component,
							       GClosure           *closure);

void               bonobo_ui_component_add_listener        (BonoboUIComponent  *component,
							    const char         *id,
							    BonoboUIListenerFn  fn,
							    gpointer            user_data);

void               bonobo_ui_component_add_listener_full   (BonoboUIComponent  *component,
							    const char         *id,
							    GClosure           *closure);

void               bonobo_ui_component_remove_listener            (BonoboUIComponent  *component,
								   const char         *cname);

void               bonobo_ui_component_remove_listener_by_closure (BonoboUIComponent  *component,
								   GClosure           *closure);

void               bonobo_ui_component_set          (BonoboUIComponent  *component,
						     const char         *path,
						     const char         *xml,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_set_translate(BonoboUIComponent  *component,
						     const char         *path,
						     const char         *xml,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_set_tree     (BonoboUIComponent  *component,
						     const char         *path,
						     BonoboUINode       *node,
						     CORBA_Environment  *ev);

void               bonobo_ui_component_rm           (BonoboUIComponent  *component,
						     const char         *path,
						     CORBA_Environment  *ev);

gboolean           bonobo_ui_component_path_exists  (BonoboUIComponent  *component,
						     const char         *path,
						     CORBA_Environment  *ev);

CORBA_char        *bonobo_ui_component_get          (BonoboUIComponent  *component,
						     const char         *path,
						     gboolean            recurse,
						     CORBA_Environment  *opt_ev);

BonoboUINode      *bonobo_ui_component_get_tree     (BonoboUIComponent  *component,
						     const char         *path,
						     gboolean            recurse,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_object_set   (BonoboUIComponent  *component,
						     const char         *path,
						     Bonobo_Unknown      control,
						     CORBA_Environment  *ev);

Bonobo_Unknown     bonobo_ui_component_object_get   (BonoboUIComponent  *component,
						     const char         *path,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_freeze       (BonoboUIComponent  *component,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_thaw         (BonoboUIComponent  *component,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_set_prop     (BonoboUIComponent  *component,
						     const char         *path,
						     const char         *prop,
						     const char         *value,
						     CORBA_Environment  *opt_ev);

gchar             *bonobo_ui_component_get_prop     (BonoboUIComponent  *component,
						     const char         *path,
						     const char         *prop,
						     CORBA_Environment  *opt_ev);

void               bonobo_ui_component_set_status   (BonoboUIComponent  *component,
						     const char         *text,
						     CORBA_Environment  *opt_ev);

typedef struct {
	const char    *cname;
	BonoboUIVerbFn cb;
	gpointer       user_data;
	gpointer       dummy;
} BonoboUIVerb;

#define BONOBO_UI_VERB(name,cb)                  { (name), (cb), NULL   }
#define BONOBO_UI_VERB_DATA(name,cb,data)        { (name), (cb), (data) }
#define BONOBO_UI_UNSAFE_VERB(name,cb)           { (name), ((BonoboUIVerbFn)(cb)), NULL   }
#define BONOBO_UI_UNSAFE_VERB_DATA(name,cb,data) { (name), ((BonoboUIVerbFn)(cb)), (data) }
#define BONOBO_UI_VERB_END                       { NULL, NULL, NULL }

void    bonobo_ui_component_add_verb_list           (BonoboUIComponent  *component,
						     const BonoboUIVerb *list);
void    bonobo_ui_component_add_verb_list_with_data (BonoboUIComponent  *component,
						     const BonoboUIVerb *list,
						     gpointer            user_data);

*/

void
Init_bonobo_ui_component()
{
    VALUE uiComponent = G_DEF_CLASS(BONOBO_TYPE_UI_COMPONENT, "Component", mBonoboUI);

    rb_define_methods(uiComponent, "construct", ucomponent_construct, 1);
    rb_define_methods(uiComponent, "initialize", ucomponent_initialize, -1);
    rb_define_methods(uiComponent, "set_name", ucomponent_set_name, 1);
    rb_define_methods(uiComponent, "name", ucomponent_get_name, 0);

    G_DEF_SETTERS(uiComponent);
}
