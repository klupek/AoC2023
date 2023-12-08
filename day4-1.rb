#!/usr/bin/env ruby

mtab = Array.new

cards = 0
File.readlines(ARGV[0]).map { |line|
  multi = mtab.empty? ? 1 : mtab.shift
  cards += multi
  m = line.match(/^Card\s+(\d+): (.+?) \| (.+?)$/)
  left = m[2].scan(/\d+/).sort
  right = m[3].scan(/\d+/).sort
  both = left & right
  #pp [left,right]
  1.upto(both.size) do |i|
    mtab << 1 if mtab.size < i
    mtab[i-1] += multi
  end
  p mtab
}

puts(cards)
