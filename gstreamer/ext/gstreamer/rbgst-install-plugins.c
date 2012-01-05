/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#ifdef HAVE_GST_PBUTILS
#include "rbgst-private.h"
#include <gst/pbutils/pbutils-enumtypes.h>
#include <gst/pbutils/install-plugins.h>

#define RG_TARGET_NAMESPACE mInstallPlugins

static VALUE RG_TARGET_NAMESPACE;
static VALUE cInstallPluginsContext;

static void
gst_install_plugins_result_func(GstInstallPluginsReturn result, VALUE data)
{
    rb_funcall(data, rb_intern("call"), 1, 
               GENUM2RVAL(result, GST_TYPE_INSTALL_PLUGINS_RETURN));
    G_CHILD_REMOVE(RG_TARGET_NAMESPACE, data);
}

static VALUE
rg_s_supported_p(VALUE self)
{
    return CBOOL2RVAL(gst_install_plugins_supported());
}

static VALUE
rg_s_progress_p(VALUE self)
{
    return CBOOL2RVAL(gst_install_plugins_installation_in_progress());
}

static VALUE
rg_s_async(int argc, VALUE *argv, VALUE self)
{
    VALUE details, rcontext, block;
    int length, i;
    char **carray;
    VALUE *str;
    GstInstallPluginsContext *context;
    GstInstallPluginsReturn result;

    rb_scan_args(argc, argv, "11", &details, &rcontext);

/*Define a macro in rbgtuil.h for this. RARRAY2CSTRARRAY(VALUE, ??)*/
    length = RARRAY_LEN(details);
    str = RARRAY_PTR(details);
    carray= ALLOCA_N (char *, length+1);
    for (i = 0; i<length; i++) {
      carray[i] = RVAL2CSTR(str[i]);
    }
    carray[length] = NULL;

    if (!NIL_P(rcontext)) {
      if (!RVAL2CBOOL(rb_obj_is_kind_of(rcontext, cInstallPluginsContext)))
        rb_raise(rb_eTypeError,
                 "2nd parameter is not Gst::InstallPluginsContext");
        context = (GstInstallPluginsContext *)RVAL2GOBJ(rcontext);
    }
    else {
      context = NULL;
    }

    block = rb_block_proc();
    G_CHILD_ADD(self, block);

    result = gst_install_plugins_async(carray, context,
               (GstInstallPluginsResultFunc)gst_install_plugins_result_func,
               (gpointer)block); 
    return  GENUM2RVAL(result, GST_TYPE_INSTALL_PLUGINS_RETURN);
}

static VALUE
rg_s_sync(int argc, VALUE *argv, VALUE self)
{
    VALUE details, context;
    int length, i;
    char **carray;
    VALUE *str;
    GstInstallPluginsReturn result;

    rb_scan_args(argc, argv, "11", &details, &context);

    length = RARRAY_LEN(details);
    str = RARRAY_PTR(details);
    carray= ALLOCA_N (char *, length+1);
    for (i = 0; i<length; i++) {
      carray[i] = RVAL2CSTR(str[i]);
    }
    carray[length] = NULL;

    result = gst_install_plugins_sync(carray, NULL);
    return  GENUM2RVAL(result, GST_TYPE_INSTALL_PLUGINS_RETURN);
}

void
Init_gst_install_plugins(VALUE mGst)
{
    RG_TARGET_NAMESPACE = rb_define_module_under(mGst, "InstallPlugins");
    cInstallPluginsContext = rb_const_get(mGst, rb_intern("InstallPluginsContext"));

    RG_DEF_SMETHOD_P(supported, 0);
    RG_DEF_SMETHOD_P(progress, 0);
    RG_DEF_SMETHOD(async, -1);
    RG_DEF_SMETHOD(sync, -1);
}
#endif /* HAVE_GST_PBUTILS */
