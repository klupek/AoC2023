max = { 'red' => 12, 'green' => 13, 'blue' => 14 }
puts File.readlines(ARGV[0]).map { |line| 
  m = line.match(/^Game (\d+): (.+)$/); 
  r = m[2].scan(/(\d+) (red|green|blue)/).map { |cnt,color|
    cnt.to_i > max[color]
  }.reduce(:|)
  r ? 0 : m[1].to_i 
}.reduce(:+)
