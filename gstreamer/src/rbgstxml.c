
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/*
 *  Class: Gst::XML
 *
 *  Convert Gst::Element objects into an XML presentation. 
 */

/*
 *  Class method: new -> aXMLobject
 *
 *  Create a new GstXML parser object.  
 */
static VALUE rb_gst_xml_new(self)
    VALUE self;
{
    GstXML *xml = gst_xml_new();
    if (xml != NULL) {
        G_INITIALIZE(self, xml);
    }
    return Qnil;
}

/*
 *  Class method: write_file(aGstElement, aFileName) -> aBoolean
 *
 *  Converts the given element into XML and writes the formatted XML 
 *  to the specified file name.
 *  
 *  Returns true on success, false on failure.
 */
static VALUE rb_gst_xml_write_file(self, element, filename)
    VALUE self, element, filename;
{
    FILE *file = fopen(RVAL2CSTR(filename), "w");
    gboolean ret = FALSE;
    if (file != NULL) {
        ret = gst_xml_write_file(RGST_ELEMENT(element), file) != -1; 
        fclose(file);
    }
    return CBOOL2RVAL(ret);
}

/*
 *  Method: parse_file(aFilename, aRootname = nil) -> aBoolean
 *
 *  Fills the Gst::XML object with the corresponding elements from 
 *  the XML file name.  Optionally it will only build the element 
 *  from the element node root (if it is not nil).  
 *
 *  This feature is useful if you only want to build a specific 
 *  element from an XML file but not the pipeline it is embedded in.
 *
 *  Pass "-" as file name to read from stdin.  You can also pass a 
 *  URI of any format that libxml supports, including http.
 *  
 *  Returns true on success, false on failure.
 */
static VALUE rb_gst_xml_parse_file(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE fname, rootname;
    rb_scan_args(argc, argv, "11", &fname, &rootname);    
    return CBOOL2RVAL(gst_xml_parse_file(RGST_XML(self),
                                         RVAL2CSTR(fname),
                                         NIL_P(rootname) 
                                            ? NULL 
                                            : RVAL2CSTR(rootname)));
}

/*
 *  Method: get_element(anElementName) -> anElement
 *
 *  This method is used to get a reference to the Gst::Element 
 *  corresponding to name in the pipeline description.  You would use 
 *  this if you have to do anything to the element after loading.
 */
static VALUE rb_gst_xml_get_element(self, element_name)
    VALUE self, element_name;
{
    GstElement *element = gst_xml_get_element(RGST_XML(self),
                                              RVAL2CSTR(element_name));
    return element != NULL
        ? RGST_ELEMENT_NEW(element)
        : Qnil;
}

/*
 *  Method: topelements -> anArray
 *
 *  Retrieve a list of all toplevel elements, in an array
 *  of Gst::Element objects.
 */
static VALUE rb_gst_xml_get_topelements(self)
    VALUE self;
{
    GList *list;
    VALUE arr;

    arr = rb_ary_new();
    for (list = gst_xml_get_topelements(RGST_XML(self));
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, RGST_ELEMENT_NEW(list->data)); 
    }
    return arr;
}

/*
 *  Method: each_topelement { |aGstElement| ... } -> nil
 *
 *  Calls the block for each topelement, passing a reference to
 *  a Gst::Element object as parameter.
 */

static VALUE rb_gst_xml_each_topelement(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_xml_get_topelements(self));
}

void Init_gst_xml(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_XML, "XML", mGst);
    rb_define_singleton_method(c, "write_file", rb_gst_xml_write_file, 2);
    rb_define_method(c, "initialize", rb_gst_xml_new, 0);
    rb_define_method(c, "parse_file", rb_gst_xml_parse_file, -1);
    rb_define_method(c, "get_element", rb_gst_xml_get_element, 1);
    rb_define_method(c, "topelements", rb_gst_xml_get_topelements, 0);
    rb_define_method(c, "each_topelement", rb_gst_xml_each_topelement, 0);
}

