#!/usr/bin/env ruby

require 'pp'

# distance = (t-x)*x = tx - x^2
# -x^2 + tx > rd
# -x^2 + tx - rd > 0
# delta = t^2 - 4*rd
# x = (t +- sqrt(delta))/2

lines = File.readlines(ARGV[0])
time_array = lines[0].match(/^Time:\s*(.+?)\s*$/)[1].scan(/\d+/).map { |i| i.to_f }
distance_array = lines[1].match(/^Distance:\s*(.+?)\s*$/)[1].scan(/\d+/).map { |i| i.to_f }

array = time_array.zip(distance_array)
pp array.map { |time, distance|
  delta = time*time - 4*distance
  x1 = (time + Math.sqrt(delta))/2
  x2 = (time - Math.sqrt(delta))/2
  x2 += 0.5 if x2 == x2.floor
  x1 -= 0.5 if x1 == x1.floor
  x1.floor - x2.ceil + 1
}.reduce(:*)


time_array = lines[0].match(/^Time:\s*(.+?)\s*$/)[1].gsub(/\s/,'').to_f
distance_array = lines[1].match(/^Distance:\s*(.+?)\s*$/)[1].gsub(/\s/,'').to_f

pp [[time_array, distance_array]].map { |time, distance|
  pp [ time, distance ]
  delta = time*time - 4*distance
  x1 = (time + Math.sqrt(delta))/2
  x2 = (time - Math.sqrt(delta))/2
  x2 += 0.5 if x2 == x2.floor
  x1 -= 0.5 if x1 == x1.floor
  x1.floor - x2.ceil + 1
}.reduce(:*)
