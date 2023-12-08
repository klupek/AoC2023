puts File.readlines(ARGV[0]).map { |line|
  numbers = line.scan(/(?=(one|two|three|four|five|six|seven|eight|nine|\d))/)  
  t = [ 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine' ]
  numbers.map! { |i|
    i = i.first
    if i.match(/^\d$/)
      i.to_i
    else
      t.index(i)+1
    end
  }
  p [ line, numbers ]
  if numbers.size == 1
    numbers[0]*10 + numbers[0]
  else
    numbers[0]*10 + numbers.last
  end
}.reduce(:+)
