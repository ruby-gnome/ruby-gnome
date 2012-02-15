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

static libvlc_media_discoverer_t *
vlc_media_discoverer_copy(const libvlc_media_discoverer_t *instance)
{
/*    libvlc_media_discoverer_retain((libvlc_media_discoverer_t *)instance);*/
    return (libvlc_media_discoverer_t *)instance;
}

static void
vlc_media_discoverer_free(libvlc_media_discoverer_t *instance)
{
    libvlc_media_discoverer_release(instance);
}

GType
vlc_media_discoverer_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaDiscoverer",
                                                (GBoxedCopyFunc)vlc_media_discoverer_copy,
                                                (GBoxedFreeFunc)vlc_media_discoverer_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMediaDiscoverer
#define _SELF(self) (RVAL2VLCMEDIADISCOVERER(self))

/*
 * Get media service discover object its localized name.
 *
 * @return [String] localized name
 * @todo fixme
 */
static VALUE
rg_localized_name(VALUE self)
{
    return CSTR2RVAL(libvlc_media_discoverer_localized_name(_SELF(self)));
}

/*
 * Get media service discover media list.
 *
 * @return [VLC::MediaList] list of media items
 * @todo fixme
 */
static VALUE
rg_media_list(VALUE self)
{
    return VLCMEDIALIST2RVAL(libvlc_media_discoverer_media_list(_SELF(self)));
}

/*
 * Query if media service discover object is running.
 *
 * @return [Boolean] true if running, false if not
 * @todo fixme
 */
static VALUE
rg_running_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_discoverer_is_running(_SELF(self)));
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
    return em_attach_event(libvlc_media_discoverer_event_manager(_SELF(self)), self, event_type);
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
    return em_detach_event(libvlc_media_discoverer_event_manager(_SELF(self)), self, event_type);
}

/*
 * Document-class: VLC::MediaDiscoverer
 *
 * LibVLC media discovery finds available media via various means.
 * This corresponds to the service discovery functionality in VLC media player.
 * Different plugins find potential medias locally (e.g. user media directory),
 * from peripherals (e.g. video capture device), on the local network
 * (e.g. SAP) or on the Internet (e.g. Internet radios).
 *
 * @todo fixme
 */
void
Init_vlc_mediadiscoverer(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_DISCOVERER, "MediaDiscoverer", mVLC);

    RG_DEF_METHOD(localized_name, 0);
    RG_DEF_METHOD(media_list, 0);
    RG_DEF_METHOD_P(running, 0);
    RG_DEF_METHOD(attach_event, 1);
    RG_DEF_METHOD(detach_event, 1);
}
