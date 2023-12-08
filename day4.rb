#!/usr/bin/env ruby

points = 0
File.readlines(ARGV[0]).map { |line|
  m = line.match(/^Card\s+(\d+): (.+?) \| (.+?)$/)
  left = m[2].scan(/\d+/).sort
  right = m[3].scan(/\d+/).sort
  both = left & right
  #pp [left,right]
  unless both.empty?
    points += 2**(both.size-1)
  end
}

puts(points)
