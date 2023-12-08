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
  def find(start, len)
    result = []
    @ranges.each { |r|
      if start >= r.src and start < r.src + r.len
        if r.src > start                  
          result << [ start, r.src - start ]
          len -= (r.src - start)
          start = r.src        
        end
        left = [ r.src, start ].max
        right = [ r.src + r.len, start + len ].min
        offset = left - r.src
        result << [ r.dst+offset, right - left ]
        start = right
        len -= (right - left)
      end
    }
    if len > 0
      result << [ start, len ]
    end
    result
  end
end

$maps = {}
$map_targets = {}
current_map = nil

require 'pp'
lines = File.readlines(ARGV[0])
seeds_line = lines.first.match(/^seeds:\s*(.+?)\s*$/)[1].scan(/\d+/).map { |i| i.to_i }
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
pp $maps

def find(type, i)
  new_idx = $maps[type].find(i)
  if $map_targets[type] == 'location'
    new_idx
  else
    find($map_targets[type], new_idx)
  end
end

queue = []
min = nil
seeds_line.each_slice(2) { |start,len| queue << [ 'seed', start, len ] }
until queue.empty? 
  puts "Queue: #{queue.size}"
  type, start, len = queue.shift
  if type == 'location' 
    if min.nil? or start < min
      min = start
    end
  else
#    print "#{type}:#{start}-#{len}-#{len+start} -> "
    add = $maps[type].find(start, len)
    add.each { |a,b|
      queue << [ $map_targets[type], a, b ]
    }
#    puts $map_targets[type] + ":" + add.map { |a,b| "#{a}-#{b}-#{a+b}" }.join(" ")
  end
end
puts min
