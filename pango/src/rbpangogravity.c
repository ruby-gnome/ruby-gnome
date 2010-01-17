/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangogravity.c -

  $Author: ito $
  $Date: 2010/01/17 03:00:50 $

************************************************/

#include "rbpango.h"


#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
to_rotation(VALUE self, VALUE gravity)
{
    return rb_float_new(pango_gravity_to_rotation(RVAL2GENUM(gravity, PANGO_TYPE_GRAVITY)));
}
#endif


void
Init_pango_gravity(void)
{
#if PANGO_CHECK_VERSION(1,16,0)
    VALUE mGravity = rb_define_module_under(mPango, "Gravity");

    rb_define_singleton_method(mGravity, "to_rotation", to_rotation, 1);

    G_DEF_CLASS(PANGO_TYPE_GRAVITY, "Gravity", mGravity);
    G_DEF_CONSTANTS(mGravity, PANGO_TYPE_GRAVITY, "PANGO_GRAVITY_");
    G_DEF_CLASS(PANGO_TYPE_GRAVITY_HINT, "Hint", mGravity);
    G_DEF_CONSTANTS(mGravity, PANGO_TYPE_GRAVITY_HINT, "PANGO_GRAVITY_");
#endif
}

