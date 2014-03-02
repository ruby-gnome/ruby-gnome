#ifndef __RBVTE3PRIVATE_H__
#define __RBVTE3PRIVATE_H__

#include <vte/vte.h>
#include <vte/vteaccess.h>
#include <vte/reaper.h>

#include <ruby.h>
#include <rbpango.h>
#include <rbgdk3.h>
#include <rbgtk3.h>
#include "rbvte3conversions.h"
#include "rbgio2conversions.h"
#ifndef VTE_CHECK_VERSION
#  include "rbvteversion.h"
#endif

G_GNUC_INTERNAL void Init_vte_access(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_reaper(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_terminal(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_charattributes(VALUE mVte);
G_GNUC_INTERNAL void Init_vte_pty(VALUE mVte);

#endif
