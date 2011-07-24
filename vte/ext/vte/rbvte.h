#ifndef __RBVTE_H__
#define __RBVTE_H__

#include <ruby.h>
#include <rbgtk.h>

#include <vte/vte.h>
#include <vte/vteaccess.h>
#include <vte/reaper.h>

#ifndef VTE_CHECK_VERSION
#  include "rbvteversion.h"
#endif

#define RVAL2TERM(obj) (RVAL2GOBJ(obj))
#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))
#define COLOR2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_COLOR))
#define RVAL2AA(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ANTI_ALIAS))
#define RVAL2EB(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ERASE_BINDING))
#define RVAL2CT(obj) (RVAL2GENUM(obj, GDK_TYPE_CURSOR_TYPE))
#define RVAL2PFD(obj) ((PangoFontDescription*)RVAL2BOXED(self, PANGO_TYPE_FONT_DESCRIPTION))
#define PFD2RVAL(obj) (BOXED2RVAL(obj, PANGO_TYPE_FONT_DESCRIPTION))

#define RG_DEF_MODFUNC(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, "method", rg_m_ ## method, argc)
#define RG_DEF_MODFUNC_P(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, "method?", rg_m_ ## method ## _p, argc)
#define RG_DEF_MODFUNC_OPERATOR(ope, func, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, ope, rg_m_operator_ ## func, argc)
#define RG_DEF_SMETHOD(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "method", rg_s_ ## method, argc)
#define RG_DEF_SMETHOD_P(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "method?", rg_s_ ## method ## _p, argc)
#define RG_DEF_SMETHOD_OPERATOR(ope, func, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, ope, rg_s_operator_ ## func, argc)
#define RG_DEF_METHOD(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, "method", rg_ ## method, argc)
#define RG_DEF_METHOD_P(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, "method?", rg_ ## method ## _p, argc)
#define RG_DEF_METHOD_BANG(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, "method!", rg_ ## method ## _bang, argc)
#define RG_DEF_METHOD_OPERATOR(ope, func, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, ope, rg_operator_ ## func, argc)
#define RG_DEF_ATTR(attr, read, write, ex) \
        rb_attr(RG_TARGET_NAMESPACE, rb_intern(attr), read, write, ex)
#define RG_DEF_ALIAS(new, old) rb_define_alias(RG_TARGET_NAMESPACE, new, old)

extern void Init_vte_access(VALUE mVte);
extern void Init_vte_reaper(VALUE mVte);
extern void Init_vte_terminal(VALUE mVte);
extern void Init_vte_charattributes(VALUE mVte);

#endif
