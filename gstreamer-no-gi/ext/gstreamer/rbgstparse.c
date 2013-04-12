/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mParse

/* Module: Gst::Parse
 * Parses command-line syntax into a pipeline. 
 */

/*
 * Class method: launch(command)
 * command: a command line.
 *
 * Creates a new pipeline based on command line syntax.
 * On failure, an exception is thrown with a specific
 * error message.   
 *
 * Returns: a reference to a new Gst::Bin on success,
 * an exception on failure.
 */
static VALUE
rg_s_launch (G_GNUC_UNUSED VALUE self, VALUE command)
{
#if !defined(GST_DISABLE_PARSE)
    GError *error;
    GstBin *bin;

    error = NULL;
    bin = (GstBin*)gst_parse_launch (RVAL2CSTR (command), &error);
    if (bin != NULL)
        return RGST_BIN_NEW (bin);
        RAISE_GERROR(error);
#else
    rb_raise (rb_eRuntimeError, 
          "This function has been disabled "
          "when GStreamer was compiled");
#endif
    return Qnil;
}

void
Init_gst_parse (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under (mGst, "Parse"); 
    RG_DEF_SMETHOD(launch, 1);

        G_DEF_ERROR(GST_PARSE_ERROR, "ParseError", mGst, rb_eRuntimeError,
                    GST_TYPE_PARSE_ERROR);
}
