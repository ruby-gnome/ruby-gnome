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

#include "rbgst.h"

#define RG_TARGET_NAMESPACE mGst

/* Module: Gst
 * The GStreamer module.
 */
VALUE RG_TARGET_NAMESPACE;

static void
Init_gst_classes (void)
{
    extern void Init_gst_bin (void);
    extern void Init_gst_buffer(void);
    extern void Init_gst_bus(void);
    extern void Init_gst_caps (void);
    extern void Init_gst_child_proxy(void);
    extern void Init_gst_clock (void);
    extern void Init_gst_clock_entry (void);
    extern void Init_gst_element (void);
    extern void Init_gst_elementfactory (void);
    extern void Init_gst_event(void);
    extern void Init_gst_format (void);
    extern void Init_gst_seek(void);
    extern void Init_gst_structure(void);
    extern void Init_gst_index (void);
    extern void Init_gst_indexentry (void);
    extern void Init_gst_indexfactory (void);
    extern void Init_gst_message(void);
    extern void Init_gst_mini_object(void);
    extern void Init_gst_object (void);
    extern void Init_gst_pad (void);
    extern void Init_gst_pad_template(void);
    extern void Init_gst_ghost_pad(void);
    extern void Init_gst_parse (void);
    extern void Init_gst_pipeline (void);
    extern void Init_gst_plugin (void);
    extern void Init_gst_plugin_feature (void);
    extern void Init_gst_query(void);
    extern void Init_gst_querytype (void);
    extern void Init_gst_registry (void);
    extern void Init_gst_registry_pool (void);
    extern void Init_gst_scheduler (void);
    extern void Init_gst_schedulerfactory (void);
    extern void Init_gst_static_caps(void);
    extern void Init_gst_static_pad_template(void);
    extern void Init_gst_systemclock (void);
    extern void Init_gst_tag (void);
    extern void Init_gst_tag_setter (void);
    extern void Init_gst_thread (void);
    extern void Init_gst_typefindfactory (void);
    extern void Init_gst_value(void);
    extern void Init_gst_xml (void);

#if defined(HAVE_GST_OVERLAY)
    extern void Init_gst_x_overlay (void);
#endif

#if defined(HAVE_MEDIA_INFO)
    extern void Init_gst_mediatype (void);
#endif

#ifdef HAVE_GST_PBUTILS
    extern void Init_gst_install_plugins (void);
    extern void Init_gst_install_plugins_return (void);
    extern void Init_gst_install_plugins_context (void);
#endif

    Init_gst_bin ();
    Init_gst_bus();
    Init_gst_caps ();
    Init_gst_child_proxy();
    Init_gst_clock ();
    Init_gst_clock_entry ();
    Init_gst_element ();
    Init_gst_elementfactory ();
    Init_gst_seek();
    Init_gst_format ();
    Init_gst_structure();
    Init_gst_index ();
    Init_gst_indexentry ();
    Init_gst_indexfactory ();
    Init_gst_object ();
    Init_gst_pad ();
    Init_gst_pad_template();
    Init_gst_ghost_pad();
    Init_gst_parse ();
    Init_gst_pipeline ();
    Init_gst_plugin ();
    Init_gst_plugin_feature();
    Init_gst_querytype ();
    Init_gst_registry ();
    Init_gst_static_caps();
    Init_gst_static_pad_template();
    Init_gst_systemclock ();
    Init_gst_tag ();
    Init_gst_tag_setter ();
    Init_gst_typefindfactory ();
    Init_gst_value();
    Init_gst_xml ();

    Init_gst_mini_object();
    Init_gst_buffer();
    Init_gst_message();
    Init_gst_query();
    Init_gst_event();

#if defined(HAVE_GST_OVERLAY)
    Init_gst_x_overlay ();
#endif

#ifdef HAVE_MEDIA_INFO
    Init_gst_mediatype ();
#endif

#ifdef HAVE_GST_PBUTILS
    Init_gst_install_plugins ();
    Init_gst_install_plugins_return ();
    Init_gst_install_plugins_context ();
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