=begin
  top-level extconf.rb for Ruby-GNOME2

  $Id: extconf.rb,v 1.17 2007/10/22 12:19:17 ktou Exp $

  Copyright (C) 2003-2005 Ruby-GNOME2 Project Team
=end

require 'English'
require 'mkmf'
require 'fileutils'
require 'pathname'

priorlibs = ["glib2", "gio2", "gdk_pixbuf2", "pango", "atk", "gtk2"]

#
# detect sub-directories
#
$ruby = File.join(Config::CONFIG['bindir'], Config::CONFIG['RUBY_INSTALL_NAME'] + Config::CONFIG['EXEEXT'])
$ruby = arg_config("--ruby", $ruby)

rm = "rm -f"
if /mswin32/ =~ RUBY_PLATFORM
  rm = "del"
end


$srcdir = File.dirname(__FILE__)
$topsrcdir = $configure_args["--topsrcdir"] ||= $srcdir
$topdir = $configure_args["--topdir"] ||= Dir.pwd
$strict = $configure_args["--strict"] ? "--strict" : ""

$srcdir = File.expand_path($srcdir)
$topsrcdir = File.expand_path($topsrcdir)
$topdir = File.expand_path($topdir)

subdirs = ARGV.select{|v|  /^--/ !~ v}

if subdirs.size == 0
  subdirs = Dir.glob($topsrcdir+"/*/extconf.rb")
  subdirs.collect! do |subdir|
    subdir[0..$topsrcdir.size] = ""
    File.dirname(subdir)
  end
  priorlibs &= subdirs
  subdirs -= priorlibs
  subdirs = priorlibs + subdirs #Change the order
end

#
# generate sub-directory Makefiles
#
targets = []
ignore = []

ruby, *ruby_args = Shellwords.shellwords($ruby)
if ARGV.grep(/\A--ruby=/)
  extra_args = ["--ruby=#{$ruby}"] + ARGV.reject {|arg| /\A--ruby=/ =~ arg}
else
  extra_args = ARGV.dup
end

subdirs.each do |subdir|
  STDERR.puts("#{$0}: Entering directory `#{subdir}'")
  FileUtils.mkdir_p(subdir)
  topdir = File.join(*([".."] * subdir.split(/\/+/).size))
  dir = $topsrcdir
  dir = File.join(topdir, dir) unless Pathname.new(dir).absolute?
  srcdir = File.join(dir, subdir)
  args = ruby_args + ["-C", subdir, File.join(srcdir, "extconf.rb"),
                      "--topsrcdir=#{dir}", "--topdir=#{topdir}",
                      *extra_args]
  ret = system(ruby, *args)
  STDERR.puts("#{$0}: Leaving directory '#{subdir}'")
  if ret
    targets << subdir
  else
    ignore << subdir
  end
end
puts "\n-----"
puts "Target libraries: #{targets.join(', ')}" if targets.size > 0
puts "Ignored libraries: #{ignore.join(', ')}" if ignore.size > 0

#
# generate top-level Makefile
#

def run_make_in_sub_dirs(command)
  if /mswin32/ =~ RUBY_PLATFORM
    "	$(COMMAND) '$(SUBDIRS)' $(MAKE) #{command}"
  else
    <<-EOS.chomp
	@(					\\
	  succeeded='';				\\
	  failed='';				\\
	  for dir in $(SUBDIRS); do		\\
	    (cd $$dir; $(MAKE) #{command});	\\
	    if [ $$? -eq 0 ]; then		\\
	      succeeded="$$succeeded $$dir";	\\
	    else				\\
	      failed="$$failed $$dir";		\\
	    fi;					\\
	  done;					\\
	  if [ "$$succeeded" = "" ]; then	\\
	    succeeded="NONE";			\\
	  fi;					\\
	  if [ "$$failed" = "" ]; then		\\
	    failed="NONE";			\\
	  fi;					\\
	  echo;					\\
	  echo "-----";				\\
	  echo "SUCCEEDED: $$succeeded";	\\
	  echo "FAILED: $$failed";		\\
	  echo "-----";				\\
	  echo "Done.";				\\
	)
    EOS
  end
end


File.open("Makefile", "w") do |makefile|
  makefile.print(<<-EOM)
TOPSRCDIR = #{$topsrcdir}
SUBDIRS = #{targets.join(' ')}
COMMAND = #{$ruby} #{$topsrcdir}/exec_make.rb #{$strict}
RM = #{rm}

all:
#{run_make_in_sub_dirs('all')}

install:
#{run_make_in_sub_dirs('install')}

site-install:
#{run_make_in_sub_dirs('site-install')}

clean:
#{run_make_in_sub_dirs('clean')}

distclean:
#{run_make_in_sub_dirs('distclean')}
	$(RM) Makefile mkmf.log
EOM
end

puts "-----"
puts "Done."

$makefile_created = true
