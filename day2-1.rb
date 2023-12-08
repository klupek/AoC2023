max = { 'red' => 12, 'green' => 13, 'blue' => 14 }
puts File.readlines(ARGV[0]).map { |line| 
  m = line.match(/^Game (\d+): (.+)$/);
  mins = { 'red' => 0, 'green' => 0, 'blue' => 0 } 
  m[2].scan(/(\d+) (red|green|blue)/).each { |cnt,color|
    mins[color] = [ mins[color], cnt.to_i ].max
  }
  power = mins['red'] * mins['green'] * mins['blue']
}.reduce(:+)
