/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_type.c -

  $Author: sakai $
  $Date: 2002/06/20 13:46:36 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"
#include "st.h"

static VALUE gobject_type_hash;

void
rbgobj_register_class(const RGObjClassInfo* cinfo)
{
    VALUE data = Data_Wrap_Struct(rb_cData, NULL, NULL,
                                  (RGObjClassInfo*)cinfo);
    rb_ivar_set(cinfo->klass, id_class_info, data);
    rb_hash_aset(gobject_type_hash, INT2NUM(cinfo->gtype), cinfo->klass);
}

const RGObjClassInfo *
rbgobj_lookup_class(klass)
    VALUE klass;
{
    RGObjClassInfo *cinfo = NULL;
    if (RTEST(rb_ivar_defined(klass, id_class_info))) {
	VALUE data = rb_ivar_get(klass, id_class_info);
	if (!NIL_P(data))
	    Data_Get_Struct(data, RGObjClassInfo, cinfo);
    }
    return cinfo;
}

const RGObjClassInfo *
rbgobj_lookup_class_by_gtype(gtype)
    GType gtype;
{
    VALUE klass = rb_hash_aref(gobject_type_hash, INT2NUM(gtype));

    if (NIL_P(klass))
        return NULL;
    else
        return rbgobj_lookup_class(klass);
}

VALUE
rbgobj_lookup_rbclass(const GObject* gobj)
{
    GType gtype;

    for (gtype = G_OBJECT_TYPE(gobj);
         gtype != G_TYPE_INVALID;
         gtype = g_type_parent(gtype))
    {
        VALUE klass = rb_hash_aref(gobject_type_hash, INT2NUM(gtype));
        if (!NIL_P(klass))
            return klass;
    }

    rb_raise(rb_eTypeError, "not a GObject");
}

/**********************************************************************/

/*
 * Init
 */
void Init_gobject_gtype()
{
    rb_global_variable(&gobject_type_hash);
    gobject_type_hash = rb_hash_new();    
}
