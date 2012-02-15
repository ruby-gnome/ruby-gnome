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

static libvlc_media_stats_t *
vlc_media_stats_copy(const libvlc_media_stats_t *instance)
{
    libvlc_media_stats_t *result = g_new(libvlc_media_stats_t, 1);
    *result = *instance;
    return result;
}

static void
vlc_media_stats_free(libvlc_media_stats_t *instance)
{
    g_free(instance);
}

GType
vlc_media_stats_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaStats",
                                                (GBoxedCopyFunc)vlc_media_stats_copy,
                                                (GBoxedFreeFunc)vlc_media_stats_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cStats
#define _SELF(self) (RVAL2VLCMEDIASTATS(self))

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_read_bytes(VALUE self)
{
    return INT2NUM(_SELF(self)->i_read_bytes);
}

/*
 *
 * @return [Float]
 * @todo fixme
 */
static VALUE
rg_input_bitrate(VALUE self)
{
    return DBL2NUM(_SELF(self)->f_input_bitrate);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_demux_read_bytes(VALUE self)
{
    return INT2NUM(_SELF(self)->i_demux_read_bytes);
}

/*
 *
 * @return [Float]
 * @todo fixme
 */
static VALUE
rg_demux_bitrate(VALUE self)
{
    return DBL2NUM(_SELF(self)->f_demux_bitrate);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_demux_corrupted(VALUE self)
{
    return INT2NUM(_SELF(self)->i_demux_corrupted);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_demux_discontinuity(VALUE self)
{
    return INT2NUM(_SELF(self)->i_demux_discontinuity);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_decoded_video(VALUE self)
{
    return INT2NUM(_SELF(self)->i_decoded_video);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_decoded_audio(VALUE self)
{
    return INT2NUM(_SELF(self)->i_decoded_audio);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_displayed_pictures(VALUE self)
{
    return INT2NUM(_SELF(self)->i_displayed_pictures);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_lost_pictures(VALUE self)
{
    return INT2NUM(_SELF(self)->i_lost_pictures);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_played_abuffers(VALUE self)
{
    return INT2NUM(_SELF(self)->i_played_abuffers);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_lost_abuffers(VALUE self)
{
    return INT2NUM(_SELF(self)->i_lost_abuffers);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_sent_packets(VALUE self)
{
    return INT2NUM(_SELF(self)->i_sent_packets);
}

/*
 *
 * @return [Integer]
 * @todo fixme
 */
static VALUE
rg_sent_bytes(VALUE self)
{
    return INT2NUM(_SELF(self)->i_sent_bytes);
}

/*
 *
 * @return [Float]
 * @todo fixme
 */
static VALUE
rg_send_bitrate(VALUE self)
{
    return DBL2NUM(_SELF(self)->f_send_bitrate);
}

/*
 * Document-class: VLC::Media::Stats
 *
 * LibVLC media statistics
 *
 * @todo fixme
 */
void
Init_vlc_media_stats(VALUE cMedia)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_STATS, "MediaStats", cMedia);

    RG_DEF_METHOD(read_bytes, 0);
    RG_DEF_METHOD(input_bitrate, 0);
    RG_DEF_METHOD(demux_read_bytes, 0);
    RG_DEF_METHOD(demux_bitrate, 0);
    RG_DEF_METHOD(demux_corrupted, 0);
    RG_DEF_METHOD(demux_discontinuity, 0);
    RG_DEF_METHOD(decoded_video, 0);
    RG_DEF_METHOD(decoded_audio, 0);
    RG_DEF_METHOD(displayed_pictures, 0);
    RG_DEF_METHOD(lost_pictures, 0);
    RG_DEF_METHOD(played_abuffers, 0);
    RG_DEF_METHOD(lost_abuffers, 0);
    RG_DEF_METHOD(sent_packets, 0);
    RG_DEF_METHOD(sent_bytes, 0);
    RG_DEF_METHOD(send_bitrate, 0);
}
