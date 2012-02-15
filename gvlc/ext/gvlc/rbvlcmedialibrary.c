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

static libvlc_media_library_t *
vlc_media_library_copy(const libvlc_media_library_t *instance)
{
    libvlc_media_library_retain((libvlc_media_library_t *)instance);
    return (libvlc_media_library_t *)instance;
}

static void
vlc_media_library_free(libvlc_media_library_t *instance)
{
    libvlc_media_library_release(instance);
}

GType
vlc_media_library_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaLibrary",
                                                (GBoxedCopyFunc)vlc_media_library_copy,
                                                (GBoxedFreeFunc)vlc_media_library_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMediaLibrary
#define _SELF(self) (RVAL2VLCMEDIALIBRARY(self))

/*
 * Load media library.
 *
 * @return [Boolean] true on success, false on error
 * @todo fixme
 */
static VALUE
rg_load(VALUE self)
{
    return ZEROBOOL2RVAL(libvlc_media_library_load(_SELF(self)));
}

/*
 * Get media library subitems.
 *
 * @return [VLC::MediaList] media list subitems
 * @todo fixme
 */
static VALUE
rg_media_list(VALUE self)
{
    return VLCMEDIALIST2RVAL(libvlc_media_library_media_list(_SELF(self)));
}

/*
 * Document-class: VLC::MediaLibrary
 *
 * @todo fixme
 */
void
Init_vlc_medialibrary(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_LIBRARY, "MediaLibrary", mVLC);

    RG_DEF_METHOD(load, 0);
    RG_DEF_METHOD(media_list, 0);
}
