=begin
top-level extconf.rb for gnome extention library
=end

require 'mkmf'

#
# detect sub-directories
#
subdirs = Dir["*"].select do |subdir|
  File.file?(subdir + "/extconf.rb")
end

#
# generate top-level Makefile
#
File.open("Makefile", "w") do |makefile|
  makefile.print("\
SUBDIRS = #{subdirs.join(' ')}

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
  Dir.chdir(subdir)
  system("#{Config::CONFIG['RUBY_INSTALL_NAME']} extconf.rb")
  Dir.chdir("..")
  STDERR.puts("#{$0}: Leaving directory `#{subdir}'")
end
