/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbvlcprivate.h"

static libvlc_media_t *
vlc_media_copy(const libvlc_media_t *instance)
{
    libvlc_media_retain((libvlc_media_t *)instance);
    return (libvlc_media_t *)instance;
}

static void
vlc_media_free(libvlc_media_t *instance)
{
    libvlc_media_release(instance);
}

GType
vlc_media_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMedia",
                                                (GBoxedCopyFunc)vlc_media_copy,
                                                (GBoxedFreeFunc)vlc_media_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMedia
#define _SELF(self) (RVAL2VLCMEDIA(self))

/*
 * @overload initialize(core: core, location: url)
 *   Create a media with a certain given media resource location,
 *   for instance a valid URL.
 *
 *   @note
 *     To refer to a local file with this function,
 *     the file://... URI syntax <b>must</b> be used (see IETF RFC3986).
 *     We recommend using libvlc_media_new_path() instead when dealing with
 *     local files.
 *
 *   @param [Hash] options specify media
 *   @option options [VLC::Core] :core (nil) the core instance
 *   @option options [String] :location the media location URL
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @overload initialize(core: core, path: file)
 *   Create a media for a certain file path.
 *
 *   @param [Hash] options specify media
 *   @option options [VLC::Core] :core (nil) the core instance
 *   @option options [String] :path local filesystem path
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @overload initialize(core: core, fd: desc)
 *   Create a media for an already open file descriptor.
 *   The file descriptor shall be open for reading (or reading and writing).
 *
 *   Regular file descriptors, pipe read descriptors and character device
 *   descriptors (including TTYs) are supported on all platforms.
 *   Block device descriptors are supported where available.
 *   Directory descriptors are supported on systems that provide fdopendir().
 *   Sockets are supported on all platforms where they are file descriptors,
 *   i.e. all except Windows.
 *
 *   @note
 *     This library will <b>not</b> automatically close the file descriptor
 *     under any circumstance. Nevertheless, a file descriptor can usually only be
 *     rendered once in a media player. To render it a second time, the file
 *     descriptor should probably be rewound to the beginning with lseek().
 *
 *   @param [Hash] options specify media
 *   @option options [VLC::Core] :core (nil) the core instance
 *   @option options [Integer] :fd open file descriptor
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @overload initialize(core: core, name: name)
 *   Create a media as an empty node with a given name.
 *
 *   @param [Hash] options specify media
 *   @option options [VLC::Core] :core (nil) the core instance
 *   @option options [String] :name the name of the node
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @todo fixme
 */
static VALUE
rg_initialize(VALUE self, VALUE options)
{
    VALUE core, location, path, fd, name;
    libvlc_media_t *instance = NULL;

    rbg_scan_options(options,
                    "core",     &core,
                    "location", &location,
                    "path",     &path,
                    "fd",       &fd,
                    "name",     &name,
                    NULL);
    if (NIL_P(core))
        core = rb_funcall(GTYPE2CLASS(VLC_TYPE_CORE), rb_intern("new"), 0);

    if (!NIL_P(location))
        instance = libvlc_media_new_location(RVAL2VLCCORE(core), RVAL2CSTR(location));
    else if (!NIL_P(path))
        instance = libvlc_media_new_path(RVAL2VLCCORE(core), RVAL2CSTR(path));
    else if (!NIL_P(fd))
        instance = libvlc_media_new_fd(RVAL2VLCCORE(core), NUM2INT(fd));
    else if (!NIL_P(name))
        instance = libvlc_media_new_as_node(RVAL2VLCCORE(core), RVAL2CSTR(name));
    if (!instance)
        rb_raise(rb_eArgError, "Invalid arguments.");
    rb_ivar_set(self, rb_intern("core"), core);

    G_INITIALIZE(self, instance);
    vlc_media_free(instance);
    return Qnil;
}

static VALUE
rg_core(VALUE self)
{
    return rb_ivar_get(self, rb_intern("core"));
}

/*
 * Add an option to the media.
 *
 * This option will be used to determine how the media_player will
 * read the media. This allows to use VLC's advanced
 * reading/streaming options on a per-media basis.
 *
 * The options are detailed in vlc --long-help, for instance "--sout-all"
 *
 * @param [String] options the options
 * @param [Integer] flags the flags for this option
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_add_option(int argc, VALUE *argv, VALUE self)
{
    VALUE options, flags;

    rb_scan_args(argc, argv, "11", &options, &flags);

    if (NIL_P(flags))
        libvlc_media_add_option(_SELF(self), RVAL2CSTR(options));
    else
        libvlc_media_add_option_flag(_SELF(self), RVAL2CSTR(options), NUM2UINT(flags));

    return self;
}

/*
 * Get the media resource locator (mrl) from a media descriptor object
 *
 * @return [String] string with mrl of media descriptor object
 * @todo fixme
 */
static VALUE
rg_mrl(VALUE self)
{
    return CSTR2RVAL(libvlc_media_get_mrl(_SELF(self)));
}

/*
 * Read the meta of the media.
 *
 * If the media has not yet been parsed this will return NULL.
 *
 * This methods automatically calls libvlc_media_parse_async(), so after calling
 * it you may receive a libvlc_MediaMetaChanged event. If you prefer a synchronous
 * version ensure that you call libvlc_media_parse() before get_meta().
 *
 * \see libvlc_media_parse
 * \see libvlc_media_parse_async
 * \see libvlc_MediaMetaChanged
 *
 * @param [VLC::Media::MetaType, Symbol] type the meta to read
 * @return [String] the media's meta
 * @todo fixme
 */
static VALUE
rg_get_meta(VALUE self, VALUE type)
{
    return CSTR2RVAL(libvlc_media_get_meta(_SELF(self), RVAL2VLCMETATYPE(type)));
}

/*
 * Set the meta of the media (this function will not save the meta, call
 * libvlc_media_save_meta in order to save the meta)
 *
 * @param [VLC::Media::MetaType, Symbol] type the meta to write
 * @param [String] value the media's meta
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_set_meta(VALUE self, VALUE type, VALUE value)
{
    libvlc_media_set_meta(_SELF(self), RVAL2VLCMETATYPE(type), RVAL2CSTR(value));
    return self;
}

/*
 * Save the meta previously set
 *
 * @return [Boolean] true if the write operation was successfull
 * @todo fixme
 */
static VALUE
rg_save_meta(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_save_meta(_SELF(self)));
}

/*
 * Get current state of media descriptor object.
 *
 * @return [VLC::State] state of media descriptor object
 * @todo fixme
 */
static VALUE
rg_state(VALUE self)
{
    return VLCSTATE2RVAL(libvlc_media_get_state(_SELF(self)));
}

/*
 * Get the current statistics about the media
 *
 * @return [VLC::Media::Stats] the statistics about the media or nil
 * @todo fixme
 */
static VALUE
rg_stats(VALUE self)
{
    libvlc_media_stats_t stats;
    if (libvlc_media_get_stats(_SELF(self), &stats))
        return VLCMEDIASTATS2RVAL(&stats);
    else
        return Qnil;
}

/*
 * Get duration (in ms) of media descriptor object item.
 *
 * @return [Integer] duration of media item or nil on error
 * @todo fixme
 */
static VALUE
rg_duration(VALUE self)
{
    libvlc_time_t duration = libvlc_media_get_duration(_SELF(self));
    return duration < 0 ? Qnil : LONG2NUM(duration);
}

/*
 * Parse a media.
 *
 * This fetches (local) meta data and tracks information.
 *
 * To track when this is over you can listen to libvlc_MediaParsedChanged
 * event. However if the media was already parsed you will not receive this
 * event.
 *
 * \see libvlc_MediaParsedChanged
 * \see libvlc_media_get_meta
 * \see libvlc_media_get_tracks_info
 *
 * @param [Hash] options specify media
 * @option options [Boolean] :async (false) whether the operation is asynchronous or not
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_parse(VALUE self, VALUE options)
{
    VALUE async;
    rbg_scan_options(options,
                    "async", &async,
                    NULL);

    if (RVAL2CBOOL(async))
        libvlc_media_parse_async(_SELF(self));
    else
        libvlc_media_parse(_SELF(self));

    return self;
}

/*
 * Get Parsed status for media descriptor object.
 *
 * \see libvlc_MediaParsedChanged
 *
 * @return [Boolean] true if media object has been parsed otherwise it returns false
 * @todo fixme
 */
static VALUE
rg_parsed_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_is_parsed(_SELF(self)));
}

/*
 * Get media descriptor's elementary streams description
 *
 * @note
 *   you need to call libvlc_media_parse() or play the media at least once
 *   before calling this function.
 *   Not doing this will result in an empty array.
 *
 * @return [Array<VLC::MediaTrackInfo>] array of Elementary Streams descriptions
 * @todo fixme
 */
static VALUE
rg_tracks_info(VALUE self)
{
    libvlc_media_track_info_t *track_info, *p;
    int track_count, i;
    VALUE result;

    track_count = libvlc_media_get_tracks_info(_SELF(self), &track_info);
    result = rb_ary_new();
    for (i = 0, p = track_info; i < track_count; i++, p++) {
        rb_ary_push(result, VLCMEDIATRACKINFO2RVAL(p));
        g_free(p);
    }

    return result;
}

static VALUE
rg_attach_event(VALUE self, VALUE event_type)
{
    return em_attach_event(libvlc_media_event_manager(_SELF(self)), self, event_type);
}

static VALUE
rg_detach_event(VALUE self, VALUE event_type)
{
    return em_detach_event(libvlc_media_event_manager(_SELF(self)), self, event_type);
}

void
Init_vlc_media(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA, "Media", mVLC);
    G_DEF_CLASS(VLC_TYPE_META_TYPE, "MetaType", RG_TARGET_NAMESPACE);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(core, 0);
    RG_DEF_METHOD(add_option, -1);
    RG_DEF_METHOD(mrl, 0);
    RG_DEF_METHOD(get_meta, 1);
    RG_DEF_METHOD(set_meta, 2);
    RG_DEF_METHOD(save_meta, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(stats, 0);
    RG_DEF_METHOD(duration, 0);
    RG_DEF_METHOD(parse, 0);
    RG_DEF_METHOD_P(parsed, 0);
    RG_DEF_METHOD(tracks_info, 0);
    RG_DEF_METHOD(attach_event, 1);
    RG_DEF_METHOD(detach_event, 1);

    Init_vlc_media_stats(RG_TARGET_NAMESPACE);
}
