puts File.readlines(ARGV[0]).map { |line| m=line.match(/^.*?(\d)(?:.*(\d)[^\d]*|[^\d]*)$/); [m[1].to_i*10+(m[2] ? m[2] : m[1]).to_i].first }.reduce(:+)
