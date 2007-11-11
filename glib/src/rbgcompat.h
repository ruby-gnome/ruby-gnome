/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************
  rbgcompat.h -

  Copyright (C) 2007  Ruby-GNOME2 Project Team
**********************************************************************/

#ifndef __RBGCOMPAT_H__
#define __RBGCOMPAT_H__

G_BEGIN_DECLS

typedef struct {
    GType type;
    VALUE (*get_superclass)(void);
    void (*type_init_hook)(VALUE);
    void (*rvalue2gvalue)(VALUE val, GValue *result);
    VALUE (*gvalue2rvalue)(const GValue *);
    void (*initialize)(VALUE, gpointer);
    gpointer (*robj2instance)(VALUE);
    VALUE (*instance2robj)(gpointer);
} RGFundamental;

#define G_DEF_FUNDAMENTAL(f) (rbgobj_fund_define_fundamental(f))

extern void rbgobj_fund_define_fundamental(RGFundamental *fundamental);

G_END_DECLS

#endif
