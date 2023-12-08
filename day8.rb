#!/usr/bin/env ruby


lines = File.readlines(ARGV[0])
trans = [ 'L', 'R' ]
seq = lines.first.match(/^([RL]+)\s*$/)[1].each_char.map { |c|
  trans.index(c)
}

network = {}
lines[2..-1].each { |line|
  src, left, right = line.match(/^(.+?) = \((.+?),\s+(.+?)\)$/)[1..3]
  network[src] = [ left, right ]
}

current = 'AAA'
seqidx = 0
count = 0
while current != 'ZZZ'
  current = network[current][seq[seqidx]]
  seqidx = (seqidx + 1) % seq.size
  count += 1
end
puts count

