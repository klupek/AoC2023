#!/usr/bin/env ruby

require 'pp'

cardmap = %w[ J 2 3 4 5 6 7 8 9 T Q K A ]
pp File.readlines(ARGV[0]).map { |line|
  cards, bid = line.split(/\s+/)
  cards = cards.each_char.map { |c| cardmap.index(c) }
  hash = cards.sort.inject({}) { |c, v|
    if c[v]
      c[v] += 1
    else
      c[v] = 1
    end
    c
  }
  sorted = hash.sort_by { |k,v|
    [ v, k ]
  }.reverse
  jokers = (hash[0] || 0)
  if sorted.size == 1 or (sorted.size == 2 and jokers > 0)
    # XXXXJ
    # JJJJJ
    # XXXJJ
    # XXJJJ
    # XJJJJ
    # five of kind
    r = [ 7, *cards ]
  elsif (sorted.size == 2 and sorted.first.last == 4) or # XXXXY
        (sorted.size == 3 and sorted.first.last == 3 and jokers == 1) or # XXXYJ 
        (sorted.size == 3 and sorted.first.last == 2 and jokers == 2) or # XXJJY
        (jokers == 3) # JJJXY 
    # four of kind
    r = [ 6, *cards ]
  elsif (sorted.size == 2 and sorted.first.last == 3) or # XXXYY
        (jokers == 1 and sorted.first.last == 2 and sorted.size == 3) # XXYYJ      
    # full house
    r = [ 5, *cards ]
  elsif (sorted.first.last == 3) or # XXXYZ
        (sorted.first.last == 2 and jokers == 1) or # XXJYZ
        (jokers == 2) # JJXYZ
    # three of kind
    r = [ 4, *cards ]
  elsif (sorted.first.last == 2 and sorted[1].last == 2) # XXYYZ
    # two pairs
    r = [ 3, *cards ]
  elsif sorted.first.last == 2 or jokers == 1
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

