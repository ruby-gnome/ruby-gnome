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

#define RG_TARGET_NAMESPACE mVLC

/*
 * Retrieve libvlc version.
 *
 * @example
 *   VLC::Core.version  # => "2.0.0-rc1 Twoflower"
 *
 * @return [String] a string containing the libvlc version
 * @todo fixme
 */
static VALUE
rg_s_version(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(libvlc_get_version());
}

/*
 * Retrieve libvlc compiler version.
 *
 * @example
 *   VLC::Core.compiler # => "gcc version 4.6.1 (Ubuntu/Linaro 4.6.1-9ubuntu3)"
 *
 * @return [String] a string containing the libvlc compiler version
 * @todo fixme
 */
static VALUE
rg_s_compiler(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(libvlc_get_compiler());
}

/*
 * Retrieve libvlc changeset.
 *
 * @example
 *   VLC::Core.changeset    # => "2.0.0+git20120127+r38"
 *
 * @return [String] a string containing the libvlc changeset
 * @todo fixme
 */
static VALUE
rg_s_changeset(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(libvlc_get_changeset());
}

static VALUE
rg_s_errmsg(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(libvlc_errmsg());
}

static VALUE
rg_s_clearerr(VALUE self)
{
    libvlc_clearerr();
    return self;
}

void
Init_vlc(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module("VLC");
    G_DEF_CLASS(VLC_TYPE_TRACK_TYPE, "TrackType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_STATE, "State", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_PLAYBACK_MODE, "PlaybackMode", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_NAVIGATE_MODE, "NavigateMode", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_VIDEO_MARQUEE_OPTION, "VideoMarqueeOption", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_VIDEO_LOGO_OPTION, "VideoLogoOption", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_VIDEO_ADJUST_OPTION, "VideoAdjustOption", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_AUDIO_OUTPUT_CHANNEL, "AudioOutputChannel", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(VLC_TYPE_AUDIO_OUTPUT_DEVICE_TYPES, "AudioOutputDeviceTypes", RG_TARGET_NAMESPACE);

    RG_DEF_SMETHOD(version, 0);
    RG_DEF_SMETHOD(compiler, 0);
    RG_DEF_SMETHOD(changeset, 0);
    RG_DEF_SMETHOD(errmsg, 0);
    RG_DEF_SMETHOD(clearerr, 0);

    Init_vlc_eventtype(RG_TARGET_NAMESPACE);

    Init_vlc_core(RG_TARGET_NAMESPACE);
    Init_vlc_media(RG_TARGET_NAMESPACE);
    Init_vlc_mediaplayer(RG_TARGET_NAMESPACE);
    Init_vlc_medialist(RG_TARGET_NAMESPACE);
    Init_vlc_medialistplayer(RG_TARGET_NAMESPACE);
    Init_vlc_medialibrary(RG_TARGET_NAMESPACE);
    Init_vlc_mediadiscoverer(RG_TARGET_NAMESPACE);
    Init_vlc_event(RG_TARGET_NAMESPACE);
    Init_vlc_mediatrackinfo(RG_TARGET_NAMESPACE);
    Init_vlc_moduledescription(RG_TARGET_NAMESPACE);
    Init_vlc_trackdescription(RG_TARGET_NAMESPACE);
    Init_vlc_audiooutput(RG_TARGET_NAMESPACE);
}
