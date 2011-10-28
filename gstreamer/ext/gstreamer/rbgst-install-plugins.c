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
#include "rbgst.h"
#include "rbgst-private.h"
#include <gst/pbutils/pbutils-enumtypes.h>
#include <gst/pbutils/install-plugins.h>

#define RG_TARGET_NAMESPACE mInstallPlugins

static RGConvertTable context_table = {0};
static VALUE RG_TARGET_NAMESPACE;
static VALUE rb_cGstInstallPluginsReturn;
static VALUE rb_cGstInstallPluginsContext;

static VALUE
return_get_name(VALUE self)
{
    return CSTR2RVAL(gst_install_plugins_return_get_name(
                     (GstInstallPluginsReturn)
                     RVAL2GENUM(self, GST_TYPE_INSTALL_PLUGINS_RETURN)));
}

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
      if (!RVAL2CBOOL(rb_obj_is_kind_of(rcontext, rb_cGstInstallPluginsContext)))
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

static VALUE
context2robj(gpointer context)
{
    return Data_Wrap_Struct(rb_cGstInstallPluginsContext, NULL,
                            gst_install_plugins_context_free,
                            (GstInstallPluginsContext *)context);
}

static gpointer
robj2context(VALUE object)
{
    gpointer instance;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, rb_cGstInstallPluginsContext))) {
      rb_raise(rb_eTypeError, "not a Gst::InstallPluginsContext");
    }
    Data_Get_Struct(object, GstInstallPluginsContext, instance);
    return instance;
}

static VALUE
context_initialize(VALUE self)
{
    GstInstallPluginsContext *context;

    context = gst_install_plugins_context_new();
    G_INITIALIZE(self, context);
    return Qnil;
}

static VALUE
context_set_xid(VALUE self, VALUE xid)
{
    GstInstallPluginsContext *context;

    context = (GstInstallPluginsContext *)RVAL2GOBJ(self);
    gst_install_plugins_context_set_xid(context, NUM2INT(xid));/*FIXME: segfault on ruby exit*/
    return self;
}

void
Init_gst_install_plugins(void)
{
    RG_TARGET_NAMESPACE = rb_define_module_under(mGst, "InstallPlugins");

    rb_cGstInstallPluginsReturn = G_DEF_CLASS(GST_TYPE_INSTALL_PLUGINS_RETURN,
                                              "InstallPluginsReturn", mGst);
    rb_define_method(rb_cGstInstallPluginsReturn, "name", return_get_name,
                     0);

    RG_DEF_SMETHOD_P(supported, 0);
    RG_DEF_SMETHOD_P(progress, 0);
    RG_DEF_SMETHOD(async, -1);
    RG_DEF_SMETHOD(sync, -1);

    context_table.type = GST_TYPE_INSTALL_PLUGINS_CONTEXT;
    context_table.instance2robj = context2robj;
    context_table.robj2instance = robj2context;
    RG_DEF_CONVERSION(&context_table);
    rb_cGstInstallPluginsContext = G_DEF_CLASS(GST_TYPE_INSTALL_PLUGINS_CONTEXT,
                                               "InstallPluginsContext", mGst);
    rb_define_method(rb_cGstInstallPluginsContext, "initialize",
                     context_initialize, 0);
    rb_define_method(rb_cGstInstallPluginsContext, "set_xid",
                     context_set_xid, 1);
    G_DEF_SETTERS(rb_cGstInstallPluginsContext);
}
#endif /* HAVE_GST_PBUTILS */