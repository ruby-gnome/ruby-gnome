=begin
top-level extconf.rb for gnome extention library
=end

require 'mkmf'
require 'ftools'

priorlibs = ["glib", "gdkpixbuf", "pango", "gtk"]

#
# detect sub-directories
#
$ruby = arg_config("--ruby", Config::CONFIG['RUBY_INSTALL_NAME'])
$srcdir = File.dirname(__FILE__)
$topsrcdir = $configure_args["--topsrcdir"] ||= $srcdir
$topdir = $configure_args["--topdir"] ||= Dir.pwd
subdirs = Dir.glob($topsrcdir+"/*/**/extconf.rb")
subdirs.collect! do |subdir|
  subdir[0..$topsrcdir.size] = ""
  File.dirname(subdir)
end

subdirs -= priorlibs

#
# generate top-level Makefile
#
File.open("Makefile", "w") do |makefile|
  makefile.print("\
TOPSRCDIR = #{$topsrcdir}
SUBDIRS = #{priorlibs.join(' ')} #{subdirs.join(' ')}

all:
	for subdir in \$(SUBDIRS); do \\
		(cd \$\${subdir} && \$(MAKE) all); \\
	done;

install:
	for subdir in \$(SUBDIRS); do \\
		(cd \$\${subdir} && \$(MAKE) install); \\
	done;

site-install:
	for subdir in \$(SUBDIRS); do \\
		(cd \$\${subdir} && \$(MAKE) site-install); \\
	done;

clean:
	for subdir in \$(SUBDIRS); do \\
		(cd \$\$subdir && \$(MAKE) clean); \\
	done; \\

distclean:
	for subdir in \$(SUBDIRS); do \\
		(cd \$\${subdir} && \$(MAKE) distclean); \\
	done;
	rm -f Makefile mkmf.log
")
end

#
# generate sub-directory Makefiles
#
subdirs.each do |subdir|
  STDERR.puts("#{$0}: Entering directory `#{subdir}'")
  File.mkpath(subdir)
  topdir = File.join(*([".."] * subdir.split(/\/+/).size))
  /^\// =~ (dir = $topsrcdir) or dir = File.join(topdir, $topsrcdir)
  srcdir = File.join(dir, subdir)
  system($ruby, "-C", subdir, File.join(srcdir, "extconf.rb"),
   "--topsrcdir=#{dir}", "--topdir=#{topdir}", "--srcdir=#{srcdir}",
   *ARGV)
  STDERR.puts("#{$0}: Leaving directory `#{subdir}'")
end
