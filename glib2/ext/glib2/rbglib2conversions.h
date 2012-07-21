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

#ifndef __GLIB2CONVERSIONS_H__
#define __GLIB2CONVERSIONS_H__

#define RVAL2GPARAMSPEC(o)                 (G_PARAM_SPEC(RVAL2GOBJ(o)))

#define RVAL2GCLOSURE(o)                   ((GClosure*)RVAL2BOXED(o, G_TYPE_CLOSURE))
#define GCLOSURE2RVAL(o)                   (BOXED2RVAL(o, G_TYPE_CLOSURE))
#define RVAL2GCOMPLETION(o)                ((GCompletion*)RVAL2BOXED(o, G_TYPE_COMPLETION))
#define GCOMPLETION2RVAL(o)                (BOXED2RVAL(o, G_TYPE_COMPLETION))
#define RVAL2GIOCHANNEL(o)                 ((GIOChannel*)RVAL2BOXED(o, G_TYPE_IO_CHANNEL))
#define GIOCHANNEL2RVAL(o)                 (BOXED2RVAL(o, G_TYPE_IO_CHANNEL))
#define RVAL2GKEYFILE(o)                   ((GKeyFile*)RVAL2BOXED(o, G_TYPE_KEY_FILE))
#define GKEYFILE2RVAL(o)                   (BOXED2RVAL(o, G_TYPE_KEY_FILE))
#define RVAL2GMAINCONTEXT(o)               ((GMainContext*)RVAL2BOXED(o, G_TYPE_MAIN_CONTEXT))
#define GMAINCONTEXT2RVAL(o)               (BOXED2RVAL(o, G_TYPE_MAIN_CONTEXT))
#define RVAL2GMAINLOOP(o)                  ((GMainLoop*)RVAL2BOXED(o, G_TYPE_MAIN_LOOP))
#define GMAINLOOP2RVAL(o)                  (BOXED2RVAL(o, G_TYPE_MAIN_LOOP))
#define RVAL2GPOLLFD(o)                    ((GPollFD*)RVAL2BOXED(o, G_TYPE_POLL_FD))
#define GPOLLFD2RVAL(o)                    (BOXED2RVAL(o, G_TYPE_POLL_FD))
#define RVAL2GSOURCE(o)                    ((GSource*)RVAL2BOXED(o, G_TYPE_SOURCE))
#define GSOURCE2RVAL(o)                    (BOXED2RVAL(o, G_TYPE_SOURCE))
#define RVAL2GTIMER(o)                     ((GTimer*)RVAL2BOXED(o, G_TYPE_TIMER))
#define GTIMER2RVAL(o)                     (BOXED2RVAL(o, G_TYPE_TIMER))
#define RVAL2GVALUE(o)                     ((GValue*)RVAL2BOXED(o, G_TYPE_VALUE))
#define GVALUE2RVAL(o)                     (BOXED2RVAL(o, G_TYPE_VALUE))

#define RVAL2GIOCONDITION(o)               (RVAL2GFLAGS(o, G_TYPE_IO_CONDITION))
#define GIOCONDITION2RVAL(o)               (GFLAGS2RVAL(o, G_TYPE_IO_CONDITION))
#define RVAL2GNORMALIZEMODE(o)             (RVAL2GENUM(o, G_TYPE_NORMALIZE_MODE))
#define GNORMALIZEMODE2RVAL(o)             (GENUM2RVAL(o, G_TYPE_NORMALIZE_MODE))

#define RVAL2GCONNECTFLAGS(o)              (RVAL2GFLAGS(o, G_TYPE_CONNECT_FLAGS))
#define GCONNECTFLAGS2RVAL(o)              (GFLAGS2RVAL(o, G_TYPE_CONNECT_FLAGS))
#define RVAL2GKEYFILEFLAGS(o)              (RVAL2GFLAGS(o, G_TYPE_KEY_FILE_FLAGS))
#define GKEYFILEFLAGS2RVAL(o)              (GFLAGS2RVAL(o, G_TYPE_KEY_FILE_FLAGS))

#endif /* __GLIB2CONVERSIONS_H__ */
