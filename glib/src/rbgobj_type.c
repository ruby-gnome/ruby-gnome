/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_type.c -

  $Author: sakai $
  $Date: 2002/08/05 17:45:56 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"
#include "st.h"

static ID id_new;
static ID id_class_info;
static VALUE gtype_to_cinfo;

static void
cinfo_mark(RGObjClassInfo* cinfo)
{
    rb_gc_mark(cinfo->klass);
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
    RGObjClassInfo* cinfo;
    VALUE c = rb_hash_aref(gtype_to_cinfo, INT2NUM(gtype));

    if (!NIL_P(c)){
        Data_Get_Struct(c, RGObjClassInfo, cinfo);
    } else {
        c = Data_Make_Struct(rb_cData, RGObjClassInfo, cinfo_mark, free, cinfo);

        switch (G_TYPE_FUNDAMENTAL(gtype)){
          case G_TYPE_OBJECT:
            if (gtype == G_TYPE_OBJECT){
                cinfo->klass = rb_funcall(rb_cClass, id_new, 0);
            } else {
                const RGObjClassInfo* cinfo_super
                    = rbgobj_lookup_class_by_gtype(g_type_parent(gtype));
                cinfo->klass = rb_funcall(rb_cClass, id_new, 1, cinfo_super->klass);
            }
            break;

          case G_TYPE_INTERFACE:
            cinfo->klass = rb_module_new();
            break;

          default:
            /* we should raise exception? */
            fprintf(stderr,
                    "%s: %s's fundamental type %s isn't supported\n",
                    "rbgobj_lookup_class_by_gtype",
                    g_type_name(gtype),
                    g_type_name(G_TYPE_FUNDAMENTAL(gtype)));
            return NULL;
        }

        cinfo->gtype = gtype;
        cinfo->mark  = NULL;
        cinfo->free  = NULL;

        rb_ivar_set(cinfo->klass, id_class_info, c);
        rb_hash_aset(gtype_to_cinfo, INT2NUM(gtype), c);

        if (G_TYPE_FUNDAMENTAL(gtype) == G_TYPE_OBJECT){
            GType* interfaces = NULL;
            guint n_interfaces = 0;
            int i;

            interfaces = g_type_interfaces(gtype, &n_interfaces);
            for (i = 0; i < n_interfaces; i++){
                rb_include_module(
                    cinfo->klass,
                    rbgobj_lookup_class_by_gtype(interfaces[i])->klass);
            }
            g_free(interfaces);

            rbgobj_define_signal_constants(cinfo->klass);
            rbgobj_define_property_accessors(cinfo->klass);
        }
    }

    return cinfo;
}

VALUE
rbgobj_define_class(gtype, name, module, mark, free)
	GType gtype;
	gchar* name;
	VALUE module;
	void* mark;
	void* free;
{
        RGObjClassInfo* cinfo = (RGObjClassInfo*)rbgobj_lookup_class_by_gtype(gtype);
        cinfo->mark = mark;
        cinfo->free = free;
        rb_define_const(module, name, cinfo->klass);
        return cinfo->klass;
}

/**********************************************************************/

/*
 * Init
 */
void Init_gobject_gtype()
{
    g_type_init();
    id_new = rb_intern("new");
    id_class_info = rb_intern("__gobject_class_info__");
    rb_global_variable(&gtype_to_cinfo);
    gtype_to_cinfo = rb_hash_new();    
}
