#ifndef __RBVTE_H__
#define __RBVTE_H__

#include <ruby.h>
#include <rbgtk.h>

#include <vte/vte.h>
#include <vte/vteaccess.h>
#include <vte/reaper.h>

#include "rbvteversion.h"

#define RVAL2TERM(obj) (RVAL2GOBJ(obj))
#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))
#define COLOR2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_COLOR))
#define RVAL2AA(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ANTI_ALIAS))
#define RVAL2EB(obj) (RVAL2GENUM(obj, VTE_TYPE_TERMINAL_ERASE_BINDING))
#define RVAL2CT(obj) (RVAL2GENUM(obj, GDK_TYPE_CURSOR_TYPE))
#define RVAL2PFD(obj) ((PangoFontDescription*)RVAL2BOXED(self, PANGO_TYPE_FONT_DESCRIPTION))
#define PFD2RVAL(obj) (BOXED2RVAL(obj, PANGO_TYPE_FONT_DESCRIPTION))

extern void Init_vte_access(VALUE mVte);
extern void Init_vte_reaper(VALUE mVte);
extern void Init_vte_terminal(VALUE mVte);

#endif
