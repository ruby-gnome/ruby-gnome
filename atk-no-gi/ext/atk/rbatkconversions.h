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

#ifndef __RBATKCONVERSIONS_H__
#define __RBATKCONVERSIONS_H__

#define RVAL2ATKACTION(o)                  (ATK_ACTION(RVAL2GOBJ(o)))
#define RVAL2ATKCOMPONENT(o)               (ATK_COMPONENT(RVAL2GOBJ(o)))
#define RVAL2ATKDOCUMENT(o)                (ATK_DOCUMENT(RVAL2GOBJ(o)))
#define RVAL2ATKEDITABLETEXT(o)            (ATK_EDITABLE_TEXT(RVAL2GOBJ(o)))
#define RVAL2ATKGOBJECTACCESSIBLE(o)       (ATK_GOBJECT_ACCESSIBLE(RVAL2GOBJ(o)))
#define RVAL2ATKHYPERLINK(o)               (ATK_HYPERLINK(RVAL2GOBJ(o)))
#define RVAL2ATKHYPERTEXT(o)               (ATK_HYPERTEXT(RVAL2GOBJ(o)))
#define RVAL2ATKIMAGE(o)                   (ATK_IMAGE(RVAL2GOBJ(o)))
#define RVAL2ATKIMPLEMENTOR(o)             (ATK_IMPLEMENTOR(RVAL2GOBJ(o)))
#define RVAL2ATKNOOPOBJECT(o)              (ATK_NOOPOBJECT(RVAL2GOBJ(o)))
#define RVAL2ATKNOOPOBJECTFACTORY(o)       (ATK_NO_OP_OBJECT_FACTORY(RVAL2GOBJ(o)))
#define RVAL2ATKOBJECT(o)                  (ATK_OBJECT(RVAL2GOBJ(o)))
#define RVAL2ATKOBJECTFACTORY(o)           (ATK_OBJECT_FACTORY(RVAL2GOBJ(o)))
#define RVAL2ATKREGISTRY(o)                (ATK_REGISTRY(RVAL2GOBJ(o)))
#define RVAL2ATKRELATION(o)                (ATK_RELATION(RVAL2GOBJ(o)))
#define RVAL2ATKRELATIONSET(o)             (ATK_RELATION_SET(RVAL2GOBJ(o)))
#define RVAL2ATKSELECTION(o)               (ATK_SELECTION(RVAL2GOBJ(o)))
#define RVAL2ATKSTATE(o)                   (ATK_STATE(RVAL2GOBJ(o)))
#define RVAL2ATKSTATESET(o)                (ATK_STATE_SET(RVAL2GOBJ(o)))
#define RVAL2ATKSTREAMABLECONTENT(o)       (ATK_STREAMABLE_CONTENT(RVAL2GOBJ(o)))
#define RVAL2ATKTABLE(o)                   (ATK_TABLE(RVAL2GOBJ(o)))
#define RVAL2ATKTEXT(o)                    (ATK_TEXT(RVAL2GOBJ(o)))
#define RVAL2ATKVALUE(o)                   (ATK_VALUE(RVAL2GOBJ(o)))

#define RVAL2ATKTEXTRANGE(o)               ((AtkTextRange*)RVAL2BOXED(o, ATK_TYPE_TEXT_RANGE))
#define ATKTEXTRANGE2RVAL(o)               (BOXED2RVAL(o, ATK_TYPE_TEXT_RANGE))
#define RVAL2ATKTEXTRECTANGLE(o)           ((AtkTextRectangle*)RVAL2BOXED(o, ATK_TYPE_TEXT_RECTANGLE))
#define ATKTEXTRECTANGLE2RVAL(o)           (BOXED2RVAL(o, ATK_TYPE_TEXT_RECTANGLE))

#define RVAL2ATKCOORDTYPE(o)               (RVAL2GENUM(o, ATK_TYPE_COORD_TYPE))
#define ATKCOORDTYPE2RVAL(o)               (GENUM2RVAL(o, ATK_TYPE_COORD_TYPE))
#define RVAL2ATKLAYER(o)                   (RVAL2GENUM(o, ATK_TYPE_LAYER))
#define ATKLAYER2RVAL(o)                   (GENUM2RVAL(o, ATK_TYPE_LAYER))
#define RVAL2ATKRELATIONTYPE(o)            (RVAL2GENUM(o, ATK_TYPE_RELATION_TYPE))
#define ATKRELATIONTYPE2RVAL(o)            (GENUM2RVAL(o, ATK_TYPE_RELATION_TYPE))
#define RVAL2ATKROLE(o)                    (RVAL2GENUM(o, ATK_TYPE_ROLE))
#define ATKROLE2RVAL(o)                    (GENUM2RVAL(o, ATK_TYPE_ROLE))
#define RVAL2ATKSTATETYPE(o)               (RVAL2GENUM(o, ATK_TYPE_STATE_TYPE))
#define ATKSTATETYPE2RVAL(o)               (GENUM2RVAL(o, ATK_TYPE_STATE_TYPE))
#define RVAL2ATKTEXTATTRIBUTE(o)           (RVAL2GENUM(o, ATK_TYPE_TEXT_ATTRIBUTE))
#define ATKTEXTATTRIBUTE2RVAL(o)           (GENUM2RVAL(o, ATK_TYPE_TEXT_ATTRIBUTE))
#define RVAL2ATKTEXTBOUNDARY(o)            (RVAL2GENUM(o, ATK_TYPE_TEXT_BOUNDARY))
#define ATKTEXTBOUNDARY2RVAL(o)            (GENUM2RVAL(o, ATK_TYPE_TEXT_BOUNDARY))
#define RVAL2ATKTEXTCLIPTYPE(o)            (RVAL2GENUM(o, ATK_TYPE_TEXT_CLIP_TYPE))
#define ATKTEXTCLIPTYPE2RVAL(o)            (GENUM2RVAL(o, ATK_TYPE_TEXT_CLIP_TYPE))

#endif /* __RBATKCONVERSIONS_H__ */
