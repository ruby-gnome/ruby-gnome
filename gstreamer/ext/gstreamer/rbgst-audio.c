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

#define AUDIO_MAKE_SAMPLE_KW_SIZE 1

static ID id_rate;
static ID audio_make_sample_kw_table[AUDIO_MAKE_SAMPLE_KW_SIZE];

static GstAudioFormat
rg_gst_audio_audio_format_from_memory_view(rb_memory_view_t *view)
{
    const rb_memory_view_item_component_t *component;

    if (!view->format) {
        return GST_AUDIO_FORMAT_UNKNOWN;
    }

    rb_memory_view_prepare_item_desc(view);

    // Currently, supports single character format (item_desc.length == 1) only
    if (view->item_desc.length != 1) {
        return GST_AUDIO_FORMAT_UNKNOWN;
    }

    component = &view->item_desc.components[0];
    if (component->offset != 0 || component->repeat != 1 || component->size != (size_t)view->item_size) {
        return GST_AUDIO_FORMAT_UNKNOWN;
    }

    const char format = component->format;
    const bool little_endian_p = component->little_endian_p;
    const size_t size = component->size;

    switch (format) {
        case 'c':
            return GST_AUDIO_FORMAT_S8;
        case 'C':
            return GST_AUDIO_FORMAT_U8;
        case 's':
        case 'i':
        case 'l':
        case 'q':
            switch (size) {
                case 1:
                    return GST_AUDIO_FORMAT_S8;
                case 2:
                    return little_endian_p ? GST_AUDIO_FORMAT_S16LE : GST_AUDIO_FORMAT_S16BE;
                case 4:
                    return little_endian_p ? GST_AUDIO_FORMAT_S32LE : GST_AUDIO_FORMAT_S32BE;
                default:
                    return GST_AUDIO_FORMAT_UNKNOWN;
            }
        case 'S':
        case 'I':
        case 'L':
        case 'Q':
        case 'n':
        case 'N':
        case 'v':
        case 'V':
            switch (size) {
                case 1:
                    return GST_AUDIO_FORMAT_U8;
                case 2:
                    return little_endian_p ? GST_AUDIO_FORMAT_U16LE : GST_AUDIO_FORMAT_U16BE;
                case 4:
                    return little_endian_p ? GST_AUDIO_FORMAT_U32LE : GST_AUDIO_FORMAT_U32BE;
                default:
                    return GST_AUDIO_FORMAT_UNKNOWN;
            }
        case 'f':
        case 'd':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            switch (size) {
                case 4:
                    return little_endian_p ? GST_AUDIO_FORMAT_F32LE : GST_AUDIO_FORMAT_F32BE;
                case 8:
                    return little_endian_p ? GST_AUDIO_FORMAT_F64LE : GST_AUDIO_FORMAT_F64BE;
                default:
                    return GST_AUDIO_FORMAT_UNKNOWN;
            }
        default:
            return GST_AUDIO_FORMAT_UNKNOWN;
    }
}

static bool
rg_gst_audio_audio_info_from_memory_view(GstAudioInfo *info, rb_memory_view_t *view, gint rate, const char **err)
{
/*
 * When Ruby < 4.1.0, rb_memory_view_is_row_major_contiguous() and
 * rb_memory_view_is_column_major_contiguous() have a bug and
 * rg_gst_audio_audio_info_from_memory_view() crashes.
 */
#if RUBY_API_VERSION_CODE < 40100
    *err = "not implemented when Ruby < 4.1.0";
    return false;
#endif

    GstAudioFormat format;
    GstAudioLayout layout;
    gint channels;

    if (view->ndim > 2) {
        *err = "ndim too large";
        return false;
    }

    if (!view->shape && (view->strides || view->ndim > 1)) {
        *err = "shape not defined";
        return false;
    }

    if (!view->shape || view->ndim == 1) {
        channels = 1;
    } else {
        if (view->shape[1] == 0) {
            *err = "channels must be >= 1";
            return false;
        }
        if (view->shape[1] > G_MAXINT) {
            *err = "channels too large";
            return false;
        }
        channels = (gint)view->shape[1];
    }

    if (rb_memory_view_is_row_major_contiguous(view)) {
        layout = GST_AUDIO_LAYOUT_INTERLEAVED;
    } else if (rb_memory_view_is_column_major_contiguous(view)) {
        *err = "currently, column-major audio not supported";
        return false;
    } else {
        *err = "currently, non-contiguous MemoryView not supported";
        return false;
    }

    format = rg_gst_audio_audio_format_from_memory_view(view);
    if (format == GST_AUDIO_FORMAT_UNKNOWN) {
        *err = "unsupported format";
        return false;
    }

    gst_audio_info_set_format(info, format, rate, channels, NULL);
    info->layout = layout;

    return true;
}

static VALUE
rg_gst_audio_release_buffer_memory_view(VALUE data)
{
    rb_memory_view_t *view = (rb_memory_view_t *)data;

    rb_memory_view_release(view);
    g_free(view);
}

static void
rg_gst_audio_queue_buffer_memory_view_release(gpointer data)
{
    rbgutil_invoke_callback_async(rg_gst_audio_release_buffer_memory_view, (VALUE)data);
}

static GstBuffer *
rg_gst_audio_buffer_from_memory_view(rb_memory_view_t *view)
{
    GstBuffer *buffer;

    buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, // TODO: handle with other flags
                                         view->data,
                                         view->byte_size,
                                         0,
                                         view->byte_size,
                                         view,
                                         rg_gst_audio_queue_buffer_memory_view_release);

    return buffer;
}

static bool
rg_gst_audio_sample_from_memory_view(GstSample **sample, VALUE src, VALUE kw_values[AUDIO_MAKE_SAMPLE_KW_SIZE], const char **err)
{
    rb_memory_view_t *view;
    GstAudioInfo audio_info;
    GstBuffer *buffer;
    GstCaps *caps;
    const VALUE rate_v = kw_values[0];
    long rate_candidate;
    gint rate;

    if (NIL_P(rate_v)) {
        *err = "missing keyword: rate";
        return false;
    }
    if (!RB_INTEGER_TYPE_P(rate_v)) {
        *err = "rate must be Integer";
        return false;
    }
    if (!FIXNUM_P(rate_v)) {
        *err = "rate must be fixnum";
        return false;
    }
    rate_candidate = FIX2LONG(rate_v);
    if (rate_candidate > G_MAXINT) {
        *err = "rate too large";
        return false;
    }
    if (rate_candidate <= 0) {
        *err = "rate must be positive";
        return false;
    }
    rate = (gint)rate_candidate;

    if (!rb_memory_view_available_p(src)) {
        *err = "MemoryView not available";
        return false;
    }

    view = g_new(rb_memory_view_t, 1);

    // Currently, supports only contiguous MemoryView for simplicity
    if (!rb_memory_view_get(src, view, RUBY_MEMORY_VIEW_FORMAT | RUBY_MEMORY_VIEW_ANY_CONTIGUOUS) &&
        !rb_memory_view_get(src, view, RUBY_MEMORY_VIEW_SIMPLE)) { // for Red Arrow
        g_free(view);
        *err = "can't get MemoryView";
        return false;
    }
    if (!rg_gst_audio_audio_info_from_memory_view(&audio_info, view, rate, err)) {
        rb_memory_view_release(view);
        g_free(view);
        return false;
    }
    buffer = rg_gst_audio_buffer_from_memory_view(view);
    caps = gst_audio_info_to_caps(&audio_info);
    if (!caps) {
        gst_buffer_unref(buffer);
        *err ="can't get caps from audio info";
        return false;
    }
    *sample = gst_sample_new(buffer, caps, NULL, NULL);
    gst_buffer_unref(buffer);
    gst_caps_unref(caps);

    return true;
}

static VALUE
rg_gst_audio_s_audio_make_sample(int argc, VALUE *argv, VALUE mod)
{
    VALUE src;
    VALUE kw_args;
    VALUE kw_values[AUDIO_MAKE_SAMPLE_KW_SIZE];
    GstSample *sample = NULL;
    const char *err = NULL;
    VALUE rb_sample;

    rb_scan_args_kw(RB_SCAN_ARGS_KEYWORDS, argc, argv, "1:", &src, &kw_args);
    rb_get_kwargs(kw_args, audio_make_sample_kw_table, AUDIO_MAKE_SAMPLE_KW_SIZE, 0, kw_values);

    if (!rg_gst_audio_sample_from_memory_view(&sample, src, kw_values, &err)) {
        rb_raise(rb_eArgError, "%s", err);
    }

    rb_sample = BOXED2RVAL(sample, GST_TYPE_SAMPLE);
    gst_sample_unref(sample);

    return rb_sample;
}

void
rb_gst_init_audio(void)
{
#if GST_CHECK_VERSION(1, 20, 0)
    VALUE mGstAudio;

    id_rate = rb_intern("rate");
    audio_make_sample_kw_table[0] = id_rate;

    mGstAudio = rb_const_get(rb_cObject, rb_intern("GstAudio"));
    rb_define_singleton_method(mGstAudio, "audio_make_sample", rg_gst_audio_s_audio_make_sample, -1);
#endif
}
