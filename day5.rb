#!/usr/bin/env ruby
require 'ostruct'

class AoCMap 
  def initialize()
    @ranges = []
  end
  def add(dst, src, len)
    insert_at = @ranges.bsearch_index { |i|
      i.src >= src
    }
    obj = OpenStruct.new(:src => src, :dst => dst, :len => len)
    unless insert_at.nil?
      @ranges.insert(insert_at, obj)
    else
      @ranges << obj
    end
  end
  def find(i)
    r = @ranges.bsearch { |x|
      x.src + x.len > i 
    }    
    if r and i >= r.src and i < r.src + r.len
      r.dst + (i - r.src)
    else
      i
    end
  end
end

$maps = {}
$map_targets = {}
current_map = nil

require 'pp'
lines = File.readlines(ARGV[0])
seeds = lines.first.match(/^seeds:\s*(.+?)\s*$/)[1].scan(/\d+/).map { |i| i.to_i }
lines[1..-1].each { |line|
  if m = line.match(/^(.+?)-to-(.+?) map:\s*$/)
    src = m[1]
    dst = m[2]
    $map_targets[src] = dst
    current_map = AoCMap.new
    $maps[src] = current_map    
  elsif m = line.match(/^(\d+) (\d+) (\d+)$/)
    dst_i = m[1].to_i
    src_i = m[2].to_i
    len = m[3].to_i
    current_map.add(dst_i, src_i, len)
  elsif m = line.match(/^\s*$/)
    # skip
  else
    puts "Invalid line: #{line}"
  end
}
pp seeds
pp $maps

def find(type, i)
  new_idx = $maps[type].find(i)
  if $map_targets[type] == 'location'
    puts "#{type}:#{i} -> #{$map_targets[type]}:#{new_idx}"
    new_idx
  else
    puts "#{type}:#{i} -> #{$map_targets[type]}:#{new_idx}"
    find($map_targets[type], new_idx)
  end
end

pp seeds.map { |i|
  find('seed', i)
}.sort.first
