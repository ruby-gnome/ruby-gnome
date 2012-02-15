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

static libvlc_audio_output_t *
vlc_audio_output_copy(const libvlc_audio_output_t *instance)
{
    libvlc_audio_output_t *result = g_new(libvlc_audio_output_t, 1);
    *result = *instance;
    result->psz_name        = g_strdup(instance->psz_name);
    result->psz_description = g_strdup(instance->psz_description);
    return result;
}

static void
vlc_audio_output_free(libvlc_audio_output_t *instance)
{
    g_free(instance->psz_name);
    g_free(instance->psz_description);
    g_free(instance);
}

GType
vlc_audio_output_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCAudioOutput",
                                                (GBoxedCopyFunc)vlc_audio_output_copy,
                                                (GBoxedFreeFunc)vlc_audio_output_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cAudioOutput
#define _SELF(self) (RVAL2VLCAUDIOOUTPUT(self))

/*
 * Get name of audio output
 *
 * @return [String] name
 * @todo fixme
 */
static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_name);
}

/*
 * Get description of audio output
 *
 * @return [String] description
 * @todo fixme
 */
static VALUE
rg_description(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->psz_description);
}

/*
 * Document-class: VLC::AudioOutput
 *
 * Description for audio output.
 *
 * @todo fixme
 */
void
Init_vlc_audiooutput(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_AUDIO_OUTPUT, "AudioOutput", mVLC);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(description, 0);
}
