
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/*
 *  Module: Gst
 *
 *  The GStreamer module.
 */

VALUE mGst;

static void Init_gst_classes(void) {
    extern void Init_gst_autoplug(void);
    extern void Init_gst_autoplugfactory(void);
    extern void Init_gst_bin(void);
    extern void Init_gst_caps(void);
    extern void Init_gst_clock(void);
    extern void Init_gst_cpu(void);
    extern void Init_gst_element(void);
    extern void Init_gst_elementfactory(void);
    extern void Init_gst_event(void);
    extern void Init_gst_eventseek(void);
    extern void Init_gst_eventsegmentseek(void);
    extern void Init_gst_eventsize(void);
    extern void Init_gst_format(void);
    extern void Init_gst_indexfactory(void);
    extern void Init_gst_object(void);
    extern void Init_gst_pad(void);
    extern void Init_gst_padtemplate(void);
    extern void Init_gst_parse(void);
    extern void Init_gst_pipeline(void);
    extern void Init_gst_plugin(void);
    extern void Init_gst_pluginfeature(void);
    extern void Init_gst_querytype(void);
    extern void Init_gst_registry(void);
    extern void Init_gst_schedulerfactory(void);
    extern void Init_gst_systemclock(void);
    extern void Init_gst_thread(void);
    extern void Init_gst_type(void);
    extern void Init_gst_typefactory(void);
    extern void Init_gst_typefind(void);
    
    Init_gst_autoplug();
    Init_gst_autoplugfactory();
    Init_gst_bin();
    Init_gst_caps();
    Init_gst_clock();
    Init_gst_cpu();
    Init_gst_element();
    Init_gst_elementfactory();
    Init_gst_event();
    Init_gst_eventseek();
    Init_gst_eventsegmentseek();
    Init_gst_eventsize();
    Init_gst_format();
    Init_gst_indexfactory();
    Init_gst_object();
    Init_gst_pad();
    Init_gst_padtemplate();
    Init_gst_parse();
    Init_gst_pipeline();
    Init_gst_plugin();
    Init_gst_pluginfeature();
    Init_gst_querytype();
    Init_gst_registry();
    Init_gst_schedulerfactory();
    Init_gst_systemclock();
    Init_gst_thread();
    Init_gst_type();
    Init_gst_typefactory();
    Init_gst_typefind();

#ifdef HAVE_MEDIA_INFO
    extern void Init_gst_mediatype(void);
    Init_gst_mediatype();
#endif
}

/*
 *  Class method: init(*args) -> true
 *
 *  Initializes the GStreamer library (using *args), setting up internal path 
 *  lists, registering built-in elements, and loading standard plugins.
 *  
 *  If *args are ommited, GStreamer will be initialized using arguments from the 
 *  Ruby command line.
 *
 *  This function will always return 'true'.  A RuntimeError exception will be
 *  raised if GStreamer could not be initialized.
 */
extern VALUE rb_progname, rb_argv;
static VALUE rb_gst_init(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{ 
    gint i, gargc;
    VALUE argary;
    gchar** gargv;

    /*
     *  Convert arguments in a Ruby array.
     *  If no arguments are provided, we use the command line ones.
     */
    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        gargc = RARRAY(rb_argv)->len;
        argary = rb_argv;
    } 
    else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY(argary)->len;
    }

    /*
     *  Fill the gargv array with the provided arguments.
     */
    gargv = ALLOCA_N(char*, gargc + 1);
    gargv[0] = (gchar *) STR2CSTR(rb_progname);

    for (i = 0; i < gargc; i++) {
        if (TYPE(RARRAY(argary)->ptr[i]) == T_STRING) {
            gargv[i+1] = (gchar *) STR2CSTR(RARRAY(argary)->ptr[i]);
        }
        else {
            gargv[i+1] = "";
        }
    }
    gargc++;

    /*
     *  Now, we can initialize GStreamer. 
     */
    if (gst_init_check(&gargc, &gargv) == FALSE) 
        rb_raise(rb_eRuntimeError, "failed to initialize GStreamer");

    /* 
     *  Initialize all included classes 
     */
    Init_gst_classes();
    
    return Qtrue;
}

/*
 *  Class method: version -> anArray
 *
 *  Gets the version number of the GStreamer library, in an array
 *  of 3 fixnums, which represent major, minor and macro numbers. 
 *
 *  Example: 
 *
 *    # Prints GStreamer version in a String 'major.minor.macro'
 *    p Gst.version.join('.')
 */
static VALUE rb_gst_version(self) 
    VALUE self;
{
    guint major, minor, micro;
    VALUE arr;

    gst_version(&major, &minor, &micro);
    arr = rb_ary_new();
    rb_ary_push(arr, INT2FIX(major));
    rb_ary_push(arr, INT2FIX(minor));
    rb_ary_push(arr, INT2FIX(micro));

    return arr;
}

/*
 *  Class method: has_threads? -> aBoolean
 *
 *  Query if GStreamer has threads enabled.
 */
static VALUE rb_gst_has_threads(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_has_threads());
}

/*
 *  Class method: use_threads(aBoolean) -> aBoolean
 *
 *  Instructs the core to turn on/off threading. 
 *
 *  When threading is turned off, all thread operations such as 
 *  mutexes and conditionals are turned into NOPs. 
 *  Use this if you want absolute minimal overhead and you don't 
 *  use any threads in the pipeline.
 *
 *  Returns the provided boolean value.
 */
static VALUE rb_gst_use_threads(self, value)
    VALUE self, value;
{
    gst_use_threads(RVAL2CBOOL(value));
    return value;
}

/*
 *  Class method: set_debug(aBoolean) -> nil
 *
 *  Enable or disable debug mode, according to the provided parameter.
 *  When debug mode is on, Ruby/GStreamer will print various information
 *  on the screen (as instance, when the GC cames), useful to trace and fix bugs.  
 */
gboolean debug_mode = FALSE;
static VALUE rb_gst_set_debug(self, value)
    VALUE self, value;
{
    debug_mode = RVAL2CBOOL(value);
    return Qnil;
}

/*
 *  Constant: SECOND
 *  Constant that defines one GStreamer second.
 */
static VALUE constSecond = INT2FIX(GST_SECOND);

/*
 *  Constant: MSECOND
 *  Constant that defines one GStreamer milmisecond.
 */
static VALUE constMsecond = INT2FIX(GST_MSECOND);

/*
 *  Constant: USECOND
 *  Constant that defines one GStreamer microsecond.
 */
static VALUE constUsecond = INT2FIX(GST_USECOND);

/*
 *  Constant: NSECOND
 *  Constant that defines one GStreamer nanosecond.
 */
static VALUE constNsecond = INT2FIX(GST_NSECOND);

/*
 *  Creates the Gst module.
 */
void Init_gst(void) {
    /* Define the module */
    mGst = rb_define_module("Gst");
    rb_define_module_function(mGst, "init",         rb_gst_init,        -1);
    rb_define_module_function(mGst, "version",      rb_gst_version,      0);
    rb_define_module_function(mGst, "has_threads?", rb_gst_has_threads,  0);
    rb_define_module_function(mGst, "use_threads",  rb_gst_use_threads,  1);
    rb_define_module_function(mGst, "set_debug",    rb_gst_set_debug,    1);

    rb_define_const(mGst, "SECOND",  constSecond);
    rb_define_const(mGst, "MSECOND", constMsecond);
    rb_define_const(mGst, "USECOND", constUsecond);
    rb_define_const(mGst, "NSECOND", constNsecond);
}
 
