/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_type.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:38:47 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"
#include "st.h"

static VALUE gobject_type_hash;

void
rbgobj_register_class(const RGObjClassInfo* cinfo)
{
    VALUE data;

    if (!RTEST(rb_ary_includes(rb_mod_ancestors(cinfo->klass), rbgobj_cGObject)))
        fprintf(stderr,
                "rbgobj_register_class: %s is not a subclass of GLib::GObject\n",
                rb_class2name(cinfo->klass));
    if (G_TYPE_FUNDAMENTAL(cinfo->gtype) != G_TYPE_OBJECT)
        fprintf(stderr,
                "rbgobj_register_class: %s is not a subtype of GObject\n",
                 g_type_name(cinfo->gtype));

    data = Data_Wrap_Struct(rb_cData, NULL, NULL, (RGObjClassInfo*)cinfo);
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

VALUE
rbgobj_define_class(gtype, name, module, mark, free)
	GType gtype;
	gchar* name;
	VALUE module;
	void* mark;
	void* free;
{
	VALUE klass;
	RGObjClassInfo* cinfo;
	klass = rb_define_class_under(module, name, GTYPE2CLASS(g_type_parent(gtype)));
	cinfo = ALLOC(RGObjClassInfo);
	
	cinfo->klass = klass;
	cinfo->gtype = gtype;
	cinfo->mark = mark;
	cinfo->free = free;
/*
	rbgobj_define_property_acccessors(klass);
*/
	rbgobj_register_class(cinfo);

	return klass;
}

/**********************************************************************/

/*
 * Init
 */
void Init_gobject_gtype()
{
    g_type_init();

    rb_global_variable(&gobject_type_hash);
    gobject_type_hash = rb_hash_new();    
}
