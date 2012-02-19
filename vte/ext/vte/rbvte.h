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

#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

#define RVAL2TERM(obj) (RVAL2GOBJ(obj))
#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))
#define COLOR2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_COLOR))
#define RVAL2AA(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ANTI_ALIAS))
#define RVAL2EB(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ERASE_BINDING))
#define RVAL2CT(obj) (RVAL2GENUM(obj, GDK_TYPE_CURSOR_TYPE))
#define RVAL2PFD(obj) ((PangoFontDescription*)RVAL2BOXED(self, PANGO_TYPE_FONT_DESCRIPTION))
#define PFD2RVAL(obj) (BOXED2RVAL(obj, PANGO_TYPE_FONT_DESCRIPTION))

G_GNUC_INTERNAL void Init_vte_access(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_reaper(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_terminal(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_charattributes(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_pty(VALUE mVte);

#endif
