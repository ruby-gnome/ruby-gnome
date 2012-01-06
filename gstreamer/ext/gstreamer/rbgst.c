/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE mGst

void Init_gstreamer (void);

/* Module: Gst
 * The GStreamer module.
 */
VALUE RG_TARGET_NAMESPACE;

static void
Init_gst_classes (void)
{
    Init_gst_bin (RG_TARGET_NAMESPACE);
    Init_gst_bus(RG_TARGET_NAMESPACE);
    Init_gst_caps (RG_TARGET_NAMESPACE);
    Init_gst_child_proxy(RG_TARGET_NAMESPACE);
    Init_gst_clock (RG_TARGET_NAMESPACE);
    Init_gst_clock_entry (RG_TARGET_NAMESPACE);
    Init_gst_element (RG_TARGET_NAMESPACE);
    Init_gst_elementfactory (RG_TARGET_NAMESPACE);
    Init_gst_seek(RG_TARGET_NAMESPACE);
    Init_gst_format (RG_TARGET_NAMESPACE);
    Init_gst_structure(RG_TARGET_NAMESPACE);
    Init_gst_index (RG_TARGET_NAMESPACE);
    Init_gst_indexentry (RG_TARGET_NAMESPACE);
    Init_gst_indexfactory (RG_TARGET_NAMESPACE);
    Init_gst_object (RG_TARGET_NAMESPACE);
    Init_gst_pad (RG_TARGET_NAMESPACE);
    Init_gst_pad_template(RG_TARGET_NAMESPACE);
    Init_gst_ghost_pad(RG_TARGET_NAMESPACE);
    Init_gst_parse (RG_TARGET_NAMESPACE);
    Init_gst_pipeline (RG_TARGET_NAMESPACE);
    Init_gst_plugin (RG_TARGET_NAMESPACE);
    Init_gst_plugin_feature(RG_TARGET_NAMESPACE);
    Init_gst_querytype (RG_TARGET_NAMESPACE);
    Init_gst_registry (RG_TARGET_NAMESPACE);
    Init_gst_static_caps(RG_TARGET_NAMESPACE);
    Init_gst_static_pad_template(RG_TARGET_NAMESPACE);
    Init_gst_systemclock (RG_TARGET_NAMESPACE);
    Init_gst_tag (RG_TARGET_NAMESPACE);
    Init_gst_tag_setter (RG_TARGET_NAMESPACE);
    Init_gst_typefindfactory (RG_TARGET_NAMESPACE);
    Init_gst_value(RG_TARGET_NAMESPACE);
    Init_gst_xml (RG_TARGET_NAMESPACE);

    Init_gst_mini_object(RG_TARGET_NAMESPACE);
    Init_gst_buffer(RG_TARGET_NAMESPACE);
    Init_gst_message(RG_TARGET_NAMESPACE);
    Init_gst_query(RG_TARGET_NAMESPACE);
    Init_gst_event(RG_TARGET_NAMESPACE);

#if defined(HAVE_GST_OVERLAY)
    Init_gst_x_overlay (RG_TARGET_NAMESPACE);
#endif

#ifdef HAVE_MEDIA_INFO
    Init_gst_mediatype (RG_TARGET_NAMESPACE);
#endif

#ifdef HAVE_GST_PBUTILS
    Init_gst_install_plugins_context (RG_TARGET_NAMESPACE);
    Init_gst_install_plugins (RG_TARGET_NAMESPACE);
    Init_gst_install_plugins_return (RG_TARGET_NAMESPACE);
#endif
}

/*
 * Class method: init(*args)
 *
 * Initializes the GStreamer library (using *args), setting up internal path 
 * lists, registering built-in elements, and loading standard plugins.
 *  
 * If *args are ommited, GStreamer will be initialized using arguments from the 
 * Ruby command line.
 *
 * A RuntimeError exception will be raised if GStreamer could not be initialized.
 *
 * Returns: always true, or an exception on failure.
 */

static VALUE
rg_m_init (int argc, VALUE * argv, VALUE self)
{
    gint i, gargc;
    VALUE argary;
    gchar **gargv;
    GError *error = NULL;
    static gboolean initialized = FALSE;

    if (initialized)
        return Qtrue;
    initialized = TRUE;

    /*
     * Convert arguments in a Ruby array.
     * If no arguments are provided, we use the command line ones.
     */
    rb_scan_args (argc, argv, "01", &argary);

    if (NIL_P (argary)) {
        argary = rb_const_get(rb_cObject, rb_intern("ARGV"));
        gargc = RARRAY_LEN(argary);
    } else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY_LEN(argary);
    }

    /*
     * Fill the gargv array with the provided arguments.
     */
    gargv = ALLOCA_N (char *, gargc + 1);

    gargv[0] = RVAL2CSTR(rb_argv0);

    for (i = 0; i < gargc; i++) {
        if (TYPE (RARRAY_PTR(argary)[i]) == T_STRING) {
            gargv[i + 1] = RVAL2CSTR(RARRAY_PTR(argary)[i]);
        } else {
            gargv[i + 1] = "";
        }
    }
    gargc++;

    /*
     * Now, we can initialize GStreamer.
     */
    if (!gst_init_check(&gargc, &gargv, &error))
        RAISE_GERROR(error);

    /*
     * Initialize all included classes 
     */
    Init_gst_classes ();

    rbgutil_start_callback_dispatch_thread();

    return Qtrue;
}

/*
 * Class method: version
 *
 * Gets the version number of the GStreamer library, in an array
 * of 3 fixnums, which represent major, minor and macro numbers. 
 *
 *  # Prints GStreamer version in a String 'major.minor.macro.nano'
 *  p Gst.version.join('.')
 *
 * Returns: an Array of 4 fixnums (major, minor, macro and nano numbers).
 */
static VALUE
rg_m_version (VALUE self)
{
    guint major, minor, micro, nano;

    gst_version(&major, &minor, &micro, &nano);
    return rb_ary_new3(4,
                       INT2NUM(major), INT2NUM(minor),
                       INT2NUM(micro), INT2NUM(nano));
}

static VALUE
rg_m_check_version_p(VALUE self, VALUE rb_major, VALUE rb_minor, VALUE rb_micro)
{
    guint major, minor, micro, nano;

    gst_version(&major, &minor, &micro, &nano);
    return CBOOL2RVAL(major > NUM2UINT(rb_major) ||
                      (major == NUM2UINT(rb_major) &&
                       minor > NUM2UINT(rb_minor)) ||
                      (major == NUM2UINT(rb_major) &&
                       minor == NUM2UINT(rb_minor) &&
                       micro >= NUM2UINT(rb_micro)));
}

void
Init_gstreamer (void)
{
    RG_TARGET_NAMESPACE = rb_define_module ("Gst");

    RG_DEF_MODFUNC(init, -1);
    RG_DEF_MODFUNC(version, 0);

    /*
     * Constant: VERSION_MAJOR
     * GStreamer's major version.
     */
    rb_define_const(RG_TARGET_NAMESPACE, "VERSION_MAJOR", INT2FIX(GST_VERSION_MAJOR));

    /*
     * Constant: VERSION_MINOR
     * GStreamer's minor version.
     */
    rb_define_const(RG_TARGET_NAMESPACE, "VERSION_MINOR", INT2FIX(GST_VERSION_MINOR));

    /*
     * Constant: VERSION_MICRO
     * GStreamer's micro version.
     */
    rb_define_const(RG_TARGET_NAMESPACE, "VERSION_MICRO", INT2FIX(GST_VERSION_MICRO));

    /*
     * Constant: BUILD_VERSION
     * GStreamer's build version.
     */
    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GST_VERSION_MAJOR),
                                INT2FIX(GST_VERSION_MINOR),
                                INT2FIX(GST_VERSION_MICRO)));

    RG_DEF_MODFUNC_P(check_version, 3);
}
