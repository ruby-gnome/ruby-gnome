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

static libvlc_instance_t *
vlc_core_copy(const libvlc_instance_t *instance)
{
    libvlc_retain((libvlc_instance_t *)instance);
    return (libvlc_instance_t *)instance;
}

static void
vlc_core_free(libvlc_instance_t *instance)
{
    libvlc_release(instance);
}

GType
vlc_core_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCCore",
                                                (GBoxedCopyFunc)vlc_core_copy,
                                                (GBoxedFreeFunc)vlc_core_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cCore
#define _SELF(self) (RVAL2VLCCORE(self))

/*
 * Create and initialize a libvlc instance.
 * This functions accept a list of "command line" arguments similar to the
 * main(). These arguments affect the LibVLC instance default configuration.
 *
 * @note
 *   Arguments are meant to be passed from the command line to LibVLC, just like
 *   VLC media player does. The list of valid arguments depends on the LibVLC
 *   version, the operating system and platform, and set of available LibVLC
 *   plugins. Invalid or unsupported arguments will cause the function to fail
 *   (i.e. return NULL). Also, some arguments may alter the behaviour or
 *   otherwise interfere with other LibVLC functions.
 *
 * @note
 *   There is absolutely no warranty or promise of forward, backward and
 *   cross-platform compatibility with regards to libvlc_new() arguments.
 *   We recommend that you do not use them, other than when debugging.
 *
 * @example
 *   core = VLC::Core.new('--rate', '2')
 *
 * @param [String] args list of arguments
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_args;
    char **args;
    long len;
    libvlc_instance_t *instance;

    rb_scan_args(argc, argv, "0*", &rb_args);

    args = RVAL2CSTRARY(rb_args, &len);
    instance = libvlc_new((int)len, (const char *const *)args);
    CSTRARY_FREE(args, len);
    if (!instance)
        rb_raise(rb_eArgError, "Invalid arguments.");

    G_INITIALIZE(self, instance);
    vlc_core_free(instance);
    return Qnil;
}

/*
 * Try to start a user interface for the libvlc instance.
 *
 * @example
 *   core.add_intf("notify")
 *
 * @param [String] name interface name, or nil for default
 * @return [self]
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_add_intf(VALUE self, VALUE name)
{
    if(libvlc_add_intf(_SELF(self), RVAL2CSTR_ACCEPT_NIL(name)))
        rb_raise(rb_eArgError, "Invalid arguments.");
    return self;
}

static void
cb_exit_handler(void *self)
{
    VALUE func = rb_ivar_get((VALUE)self, rb_intern("exit_handler"));
    if (NIL_P(func))
        rb_warn("exit handler not found");
    else
        rb_funcall(func, rb_intern("call"), 1, self);
}

static VALUE
rg_set_exit_handler(VALUE self)
{
    rb_ivar_set(self, rb_intern("exit_handler"), rb_block_proc());
    libvlc_set_exit_handler(_SELF(self), cb_exit_handler, (void *)self);
    return self;
}

static VALUE
rg_wait(VALUE self)
{
    libvlc_wait(_SELF(self));
    return self;
}

/*
 * Sets the application name. LibVLC passes this as the user agent string
 * when a protocol requires it.
 *
 * @example
 *   core.set_user_agent("FooBar player 1.2.3", "FooBar/1.2.3 Ruby/1.9.3p0")
 *
 * @param [String] name human-readable application name
 * @param [String] http HTTP User Agent
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_set_user_agent(VALUE self, VALUE name, VALUE http)
{
    libvlc_set_user_agent(_SELF(self), RVAL2CSTR(name), RVAL2CSTR(http));
    return self;
}

/*
 * Returns a list of video filters that are available.
 *
 * @return [Array<VLC::ModuleDescription>] a list of module descriptions
 * @todo fixme
 */
static VALUE
rg_video_filter_list(VALUE self)
{
    return MODULEDESCRIPTIONLIST2RVAL(libvlc_video_filter_list_get(_SELF(self)));
}

/*
 * Returns a list of audio filters that are available.
 *
 * @return [Array<VLC::ModuleDescription>] a list of module descriptions
 * @todo fixme
 */
static VALUE
rg_audio_filter_list(VALUE self)
{
    return MODULEDESCRIPTIONLIST2RVAL(libvlc_audio_filter_list_get(_SELF(self)));
}

/*
 * Get the list of available audio outputs
 *
 * @return [Array<VLC::AudioOutput>] list of available audio outputs.
 * @todo fixme
 */
static VALUE
rg_audio_output_list(VALUE self)
{
    return AUDIOOUTPUTLIST2RVAL(libvlc_audio_output_list_get(_SELF(self)));
}

/*
 * Get count of devices for audio output, these devices are hardware oriented
 * like analor or digital output of sound card
 *
 * @param [String] name name of audio output (@see VLC::AudioOutput)
 * @return [Integer] number of devices
 * @todo fixme
 */
static VALUE
rg_audio_output_device_count(VALUE self, VALUE name)
{
    return INT2NUM(libvlc_audio_output_device_count(_SELF(self), RVAL2CSTR(name)));
}

/*
 * Get long name of device, if not available short name given
 *
 * @param [String] name name of audio output (@see VLC::AudioOutput)
 * @param [Integer] index device index
 * @return [String] long name of device
 * @todo fixme
 */
static VALUE
rg_audio_output_device_longname(VALUE self, VALUE name, VALUE index)
{
    return CSTR2RVAL(libvlc_audio_output_device_longname(_SELF(self), RVAL2CSTR(name), NUM2INT(index)));
}

/*
 * Get id name of device
 *
 * @param [String] name name of audio output (@see VLC::AudioOutput)
 * @param [Integer] index device index
 * @return [String] id name of device, use for setting device
 * @todo fixme
 */
static VALUE
rg_audio_output_device_id(VALUE self, VALUE name, VALUE index)
{
    return CSTR2RVAL_FREE(libvlc_audio_output_device_id(_SELF(self), RVAL2CSTR(name), NUM2INT(index)));
}

/*
 * Discover media service by name.
 *
 * @param [String] service_name service name
 * @return [VLC::MediaDiscoverer] media discover object or nil in case of error
 * @todo fixme
 */
static VALUE
rg_create_discoverer(VALUE self, VALUE service_name)
{
    return VLCMEDIADISCOVERER2RVAL(libvlc_media_discoverer_new_from_name(_SELF(self), RVAL2CSTR(service_name)));
}

/*
 * Create an new Media Library object
 *
 * @return [VLC::MediaLibrary] a new object or nil on error
 * @todo fixme
 */
static VALUE
rg_create_library(VALUE self)
{
    return VLCMEDIALIBRARY2RVAL(libvlc_media_library_new(_SELF(self)));
}

void
Init_vlc_core(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_CORE, "Core", mVLC);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(add_intf, 1);
    RG_DEF_METHOD(set_exit_handler, 0);
    RG_DEF_METHOD(wait, 0);
    RG_DEF_METHOD(set_user_agent, 2);
    RG_DEF_METHOD(video_filter_list, 0);
    RG_DEF_METHOD(audio_filter_list, 0);
    RG_DEF_METHOD(audio_output_list, 0);
    RG_DEF_METHOD(audio_output_device_count, 1);
    RG_DEF_METHOD(audio_output_device_longname, 2);
    RG_DEF_METHOD(audio_output_device_id, 2);
    RG_DEF_METHOD(create_discoverer, 1);
    RG_DEF_METHOD(create_library, 0);
}
