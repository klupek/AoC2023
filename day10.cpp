#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <algorithm>

// g++ -std=c++17 -Wl,-z,stack-size=1000000000 -o day10 day10.cpp
// ulimit -s unlimited
// yes, i am too lazy to write iterative version.
enum PIPE_SEGMENT {
	GROUND = 0,
	VERTICAL = 1,
	HORIZONTAL = 2,
	NE,
	NW,
	SW,
	SE,	
	JOKER
};

typedef std::pair<int, int> pos_t;
typedef std::pair<int, int> vec_t;

typedef std::vector<vec_t> links_t;
template<typename T>
using mymap = std::vector<std::vector<T>>;
typedef mymap<PIPE_SEGMENT> map_t;

std::map<PIPE_SEGMENT, links_t> allowed_links;
PIPE_SEGMENT input_translation[256] = { GROUND };
char input_reverse_translation[30];

pos_t add(pos_t a, vec_t b) {
	return pos_t(a.first + b.first, a.second + b.second);
}

vec_t diff(pos_t a, pos_t b) {
	return vec_t(a.first - b.first, a.second - b.second);
}

bool valid(pos_t a, int width, int height) {
	return a.first >= 0 && a.second >= 0 && a.first < height && a.second < width;
}

pos_t invalid_pos(-1, -1);


bool dfsIsCyclic(std::string, map_t& map, std::set<pos_t>& visited, pos_t joker, pos_t start, pos_t parent, std::vector<pos_t>& history);

int main(int argc, char* argv[]) {	
	input_translation['|'] = VERTICAL;
	input_translation['-'] = HORIZONTAL;
	input_translation['L'] = NE;
	input_translation['J'] = NW;
	input_translation['7'] = SW;
	input_translation['F'] = SE;
	input_translation['.'] = GROUND;
	input_translation['S'] = JOKER;
	input_reverse_translation[VERTICAL] = '|';
	input_reverse_translation[HORIZONTAL] = '-';
	input_reverse_translation[NE] = 'L';
	input_reverse_translation[NW] = 'J';
	input_reverse_translation[SW] = '7';
	input_reverse_translation[SE] = 'F';
	input_reverse_translation[GROUND] = '.';
	input_reverse_translation[JOKER] = 'S';

	
	allowed_links[VERTICAL].emplace_back(-1, 0);
	allowed_links[VERTICAL].emplace_back(1, 0);
	allowed_links[HORIZONTAL].emplace_back(0, -1);
	allowed_links[HORIZONTAL].emplace_back(0, 1);
	allowed_links[NE].emplace_back(-1, 0);
	allowed_links[NE].emplace_back(0, 1);
	allowed_links[NW].emplace_back(-1, 0);
	allowed_links[NW].emplace_back(0, -1);
	allowed_links[SW].emplace_back(1, 0);
	allowed_links[SW].emplace_back(0, -1);
	allowed_links[SE].emplace_back(1, 0);
	allowed_links[SE].emplace_back(0, 1);
	allowed_links[JOKER].emplace_back(-1, -1);
	allowed_links[JOKER].emplace_back(-1, 0);
	allowed_links[JOKER].emplace_back(-1, 1);
	allowed_links[JOKER].emplace_back(0, -1);	
	allowed_links[JOKER].emplace_back(0, 1);
	allowed_links[JOKER].emplace_back(1, -1);
	allowed_links[JOKER].emplace_back(1, 0);
	allowed_links[JOKER].emplace_back(1, 1);

	for (int i = 1; i < argc; ++i) {
		map_t map;
		std::cout << "Reading " << argv[i] << std::endl;
		std::ifstream input_file(argv[i]);
		std::string line;
		while (input_file >> line) {
			map.emplace_back();
			for (const char c : line) {
				map.back().push_back(input_translation[c]);
			}
		}

		pos_t s;
		int width = map.front().size();
		int height = map.size();
		for (int i = 0; i < height; ++i) {
			auto it = std::find(map[i].begin(), map[i].end(), JOKER);
			if (it != map[i].end()) {
				s.first = i;
				s.second = std::distance(map[i].begin(), it);
			}
		}

		std::set<pos_t> visited;
		std::vector<pos_t> history;
		history.push_back(s);
		dfsIsCyclic(argv[i], map, visited, s, s, invalid_pos, history);

	}
	return 0;
}

std::ostream& operator<<(std::ostream& o, pos_t v) {
	return o << "[ " << v.first << ", " << v.second << " ]";
}

bool dfsIsCyclic(std::string name, map_t& map, std::set<pos_t>& visited, pos_t joker, pos_t current, pos_t parent, std::vector<pos_t>& history) {
	visited.insert(current);

	PIPE_SEGMENT segment = map[current.first][current.second];
	for (const vec_t& v : allowed_links[segment]) {
		const pos_t np = add(current, v);
		if (!valid(np, map.front().size(), map.size()))
			continue; 

		PIPE_SEGMENT next = map[np.first][np.second];
		bool found = false;
		for (const vec_t& nv : allowed_links[next]) {
			pos_t test = add(np, nv);
			if (test == current) {
				found = true;
				break;
			}
		}
		if (!found) {
			continue;
		}

		//std::cout << "Link: " << current << " <- " << v << " -> " << np << std::endl;

		if (visited.find(np) == visited.end()) {
			//std::cout << current << "(" << input_reverse_translation[segment] << ") -> " << np << "(" << input_reverse_translation[next] << ")" << std::endl;
			history.push_back(np);
			if (dfsIsCyclic(name, map, visited, joker, np, current, history)) {
				return true;
			}
			history.pop_back();
		} else if (np != parent) {
			//history.push_back(np);
			/*std::cout << "Cycle: ";
			for (pos_t pos : visited) {
				std::cout << pos << " ";
			}
			puts("");*/
			pos_t maxdistance = history[history.size() / 2 + history.size() % 2 - 1];
			std::cout << "Max distance: " << history.size() / 2 + history.size() % 2 << std::endl;
			std::ofstream f(name + "-out.txt");
			int scale = 10, off = 50;
			auto i = history.begin();
			++i;
			for(;i != history.end();++i) {
				pos_t prev = *std::prev(i);
				pos_t cur = *i;
				f << "canvas.create_line(" << prev.second * scale + off << ", " << prev.first * scale + off << ", " << cur.second * scale + off << ", " << cur.first * scale + off << ")\n";
			}
			
			vec_t v1 = diff(history[1], history[0]);
			vec_t v2 = diff(history.back(), history.front());
			if (v1 > v2) std::swap(v1, v2);
			/*
			allowed_links[NE].emplace_back(-1, 0);
			allowed_links[NE].emplace_back(0, 1);
			allowed_links[NW].emplace_back(-1, 0);
			allowed_links[NW].emplace_back(0, -1);
			allowed_links[SW].emplace_back(1, 0);
			allowed_links[SW].emplace_back(0, -1);
			allowed_links[SE].emplace_back(1, 0);
			allowed_links[SE].emplace_back(0, 1);
			*/
			int y = history.front().first;
			int x = history.front().second;
			if (v1 == pos_t(-1, 0) && v2 == pos_t(0, 1)) {
				map[y][x] = NE;
			} else if (v1 == pos_t(-1, 0) && v2 == pos_t(0, -1)) {
				map[y][x] = NW;
			} else if (v1 == pos_t(0, -1) && v2 == pos_t(1, 0)) {
				map[y][x] = SW;
			} else if (v1 == pos_t(0, 1) && v2 == pos_t(1, 0)) {
				map[y][x] = SE;
			}

			std::vector<std::vector<int>> imap;
			int insideBlocks = 0;
			int insideLine = 0;
			PIPE_SEGMENT lineStart = GROUND;
			imap.resize(map.size(), std::vector<int>(map.front().size(), 0));
			for (int y = 0; y < map.size(); ++y) {
				int intersections = 0;
				for (int x = 0; x < map.front().size(); ++x) {
					if (std::find(history.begin(), history.end(), pos_t(y, x)) != history.end()) {
						if(map[y][x] == NE || map[y][x] == SE || map[y][x] == NW || map[y][x] == SW || map[y][x] == JOKER) {
							if (!insideLine) {
								insideLine = 1;
								lineStart = map[y][x];
								intersections++;
							} else {
								if (lineStart == NE && map[y][x] == NW || lineStart == SE && map[y][x] == SW) {
									intersections++;
								}
								insideLine = 0;
							}							
						} else if (map[y][x] == VERTICAL) {
							intersections++;
						}					
					} else {
						imap[y][x] = intersections;
						if (intersections % 2 == 1) insideBlocks++;
					}
				}
			}
			std::cout << "Inside blocks: " << insideBlocks << std::endl;
			for (int y = 0; y < map.size(); ++y) {
				for (int x = 0; x < map.front().size(); ++x) {					
					if (std::find(history.begin(), history.end(), pos_t(y, x)) == history.end()) {
						if (imap[y][x] % 2 == 1) {
							printf("%c[%dm%c", 0x1B, 95, input_reverse_translation[map[y][x]]);
						}
						else if (imap[y][x] % 2 == 0) {
							printf("%c[%dm%c", 0x1B, 96, input_reverse_translation[map[y][x]]);
						}
					} else if (maxdistance == pos_t(y, x)) {
						printf("%c[%dm%c", 0x1B, 92, input_reverse_translation[map[y][x]]);
					} else {
						printf("%c[%dm%c", 0x1B, 91, input_reverse_translation[map[y][x]]);
					}					
				}
				puts("");
			}
			std::cout << std::endl;
			return true;
		}
	}
	return false;
}
