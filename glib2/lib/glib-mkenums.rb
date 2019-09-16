#
# glib-mkenums.rb
#
# C language enum description generation library like as glib-mkenums tool.
#
# Copyright(C) 2006-2015 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.
#

module GLib
  class EnumDefinition
    attr_accessor :EnumName, :enum_name, :ENUM_NAME, :ENUM_SHORT
    attr_accessor :type, :Type
    attr_accessor :g_type_prefix, :prefix

    attr_reader :constants

    def initialize(name, const_lines, g_type_prefix, options={})
      @options = options || {}
      @EnumName = name
      @g_type_prefix = g_type_prefix
      @constants = []
      @enum_name = to_snail_case(@EnumName)
      @ENUM_NAME = @enum_name.upcase
      @ENUM_SHORT = @ENUM_NAME.sub(/^#{@g_type_prefix.sub(/_TYPE.*$/, "")}/, "").sub(/^_/, "")

      parse_const_lines(const_lines)
    end

    def parse_const_lines(const_lines)
      if @options[:force_flags] or /<</ =~ const_lines
        @type = "flags"
        @Type = "Flags"
      else
        @type = "enum"
        @Type = "Enum"
      end
      constants = []
      const_lines.scan(/^\s*([^\s,]*).*\n/) do |name|
        constants << name[0] unless name[0] =~ /(^[\/\*]|^#|^$)/
      end
      @prefix = extract_prefix(constants)
      constants.each do |name|
        @constants << [name, name.sub(/#{@prefix}/, "").gsub(/_/, "-").downcase]
      end
    end

    def extract_prefix(ary)
      return [] if ary == nil
      a = ary[0].split(//)
      if ary.size == 1
        @ENUM_NAME + "_"
      else
        ary[1..-1].each do |b|
          b = b.split(//)
          l = [a.length, b.length].min
          a = a[0, (0...l).find{|i| a[i] != b[i] } || l]
        end 
        a.join('')
      end
    end

    def create_c
      constants = "\n" + @constants.collect{|name, nick|
        %Q[      { #{name}, "#{name}", "#{nick}" },\n] 
      }.join +
        %Q[      { 0, NULL, NULL }]

      ret = <<-CREATE_C

GType
#{@enum_name}_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const G#{@Type}Value values[] = {#{constants}
    };
    etype = g_#{@type}_register_static ("#{@EnumName}", values);
  }
  return etype;
}
      CREATE_C
      ret
    end

    def create_h
      %Q[
GType #{@enum_name}_get_type (void);
#define #{@g_type_prefix}#{@ENUM_SHORT} (#{@enum_name}_get_type())]
    end

    private
    def to_snail_case(name)
      prefix_processed_name = name.sub(/^[A-Z]/) do |prefix|
        prefix.downcase
      end
      snail_cased_name = prefix_processed_name.gsub(/[A-Z]+/) do |upper_case|
        down_case = upper_case.downcase
        if down_case.size == 1
          "_#{down_case}"
        else
          "_#{down_case[0..-2]}_#{down_case[-1..-1]}"
        end
      end
      snail_cased_name.sub(/(^_|_$)/, "")
    end

    def self.parse(data, g_type_prefix, options={})
      options ||= {}
      enums = []
      data.force_encoding("utf-8") if data.respond_to?(:force_encoding)
      data.scan(/^\s*typedef\s+enum\s*(\/\*<\s*flags\s*>\*\/)?\s*
                \{?\s*(.*?)
                \}\s*(\w+)
                # GLIB_DEPRECATED_TYPE_IN_2_38_FOR(GTestSubprocessFlags)
                (?:\s+[\w()\s]+)?
                ;/mx) do |force_flags, constants, name|
        enum_options = {}
        enum_options[:force_flags] = !force_flags.nil?
        force_flags_patterns = [(options[:force_flags] || [])].flatten
        if force_flags_patterns.any? {|pattern| pattern === name}
          enum_options[:force_flags] = true
        end
        enum = new(name, constants, g_type_prefix, enum_options)
        enums << enum
      end
      enums
    end
  end

  class MkEnums
    # Create target_filename.c and target_filename.h from files
    # with g_type_prefix and include_files.
    # * target_filename: the target file name. This creates #{target_filename.c} and #{target_filename.h}.
    # * files: header files to parse
    # * g_type_prefix: the gtype prefix such as GTK_TYPE_
    # * include_files: define #include <file> lines into target_filename.c
    def self.create(target_filename, files, g_type_prefix, include_files,
                    options)
      puts "creating #{target_filename}.c"
      mkenums = MkEnums.new(target_filename, files, g_type_prefix, include_files,
                            options)

      open("#{target_filename}.c", "w") do |out|
        out.write(mkenums.create_c)
      end
      puts "creating #{target_filename}.h"
      open("#{target_filename}.h", "w") do |out|
        out.write(mkenums.create_h)
      end
    end

    # Initialize GLib::MkEnums
    #
    # * target_filename: the target file name. This creates #{target_filename.c} and #{target_filename.h}.
    # * files: header files to parse
    # * g_type_prefix: the gtype prefix such as GTK_TYPE_
    # * include_files: define #include <file> lines into target_filename.c
    def initialize(target_filename, files, g_type_prefix, include_files, options)
      @target_filename = target_filename
      @include_files = include_files
      @targets = []
      files.sort.each do |path|
        data = ""
        File.open(path) do |i|
          data = i.read
        end
        @targets << [path, EnumDefinition.parse(data, g_type_prefix, options)]
      end
      @preamble = options[:preamble]
    end

    def create_enums(meth)  # :nodoc:
      ret = ""
      @targets.each do |target|
        if target[1].size > 0
          ret << %Q[\n\n/* enumerations from "#{target[0]}" */]
          target[1].each{|enum|
            ret << enum.__send__(meth)
          }
        end
      end
      ret
    end

    # Create a C source as a String.
    def create_c
      ret = "\n/* Generated by glib-mkenums.rb ($Id$) */ \n\n"
      ret << "#{@preamble}\n" if @preamble
      ret << %Q[#include "#{@target_filename}.h"\n]
      @include_files.each do |file|
        ret << "#include <#{file}>\n"
      end
      ret << "\n"
      ret << create_enums(:create_c)
      ret << "\n\n/* Generated data ends here */\n\n"
      ret
    end

    # Create a C header as a String.
    def create_h
      header = "#{@target_filename}.h"
      const = "__#{File.basename(header).upcase.gsub(/-|\./, '_')}__"

      ret = "\n/* Generated by glib-mkenums.rb ($Id$) */ \n\n"
      ret << "#ifndef #{const}\n"
      ret << "#define #{const}\n\n"
      ret << "#include <glib-object.h>\n\n"
      ret << "G_BEGIN_DECLS"
      ret << create_enums(:create_h)
      ret << "\n\nG_END_DECLS\n\n"
      ret << "#endif /* #{const} */\n"
      ret << "\n/* Generated data ends here */\n\n"
      ret
    end
  end
end

