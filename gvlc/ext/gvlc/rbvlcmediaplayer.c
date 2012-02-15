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

static libvlc_media_player_t *
vlc_media_player_copy(const libvlc_media_player_t *instance)
{
    libvlc_media_player_retain((libvlc_media_player_t *)instance);
    return (libvlc_media_player_t *)instance;
}

static void
vlc_media_player_free(libvlc_media_player_t *instance)
{
    libvlc_media_player_release(instance);
}

GType
vlc_media_player_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaPlayer",
                                                (GBoxedCopyFunc)vlc_media_player_copy,
                                                (GBoxedFreeFunc)vlc_media_player_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMediaPlayer
#define _SELF(self) (RVAL2VLCMEDIAPLAYER(self))

static ID id_core;

/*
 * @overload initialize(core = nil)
 *   Create an empty Media Player object
 *
 *   @param [VLC::Core] core the core instance
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @overload initialize(media)
 *   Create a Media Player object from a Media
 *
 *   @param [VLC::Media, Hash] media the media instance or specify media Hash (see {VLC::Media#initialize})
 *   @raise [ArgumentError] Invalid or unsupported arguments
 *
 * @todo fixme
 */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg, core;
    libvlc_media_player_t *instance;

    rb_scan_args(argc, argv, "01", &arg);
    if (NIL_P(arg))
        arg = rb_funcall(GTYPE2CLASS(VLC_TYPE_CORE), rb_intern("new"), 0);
    else if (TYPE(arg) == T_HASH)
        arg = rb_funcall(GTYPE2CLASS(VLC_TYPE_MEDIA), rb_intern("new"), 1, arg);

    GType gtype = RVAL2GTYPE(arg);
    if (gtype == VLC_TYPE_MEDIA) {
        instance = libvlc_media_player_new_from_media(RVAL2VLCMEDIA(arg));
        core = rb_funcall(arg, id_core, 0);
    } else {
        instance = libvlc_media_player_new(RVAL2VLCCORE(arg));
        core = arg;
    }
    if (!instance)
        rb_raise(rb_eArgError, "Invalid arguments.");
    rb_ivar_set(self, id_core, core);

    G_INITIALIZE(self, instance);
    vlc_media_player_free(instance);
    return Qnil;
}

/*
 *
 * @return [VLC::Core]
 * @todo fixme
 */
static VALUE
rg_core(VALUE self)
{
    return rb_ivar_get(self, id_core);
}

/*
 * Set the media that will be used by the media_player.
 *
 * @param [VLC::Media, Hash] media the media instance or specify media Hash (see {VLC::Media#initialize})
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_set_media(VALUE self, VALUE media)
{
    if (TYPE(media) == T_HASH) {
        if (NIL_P(rb_hash_lookup(media, ID2SYM(id_core))))
            rb_hash_aset(media, ID2SYM(id_core), rg_core(self));
        media = rb_funcall(GTYPE2CLASS(VLC_TYPE_MEDIA), rb_intern("new"), 1, media);
    }

    libvlc_media_player_set_media(_SELF(self), RVAL2VLCMEDIA(media));
    return self;
}

/*
 * Get the media used by the media_player.
 *
 * @return [VLC::Media] the media associated with the media_player or nil
 * @todo fixme
 */
static VALUE
rg_media(VALUE self)
{
    return VLCMEDIA2RVAL(libvlc_media_player_get_media(_SELF(self)));
}

/*
 * is_playing
 *
 * @return [Boolean] true if the media player is playing, false otherwise
 * @todo fixme
 */
static VALUE
rg_playing_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_player_is_playing(_SELF(self)));
}

/*
 * Play
 *
 * @return [Boolean] true if playback started (and was already started), or false on error
 * @todo fixme
 */
static VALUE
rg_play(VALUE self)
{
    return ZEROBOOL2RVAL(libvlc_media_player_play(_SELF(self)));
}

/*
 * Pause or resume (no effect if there is no media)
 *
 * @param [Boolean] do_pause play/resume if false, pause if true
 * @return self
 * @todo fixme
 */
static VALUE
rg_pause(int argc, VALUE *argv, VALUE self)
{
    VALUE do_pause;

    rb_scan_args(argc, argv, "01", &do_pause);
    if (RVAL2CBOOL(do_pause))
        libvlc_media_player_set_pause(_SELF(self), true);
    else
        libvlc_media_player_pause(_SELF(self));

    return self;
}

/*
 * Stop (no effect if there is no media)
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_stop(VALUE self)
{
    libvlc_media_player_stop(_SELF(self));
    return self;
}

/*
 * Set an X Window System drawable where the media player should render its
 * video output. If LibVLC was built without X11 output support, then this has
 * no effects.
 *
 * The specified identifier must correspond to an existing Input/Output class
 * X11 window. Pixmaps are <b>not</b> supported. The caller shall ensure that
 * the X11 server is the same as the one the VLC instance has been configured
 * with. This function must be called before video playback is started;
 * otherwise it will only take effect after playback stop and restart.
 *
 * @param [Integer] xid the ID of the X window
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_xwindow(VALUE self, VALUE xid)
{
    libvlc_media_player_set_xwindow(_SELF(self), NUM2INT(xid));
    return self;
}

/*
 * Get the X Window System window identifier previously set with
 * {#set_xwindow}. Note that this will return the identifier
 * even if VLC is not currently using it (for instance if it is playing an
 * audio-only input).
 *
 * @return [Integer] an X window ID, or nil if none where set.
 * @todo fixme
 */
static VALUE
rg_xwindow(VALUE self)
{
    uint32_t xid = libvlc_media_player_get_xwindow(_SELF(self));
    return xid == 0 ? Qnil : UINT2NUM(xid);
}

/*
 * Get the current movie length (in ms).
 *
 * @return [Integer] the movie length (in ms), or nil if there is no media.
 * @todo fixme
 */
static VALUE
rg_length(VALUE self)
{
    libvlc_time_t len = libvlc_media_player_get_length(_SELF(self));
    return len < 0 ? Qnil : LONG2NUM(len);
}

/*
 * Set the movie time (in ms). This has no effect if no media is being played.
 * Not all formats and protocols support this.
 *
 * @param [Integer] time the movie time (in ms).
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_time(VALUE self, VALUE time)
{
    libvlc_media_player_set_time(_SELF(self), NUM2LONG(time));
    return self;
}

/*
 * Get the current movie time (in ms).
 *
 * @return [Integer] the movie time (in ms), or nil if there is no media.
 * @todo fixme
 */
static VALUE
rg_time(VALUE self)
{
    libvlc_time_t time = libvlc_media_player_get_time(_SELF(self));
    return time < 0 ? Qnil : LONG2NUM(time);
}

/*
 * Set movie position. This has no effect if playback is not enabled.
 * This might not work depending on the underlying input format and protocol.
 *
 * @param [Float] pos the position
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_position(VALUE self, VALUE pos)
{
    libvlc_media_player_set_position(_SELF(self), NUM2DBL(pos));
    return self;
}

/*
 * Get movie position.
 *
 * @return [Float] movie position, or nil in case of error
 * @todo fixme
 */
static VALUE
rg_position(VALUE self)
{
    float pos = libvlc_media_player_get_position(_SELF(self));
    return pos < 0 ? Qnil : DBL2NUM(pos);
}

/*
 * Set movie chapter (if applicable).
 *
 * @param [Integer] chapter chapter number to play
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_chapter(VALUE self, VALUE chapter)
{
    libvlc_media_player_set_chapter(_SELF(self), NUM2INT(chapter));
    return self;
}

/*
 * Get movie chapter.
 *
 * @return [Integer] chapter number currently playing, or nil if there is no media.
 * @todo fixme
 */
static VALUE
rg_chapter(VALUE self)
{
    int chapter = libvlc_media_player_get_chapter(_SELF(self));
    return chapter < 0 ? Qnil : INT2NUM(chapter);
}

/*
 * Get movie or title chapter count
 *
 * @param [Integer] title title number
 * @return [Integer] number of chapters in movie or title, or nil
 * @todo fixme
 */
static VALUE
rg_chapter_count(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    int count;

    rb_scan_args(argc, argv, "01", &title);

    if (NIL_P(title))
        count = libvlc_media_player_get_chapter_count(_SELF(self));
    else
        count = libvlc_media_player_get_chapter_count_for_title(_SELF(self), NUM2INT(title));

    return count < 0 ? Qnil : INT2NUM(count);
}

/*
 * Get the description of available chapters for specific title.
 *
 * @param [Integer] title title number
 * @return [Array<VLC::TrackDescription>] list containing description of available chapter for title
 * @todo fixme
 */
static VALUE
rg_chapter_description(VALUE self, VALUE title)
{
    return TRACKDESCRIPTIONLIST2RVAL(libvlc_video_get_chapter_description(_SELF(self), NUM2INT(title)));
}

/*
 * Is the player able to play
 *
 * @return [Boolean]
 * @todo fixme
 */
static VALUE
rg_will_play_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_player_will_play(_SELF(self)));
}

/*
 * Set movie title
 *
 * @param [Integer] title title number to play
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_title(VALUE self, VALUE title)
{
    libvlc_media_player_set_title(_SELF(self), NUM2INT(title));
    return self;
}

/*
 * Get movie title
 *
 * @return [Integer] title number currently playing, or nil
 * @todo fixme
 */
static VALUE
rg_title(VALUE self)
{
    int title = libvlc_media_player_get_title(_SELF(self));
    return title < 0 ? Qnil : INT2NUM(title);
}

/*
 * Get movie title count
 *
 * @return [Integer] title number count, or nil
 * @todo fixme
 */
static VALUE
rg_title_count(VALUE self)
{
    int count = libvlc_media_player_get_title_count(_SELF(self));
    return count < 0 ? Qnil : INT2NUM(count);
}

/*
 * Get the description of available titles.
 *
 * @return [Array<VLC::TrackDescription>] list containing description of available titles
 * @todo fixme
 */
static VALUE
rg_title_description(VALUE self)
{
    return TRACKDESCRIPTIONLIST2RVAL(libvlc_video_get_title_description(_SELF(self)));
}

/*
 * Set previous chapter (if applicable)
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_previous_chapter(VALUE self)
{
    libvlc_media_player_previous_chapter(_SELF(self));
    return self;
}

/*
 * Set next chapter (if applicable)
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_next_chapter(VALUE self)
{
    libvlc_media_player_next_chapter(_SELF(self));
    return self;
}

/*
 * Set movie play rate
 *
 * @param [Float] rate movie play rate to set
 * @return [Boolean] false if an error was detected, true otherwise (but even then, it might
 * not actually work depending on the underlying media protocol)
 * @todo fixme
 */
static VALUE
rg_set_rate(VALUE self, VALUE rate)
{
    return ZEROBOOL2RVAL(libvlc_media_player_set_rate(_SELF(self), NUM2DBL(rate)));
}

/*
 * Get the requested movie play rate.
 *
 * @note
 *   Depending on the underlying media, the requested rate may be
 *   different from the real playback rate.
 *
 * @return [Float] movie play rate
 * @todo fixme
 */
static VALUE
rg_rate(VALUE self)
{
    return DBL2NUM(libvlc_media_player_get_rate(_SELF(self)));
}

/*
 * Get current movie state
 *
 * @return [VLC::State] the current state of the media player
 * @todo fixme
 */
static VALUE
rg_state(VALUE self)
{
    return VLCSTATE2RVAL(libvlc_media_player_get_state(_SELF(self)));
}

/*
 * Get movie fps rate
 *
 * @return [Float] frames per second (fps) for this playing movie, or nil if unspecified
 * @todo fixme
 */
static VALUE
rg_fps(VALUE self)
{
    float fps = libvlc_media_player_get_fps(_SELF(self));
    return fps == 0 ? Qnil : DBL2NUM(fps);
}

/*
 * How many video outputs does this media player have?
 *
 * @return [Integer] the number of video outputs
 * @todo fixme
 */
static VALUE
rg_has_vout(VALUE self)
{
    return UINT2NUM(libvlc_media_player_has_vout(_SELF(self)));
}

/*
 * Is this media player seekable?
 *
 * @return [Boolean] true if the media player can seek
 * @todo fixme
 */
static VALUE
rg_seekable_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_player_is_seekable(_SELF(self)));
}

/*
 * Can this media player be paused?
 *
 * @return [Boolean] true if the media player can pause
 * @todo fixme
 */
static VALUE
rg_can_pause_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_player_can_pause(_SELF(self)));
}

/*
 * Display the next frame (if supported)
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_next_frame(VALUE self)
{
    libvlc_media_player_next_frame(_SELF(self));
    return self;
}

/*
 * Navigate through DVD Menu
 *
 * @param [VLC::NavigateMode] mode the Navigation mode
 * @return self
 * @todo fixme
 */
static VALUE
rg_navigate(VALUE self, VALUE mode)
{
    libvlc_media_player_navigate(_SELF(self), RVAL2VLCNAVIGATEMODE(mode));
    return self;
}

/*
 * Toggle fullscreen status on non-embedded video outputs.
 *
 * @note
 *   The same limitations applies to this function as to {#set_fullscreen}.
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_toggle_fullscreen(VALUE self)
{
    libvlc_toggle_fullscreen(_SELF(self));
    return self;
}

/*
 * Enable or disable fullscreen.
 *
 * @note
 *   With most window managers, only a top-level windows can be in
 *   full-screen mode. Hence, this function will not operate properly if
 *   {#set_xwindow} was used to embed the video in a
 *   non-top-level window. In that case, the embedding window must be reparented
 *   to the root window <b>before</b> fullscreen mode is enabled. You will want
 *   to reparent it back to its normal parent when disabling fullscreen.
 *
 * @param [Boolean] fullscreen boolean for fullscreen status
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_fullscreen(VALUE self, VALUE fullscreen)
{
    libvlc_set_fullscreen(_SELF(self), RVAL2CBOOL(fullscreen));
    return self;
}

/*
 * Get current fullscreen status.
 *
 * @return [Boolean] the fullscreen status
 * @todo fixme
 */
static VALUE
rg_fullscreen(VALUE self)
{
    return CBOOL2RVAL(libvlc_get_fullscreen(_SELF(self)));
}

/*
 * Enable or disable key press events handling, according to the LibVLC hotkeys
 * configuration. By default and for historical reasons, keyboard events are
 * handled by the LibVLC video widget.
 *
 * @note
 *   On X11, there can be only one subscriber for key press and mouse
 *   click events per window. If your application has subscribed to those events
 *   for the X window ID of the video widget, then LibVLC will not be able to
 *   handle key presses and mouse clicks in any case.
 *
 * @note
 *   This function is only implemented for X11 and Win32 at the moment.
 *
 * @param [Boolean] on true to handle key press events, false to ignore them.
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_key_input(VALUE self, VALUE on)
{
    libvlc_video_set_key_input(_SELF(self), RVAL2CBOOL(on));
    return self;
}

/*
 * Enable or disable mouse click events handling. By default, those events are
 * handled. This is needed for DVD menus to work, as well as a few video
 * filters such as "puzzle".
 *
 * @note
 *   This function is only implemented for X11 and Win32 at the moment.
 *
 * @param [Boolean] on true to handle mouse click events, false to ignore them.
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_mouse_input(VALUE self, VALUE on)
{
    libvlc_video_set_mouse_input(_SELF(self), RVAL2CBOOL(on));
    return self;
}

/*
 * Get the pixel dimensions of a video.
 *
 * @param [Integer] video number of the video (starting from, and most commonly 0)
 * @return [Array(Integer, Integer)] the pixel dimensions(width and height) or nil
 * @todo fixme
 */
static VALUE
rg_get_size(VALUE self, VALUE video)
{
    unsigned int width, height;
    int result = libvlc_video_get_size(_SELF(self), NUM2UINT(video), &width, &height);
    return result == 0 ? rb_ary_new3(2, UINT2NUM(width), UINT2NUM(height)) : Qnil;
}

/*
 * Get the mouse pointer coordinates over a video.
 * Coordinates are expressed in terms of the decoded video resolution,
 * <b>not</b> in terms of pixels on the screen/viewport (to get the latter,
 * you can query your windowing system directly).
 *
 * Either of the coordinates may be negative or larger than the corresponding
 * dimension of the video, if the cursor is outside the rendering area.
 *
 * @note
 *   The coordinates may be out-of-date if the pointer is not located
 *   on the video rendering area. LibVLC does not track the pointer if it is
 *   outside of the video widget.
 *
 * @note
 *   LibVLC does not support multiple pointers (it does of course support
 *   multiple input devices sharing the same pointer) at the moment.
 *
 * @param [Integer] video number of the video (starting from, and most commonly 0)
 * @return [Array(Integer, Integer)] the mouse pointer coordinates(abscissa and ordinate) or nil
 * @todo fixme
 */
static VALUE
rg_get_cursor(VALUE self, VALUE video)
{
    int x, y, result;
    result = libvlc_video_get_cursor(_SELF(self), NUM2UINT(video), &x, &y);
    return result == 0 ? rb_ary_new3(2, INT2NUM(x), INT2NUM(y)) : Qnil;
}

/*
 * Set the video scaling factor. That is the ratio of the number of pixels on
 * screen to the number of pixels in the original decoded video in each
 * dimension. Zero is a special value; it will adjust the video to the output
 * window/drawable (in windowed mode) or the entire screen.
 *
 * Note that not all video outputs support scaling.
 *
 * @param [Float] scale the scaling factor, or zero
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_scale(VALUE self, VALUE scale)
{
    libvlc_video_set_scale(_SELF(self), NUM2DBL(scale));
    return self;
}

/*
 * Get the current video scaling factor.
 * See also {#set_scale}.
 *
 * @return [Float] the currently configured zoom factor, or 0. if the video is set
 * to fit to the output window/drawable automatically.
 * @todo fixme
 */
static VALUE
rg_scale(VALUE self)
{
    return DBL2NUM(libvlc_video_get_scale(_SELF(self)));
}

/*
 * Set new video aspect ratio.
 *
 * @note
 *   Invalid aspect ratios are ignored.
 *
 * @param [Float] ratio new video aspect-ratio or nil to reset to default
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_aspect_ratio(VALUE self, VALUE ratio)
{
    libvlc_video_set_aspect_ratio(_SELF(self), RVAL2CSTR_ACCEPT_NIL(ratio));
    return self;
}

/*
 * Get current video aspect ratio.
 *
 * @return [String] the video aspect ratio or nil if unspecified
 * @todo fixme
 */
static VALUE
rg_aspect_ratio(VALUE self)
{
    return CSTR2RVAL_FREE(libvlc_video_get_aspect_ratio(_SELF(self)));
}

/*
 * Set new video subtitle.
 *
 * @param [Integer] spu new video subtitle to select
 * @return [Boolean] true on success, false if out of range
 * @todo fixme
 */
static VALUE
rg_set_spu(VALUE self, VALUE spu)
{
    return ZEROBOOL2RVAL(libvlc_video_set_spu(_SELF(self), NUM2UINT(spu)));
}

/*
 * Get current video subtitle.
 *
 * @return [Integer] the video subtitle selected, or nil if none
 * @todo fixme
 */
static VALUE
rg_spu(VALUE self)
{
    int spu = libvlc_video_get_spu(_SELF(self));
    return spu < 0 ? Qnil : INT2NUM(spu);
}

/*
 * Get the number of available video subtitles.
 *
 * @return [Integer] the number of available video subtitles
 * @todo fixme
 */
static VALUE
rg_spu_count(VALUE self)
{
    return INT2NUM(libvlc_video_get_spu_count(_SELF(self)));
}

/*
 * Get the description of available video subtitles.
 *
 * @return [Array<VLC::TrackDescription>] list containing description of available video subtitles
 * @todo fixme
 */
static VALUE
rg_spu_description(VALUE self)
{
    return TRACKDESCRIPTIONLIST2RVAL(libvlc_video_get_spu_description(_SELF(self)));
}

/*
 * Set new video subtitle file.
 *
 * @param [String] file new video subtitle file
 * @return [Boolean] the success status
 * @todo fixme
 */
static VALUE
rg_set_subtitle_file(VALUE self, VALUE file)
{
    return CBOOL2RVAL(libvlc_video_set_subtitle_file(_SELF(self), RVAL2CSTR(file)));
}

/*
 * Set the subtitle delay. This affects the timing of when the subtitle will
 * be displayed. Positive values result in subtitles being displayed later,
 * while negative values will result in subtitles being displayed earlier.
 *
 * The subtitle delay will be reset to zero each time the media changes.
 *
 * @param [Integer] delay the display of subtitles should be delayed (in microseconds)
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_set_spu_delay(VALUE self, VALUE delay)
{
    return ZEROBOOL2RVAL(libvlc_video_set_spu_delay(_SELF(self), NUM2LONG(delay)));
}

/*
 * Get the current subtitle delay. Positive values means subtitles are being
 * displayed later, negative values earlier.
 *
 * @return [Integer] the display of subtitles is being delayed (in microseconds)
 * @todo fixme
 */
static VALUE
rg_spu_delay(VALUE self)
{
    return LONG2NUM(libvlc_video_get_spu_delay(_SELF(self)));
}

/*
 * Set new crop filter geometry.
 *
 * @param [String] geometry new crop filter geometry (nil to unset)
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_crop_geometry(VALUE self, VALUE geometry)
{
    libvlc_video_set_crop_geometry(_SELF(self), RVAL2CSTR_ACCEPT_NIL(geometry));
    return self;
}

/*
 * Set new teletext page to retrieve.
 *
 * @param [Integer] page teletex page number requested
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_teletext(VALUE self, VALUE page)
{
    libvlc_video_set_teletext(_SELF(self), NUM2INT(page));
    return self;
}

/*
 * Get current teletext page requested.
 *
 * @return [Integer] the current teletext page requested.
 * @todo fixme
 */
static VALUE
rg_teletext(VALUE self)
{
    return INT2NUM(libvlc_video_get_teletext(_SELF(self)));
}

/*
 * Toggle teletext transparent status on video output.
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_toggle_teletext(VALUE self)
{
    libvlc_toggle_teletext(_SELF(self));
    return self;
}

/*
 * Set video track.
 *
 * @param [Integer] track the track
 * @return [Boolean] true on success, false if out of range
 * @todo fixme
 */
static VALUE
rg_set_video_track(VALUE self, VALUE track)
{
    return ZEROBOOL2RVAL(libvlc_video_set_track(_SELF(self), NUM2INT(track)));
}

/*
 * Get current video track.
 *
 * @return [Integer] the video track or nil if none
 * @todo fixme
 */
static VALUE
rg_video_track(VALUE self)
{
    int track = libvlc_video_get_track(_SELF(self));
    return track < 0 ? Qnil : INT2NUM(track);
}

/*
 * Get number of available video tracks.
 *
 * @return [Integer] the number of available video tracks
 * @todo fixme
 */
static VALUE
rg_video_track_count(VALUE self)
{
    return INT2NUM(libvlc_video_get_track_count(_SELF(self)));
}

/*
 * Get the description of available video tracks.
 *
 * @return [Array<VLC::TrackDescription>] list with description of available video tracks
 * @todo fixme
 */
static VALUE
rg_video_track_description(VALUE self)
{
    return TRACKDESCRIPTIONLIST2RVAL(libvlc_video_get_track_description(_SELF(self)));
}

/*
 * Take a snapshot of the current video window.
 *
 * If :width AND :height is 0, original size is used.
 * If :width XOR :height is 0, original aspect-ratio is preserved.
 *
 * @param [Hash] options snapshot options
 * @option options [Integer] :video (0) number of video output (typically 0 for the first/only one)
 * @option options [String] :file the path where to save the screenshot to
 * @option options [Integer] :width (0) the snapshot's width
 * @option options [Integer] :height (0) the snapshot's height
 * @return [Boolean] true on success, false if the video was not found
 * @todo fixme
 */
static VALUE
rg_take_snapshot(VALUE self, VALUE options)
{
    VALUE video, file, width, height;
    int result;

    rbg_scan_options(options,
                    "video",    &video,
                    "file",     &file,
                    "width",    &width,
                    "height",   &height,
                    NULL);
    result = libvlc_video_take_snapshot(_SELF(self),
                                        NIL_P(video) ? 0 : NUM2UINT(video),
                                        RVAL2CSTR(file),
                                        NIL_P(width) ? 0 : NUM2UINT(width),
                                        NIL_P(height) ? 0 : NUM2UINT(height));
    return ZEROBOOL2RVAL(result);
}

/*
 * Enable or disable deinterlace filter
 *
 * @param [String] mode type of deinterlace filter, nil to disable
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_deinterlace(VALUE self, VALUE mode)
{
    libvlc_video_set_deinterlace(_SELF(self), RVAL2CSTR_ACCEPT_NIL(mode));
    return self;
}

/*
 * Enable, disable or set an integer marquee option
 *
 * Setting {VLC::VideoMarqueeOption::ENABLE} has the side effect of enabling (arg !0)
 * or disabling (arg 0) the marq filter.
 *
 * @param [VLC::VideoMarqueeOption] option marq option to set
 * @param [Integer] value marq option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_marquee_int(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_marquee_int(_SELF(self), RVAL2VLCVIDEOMARQUEEOPTION(option), NUM2INT(value));
    return self;
}

/*
 * Set a marquee string option
 *
 * @param [VLC::VideoMarqueeOption] option marq option to set
 * @param [String] value marq option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_marquee_string(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_marquee_string(_SELF(self), RVAL2VLCVIDEOMARQUEEOPTION(option), RVAL2CSTR(value));
    return self;
}

/*
 * Get an integer marquee option value
 *
 * @param [VLC::VideoMarqueeOption] option marq option to get
 * @return [Integer] marq option value
 * @todo fixme
 */
static VALUE
rg_get_marquee_int(VALUE self, VALUE option)
{
    return INT2NUM(libvlc_video_get_marquee_int(_SELF(self), RVAL2VLCVIDEOMARQUEEOPTION(option)));
}

/*
 * Get a string marquee option value
 *
 * @param [VLC::VideoMarqueeOption] option marq option to get
 * @return [String] marq option value
 * @todo fixme
 */
static VALUE
rg_get_marquee_string(VALUE self, VALUE option)
{
    return CSTR2RVAL(libvlc_video_get_marquee_string(_SELF(self), RVAL2VLCVIDEOMARQUEEOPTION(option)));
}

/*
 * Set logo option as integer. Options that take a different type value
 * are ignored.
 * Passing {VLC::VideoLogoOption::ENABLE} as option value has the side effect of
 * starting (arg !0) or stopping (arg 0) the logo filter.
 *
 * @param [VLC::VideoLogoOption] option logo option to set
 * @param [Integer] value logo option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_logo_int(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_logo_int(_SELF(self), RVAL2VLCVIDEOLOGOOPTION(option), NUM2INT(value));
    return self;
}

/*
 * Set logo option as string. Options that take a different type value
 * are ignored.
 *
 * @param [VLC::VideoLogoOption] option logo option to set
 * @param [String] value logo option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_logo_string(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_logo_string(_SELF(self), RVAL2VLCVIDEOLOGOOPTION(option), RVAL2CSTR(value));
    return self;
}

/*
 * Get integer logo option.
 *
 * @param [VLC::VideoLogoOption] option logo option to get
 * @return [Integer] logo option value
 * @todo fixme
 */
static VALUE
rg_get_logo_int(VALUE self, VALUE option)
{
    return INT2NUM(libvlc_video_get_logo_int(_SELF(self), RVAL2VLCVIDEOLOGOOPTION(option)));
}

/*
 * Get a string logo option value
 *
 * @param [VLC::VideoLogoOption] option marq option to get
 * @return [String] logo option value
 * @todo fixme
 */
/*
static VALUE
rg_get_logo_string(VALUE self, VALUE option)
{
    CSTR2RVAL(libvlc_video_get_logo_string(_SELF(self), RVAL2VLCVIDEOLOGOOPTION(option)));
}
*/

/*
 * Set adjust option as integer. Options that take a different type value
 * are ignored.
 * Passing {VLC::VideoAdjustOption::ENABLE} as option value has the side effect of
 * starting (arg !0) or stopping (arg 0) the adjust filter.
 *
 * @param [VLC::VideoAdjustOption] option adust option to set
 * @param [Integer] value adjust option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_adjust_int(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_adjust_int(_SELF(self), RVAL2VLCVIDEOADJUSTOPTION(option), NUM2INT(value));
    return self;
}

/*
 * Set adjust option as float. Options that take a different type value
 * are ignored.
 *
 * @param [VLC::VideoAdjustOption] option adust option to set
 * @param [Float] value adjust option value
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_adjust_float(VALUE self, VALUE option, VALUE value)
{
    libvlc_video_set_adjust_float(_SELF(self), RVAL2VLCVIDEOADJUSTOPTION(option), NUM2DBL(value));
    return self;
}

/*
 * Get integer adjust option.
 *
 * @param [VLC::VideoAdjustOption] option adjust option to get
 * @return [Integer] adjust option value
 * @todo fixme
 */
static VALUE
rg_get_adjust_int(VALUE self, VALUE option)
{
    return INT2NUM(libvlc_video_get_adjust_int(_SELF(self), RVAL2VLCVIDEOADJUSTOPTION(option)));
}

/*
 * Get float adjust option.
 *
 * @param [VLC::VideoAdjustOption] option adjust option to get
 * @return [Float] adjust option value
 * @todo fixme
 */
static VALUE
rg_get_adjust_float(VALUE self, VALUE option)
{
    return DBL2NUM(libvlc_video_get_adjust_float(_SELF(self), RVAL2VLCVIDEOADJUSTOPTION(option)));
}

/*
 * Set the audio output.
 * Change will be applied after stop and play.
 *
 * @param [String] name name of audio output (@see VLC::AudioOutput)
 * @return [Boolean] true if function succeded, false on error
 * @todo fixme
 */
static VALUE
rg_set_audio_output(VALUE self, VALUE name)
{
    return ZEROBOOL2RVAL(libvlc_audio_output_set(_SELF(self), RVAL2CSTR(name)));
}

/*
 * Set audio output device. Changes are only effective after stop and play.
 *
 * @param [String] name name of audio output (@see VLC::AudioOutput)
 * @param [String] id id name of device
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_audio_output_device(VALUE self, VALUE name, VALUE id)
{
    libvlc_audio_output_device_set(_SELF(self), RVAL2CSTR(name), RVAL2CSTR(id));
    return self;
}

/*
 * Set current audio device type.
 *
 * @param [VLC::AudioOutputDeviceTypes] type the audio device type
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_audio_output_device_type(VALUE self, VALUE type)
{
    libvlc_audio_output_set_device_type(_SELF(self), RVAL2VLCAUDIOOUTPUTDEVICETYPES(type));
    return self;
}

/*
 * Get current audio device type. Device type describes something like
 * character of output sound - stereo sound, 2.1, 5.1 etc
 *
 * @return [VLC::AudioOutputDeviceTypes] the audio device type
 * @todo fixme
 */
static VALUE
rg_audio_output_device_type(VALUE self)
{
    return VLCAUDIOOUTPUTDEVICETYPES2RVAL(libvlc_audio_output_get_device_type(_SELF(self)));
}

/*
 * Toggle mute status.
 *
 * @return self
 * @todo fixme
 */
static VALUE
rg_toggle_mute(VALUE self)
{
    libvlc_audio_toggle_mute(_SELF(self));
    return self;
}

/*
 * Set mute status.
 *
 * @param [Boolean] mute If status is true then mute, otherwise unmute
 * @return self
 * @todo fixme
 */
static VALUE
rg_set_mute(VALUE self, VALUE mute)
{
    libvlc_audio_set_mute(_SELF(self), RVAL2CBOOL(mute));
    return self;
}

/*
 * Get current mute status.
 *
 * @return [Boolean] the mute status
 * @todo fixme
 */
static VALUE
rg_mute(VALUE self)
{
    return CBOOL2RVAL(libvlc_audio_get_mute(_SELF(self)));
}

/*
 * Set current software audio volume.
 *
 * @param [Integer] volume the volume in percents (0 = mute, 100 = 0dB)
 * @return [Boolean] true if the volume was set, false if it was out of range
 * @todo fixme
 */
static VALUE
rg_set_volume(VALUE self, VALUE volume)
{
    return ZEROBOOL2RVAL(libvlc_audio_set_volume(_SELF(self), NUM2INT(volume)));
}

/*
 * Get current software audio volume.
 *
 * @return [Integer] the software volume in percents (0 = mute, 100 = nominal / 0dB)
 * @todo fixme
 */
static VALUE
rg_volume(VALUE self)
{
    return INT2NUM(libvlc_audio_get_volume(_SELF(self)));
}

/*
 * Set current audio track.
 *
 * @param [Integer] track the track
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_set_audio_track(VALUE self, VALUE track)
{
    return ZEROBOOL2RVAL(libvlc_audio_set_track(_SELF(self), NUM2INT(track)));
}

/*
 * Get current audio track.
 *
 * @return [Integer] the audio track or nil if none
 * @todo fixme
 */
static VALUE
rg_audio_track(VALUE self)
{
    int track = libvlc_audio_get_track(_SELF(self));
    return track < 0 ? Qnil : INT2NUM(track);
}

/*
 * Get number of available audio tracks.
 *
 * @return [Integer] the number of available audio tracks, or nil if unavailable
 * @todo fixme
 */
static VALUE
rg_audio_track_count(VALUE self)
{
    int count = libvlc_audio_get_track_count(_SELF(self));
    return count < 0 ? Qnil : INT2NUM(count);
}

/*
 * Get the description of available audio tracks.
 *
 * @return [Array<VLC::TrackDescription>] list with description of available audio tracks
 * @todo fixme
 */
static VALUE
rg_audio_track_description(VALUE self)
{
    return TRACKDESCRIPTIONLIST2RVAL(libvlc_audio_get_track_description(_SELF(self)));
}

/*
 * Set current audio channel.
 *
 * @param [VLC::AudioOutputChannel] channel the audio channel
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_set_channel(VALUE self, VALUE channel)
{
    return ZEROBOOL2RVAL(libvlc_audio_set_channel(_SELF(self), RVAL2VLCAUDIOOUTPUTCHANNEL(channel)));
}

/*
 * Get current audio channel.
 *
 * @return [VLC::AudioOutputChannel] the audio channel
 * @todo fixme
 */
static VALUE
rg_channel(VALUE self)
{
    return VLCAUDIOOUTPUTCHANNEL2RVAL(libvlc_audio_get_channel(_SELF(self)));
}

/*
 * Set current audio delay. The audio delay will be reset to zero each time the media changes.
 *
 * @param [Integer] delay the audio delay (microseconds)
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_set_delay(VALUE self, VALUE delay)
{
    return ZEROBOOL2RVAL(libvlc_audio_set_delay(_SELF(self), NUM2LONG(delay)));
}

/*
 * Get current audio delay.
 *
 * @return [Integer] the audio delay (microseconds)
 * @todo fixme
 */
static VALUE
rg_delay(VALUE self)
{
    return LONG2NUM(libvlc_audio_get_delay(_SELF(self)));
}

/*
 * Register for an event notification.
 *
 * @param [VLC::Event::Type] event_type the desired event to which we want to listen
 * @yield call when event_type occurs
 * @yieldparam self
 * @yieldparam [VLC::Event] event
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_attach_event(VALUE self, VALUE event_type)
{
    return em_attach_event(libvlc_media_player_event_manager(_SELF(self)), self, event_type);
}

/*
 * Unregister an event notification.
 *
 * @param [VLC::Event::Type] event_type the desired event to which we want to unregister
 * @return self
 * @todo fixme
 */
static VALUE
rg_detach_event(VALUE self, VALUE event_type)
{
    return em_detach_event(libvlc_media_player_event_manager(_SELF(self)), self, event_type);
}

/*
 * Document-class: VLC::MediaPlayer
 *
 * A LibVLC media player plays one media (usually in a custom drawable).
 *
 * @todo fixme
 */
void
Init_vlc_mediaplayer(VALUE mVLC)
{
    id_core = rb_intern("core");

    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_PLAYER, "MediaPlayer", mVLC);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(core, 0);
    RG_DEF_METHOD(set_media, 1);
    RG_DEF_METHOD(media, 0);
    RG_DEF_METHOD_P(playing, 0);
    RG_DEF_METHOD(play, 0);
    RG_DEF_METHOD(pause, -1);
    RG_DEF_METHOD(stop, 0);
    RG_DEF_METHOD(set_xwindow, 1);
    RG_DEF_METHOD(xwindow, 0);
    RG_DEF_METHOD(length, 0);
    RG_DEF_METHOD(set_time, 1);
    RG_DEF_METHOD(time, 0);
    RG_DEF_METHOD(set_position, 1);
    RG_DEF_METHOD(position, 0);
    RG_DEF_METHOD(set_chapter, 1);
    RG_DEF_METHOD(chapter, 0);
    RG_DEF_METHOD(chapter_count, -1);
    RG_DEF_METHOD(chapter_description, 1);
    RG_DEF_METHOD_P(will_play, 0);
    RG_DEF_METHOD(set_title, 1);
    RG_DEF_METHOD(title, 0);
    RG_DEF_METHOD(title_count, 0);
    RG_DEF_METHOD(title_description, 0);
    RG_DEF_METHOD(previous_chapter, 0);
    RG_DEF_METHOD(next_chapter, 0);
    RG_DEF_METHOD(set_rate, 1);
    RG_DEF_METHOD(rate, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(fps, 0);
    RG_DEF_METHOD(has_vout, 0);
    RG_DEF_METHOD_P(seekable, 0);
    RG_DEF_METHOD_P(can_pause, 0);
    RG_DEF_METHOD(next_frame, 0);
    RG_DEF_METHOD(navigate, 1);
    RG_DEF_METHOD(toggle_fullscreen, 0);
    RG_DEF_METHOD(set_fullscreen, 1);
    RG_DEF_METHOD(fullscreen, 0);
    RG_DEF_METHOD(set_key_input, 1);
    RG_DEF_METHOD(set_mouse_input, 1);
    RG_DEF_METHOD(get_size, 1);
    RG_DEF_METHOD(get_cursor, 1);
    RG_DEF_METHOD(set_scale, 1);
    RG_DEF_METHOD(scale, 0);
    RG_DEF_METHOD(set_aspect_ratio, 1);
    RG_DEF_METHOD(aspect_ratio, 0);
    RG_DEF_METHOD(set_spu, 1);
    RG_DEF_METHOD(spu, 0);
    RG_DEF_METHOD(spu_count, 0);
    RG_DEF_METHOD(spu_description, 0);
    RG_DEF_METHOD(set_subtitle_file, 1);
    RG_DEF_METHOD(set_spu_delay, 1);
    RG_DEF_METHOD(spu_delay, 0);
    RG_DEF_METHOD(set_crop_geometry, 1);
    RG_DEF_METHOD(set_teletext, 1);
    RG_DEF_METHOD(teletext, 0);
    RG_DEF_METHOD(toggle_teletext, 0);
    RG_DEF_METHOD(set_video_track, 1);
    RG_DEF_METHOD(video_track, 0);
    RG_DEF_METHOD(video_track_count, 0);
    RG_DEF_METHOD(video_track_description, 0);
    RG_DEF_METHOD(take_snapshot, 1);
    RG_DEF_METHOD(set_deinterlace, 1);
    RG_DEF_METHOD(set_marquee_int, 2);
    RG_DEF_METHOD(set_marquee_string, 2);
    RG_DEF_METHOD(get_marquee_int, 1);
    RG_DEF_METHOD(get_marquee_string, 1);
    RG_DEF_METHOD(set_logo_int, 2);
    RG_DEF_METHOD(set_logo_string, 2);
    RG_DEF_METHOD(get_logo_int, 1);
    RG_DEF_METHOD(set_adjust_int, 2);
    RG_DEF_METHOD(set_adjust_float, 2);
    RG_DEF_METHOD(get_adjust_int, 1);
    RG_DEF_METHOD(get_adjust_float, 1);
    RG_DEF_METHOD(set_audio_output, 1);
    RG_DEF_METHOD(set_audio_output_device, 2);
    RG_DEF_METHOD(set_audio_output_device_type, 1);
    RG_DEF_METHOD(audio_output_device_type, 0);
    RG_DEF_METHOD(toggle_mute, 0);
    RG_DEF_METHOD(set_mute, 1);
    RG_DEF_METHOD(mute, 0);
    RG_DEF_METHOD(set_volume, 1);
    RG_DEF_METHOD(volume, 0);
    RG_DEF_METHOD(set_audio_track, 1);
    RG_DEF_METHOD(audio_track, 0);
    RG_DEF_METHOD(audio_track_count, 0);
    RG_DEF_METHOD(audio_track_description, 0);
    RG_DEF_METHOD(set_channel, 1);
    RG_DEF_METHOD(channel, 0);
    RG_DEF_METHOD(set_delay, 1);
    RG_DEF_METHOD(delay, 0);
    RG_DEF_METHOD(attach_event, 1);
    RG_DEF_METHOD(detach_event, 1);
}
