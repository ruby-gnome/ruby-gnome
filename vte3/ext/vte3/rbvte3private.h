#ifndef __RBVTE3PRIVATE_H__
#define __RBVTE3PRIVATE_H__

#include <vte/vte.h>
#include <vte/vteaccess.h>
#include <vte/reaper.h>

#include <ruby.h>
#include <rbgio2.h>
#include <rbpango.h>
#include <rbgdk3.h>
#include <rbgtk3.h>
#include "rbvte3conversions.h"
#ifndef VTE_CHECK_VERSION
#  include "rbvteversion.h"
#endif

/* TODO: move to rbgutil.h */
#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rbg_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rbg_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

G_GNUC_INTERNAL void Init_vte_access(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_reaper(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_terminal(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_charattributes(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_pty(VALUE mVte);

#endif
