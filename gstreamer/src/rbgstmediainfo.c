
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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
 *  This class is not yet documented.
 */

#ifdef HAVE_MEDIA_INFO

#include <gst/media-info/media-info.h>

#define GST_MEDIA_INFO_STREAM_TYPE        (stream_get_type())
#define GST_MEDIA_INFO_TRACK_TYPE         (track_get_type())

#define RGST_MEDIA_INFO_STREAM(o) \
    ((GstMediaInfoStream *) (RVAL2BOXED(o, GST_MEDIA_INFO_STREAM_TYPE)))

#define RGST_MEDIA_INFO_TRACK(o) \
    ((GstMediaInfoTrack *) (RVAL2BOXED(o, GST_MEDIA_INFO_TRACK_TYPE)))

#define RGST_MEDIA_INFO_STREAM_NEW(o) \
    (BOXED2RVAL((GstMediaInfoStream *)o, GST_MEDIA_INFO_STREAM_TYPE))

#define RGST_MEDIA_INFO_TRACK_NEW(o) \
    (BOXED2RVAL((GstMediaInfoTrack *)o, GST_MEDIA_INFO_TRACK_TYPE))

#define RGST_MEDIA_INFO(o)          (GST_MEDIA_INFO(RVAL2GOBJ(o)))
#define RGST_MEDIA_INFO_NEW(o)      (RGST_GOBJ_NEW(GST_MEDIA_INFO(o)))

static GstMediaInfoStream* stream_copy(const GstMediaInfoStream* stream) {
    GstMediaInfoStream* new_stream;
    g_return_val_if_fail (stream != NULL, NULL);
    new_stream = g_new(GstMediaInfoStream, 1);
    *new_stream = *stream;
    return new_stream;
}
 
GType stream_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstMediaInfoStream",
            (GBoxedCopyFunc)stream_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

static GstMediaInfoTrack* track_copy(const GstMediaInfoTrack* track) {
    GstMediaInfoTrack* new_track;
    g_return_val_if_fail (track != NULL, NULL);
    new_track = g_new(GstMediaInfoTrack, 1);
    *new_track = *track;
    return new_track;
}
 
GType track_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstMediaInfoTrack",
            (GBoxedCopyFunc)track_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

static VALUE rb_gst_mediatype_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE r_source;
    GstMediaInfo *info;
    const char *source;

    rb_scan_args(argc, argv, "01", &r_source);

    source = NIL_P(r_source) ? NULL : RVAL2CSTR(r_source);
    info = gst_media_info_new(source);

    if (info != NULL) {
        G_INITIALIZE(self, info);
        return RGST_MEDIA_INFO_NEW(info);
    }
    return Qnil;
}

static VALUE rb_gst_mediatype_read(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE location, r_flags;
    GstMediaInfoStream *stream;
    int flags;

    rb_scan_args(argc, argv, "11", &location, &r_flags);

    flags =  NIL_P(r_flags) ? GST_MEDIA_INFO_ALL : FIX2INT(r_flags);
    stream = gst_media_info_read(RGST_MEDIA_INFO(self),
                                 RVAL2CSTR(location),
                                 flags);

    return stream != NULL
        ? RGST_MEDIA_INFO_STREAM_NEW(stream)
        : Qnil;
}

static VALUE stream_seekable(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return CBOOL2RVAL(stream->seekable);
}

static VALUE stream_mime(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return CSTR2RVAL(stream->mime);
}

static VALUE stream_path(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return CSTR2RVAL(stream->path);
}

static VALUE stream_caps(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return RGST_CAPS_NEW(stream->caps);
}

static VALUE stream_length_time(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return ULL2NUM(stream->length_time);
}

static VALUE stream_length_tracks(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return INT2NUM(stream->length_tracks);
}

static VALUE stream_bitrate(self)
    VALUE self;
{
    GstMediaInfoStream *stream = RGST_MEDIA_INFO_STREAM(self);
    return INT2NUM(stream->bitrate);
}

static VALUE stream_tracks(self)
    VALUE self;
{
    GstMediaInfoStream *stream;
    GList *list;
    VALUE arr;

    stream = RGST_MEDIA_INFO_STREAM(self);
    arr = rb_ary_new();

    for (list = stream->tracks;
         list != NULL;
         list = g_list_next(list))
    {
        GstMediaInfoTrack *track = (GstMediaInfoTrack *) list->data;
        rb_ary_push(arr, RGST_MEDIA_INFO_TRACK_NEW(track));        
    }

    return arr;
}

static VALUE track_metadata(self)
    VALUE self;
{
    GstMediaInfoTrack *track = RGST_MEDIA_INFO_TRACK(self);
    return track->metadata != NULL
        ? RGST_CAPS_NEW(track->metadata)
        : Qnil;
}

static VALUE track_streaminfo(self)
    VALUE self;
{
    GstMediaInfoTrack *track = RGST_MEDIA_INFO_TRACK(self);
    return track->streaminfo != NULL
        ? RGST_CAPS_NEW(track->streaminfo)
        : Qnil;
}

static VALUE track_format(self)
    VALUE self;
{
    GstMediaInfoTrack *track = RGST_MEDIA_INFO_TRACK(self);
    return track->format != NULL
        ? RGST_CAPS_NEW(track->format)
        : Qnil;
}

static VALUE track_length_time(self)
    VALUE self;
{
    GstMediaInfoTrack *track = RGST_MEDIA_INFO_TRACK(self);
    return ULL2NUM(track->length_time);
}

static VALUE track_con_streams(self)
    VALUE self;
{
    GstMediaInfoTrack *track;
    const GList *list;
    VALUE arr;
    
    track = RGST_MEDIA_INFO_TRACK(self);
    arr = rb_ary_new();
    for (list = track->con_streams;
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, RGST_MEDIA_INFO_STREAM_NEW(list->data));
    }
    return arr;
}

void Init_gst_mediatype(void) {
    VALUE c = G_DEF_CLASS(GST_MEDIA_INFO_TYPE, "MediaInfo", mGst);
    rb_define_method(c, "initialize", rb_gst_mediatype_new, -1);
    rb_define_method(c, "read", rb_gst_mediatype_read, -1);
    rb_define_const(c, "STREAM", INT2FIX(GST_MEDIA_INFO_STREAM));
    rb_define_const(c, "MIME", INT2FIX(GST_MEDIA_INFO_MIME));
    rb_define_const(c, "METADATA", INT2FIX(GST_MEDIA_INFO_METADATA));
    rb_define_const(c, "FORMAT", INT2FIX(GST_MEDIA_INFO_FORMAT));
    rb_define_const(c, "ALL", INT2FIX(GST_MEDIA_INFO_ALL));

    c = G_DEF_CLASS(GST_MEDIA_INFO_STREAM_TYPE, "MediaInfoStream", mGst);
    rb_define_method(c, "seekable?", stream_seekable, 0);
    rb_define_method(c, "mime", stream_mime, 0);
    rb_define_method(c, "path", stream_path, 0);
    rb_define_method(c, "caps", stream_caps, 0);
    rb_define_method(c, "length_time", stream_length_time, 0);
    rb_define_method(c, "length_tracks", stream_length_tracks, 0);
    rb_define_method(c, "bitrate", stream_bitrate, 0);
    rb_define_method(c, "tracks", stream_tracks, 0);

    c = G_DEF_CLASS(GST_MEDIA_INFO_TRACK_TYPE, "MediaInfoTrack", mGst);
    rb_define_method(c, "metadata", track_metadata, 0);
    rb_define_method(c, "streaminfo", track_streaminfo, 0);
    rb_define_method(c, "format", track_format, 0);
    rb_define_method(c, "length_time", track_length_time, 0);
    rb_define_method(c, "con_streams", track_con_streams, 0);
}

#endif /* HAVE_MEDIA_INFO */

