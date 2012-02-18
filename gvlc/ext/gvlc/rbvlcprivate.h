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

#ifndef __RBVLC_PRIVATE_H__
#define __RBVLC_PRIVATE_H__

#include <ruby.h>
#include <vlc/vlc.h>
#include <rbgobject.h>

#include "rbvlcconversions.h"

G_BEGIN_DECLS

#define ZEROBOOL2RVAL(bool) ((bool == 0) ? Qtrue : Qfalse)
#define RVAL2CSTRARY(ary, len) rval2cstrary(ary, len)
#define CSTRARY_FREE(ary, len) cstrary_free(ary, len)
#define MODULEDESCRIPTIONLIST2RVAL(list) moduledescriptionlist2rval(list)
#define TRACKDESCRIPTIONLIST2RVAL(list) trackdescriptionlist2rval(list)
#define AUDIOOUTPUTLIST2RVAL(list) audiooutputlist2rval(list)

G_GNUC_INTERNAL char **rval2cstrary(VALUE ary, long *len);
G_GNUC_INTERNAL void cstrary_free(char **ary, long len);
G_GNUC_INTERNAL VALUE moduledescriptionlist2rval(libvlc_module_description_t *list);
G_GNUC_INTERNAL VALUE trackdescriptionlist2rval(libvlc_track_description_t *list);
G_GNUC_INTERNAL VALUE audiooutputlist2rval(libvlc_audio_output_t *list);
G_GNUC_INTERNAL VALUE em_attach_event(libvlc_event_manager_t *event_manager, VALUE self, VALUE event_type);
G_GNUC_INTERNAL VALUE em_detach_event(libvlc_event_manager_t *event_manager, VALUE self, VALUE event_type);

G_GNUC_INTERNAL void Init_vlc(void);

G_GNUC_INTERNAL void Init_vlc_core(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_media(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_mediaplayer(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_medialist(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_medialistplayer(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_medialibrary(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_mediadiscoverer(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_event(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_moduledescription(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_trackdescription(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_audiooutput(VALUE mVLC);
G_GNUC_INTERNAL void Init_vlc_media_trackinfo(VALUE cMedia);
G_GNUC_INTERNAL void Init_vlc_media_stats(VALUE cMedia);

G_GNUC_INTERNAL void Init_vlc_event_type(VALUE cEvent);

#define VLC_TYPE_CORE (vlc_core_get_type())
G_GNUC_INTERNAL GType vlc_core_get_type(void);
#define VLC_TYPE_MEDIA (vlc_media_get_type())
G_GNUC_INTERNAL GType vlc_media_get_type(void);
#define VLC_TYPE_MEDIA_PLAYER (vlc_media_player_get_type())
G_GNUC_INTERNAL GType vlc_media_player_get_type(void);
#define VLC_TYPE_MEDIA_LIST (vlc_media_list_get_type())
G_GNUC_INTERNAL GType vlc_media_list_get_type(void);
#define VLC_TYPE_MEDIA_LIST_PLAYER (vlc_media_list_player_get_type())
G_GNUC_INTERNAL GType vlc_media_list_player_get_type(void);
#define VLC_TYPE_MEDIA_LIBRARY (vlc_media_library_get_type())
G_GNUC_INTERNAL GType vlc_media_library_get_type(void);
#define VLC_TYPE_MEDIA_DISCOVERER (vlc_media_discoverer_get_type())
G_GNUC_INTERNAL GType vlc_media_discoverer_get_type(void);
#define VLC_TYPE_EVENT (vlc_event_get_type())
G_GNUC_INTERNAL GType vlc_event_get_type(void);
#define VLC_TYPE_MEDIA_TRACK_INFO (vlc_media_track_info_get_type())
G_GNUC_INTERNAL GType vlc_media_track_info_get_type(void);
#define VLC_TYPE_MODULE_DESCRIPTION (vlc_module_description_get_type())
G_GNUC_INTERNAL GType vlc_module_description_get_type(void);
#define VLC_TYPE_TRACK_DESCRIPTION (vlc_track_description_get_type())
G_GNUC_INTERNAL GType vlc_track_description_get_type(void);
#define VLC_TYPE_AUDIO_OUTPUT (vlc_audio_output_get_type())
G_GNUC_INTERNAL GType vlc_audio_output_get_type(void);
#define VLC_TYPE_MEDIA_STATS (vlc_media_stats_get_type())
G_GNUC_INTERNAL GType vlc_media_stats_get_type(void);

#define VLC_TYPE_TRACK_TYPE (vlc_track_type_get_type())
G_GNUC_INTERNAL GType vlc_track_type_get_type(void);
#define VLC_TYPE_META_TYPE (vlc_meta_type_get_type())
G_GNUC_INTERNAL GType vlc_meta_type_get_type(void);
#define VLC_TYPE_STATE (vlc_state_get_type())
G_GNUC_INTERNAL GType vlc_state_get_type(void);
#define VLC_TYPE_PLAYBACK_MODE (vlc_playback_mode_get_type())
G_GNUC_INTERNAL GType vlc_playback_mode_get_type(void);
#define VLC_TYPE_NAVIGATE_MODE (vlc_navigate_mode_get_type())
G_GNUC_INTERNAL GType vlc_navigate_mode_get_type(void);
#define VLC_TYPE_VIDEO_MARQUEE_OPTION (vlc_video_marquee_option_get_type())
G_GNUC_INTERNAL GType vlc_video_marquee_option_get_type(void);
#define VLC_TYPE_VIDEO_LOGO_OPTION (vlc_video_logo_option_get_type())
G_GNUC_INTERNAL GType vlc_video_logo_option_get_type(void);
#define VLC_TYPE_VIDEO_ADJUST_OPTION (vlc_video_adjust_option_get_type())
G_GNUC_INTERNAL GType vlc_video_adjust_option_get_type(void);
#define VLC_TYPE_AUDIO_OUTPUT_CHANNEL (vlc_audio_output_channel_get_type())
G_GNUC_INTERNAL GType vlc_audio_output_channel_get_type(void);
#define VLC_TYPE_AUDIO_OUTPUT_DEVICE_TYPES (vlc_audio_output_device_types_get_type())
G_GNUC_INTERNAL GType vlc_audio_output_device_types_get_type(void);
#define VLC_TYPE_EVENT_TYPE (vlc_event_type_get_type())
G_GNUC_INTERNAL GType vlc_event_type_get_type(void);

G_END_DECLS

#endif
