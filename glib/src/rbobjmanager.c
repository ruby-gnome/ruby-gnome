/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbobjmanager.c -

  $Author: sakai $
  $Date: 2002/09/01 13:19:22 $

  Copyright (C) 2002 Masao Mutoh
************************************************/
#include "rbobjmanager.h"

static VALUE rbobj_classinfo_hash;

static void
rbobj_cinfo_mark(RBClassInfo* cinfo)
{
	rb_gc_mark(cinfo->klass);
}

RBClassInfo*
rbobj_lookup_cinfo(id)
    ID id; /* ID */
{
	RBClassInfo* cinfo = NULL;
	VALUE data = rb_hash_aref(rbobj_classinfo_hash, id);

    if (!NIL_P(data)){
        Data_Get_Struct(data, RBClassInfo, cinfo);
    } else {
		rb_bug("ruby-util: there are no RBClassInfo. <%s>\n", 
			   rb_id2name(id));
	}
	return cinfo;
}

VALUE
rbobj_define_class(name, module, parent_id, size, mark, rmalloc, rfree)
	char* name;
	VALUE module;
	char* parent_id;
	int size;
	void* mark;
	void* rmalloc;
	void* rfree;
{
	RBClassInfo* cinfo;
	VALUE klass, value;

	klass = rb_define_class_under(module, name, RB_STR2CLASS(parent_id));
	value = Data_Make_Struct(rb_cData, RBClassInfo, 
							 rbobj_cinfo_mark, free, cinfo);
	cinfo->klass = klass;
	cinfo->mark = mark;
	cinfo->malloc = rmalloc;
	cinfo->free = rfree;
	cinfo->size = size;

	rb_hash_aset(rbobj_classinfo_hash, 
				 rb_intern(rb_class2name(klass)), value);

	return klass;
}

void*
rbobj_rval2cobj(id, obj)
	char* id;
	VALUE obj;
{
	void* cobj;
	VALUE klass;

	if (NIL_P(obj)) return NULL;

	klass = RB_STR2CLASS(id);

	if (!rb_obj_is_instance_of(obj, klass)) {
		rb_raise(rb_eTypeError, "not a %s", rb_class2name(klass));
	}
	Data_Get_Struct(obj, void, cobj);

	return cobj;
}
 
VALUE
rbobj_cobj2rval(id, cobj)
	char* id;
    void* cobj;
{
	RBClassInfo* cinfo;
	VALUE obj;
	void* copy = NULL;

	cinfo = RB_STR2CINFO(id);
	if (cinfo->malloc != 0){
		copy = (void*)cinfo->malloc(cobj);
	} else {
		copy = xmalloc(cinfo->size);
		if (cobj != NULL){
			memcpy(copy, cobj, cinfo->size);
		} 
	}
	if (copy == NULL){
		rb_raise(rb_eTypeError, "can not allocate %s", id);
	}

	obj = Data_Wrap_Struct(cinfo->klass, cinfo->mark, 
						   cinfo->free, copy);
	return obj;
}

void 
Init_objmanager()
{
	VALUE value;
	RBClassInfo* cinfo;

    rb_global_variable(&rbobj_classinfo_hash);
    rbobj_classinfo_hash = rb_hash_new();

	value = Data_Make_Struct(rb_cData, RBClassInfo, 
							 rbobj_cinfo_mark, free, cinfo);
	cinfo->klass = rb_cData;
	cinfo->mark = 0;
	cinfo->free = 0;
	cinfo->size = 0;

	rb_hash_aset(rbobj_classinfo_hash, rb_intern(rb_class2name(rb_cData)), value);
}
