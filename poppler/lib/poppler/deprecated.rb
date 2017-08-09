# Copyright (C) 2017  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

module Poppler
  extend GLib::Deprecatable

  define_deprecated_singleton_method(:cairo_available?,
                                     :warn => "cairo is always available") do
    true
  end

  define_deprecated_const(:Annotation, "Poppler::Annot")
  define_deprecated_const(:AnnotationType, "Poppler::AnnotType")
  define_deprecated_const(:AnnotationFlag, "Poppler::AnnotFlag")
  define_deprecated_const(:AnnotationMarkupReplyType,
                          "Poppler::AnnotMarkupReplyType")
  define_deprecated_const(:AnnotationExternalDataType,
                          "Poppler::AnnotExternalDataType")
  define_deprecated_const(:AnnotationTextState, "Poppler::AnnotTextSate")
  define_deprecated_const(:AnnotationFreeTextQuadding,
                          "Poppler::AnnotFreeTextQuadding")
  define_deprecated_const(:AnnotationMarkup,
                          "Poppler::AnnotMarkup")
  define_deprecated_const(:AnnotationText,
                          "Poppler::AnnotText")
  define_deprecated_const(:AnnotationFreeText,
                          "Poppler::AnnotFreeText")
  define_deprecated_const(:AnnotationMapping,
                          "Poppler::AnnotMapping")

  define_deprecated_const(:ActionURI,
                          "Poppler::ActionUri")
  define_deprecated_const(:ActionJavaScript,
                          "Poppler::ActionJavascript")

  class AnnotMapping
    extend GLib::Deprecatable

    define_deprecated_method(:annotation, :annot)
  end

  class Page
    extend GLib::Deprecatable

    define_deprecated_method(:annotation_mapping, :annot_mapping)
    define_deprecated_method_by_hash_args(:get_text,
                                          "area=nil, style=nil",
                                          ":area => Poppler::Rectangle, " +
                                          ":style => Poppler::SelectionStyle"
                                          ) do |page, *args, &block|
      [{:area => args[0], :style => args[1]}]
    end
  end
end
