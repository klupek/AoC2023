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

start = network.map { |k,v| k }.find_all { |k| k.end_with?('A') }
diffs = start.map { |k|
  seqidx = 0
  count = 0
  orig = k
  while !k.end_with?('Z')
    k = network[k][seq[seqidx]]
    seqidx = (seqidx + 1) % seq.size
    count += 1
  end
  mid = k
  midcount = count
  k = network[k][seq[seqidx]]
  seqidx = (seqidx + 1) % seq.size
  count += 1
  while !k.end_with?('Z')
    k = network[k][seq[seqidx]]
    seqidx = (seqidx + 1) % seq.size
    count += 1
  end
  puts "#{orig} -> #{mid} (#{midcount} steps) -> #{k} (#{count} steps) (ratio #{count.to_f/midcount})"
  [ midcount, count - midcount ]
}
pp diffs
lcm = diffs.inject(1) { |acc, (midcount, diff)|
  acc.lcm(diff)
}
lcm2 = seq.size.lcm(lcm)
puts lcm
puts lcm2
exit

# it works only because test data was crafted in a way
# that 'whateverA' -> 'whateverZ' requires 
# same number of steps as 'whateverZ' -> 'whateverMaybeOtherZ' (cycling/looping)
# it would require some handcrafted LCM-like algorithm 
# to work with more random data
