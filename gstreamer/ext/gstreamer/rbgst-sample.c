/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2026  Ruby-GNOME Project Team
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

#include <ruby.h>
#include <ruby/version.h>
#include <ruby/memory_view.h>
#include <gst/gstversion.h>
#include <gst/audio/audio.h>
#include "rbgst.h"

typedef struct memview_private_data {
    GstBuffer *buffer;
    GstMapInfo *map_info;
} memview_private_data;

static gboolean
rg_gst_combine_buffer_list(GstBuffer **buffer, guint idx, gpointer user_data)
{
    GstBuffer **acc;

    acc = (GstBuffer **)user_data;
    *acc = gst_buffer_append(*acc, gst_buffer_ref(*buffer));

    return TRUE;
}

static gboolean
rg_gst_is_supported_caps(const gchar *name)
{
    return g_str_has_prefix(name, "audio/x-raw");
}

static bool
rg_gst_memory_view_init_from_audio(VALUE obj, rb_memory_view_t *view, int flags, GstSample *sample, GstCaps *caps)
{
    GstBuffer *buffer;
    GstBufferList *buffer_list;
    GstAudioInfo *audio_info;
    GstMapInfo *map_info;
    bool is_writable_requested;
    gboolean is_writable;
    bool is_interleaved;
    ssize_t *shape;
    ssize_t *strides;
    gint width;
    const char *format = NULL;
    gsize n_samples;
    memview_private_data *private_data;

    audio_info = gst_audio_info_new_from_caps(caps);
    if (!audio_info) {
        rb_warn("Gst::Sample: failed to get audio info");

        return false;
    }
    if (!audio_info->finfo) {
        gst_audio_info_free(audio_info);
        rb_warn("Gst::Sample: failed to retrieve audio format info");

        return false;
    }

    is_interleaved = audio_info->layout == GST_AUDIO_LAYOUT_INTERLEAVED;
    if (!is_interleaved) {
        // TODO: non-interleaved layout support
        gst_audio_info_free(audio_info);
        rb_warn("Gst::Sample: currently only interleaved layout is supported");

        return false;
    }

    is_writable_requested = flags & RUBY_MEMORY_VIEW_WRITABLE;

    buffer_list = gst_sample_get_buffer_list(sample);
    if (buffer_list) {
      if (is_writable_requested) {
          gst_audio_info_free(audio_info);
          rb_warn("Gst::Sample: currently writable buffer list is not supported");

          return false;
      }
      buffer = gst_buffer_new();
      gst_buffer_list_foreach(buffer_list, rg_gst_combine_buffer_list, &buffer);
    } else {
        buffer = gst_sample_get_buffer(sample);
        if (!buffer) {
            gst_audio_info_free(audio_info);
            rb_warn("Gst::Sample: failed to get buffer");

            return false;
        }
        buffer = gst_buffer_ref(buffer);
    }

    is_writable = gst_sample_is_writable(sample);
    if (is_writable_requested && !is_writable) {
        rb_warn("Gst::Sample: sample is not writable but writable memory view is requested");
        gst_audio_info_free(audio_info);
        gst_buffer_unref(buffer);

        return false;
    }
    // Doesn't use ALLOC/ALLOC_N because they may raise an exception and then
    // the map_info and audio_info won't be freed and the buffer won't be unrefed.
    map_info = g_new(GstMapInfo, 1);
    if (!gst_buffer_map(buffer, map_info, is_writable_requested ? GST_MAP_WRITE : GST_MAP_READ)) {
        rb_warn("Gst::Sample: failed to map buffer");
        g_free(map_info);
        gst_audio_info_free(audio_info);
        gst_buffer_unref(buffer);

        return false;
    }

    width = audio_info->finfo->width / 8;

    switch (audio_info->finfo->format) {
    case GST_AUDIO_FORMAT_UNKNOWN:
        rb_warn("Gst::Sample: format is unknown");
        break;
    case GST_AUDIO_FORMAT_ENCODED:
        rb_warn("Gst::Sample: encoded format is not supported");
        break;
    case GST_AUDIO_FORMAT_S8:
        format = "c";
        break;
    case GST_AUDIO_FORMAT_U8:
        format = "C";
        break;
    case GST_AUDIO_FORMAT_S16LE:
        format = "s<";
        break;
    case GST_AUDIO_FORMAT_S16BE:
        format = "s>";
        break;
    case GST_AUDIO_FORMAT_U16LE:
        format = "S<";
        break;
    case GST_AUDIO_FORMAT_U16BE:
        format = "S>";
        break;
    case GST_AUDIO_FORMAT_S24_32LE:
        rb_warn("Gst::Sample: S24_32LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S24_32BE:
        rb_warn("Gst::Sample: S24_32BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U24_32LE:
        rb_warn("Gst::Sample: U24_32LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U24_32BE:
        rb_warn("Gst::Sample: U24_32BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S32LE:
        format = "l<";
        break;
    case GST_AUDIO_FORMAT_S32BE:
        format = "l>";
        break;
    case GST_AUDIO_FORMAT_U32LE:
        format = "L<";
        break;
    case GST_AUDIO_FORMAT_U32BE:
        format = "L>";
        break;
    case GST_AUDIO_FORMAT_S24LE:
        rb_warn("Gst::Sample: S24LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S24BE:
        rb_warn("Gst::Sample: S24BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U24LE:
        rb_warn("Gst::Sample: U24LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U24BE:
        rb_warn("Gst::Sample: U24BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S20LE:
        rb_warn("Gst::Sample: S20LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S20BE:
        rb_warn("Gst::Sample: S20BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U20LE:
        rb_warn("Gst::Sample: U20LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U20BE:
        rb_warn("Gst::Sample: U20BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S18LE:
        rb_warn("Gst::Sample: S18LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S18BE:
        rb_warn("Gst::Sample: S18BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U18LE:
        rb_warn("Gst::Sample: U18LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U18BE:
        rb_warn("Gst::Sample: U18BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_F32LE:
        if (rb_memory_view_item_size_from_format("e", NULL) == 4) {
            format = "e";
        } else {
            rb_warn("Gst::Sample: only environment float size is 4 bytes is supported");
        }
        break;
    case GST_AUDIO_FORMAT_F32BE:
        if (rb_memory_view_item_size_from_format("g", NULL) == 4) {
            format = "g";
        } else {
            rb_warn("Gst::Sample: only environment float size is 4 bytes is supported");
        }
        break;
    case GST_AUDIO_FORMAT_F64LE:
        if (rb_memory_view_item_size_from_format("E", NULL) == 8) {
            format = "E";
        } else if (rb_memory_view_item_size_from_format("e", NULL) == 8) {
            format = "e";
        } else {
            rb_warn("Gst::Sample: only environment double or float size is 8 bytes is supported");
        }
        break;
    case GST_AUDIO_FORMAT_F64BE:
        if (rb_memory_view_item_size_from_format("G", NULL) == 8) {
            format = "G";
        } else if (rb_memory_view_item_size_from_format("g", NULL) == 8) {
            format = "g";
        } else {
            rb_warn("Gst::Sample: only environment double or float size is 8 bytes is supported");
        }
        break;
#if GST_CHECK_VERSION(1, 28, 0)
    case GST_AUDIO_FORMAT_S20_32LE:
        rb_warn("Gst::Sample: S20_32LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_S20_32BE:
        rb_warn("Gst::Sample: S20_32BE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U20_32LE:
        rb_warn("Gst::Sample: U20_32LE format is not supported");
        break;
    case GST_AUDIO_FORMAT_U20_32BE:
        rb_warn("Gst::Sample: U20_32BE format is not supported");
        break;
#endif
    }

    if (!format) {
        gst_buffer_unmap(buffer, map_info);
        g_free(map_info);
        gst_buffer_unref(buffer);
        gst_audio_info_free(audio_info);

        return false;
    }

    if (map_info->size % (width * audio_info->channels) != 0) {
        rb_warn("Gst::Sample: buffer size is not aligned with sample width and channels");
        gst_buffer_unmap(buffer, map_info);
        g_free(map_info);
        gst_buffer_unref(buffer);
        gst_audio_info_free(audio_info);

        return false;
    }
    n_samples = map_info->size / width / audio_info->channels;
    // Doesn't use ALLOC/ALLOC_N because they may raise an exception and then
    // the audio_info won't be freed.
    shape = g_new(ssize_t, 2);
    strides = g_new(ssize_t, 2);
    // Currently, interleaved and row-major audio is supported
    shape[0] = n_samples;
    shape[1] = audio_info->channels;
    strides[0] = width * audio_info->channels;
    strides[1] = width;

    gst_audio_info_free(audio_info);

    // Uses g_new instead of ALLOC/ALLOC_N for consistency.
    private_data = g_new(memview_private_data, 1);
    private_data->buffer = buffer;
    private_data->map_info = map_info;

    view->format = format;
    view->data = map_info->data;
    view->obj = obj;
    view->item_size = width;
    view->byte_size = map_info->size;
    view->readonly = !is_writable_requested;
    view->ndim = 2;
    view->shape = shape;
    view->strides = strides;
    view->sub_offsets = NULL;
#if RUBY_API_VERSION_MAJOR == 3 && RUBY_API_VERSION_MINOR == 0
    *((void **)&view->private) = private_data;
#else
    view->private_data = (void *)private_data;
#endif

    return true;
}

static bool
rg_gst_sample_get(VALUE obj, rb_memory_view_t *view, int flags)
{
    GstSample *sample;
    GstCaps *caps;
    bool is_column_major_requested;

    is_column_major_requested = flags & RUBY_MEMORY_VIEW_COLUMN_MAJOR;
    if (is_column_major_requested) {
        // TODO: column-major support
        rb_warn("Gst::Sample: currently column-major is not supported");

        return false;
    }

    sample = GST_SAMPLE(RVAL2GOBJ(obj));

    caps = gst_sample_get_caps(sample);

    if (rg_gst_memory_view_init_from_audio(obj, view, flags, sample, caps)) {
        gst_sample_ref(sample);
        return true;
    }

    return false;
}

static bool
rg_gst_sample_release(VALUE obj, rb_memory_view_t *view)
{
    GstSample *sample;
    memview_private_data *private_data;

    g_free((gpointer)view->shape);
    g_free((gpointer)view->strides);
#if RUBY_API_VERSION_MAJOR == 3 && RUBY_API_VERSION_MINOR == 0
    private_data = (memview_private_data *)view->private;
#else
    private_data = (memview_private_data *)view->private_data;
#endif
    gst_buffer_unmap(private_data->buffer, private_data->map_info);
    gst_buffer_unref(private_data->buffer);
    g_free(private_data->map_info);
    g_free(private_data);
    sample = GST_SAMPLE(RVAL2GOBJ(obj));
    gst_sample_unref(sample);

    return true;
}

static bool
rg_gst_sample_available_p(VALUE obj)
{
    GstSample *sample;
    GstCaps *caps;
    gchar *name;
    gboolean is_supported_caps;

    sample = GST_SAMPLE(RVAL2GOBJ(obj));

    caps = gst_sample_get_caps(sample);
    if (!caps) {
        return false;
    }
    if (gst_caps_get_size(caps) != 1) {
        return false;
    }

    name = gst_caps_to_string(caps);
    is_supported_caps = rg_gst_is_supported_caps(name);
    if (!is_supported_caps) {
        rb_warn("Gst::Sample: currently audio/x-raw caps only supported but is %s", name);
    }

    g_free(name);

    return is_supported_caps;
}

static rb_memory_view_entry_t rg_gst_sample_entry = {
    rg_gst_sample_get,
    rg_gst_sample_release,
    rg_gst_sample_available_p,
};

void
rb_gst_init_sample(void)
{
#if GST_CHECK_VERSION(1, 20, 0)
    VALUE mGst;
    VALUE cSample;

    mGst = rb_const_get(rb_cObject, rb_intern("Gst"));
    cSample = rb_const_get(mGst, rb_intern("Sample"));
    rb_memory_view_register(cSample, &rg_gst_sample_entry);
#endif
}
