#ifndef _RBBONOBO_H_included
#define _RBBONOBO_H_included

#define ENABLE_NLS 1

#include <bonobo.h>
#include "rbgtk.h"

void Init_bonobo_dock(VALUE);
void Init_bonobo_dock_layout(VALUE);
void Init_bonobo_dock_item(VALUE);
void Init_bonobo_dock_band(VALUE);
void Init_bonobo_i18n(VALUE);

#endif /* ! _RBBONOBO_H_included */
