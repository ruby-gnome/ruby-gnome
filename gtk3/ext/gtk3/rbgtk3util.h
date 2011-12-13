/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifndef __RBGTK3UTIL_H__
#define __RBGTK3UTIL_H__

G_BEGIN_DECLS

typedef VALUE (*RBGTKAddButtonFunc)(VALUE self, VALUE button_text, VALUE response_id);

G_GNUC_INTERNAL VALUE rbgtk_add_buttons(VALUE self, VALUE buttons, RBGTKAddButtonFunc add_button);

G_END_DECLS

#endif /* __RBGTK3UTIL_H__ */
