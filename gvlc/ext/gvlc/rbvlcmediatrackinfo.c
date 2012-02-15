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

static libvlc_media_track_info_t *
vlc_media_track_info_copy(const libvlc_media_track_info_t *instance)
{
    libvlc_media_track_info_t *result = g_new(libvlc_media_track_info_t, 1);
    *result = *instance;
    return result;
}

static void
vlc_media_track_info_free(libvlc_media_track_info_t *instance)
{
    g_free(instance);
}

GType
vlc_media_track_info_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaTrackInfo",
                                                (GBoxedCopyFunc)vlc_media_track_info_copy,
                                                (GBoxedFreeFunc)vlc_media_track_info_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cTrackInfo
#define _SELF(self) (RVAL2VLCMEDIATRACKINFO(self))

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_codec(VALUE self)
{
    return UINT2NUM(_SELF(self)->i_codec);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_id(VALUE self)
{
    return INT2NUM(_SELF(self)->i_id);
}

/*
 *
 * @return [VLC::TrackType]
 * @todo fixme
 */
static VALUE
rg_type(VALUE self)
{
    return VLCTRACKTYPE2RVAL(_SELF(self)->i_type);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_profile(VALUE self)
{
    return INT2NUM(_SELF(self)->i_profile);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_level(VALUE self)
{
    return INT2NUM(_SELF(self)->i_level);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_channels(VALUE self)
{
    if (_SELF(self)->i_type == libvlc_track_audio)
        return UINT2NUM(_SELF(self)->u.audio.i_channels);
    else
        return Qnil;
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_rate(VALUE self)
{
    if (_SELF(self)->i_type == libvlc_track_audio)
        return UINT2NUM(_SELF(self)->u.audio.i_rate);
    else
        return Qnil;
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_width(VALUE self)
{
    if (_SELF(self)->i_type == libvlc_track_video)
        return UINT2NUM(_SELF(self)->u.video.i_width);
    else
        return Qnil;
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_height(VALUE self)
{
    if (_SELF(self)->i_type == libvlc_track_video)
        return UINT2NUM(_SELF(self)->u.video.i_height);
    else
        return Qnil;
}

/*
 * Document-class: VLC::Media::TrackInfo
 *
 * @todo fixme
 */
void
Init_vlc_media_trackinfo(VALUE cMedia)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_TRACK_INFO, "TrackInfo", cMedia);

    RG_DEF_METHOD(codec, 0);
    RG_DEF_METHOD(id, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(profile, 0);
    RG_DEF_METHOD(level, 0);
    RG_DEF_METHOD(channels, 0);
    RG_DEF_METHOD(rate, 0);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
}
