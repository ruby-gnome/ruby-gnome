/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbobjmanager.h -

  $Author: mutoh $
  $Date: 2002/08/18 06:24:51 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "ruby.h"

#ifndef _RBOBJ_H
#define _RBOBJ_H

/* Class/Object Management */
typedef struct {
    VALUE klass;
	int size;
    void (*mark)(void*);
	void* (*malloc)(void*);
    void (*free)(void*);
} RBClassInfo;

#define RB_DEF_CLASS0(name, module, type)\
   (rbobj_define_class(name, module, "Data", sizeof(type), 0, 0, free))
#define RB_DEF_CLASS(name, module, parent_id, type)\
   (rbobj_define_class(name, module, parent_id, sizeof(type), 0, 0, free))
#define RB_DEF_CLASS2(name, module, parent_id, type, mark, rmalloc, rfree)\
   (rbobj_define_class(name, module, parent_id, sizeof(type), mark, rmalloc, rfree))

#define RB_STR2CINFO(id) (rbobj_lookup_cinfo(rb_intern(id)))
#define RB_STR2CLASS(id) (RB_STR2CINFO(id)->klass)
#define COBJ2RVAL(id, cobj) (rbobj_cobj2rval(id, (void*)cobj))
#define RVAL2COBJ(id, rval) (rbobj_rval2cobj(id, rval))

extern VALUE rbobj_define_class(char* name, VALUE module, char* parent_id, int size, 
								 void* mark, void* rmalloc, void* rfree);
extern RBClassInfo* rbobj_lookup_cinfo(ID id);
extern void* rbobj_rval2cobj(char* id, VALUE obj);
extern VALUE rbobj_cobj2rval(char* id, void* cobj);
extern void Init_objmanager();

#endif /* _RBOBJ_H */
