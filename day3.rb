lines = File.readlines(ARGV[0]).map { |line|
  line.gsub(/^\s*(.+?)\s*$/,'\1')
}

width = lines.first.length
ids = []

def at(line, character)
  line * 1000000 + character
end

$symbols = Hash.new
def add(x,y,r, c) 
  if c == '*'
    idx = at(x,y)
    $symbols[idx] = [] unless $symbols[idx]
    $symbols[idx] << r.to_i
  end
end
lines.each_with_index { |line, i|
  line.scan(/\d+/) { |r|
    fst,lst = $~.offset(0)
    mark = false
    # next to symbol?
    if (fst >= 1 and lines[i][fst-1] != '.') 
      mark = true
      add(i,fst-1,r,lines[i][fst-1])
    end
    if (lst < width and lines[i][lst] != '.') 
      mark = true
      add(i,lst,r,lines[i][lst])
    end
    p [ r, :leftright, i, fst, lst, lines[i][lst] != '.', lines[i][fst-1..lst] ]
    if i > 0
      left = [ fst - 1, 0 ].max
      right = [ lst, width - 1 ].min
      lines[i-1][left..right].each_char.each_with_index { |x, ci|
        if !(x.ord >= '0'.ord and x.ord <= '9'.ord) and x != '.'
          add(i-1,left+ci,r,x)
          mark = true
        end
      }
#      mark |= lines[i-1][left..right].each_char.detect { |x| !x.match(/^\d+$/) and x != '.' }
      p [ r, :top , lines[i-1][left..right], mark] if !mark
    end
    if i < lines.size - 1
      left = [ fst - 1, 0 ].max
      right = [ lst, width - 1 ].min
      lines[i+1][left..right].each_char.each_with_index { |x, ci|
        if !(x.ord >= '0'.ord and x.ord <= '9'.ord) and x != '.'
          add(i+1,left+ci,r,x)
          mark = true
        end
      }
#      mark |= lines[i+1][left..right].each_char.detect { |x| !x.match(/^\d+$/) and x != '.' }
      p [ r, :bottom, lines[i+1][left..right]] if !mark
    end
    ids << r.to_i if mark
  }
}

sum = 0
$symbols.each { |key, v|
  if v.size == 2
    pp v
    sum += v.reduce(:*)
  end
}

require 'pp'
pp $symbols
puts ids.reduce(:+)
puts sum
