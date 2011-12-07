/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2011  Ruby-GNOME2 Project Team
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

#ifndef __RBPOPPLERCONVERSIONS_H__
#define __RBPOPPLERCONVERSIONS_H__

#define RVAL2POPPLERDEST(o)       ((Popper Dest *)RVAL2BOXED(o, POPPLER_TYPE_DEST))
#define POPPLERDEST2RVAL(o)       (BOXED2RVAL(o, POPPLER_TYPE_DEST))


#define POPPLERANNOT2RVAL(o)      (GOBJ2RVAL(o))
#define RVAL2POPPLERANNOT(o)      (POPPLER_ANNOT(RVAL2GOBJ(o)))

#define POPPLERACTION2RVAL(o)     (rb_poppler_ruby_object_from_action(o))
#define RVAL2POPPLERACTION(o)     (rb_poppler_action_from_ruby_object(o))
#define POPPLERFORMFIELD2RVAL(o)  (rb_poppler_ruby_object_from_form_field(o))
#define RVAL2POPPLERFORMFIELD(o)  (POPPLER_FORM_FIELD(RVAL2GOBJ(o)))

#define RVAL2POPPLERCOLOR(o)      (rb_poppler_ruby_object_to_color(o))
#define POPPLERCOLOR2RVAL(o)      (BOXED2RVAL(o, POPPLER_TYPE_COLOR))
#define POPPLERCOLOR2RVAL_FREE(o) (rb_poppler_ruby_object_from_color_with_free(o))

#endif /* __RBPOPPLERCONVERSIONS_H__ */
