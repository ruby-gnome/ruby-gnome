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

GType
vlc_track_type_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_track_unknown,  "VLC_TRACK_TYPE_UNKNOWN",   "unknown"},
            {libvlc_track_audio,    "VLC_TRACK_TYPE_AUDIO",     "audio"},
            {libvlc_track_video,    "VLC_TRACK_TYPE_VIDEO",     "video"},
            {libvlc_track_text,     "VLC_TRACK_TYPE_TEXT",      "text"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCTrackType"), values);
    }
    return etype;
}

GType
vlc_meta_type_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_meta_Title,         "VLC_META_TYPE_TITLE",          "title"},
            {libvlc_meta_Artist,        "VLC_META_TYPE_ARTIST",         "artist"},
            {libvlc_meta_Genre,         "VLC_META_TYPE_GENRE",          "genre"},
            {libvlc_meta_Copyright,     "VLC_META_TYPE_COPYRIGHT",      "copyright"},
            {libvlc_meta_Album,         "VLC_META_TYPE_ALBUM",          "album"},
            {libvlc_meta_TrackNumber,   "VLC_META_TYPE_TRACKNUMBER",    "tracknumber"},
            {libvlc_meta_Description,   "VLC_META_TYPE_DESCRIPTION",    "description"},
            {libvlc_meta_Rating,        "VLC_META_TYPE_RATING",         "rating"},
            {libvlc_meta_Date,          "VLC_META_TYPE_DATE",           "date"},
            {libvlc_meta_Setting,       "VLC_META_TYPE_SETTING",        "setting"},
            {libvlc_meta_URL,           "VLC_META_TYPE_URL",            "url"},
            {libvlc_meta_Language,      "VLC_META_TYPE_LANGUAGE",       "language"},
            {libvlc_meta_NowPlaying,    "VLC_META_TYPE_NOWPLAYING",     "nowplaying"},
            {libvlc_meta_Publisher,     "VLC_META_TYPE_PUBLISHER",      "publisher"},
            {libvlc_meta_EncodedBy,     "VLC_META_TYPE_ENCODEDBY",      "encodedby"},
            {libvlc_meta_ArtworkURL,    "VLC_META_TYPE_ARTWORKURL",     "artworkurl"},
            {libvlc_meta_TrackID,       "VLC_META_TYPE_TRACKID",        "trackid"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCMetaType"), values);
    }
    return etype;
}

GType
vlc_state_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_NothingSpecial,     "VLC_STATE_NOTHINGSPECIAL",     "nothingspecial"},
            {libvlc_Opening,            "VLC_STATE_OPENING",            "opening"},
            {libvlc_Buffering,          "VLC_STATE_BUFFERING",          "buffering"},
            {libvlc_Playing,            "VLC_STATE_PLAYING",            "playing"},
            {libvlc_Paused,             "VLC_STATE_PAUSED",             "paused"},
            {libvlc_Stopped,            "VLC_STATE_STOPPED",            "stopped"},
            {libvlc_Ended,              "VLC_STATE_ENDED",              "ended"},
            {libvlc_Error,              "VLC_STATE_ERROR",              "error"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCState"), values);
    }
    return etype;
}

GType
vlc_playback_mode_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_playback_mode_default,  "VLC_PLAYBACK_MODE_DEFAULT",    "default"},
            {libvlc_playback_mode_loop,     "VLC_PLAYBACK_MODE_LOOP",       "loop"},
            {libvlc_playback_mode_repeat,   "VLC_PLAYBACK_MODE_REPEAT",     "repeat"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCPlaybackMode"), values);
    }
    return etype;
}

GType
vlc_navigate_mode_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_navigate_activate,  "VLC_NAVIGATE_MODE_ACTIVATE",   "activate"},
            {libvlc_navigate_up,        "VLC_NAVIGATE_MODE_UP",         "up"},
            {libvlc_navigate_down,      "VLC_NAVIGATE_MODE_DOWN",       "down"},
            {libvlc_navigate_left,      "VLC_NAVIGATE_MODE_LEFT",       "left"},
            {libvlc_navigate_right,     "VLC_NAVIGATE_MODE_RIGHT",      "right"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCNavigateMode"), values);
    }
    return etype;
}

GType
vlc_video_marquee_option_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_marquee_Enable,     "VLC_VIDEO_MARQUEE_OPTION_ENABLE",      "enable"},
            {libvlc_marquee_Text,       "VLC_VIDEO_MARQUEE_OPTION_TEXT",        "text"},
            {libvlc_marquee_Color,      "VLC_VIDEO_MARQUEE_OPTION_COLOR",       "color"},
            {libvlc_marquee_Opacity,    "VLC_VIDEO_MARQUEE_OPTION_OPACITY",     "opacity"},
            {libvlc_marquee_Position,   "VLC_VIDEO_MARQUEE_OPTION_POSITION",    "position"},
            {libvlc_marquee_Refresh,    "VLC_VIDEO_MARQUEE_OPTION_REFRESH",     "refresh"},
            {libvlc_marquee_Size,       "VLC_VIDEO_MARQUEE_OPTION_SIZE",        "size"},
            {libvlc_marquee_Timeout,    "VLC_VIDEO_MARQUEE_OPTION_TIMEOUT",     "timeout"},
            {libvlc_marquee_X,          "VLC_VIDEO_MARQUEE_OPTION_X",           "x"},
            {libvlc_marquee_Y,          "VLC_VIDEO_MARQUEE_OPTION_Y",           "y"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCVideoMarqueeOption"), values);
    }
    return etype;
}

GType
vlc_video_logo_option_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_logo_enable,    "VLC_VIDEO_LOGO_OPTION_ENABLE",     "enable"},
            {libvlc_logo_file,      "VLC_VIDEO_LOGO_OPTION_FILE",       "file"},
            {libvlc_logo_x,         "VLC_VIDEO_LOGO_OPTION_X",          "x"},
            {libvlc_logo_y,         "VLC_VIDEO_LOGO_OPTION_Y",          "y"},
            {libvlc_logo_delay,     "VLC_VIDEO_LOGO_OPTION_DELAY",      "delay"},
            {libvlc_logo_repeat,    "VLC_VIDEO_LOGO_OPTION_REPEAT",     "repeat"},
            {libvlc_logo_opacity,   "VLC_VIDEO_LOGO_OPTION_OPACITY",    "opacity"},
            {libvlc_logo_position,  "VLC_VIDEO_LOGO_OPTION_POSITION",   "position"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCVideoLogoOption"), values);
    }
    return etype;
}

GType
vlc_video_adjust_option_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_adjust_Enable,      "VLC_VIDEO_ADJUST_OPTION_ENABLE",       "enable"},
            {libvlc_adjust_Contrast,    "VLC_VIDEO_ADJUST_OPTION_CONTRAST",     "contrast"},
            {libvlc_adjust_Brightness,  "VLC_VIDEO_ADJUST_OPTION_BRIGHTNESS",   "brightness"},
            {libvlc_adjust_Hue,         "VLC_VIDEO_ADJUST_OPTION_HUE",          "hue"},
            {libvlc_adjust_Saturation,  "VLC_VIDEO_ADJUST_OPTION_SATURATION",   "saturation"},
            {libvlc_adjust_Gamma,       "VLC_VIDEO_ADJUST_OPTION_GAMMA",        "gamma"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCVideoAdjustOption"), values);
    }
    return etype;
}

GType
vlc_audio_output_channel_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_AudioChannel_Error,     "VLC_AUDIO_OUTPUT_CHANNEL_ERROR",       "error"},
            {libvlc_AudioChannel_Stereo,    "VLC_AUDIO_OUTPUT_CHANNEL_STEREO",      "stereo"},
            {libvlc_AudioChannel_RStereo,   "VLC_AUDIO_OUTPUT_CHANNEL_RSTEREO",     "rstereo"},
            {libvlc_AudioChannel_Left,      "VLC_AUDIO_OUTPUT_CHANNEL_LEFT",        "left"},
            {libvlc_AudioChannel_Right,     "VLC_AUDIO_OUTPUT_CHANNEL_RIGHT",       "right"},
            {libvlc_AudioChannel_Dolbys,    "VLC_AUDIO_OUTPUT_CHANNEL_DOLBYS",      "dolbys"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCAudioOutputChannel"), values);
    }
    return etype;
}

GType
vlc_audio_output_device_types_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_AudioOutputDevice_Error,    "VLC_AUDIO_OUTPUT_DEVICE_TYPES_ERROR",      "error"},
            {libvlc_AudioOutputDevice_Mono,     "VLC_AUDIO_OUTPUT_DEVICE_TYPES_MONO",       "mono"},
            {libvlc_AudioOutputDevice_Stereo,   "VLC_AUDIO_OUTPUT_DEVICE_TYPES_STEREO",     "stereo"},
            {libvlc_AudioOutputDevice_2F2R,     "VLC_AUDIO_OUTPUT_DEVICE_TYPES_2F2R",       "ch2f2r"},
            {libvlc_AudioOutputDevice_3F2R,     "VLC_AUDIO_OUTPUT_DEVICE_TYPES_3F2R",       "ch3f2r"},
            {libvlc_AudioOutputDevice_5_1,      "VLC_AUDIO_OUTPUT_DEVICE_TYPES_5_1",        "ch5_1"},
            {libvlc_AudioOutputDevice_6_1,      "VLC_AUDIO_OUTPUT_DEVICE_TYPES_6_1",        "ch6_1"},
            {libvlc_AudioOutputDevice_7_1,      "VLC_AUDIO_OUTPUT_DEVICE_TYPES_7_1",        "ch7_1"},
            {libvlc_AudioOutputDevice_SPDIF,    "VLC_AUDIO_OUTPUT_DEVICE_TYPES_SPDIF",      "spdif"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCAudioOutputDeviceTypes"), values);
    }
    return etype;
}

GType
vlc_event_type_get_type(void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            {libvlc_MediaMetaChanged,               "VLC_EVENT_TYPE_MEDIA_METACHANGED",                 "media_metachanged"},
            {libvlc_MediaSubItemAdded,              "VLC_EVENT_TYPE_MEDIA_SUBITEMADDED",                "media_subitemadded"},
            {libvlc_MediaDurationChanged,           "VLC_EVENT_TYPE_MEDIA_DURATIONCHANGED",             "media_durationchanged"},
            {libvlc_MediaParsedChanged,             "VLC_EVENT_TYPE_MEDIA_PARSEDCHANGED",               "media_parsedchanged"},
            {libvlc_MediaFreed,                     "VLC_EVENT_TYPE_MEDIA_FREED",                       "media_freed"},
            {libvlc_MediaStateChanged,              "VLC_EVENT_TYPE_MEDIA_STATECHANGED",                "media_statechanged"},
            {libvlc_MediaPlayerMediaChanged,        "VLC_EVENT_TYPE_MEDIAPLAYER_MEDIACHANGED",          "mediaplayer_mediachanged"},
            {libvlc_MediaPlayerNothingSpecial,      "VLC_EVENT_TYPE_MEDIAPLAYER_NOTHINGSPECIAL",        "mediaplayer_nothingspecial"},
            {libvlc_MediaPlayerOpening,             "VLC_EVENT_TYPE_MEDIAPLAYER_OPENING",               "mediaplayer_opening"},
            {libvlc_MediaPlayerBuffering,           "VLC_EVENT_TYPE_MEDIAPLAYER_BUFFERING",             "mediaplayer_buffering"},
            {libvlc_MediaPlayerPlaying,             "VLC_EVENT_TYPE_MEDIAPLAYER_PLAYING",               "mediaplayer_playing"},
            {libvlc_MediaPlayerPaused,              "VLC_EVENT_TYPE_MEDIAPLAYER_PAUSED",                "mediaplayer_paused"},
            {libvlc_MediaPlayerStopped,             "VLC_EVENT_TYPE_MEDIAPLAYER_STOPPED",               "mediaplayer_stopped"},
            {libvlc_MediaPlayerForward,             "VLC_EVENT_TYPE_MEDIAPLAYER_FORWARD",               "mediaplayer_forward"},
            {libvlc_MediaPlayerBackward,            "VLC_EVENT_TYPE_MEDIAPLAYER_BACKWARD",              "mediaplayer_backward"},
            {libvlc_MediaPlayerEndReached,          "VLC_EVENT_TYPE_MEDIAPLAYER_ENDREACHED",            "mediaplayer_endreached"},
            {libvlc_MediaPlayerEncounteredError,    "VLC_EVENT_TYPE_MEDIAPLAYER_ENCOUNTEREDERROR",      "mediaplayer_encounterederror"},
            {libvlc_MediaPlayerTimeChanged,         "VLC_EVENT_TYPE_MEDIAPLAYER_TIMECHANGED",           "mediaplayer_timechanged"},
            {libvlc_MediaPlayerPositionChanged,     "VLC_EVENT_TYPE_MEDIAPLAYER_POSITIONCHANGED",       "mediaplayer_positionchanged"},
            {libvlc_MediaPlayerSeekableChanged,     "VLC_EVENT_TYPE_MEDIAPLAYER_SEEKABLECHANGED",       "mediaplayer_seekablechanged"},
            {libvlc_MediaPlayerPausableChanged,     "VLC_EVENT_TYPE_MEDIAPLAYER_PAUSABLECHANGED",       "mediaplayer_pausablechanged"},
            {libvlc_MediaPlayerTitleChanged,        "VLC_EVENT_TYPE_MEDIAPLAYER_TITLECHANGED",          "mediaplayer_titlechanged"},
            {libvlc_MediaPlayerSnapshotTaken,       "VLC_EVENT_TYPE_MEDIAPLAYER_SNAPSHOTTAKEN",         "mediaplayer_snapshottaken"},
            {libvlc_MediaPlayerLengthChanged,       "VLC_EVENT_TYPE_MEDIAPLAYER_LENGTHCHANGED",         "mediaplayer_lengthchanged"},
            {libvlc_MediaPlayerVout,                "VLC_EVENT_TYPE_MEDIAPLAYER_VOUT",                  "mediaplayer_vout"},
            {libvlc_MediaListItemAdded,             "VLC_EVENT_TYPE_MEDIALIST_ITEMADDED",               "medialist_itemadded"},
            {libvlc_MediaListWillAddItem,           "VLC_EVENT_TYPE_MEDIALIST_WILLADDITEM",             "medialist_willadditem"},
            {libvlc_MediaListItemDeleted,           "VLC_EVENT_TYPE_MEDIALIST_ITEMDELETED",             "medialist_itemdeleted"},
            {libvlc_MediaListWillDeleteItem,        "VLC_EVENT_TYPE_MEDIALIST_WILLDELETEITEM",          "medialist_willdeleteitem"},
            {libvlc_MediaListViewItemAdded,         "VLC_EVENT_TYPE_MEDIALISTVIEW_ITEMADDED",           "medialistview_itemadded"},
            {libvlc_MediaListViewWillAddItem,       "VLC_EVENT_TYPE_MEDIALISTVIEW_WILLADDITEM",         "medialistview_willadditem"},
            {libvlc_MediaListViewItemDeleted,       "VLC_EVENT_TYPE_MEDIALISTVIEW_ITEMDELETED",         "medialistview_itemdeleted"},
            {libvlc_MediaListViewWillDeleteItem,    "VLC_EVENT_TYPE_MEDIALISTVIEW_WILLDELETEITEM",      "medialistview_willdeleteitem"},
            {libvlc_MediaListPlayerPlayed,          "VLC_EVENT_TYPE_MEDIALISTPLAYER_PLAYED",            "medialistplayer_played"},
            {libvlc_MediaListPlayerNextItemSet,     "VLC_EVENT_TYPE_MEDIALISTPLAYER_NEXTITEMSET",       "medialistplayer_nextitemset"},
            {libvlc_MediaListPlayerStopped,         "VLC_EVENT_TYPE_MEDIALISTPLAYER_STOPPED",           "medialistplayer_stopped"},
            {libvlc_MediaDiscovererStarted,         "VLC_EVENT_TYPE_MEDIADISCOVERER_STARTED",           "mediadiscoverer_started"},
            {libvlc_MediaDiscovererEnded,           "VLC_EVENT_TYPE_MEDIADISCOVERER_ENDED",             "mediadiscoverer_ended"},
            {libvlc_VlmMediaAdded,                  "VLC_EVENT_TYPE_VLMMEDIA_ADDED",                    "vlmmedia_added"},
            {libvlc_VlmMediaRemoved,                "VLC_EVENT_TYPE_VLMMEDIA_REMOVED",                  "vlmmedia_removed"},
            {libvlc_VlmMediaChanged,                "VLC_EVENT_TYPE_VLMMEDIA_CHANGED",                  "vlmmedia_changed"},
            {libvlc_VlmMediaInstanceStarted,        "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STARTED",          "vlmmediainstance_started"},
            {libvlc_VlmMediaInstanceStopped,        "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STOPPED",          "vlmmediainstance_stopped"},
            {libvlc_VlmMediaInstanceStatusInit,     "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSINIT",       "vlmmediainstance_statusinit"},
            {libvlc_VlmMediaInstanceStatusOpening,  "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSOPENING",    "vlmmediainstance_statusopening"},
            {libvlc_VlmMediaInstanceStatusPlaying,  "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSPLAYING",    "vlmmediainstance_statusplaying"},
            {libvlc_VlmMediaInstanceStatusPause,    "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSPAUSE",      "vlmmediainstance_statuspause"},
            {libvlc_VlmMediaInstanceStatusEnd,      "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSEND",        "vlmmediainstance_statusend"},
            {libvlc_VlmMediaInstanceStatusError,    "VLC_EVENT_TYPE_VLMMEDIAINSTANCE_STATUSERROR",      "vlmmediainstance_statuserror"},
            {0, NULL, NULL}
        };
        etype = g_enum_register_static(g_intern_static_string("VLCEventType"), values);
    }
    return etype;
}
