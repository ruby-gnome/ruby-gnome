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

#ifndef __RBVLC_CONVERSIONS_H__
#define __RBVLC_CONVERSIONS_H__

#define RVAL2VLCCORE(o)                     ((libvlc_instance_t*)RVAL2BOXED(o, VLC_TYPE_CORE))
#define VLCCORE2RVAL(o)                     (BOXED2RVAL(o, VLC_TYPE_CORE))
#define RVAL2VLCMEDIA(o)                    ((libvlc_media_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA))
#define VLCMEDIA2RVAL(o)                    (BOXED2RVAL(o, VLC_TYPE_MEDIA))
#define RVAL2VLCMEDIAPLAYER(o)              ((libvlc_media_player_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_PLAYER))
#define VLCMEDIAPLAYER2RVAL(o)              (BOXED2RVAL(o, VLC_TYPE_MEDIA_PLAYER))
#define RVAL2VLCMEDIALIST(o)                ((libvlc_media_list_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_LIST))
#define VLCMEDIALIST2RVAL(o)                (BOXED2RVAL(o, VLC_TYPE_MEDIA_LIST))
#define RVAL2VLCMEDIALISTPLAYER(o)          ((libvlc_media_list_player_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_LIST_PLAYER))
#define VLCMEDIALISTPLAYER2RVAL(o)          (BOXED2RVAL(o, VLC_TYPE_MEDIA_LIST_PLAYER))
#define RVAL2VLCMEDIALIBRARY(o)             ((libvlc_media_library_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_LIBRARY))
#define VLCMEDIALIBRARY2RVAL(o)             (BOXED2RVAL(o, VLC_TYPE_MEDIA_LIBRARY))
#define RVAL2VLCMEDIADISCOVERER(o)          ((libvlc_media_discoverer_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_DISCOVERER))
#define VLCMEDIADISCOVERER2RVAL(o)          (BOXED2RVAL(o, VLC_TYPE_MEDIA_DISCOVERER))
#define RVAL2VLCEVENT(o)                    ((libvlc_event_t*)RVAL2BOXED(o, VLC_TYPE_EVENT))
#define VLCEVENT2RVAL(o)                    (BOXED2RVAL(o, VLC_TYPE_EVENT))
#define RVAL2VLCMEDIATRACKINFO(o)           ((libvlc_media_track_info_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_TRACK_INFO))
#define VLCMEDIATRACKINFO2RVAL(o)           (BOXED2RVAL(o, VLC_TYPE_MEDIA_TRACK_INFO))
#define RVAL2VLCMODULEDESCRIPTION(o)        ((libvlc_module_description_t*)RVAL2BOXED(o, VLC_TYPE_MODULE_DESCRIPTION))
#define VLCMODULEDESCRIPTION2RVAL(o)        (BOXED2RVAL(o, VLC_TYPE_MODULE_DESCRIPTION))
#define RVAL2VLCTRACKDESCRIPTION(o)         ((libvlc_track_description_t*)RVAL2BOXED(o, VLC_TYPE_TRACK_DESCRIPTION))
#define VLCTRACKDESCRIPTION2RVAL(o)         (BOXED2RVAL(o, VLC_TYPE_TRACK_DESCRIPTION))
#define RVAL2VLCAUDIOOUTPUT(o)              ((libvlc_audio_output_t*)RVAL2BOXED(o, VLC_TYPE_AUDIO_OUTPUT))
#define VLCAUDIOOUTPUT2RVAL(o)              (BOXED2RVAL(o, VLC_TYPE_AUDIO_OUTPUT))
#define RVAL2VLCMEDIASTATS(o)               ((libvlc_media_stats_t*)RVAL2BOXED(o, VLC_TYPE_MEDIA_STATS))
#define VLCMEDIASTATS2RVAL(o)               (BOXED2RVAL(o, VLC_TYPE_MEDIA_STATS))

#define RVAL2VLCTRACKTYPE(o)                (RVAL2GENUM(o, VLC_TYPE_TRACK_TYPE))
#define VLCTRACKTYPE2RVAL(o)                (GENUM2RVAL(o, VLC_TYPE_TRACK_TYPE))
#define RVAL2VLCMETATYPE(o)                 (RVAL2GENUM(o, VLC_TYPE_META_TYPE))
#define VLCMETATYPE2RVAL(o)                 (GENUM2RVAL(o, VLC_TYPE_META_TYPE))
#define RVAL2VLCSTATE(o)                    (RVAL2GENUM(o, VLC_TYPE_STATE))
#define VLCSTATE2RVAL(o)                    (GENUM2RVAL(o, VLC_TYPE_STATE))
#define RVAL2VLCPLAYBACKMODE(o)             (RVAL2GENUM(o, VLC_TYPE_PLAYBACK_MODE))
#define VLCPLAYBACKMODE2RVAL(o)             (GENUM2RVAL(o, VLC_TYPE_PLAYBACK_MODE))
#define RVAL2VLCNAVIGATEMODE(o)             (RVAL2GENUM(o, VLC_TYPE_NAVIGATE_MODE))
#define VLCNAVIGATEMODE2RVAL(o)             (GENUM2RVAL(o, VLC_TYPE_NAVIGATE_MODE))
#define RVAL2VLCVIDEOMARQUEEOPTION(o)       (RVAL2GENUM(o, VLC_TYPE_VIDEO_MARQUEE_OPTION))
#define VLCVIDEOMARQUEEOPTION2RVAL(o)       (GENUM2RVAL(o, VLC_TYPE_VIDEO_MARQUEE_OPTION))
#define RVAL2VLCVIDEOLOGOOPTION(o)          (RVAL2GENUM(o, VLC_TYPE_VIDEO_LOGO_OPTION))
#define VLCVIDEOLOGOOPTION2RVAL(o)          (GENUM2RVAL(o, VLC_TYPE_VIDEO_LOGO_OPTION))
#define RVAL2VLCVIDEOADJUSTOPTION(o)        (RVAL2GENUM(o, VLC_TYPE_VIDEO_ADJUST_OPTION))
#define VLCVIDEOADJUSTOPTION2RVAL(o)        (GENUM2RVAL(o, VLC_TYPE_VIDEO_ADJUST_OPTION))
#define RVAL2VLCAUDIOOUTPUTCHANNEL(o)       (RVAL2GENUM(o, VLC_TYPE_AUDIO_OUTPUT_CHANNEL))
#define VLCAUDIOOUTPUTCHANNEL2RVAL(o)       (GENUM2RVAL(o, VLC_TYPE_AUDIO_OUTPUT_CHANNEL))
#define RVAL2VLCAUDIOOUTPUTDEVICETYPES(o)   (RVAL2GENUM(o, VLC_TYPE_AUDIO_OUTPUT_DEVICE_TYPES))
#define VLCAUDIOOUTPUTDEVICETYPES2RVAL(o)   (GENUM2RVAL(o, VLC_TYPE_AUDIO_OUTPUT_DEVICE_TYPES))
#define RVAL2VLCEVENTTYPE(o)                (RVAL2GENUM(o, VLC_TYPE_EVENT_TYPE))
#define VLCEVENTTYPE2RVAL(o)                (GENUM2RVAL(o, VLC_TYPE_EVENT_TYPE))

#endif

