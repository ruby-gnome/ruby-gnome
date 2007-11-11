/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************
  rbgcompat.h -

  Copyright (C) 2007  Ruby-GNOME2 Project Team
**********************************************************************/

#ifndef __RBGCOMPAT_H__
#define __RBGCOMPAT_H__

G_BEGIN_DECLS

typedef RGConvertTable RGFundamental;

#define G_DEF_FUNDAMENTAL(f) RG_DEF_CONVERSION(f)
#define rbgobj_fund_define_fundamental(f) rbgobj_convert_define(f)

G_END_DECLS

#endif
