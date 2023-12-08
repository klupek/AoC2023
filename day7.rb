#!/usr/bin/env ruby

require 'pp'

cardmap = %w[ 2 3 4 5 6 7 8 9 T J Q K A ]
pp File.readlines(ARGV[0]).map { |line|
  cards, bid = line.split(/\s+/)
  cards = cards.each_char.map { |c| cardmap.index(c) }
  sorted = cards.sort.inject({}) { |c, v|
    if c[v]
      c[v] += 1
    else
      c[v] = 1
    end
    c
  }.sort_by { |k,v|
    [ v, k ]
  }.reverse
  if sorted.size == 1
    # five of kind
    r = [ 7, *cards ]
  elsif sorted.size == 2 and sorted.first.last == 4
    # four of kind
    r = [ 6, *cards ]
  elsif sorted.size == 2 and sorted.first.last == 3
    # full house
    r = [ 5, *cards ]
  elsif sorted.first.last == 3
    # three of kind
    r = [ 4, *cards ]
  elsif sorted.first.last == 2 and sorted[1].last == 2
    # two pairs
    r = [ 3, *cards ]
  elsif sorted.first.last == 2
    # one pair
    r = [ 2, *cards ]
  else
    r = [ 1, *cards ]
  end
  [ r, bid.to_i ]
}.sort_by { |a,b| 
  a 
}.each_with_index.map { |(r, bid), idx|
  bid * (idx+1)
}.reduce(:+)

