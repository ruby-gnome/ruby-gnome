require 'gtksourceview'

s = Gtk::SourceLanguagesManager.new
s.available_languages.each do |v|
  puts v.name
end
s.lang_files_dirs.each do |v|
  puts v
end
puts s.get_language("text/html").name

