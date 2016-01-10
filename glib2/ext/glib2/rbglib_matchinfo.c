/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rbgprivate.h"

#ifdef G_OS_WIN32
#include <windows.h>
#endif

#define RG_TARGET_NAMESPACE cMatchInfo
#define _SELF(s) ((GMatchInfo*)RVAL2BOXED(s, G_TYPE_MATCH_INFO))

static void
match_info_free(gpointer object)
{
    GMatchInfo *match_info = object;

    g_match_info_free(match_info);
}

/* TODO
 *
 * g_match_info_ref
 * g_match_info_unref
 * g_match_info_free
 * g_match_info_matches
 * g_match_info_next
 * g_match_info_get_match_count
 * g_match_info_is_partial_match
 * g_match_info_expand_references
 * g_match_info_fetch
 * g_match_info_fetch_pos
 * g_match_info_fetch_named
 * g_match_info_fetch_named_pos
 * g_match_info_fetch_all
 * */
void
Init_glib_matchinfo(void)
{
  
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(G_TYPE_MATCH_INFO, "MatchInfo", mGLib,
                                                         NULL, match_info_free);
}

