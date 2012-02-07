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

static libvlc_media_list_player_t *
vlc_media_list_player_copy(const libvlc_media_list_player_t *instance)
{
    libvlc_media_list_player_retain((libvlc_media_list_player_t *)instance);
    return (libvlc_media_list_player_t *)instance;
}

static void
vlc_media_list_player_free(libvlc_media_list_player_t *instance)
{
    libvlc_media_list_player_release(instance);
}

GType
vlc_media_list_player_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaListPlayer",
                                                (GBoxedCopyFunc)vlc_media_list_player_copy,
                                                (GBoxedFreeFunc)vlc_media_list_player_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMediaListPlayer
#define _SELF(self) (RVAL2VLCMEDIALISTPLAYER(self))

/*
 * Create new media_list_player.
 *
 * @param [Hash] options media list player options
 * @option options [VLC::Core] :core (nil) the core instance
 * @option options [VLC::MediaList] :list (nil) the media list instance
 * @option options [VLC::MediaPlayer] :player (nil) the media player instance
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, core, list, player;
    libvlc_media_list_player_t *instance;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "core",    &core,
                     "list",    &list,
                     "player",  &player,
                     NULL);

    if (NIL_P(core) && !NIL_P(list))
        core = rb_funcall(list, rb_intern("core"), 0);
    if (NIL_P(core) && !NIL_P(player))
        core = rb_funcall(player, rb_intern("core"), 0);
    if (NIL_P(core))
        core = rb_funcall(GTYPE2CLASS(VLC_TYPE_CORE), rb_intern("new"), 0);

    instance = libvlc_media_list_player_new(RVAL2VLCCORE(core));
    if (!instance)
        rb_raise(rb_eArgError, "Invalid arguments.");
    rb_ivar_set(self, rb_intern("core"), core);

    G_INITIALIZE(self, instance);
    vlc_media_list_player_free(instance);

    if (NIL_P(list))
        list = rb_funcall(GTYPE2CLASS(VLC_TYPE_MEDIA_LIST), rb_intern("new"), 1, core);
    rb_funcall(self, rb_intern("set_media_list"), 1, list);

    if (NIL_P(player))
        player = rb_funcall(GTYPE2CLASS(VLC_TYPE_MEDIA_PLAYER), rb_intern("new"), 1, core);
    rb_funcall(self, rb_intern("set_media_player"), 1, player);

    return Qnil;
}

static VALUE
rg_core(VALUE self)
{
    return rb_ivar_get(self, rb_intern("core"));
}

/*
 * Replace media player in media_list_player with this instance.
 *
 * @param [VLC::MediaPlayer] player media player instance
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_set_media_player(VALUE self, VALUE player)
{
    libvlc_media_list_player_set_media_player(_SELF(self), RVAL2VLCMEDIAPLAYER(player));
    return self;
}

/*
 * Set the media list associated with the player
 *
 * @param [VLC::MediaList] list list of media
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_set_media_list(VALUE self, VALUE list)
{
    libvlc_media_list_player_set_media_list(_SELF(self), RVAL2VLCMEDIALIST(list));
    return self;
}

/*
 * Is media list playing?
 *
 * @return [Boolean] true for playing and false for not playing
 * @todo fixme
 */
static VALUE
rg_playing_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_list_player_is_playing(_SELF(self)));
}

/*
 * Play media list
 *
 * @param [VLC::Media, Integer] the media instance or index in media list
 * @return [Boolean] true upon success false if the item wasn't found
 * @todo fixme
 */
static VALUE
rg_play(int argc, VALUE *argv, VALUE self)
{
    VALUE media, result = Qtrue;

    rb_scan_args(argc, argv, "01", &media);
    if (NIL_P(media))
        libvlc_media_list_player_play(_SELF(self));
    else if (TYPE(media) == T_FIXNUM)
        result = ZEROBOOL2RVAL(libvlc_media_list_player_play_item_at_index(_SELF(self), NUM2INT(media)));
    else
        result = ZEROBOOL2RVAL(libvlc_media_list_player_play_item(_SELF(self), RVAL2VLCMEDIA(media)));

    return result;
}

/*
 * Pause media list
 *
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_pause(VALUE self)
{
    libvlc_media_list_player_pause(_SELF(self));
    return self;
}

/*
 * Stop playing media list
 *
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_stop(VALUE self)
{
    libvlc_media_list_player_stop(_SELF(self));
    return self;
}

/*
 * Get current state of media list player
 *
 * @return [VLC::State] state for media list player
 * @todo fixme
 */
static VALUE
rg_state(VALUE self)
{
    return VLCSTATE2RVAL(libvlc_media_list_player_get_state(_SELF(self)));
}

/*
 * Play previous item from media list
 *
 * @return [Boolean] true upon success false if there is no previous item
 * @todo fixme
 */
static VALUE
rg_previous(VALUE self)
{
    return ZEROBOOL2RVAL(libvlc_media_list_player_previous(_SELF(self)));
}

/*
 * Play next item from media list
 *
 * @return [Boolean] true upon success false if there is no next item
 * @todo fixme
 */
static VALUE
rg_next(VALUE self)
{
    return ZEROBOOL2RVAL(libvlc_media_list_player_next(_SELF(self)));
}

/*
 * Sets the playback mode for the playlist
 *
 * @param [VLC::PlaybackMode] mode playback mode specification
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_set_playback_mode(VALUE self, VALUE mode)
{
    libvlc_media_list_player_set_playback_mode(_SELF(self), RVAL2VLCPLAYBACKMODE(mode));
    return self;
}

static VALUE
rg_attach_event(VALUE self, VALUE event_type)
{
    return em_attach_event(libvlc_media_list_player_event_manager(_SELF(self)), self, event_type);
}

static VALUE
rg_detach_event(VALUE self, VALUE event_type)
{
    return em_detach_event(libvlc_media_list_player_event_manager(_SELF(self)), self, event_type);
}

void
Init_vlc_medialistplayer(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_LIST_PLAYER, "MediaListPlayer", mVLC);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(core, 0);
    RG_DEF_METHOD(set_media_player, 1);
    RG_DEF_METHOD(set_media_list, 1);
    RG_DEF_METHOD_P(playing, 0);
    RG_DEF_METHOD(play, -1);
    RG_DEF_METHOD(pause, 0);
    RG_DEF_METHOD(stop, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(previous, 0);
    RG_DEF_METHOD(next, 0);
    RG_DEF_METHOD(set_playback_mode, 1);
    RG_DEF_METHOD(attach_event, 1);
    RG_DEF_METHOD(detach_event, 1);
}
