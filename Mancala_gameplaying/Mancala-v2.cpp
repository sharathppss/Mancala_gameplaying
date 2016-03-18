#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <random>
	using namespace std;
ofstream o_file1("output.txt");
unsigned long table[25][1000];
int start_player;
int depth, cutoff = 100;
int strategy;
bool trans = true;
float time_r;
using namespace std;

struct firstguess
{
	int value;
	int pos;
};
class board
{
public:
	array<int, 100> b_node;
	int b_size;
	board(array<int, 100> player1, array<int, 100> player2, int p1_size, int p2_size, int player1_m, int player2_m)
	{
		int k = 0;
		for (int i = 0; i < p1_size; i++)	b_node[k++] = player1[i];
		b_node[k++] = player1_m;
		for (int j = p2_size - 1; j >= 0; j--)	b_node[k++] = player2[j];
		b_node[k] = player2_m;
		b_size = k + 1;
	}

	int board_mancala(int player)
	{
		if (player == 1) return (b_size - 2) / 2;
		else return b_size - 1;
	}
	bool terminal_check()
	{
		bool terminal = true;
		int manc;
		manc = (b_size - 2) / 2;
		for (int i = 0; i < manc; i++)
			if (b_node[i] != 0) terminal = false;
		if (terminal == true) return true;
		terminal = true
			;
		manc = b_size - 1;
		for (int i = (manc - 1); i >(b_size - 2) / 2; i--)
			if (b_node[i] != 0) terminal = false;
		return terminal;
	}
	void end_game(int player)
	{
		int manc;
		manc = (b_size - 2) / 2;
		for (int i = 0; i < manc; i++)
		{
			b_node[manc] = b_node[manc] + b_node[i];
			b_node[i] = 0;
		}
		manc = b_size - 1;
		for (int i = (manc - 1); i >(b_size - 2) / 2; i--)
		{
			b_node[manc] = b_node[manc] + b_node[i];
			b_node[i] = 0;
		}
	}
	int board_position(int player, int pos)
	{
		int abs_pos;
		if (player == 1)	return abs_pos = pos - 2;
		else return abs_pos = b_size - pos;
	}
	int board_value(int player, int pos)
	{
		int abs_pos;
		if (player == 1)
		{
			abs_pos = pos - 2;
		}
		else
		{
			abs_pos = b_size - pos;
		}
		return b_node[abs_pos];
	}
	bool board_update(int player, int pos)
	{
		int abs_pos, i, manc;
		if (player == 1)
		{
			manc = (b_size - 2) / 2;
			abs_pos = pos - 2;
		}
		else
		{
			manc = b_size - 1;
			abs_pos = b_size - pos;
		}
		int stones = b_node[abs_pos];
		if (stones == 0) return 0;
		int sto = stones / (b_size - 1);
		int sto_size = sto*(b_size - 1);
		int smod = stones % (b_size - 1);
		b_node[abs_pos] = 0;
		for (i = abs_pos + 1;; i = (++i % (b_size)))
		{
			if (player == 0 && i == (b_size - 2) / 2) {}
			else if (player == 1 && i == b_size - 1) {}
			else
			{
				b_node[i] += sto;
				sto_size = (sto_size - sto);
				if (sto_size == 0) break;

			}
			if (sto_size == 0) break;
		}
		if (smod != 0)
		{
			for (i = abs_pos + 1;; i = (++i % (b_size)))
			{
				if (player == 0 && i == (b_size - 2) / 2) {}
				else if (player == 1 && i == b_size - 1) {}
				else
				{
					b_node[i] += 1;
					smod--;
					if (smod == 0) break;

				}
				if (smod == 0) break;
			}
		}
		i = i%b_size;
		if (b_node[i] == 1 && player == 1 && manc>i)
		{
			b_node[manc] += b_node[i];
			b_node[manc] += b_node[b_size - (i + 2)];
			b_node[b_size - (i + 2)] = 0;
			b_node[i] = 0;
		}
		else if (b_node[i] == 1 && player == 0 && (manc>i&&i>(b_size - 2) / 2))
		{
			b_node[manc] += b_node[i];
			b_node[manc] += b_node[b_size - (i + 2)];
			b_node[b_size - (i + 2)] = 0;
			b_node[i] = 0;
		}

		if (player == 0 && i == (b_size - 1)) return true;
		else if (player == 1 && i == (b_size - 2) / 2) return true;
		else return false;
	}
	void board_print()
	{
		ofstream next_State("next_state.txt");
		for (int i = b_size - 2; i > (b_size - 2) / 2; i--) next_State << b_node[i] << " ";
		next_State << endl;
		for (int i = 0; i < (b_size - 2) / 2; i++) next_State << b_node[i] << " ";
		next_State << endl;
		next_State << b_node[b_size - 1] << endl;
		next_State << b_node[(b_size - 2) / 2] << endl;
	}
};

struct hash_comp
{
	board b;
	int depth;
	int value;
};
unordered_map<unsigned long, hash_comp> hash_c;

void init_zobrist(int b_size)
{
	std::mt19937_64 rng;
	std::uniform_int_distribution<long long int> udist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
	std::random_device rd;
	std::mt19937::result_type const seedval = rd();
	rng.seed(seedval);
	for (int i = 0; i <(2 * b_size); i++)
		for (int j = 0; j <= 1000; j++)
		{
			table[i][j] = udist(rng);
			//o_file<<i << "\t" << j << "\t" << table[i][j] << endl;
		}
}
unsigned long hash_func(board b)
{
	unsigned long h = 0;
	for (int i = 0; i < (b.b_size - 1); i++)
	{
		if (i == (b.b_size - 2) / 2) {}
		else
		{
			h ^= table[i][b.b_node[i] % 1000];
		}
	}
	return h;
}

void hash_insert(board b, int depth, int value)
{
	hash_comp hc = { b,depth,value };
	hash_c.insert(make_pair(hash_func(b), hc));
}
int hash_extract(board b, int depth)
{
	unordered_map<unsigned long, hash_comp>::const_iterator hc1 = hash_c.find(hash_func(b));
	if (hc1 == hash_c.end()) return 0;
	else if (hc1->second.depth >= depth)
	{
		return hc1->second.value;
	}
	else return 0;
}
int max_value_purn(board b, int d, int player, int pos, int alpha, int beta);
int min_value_purn(board b, int d, int player, int pos, int alpha, int beta);

int evaluation(board b)
{
	if (start_player == 1) return b.b_node[(b.b_size - 2) / 2] - b.b_node[(b.b_size - 1)];
	else return b.b_node[(b.b_size - 1)] - b.b_node[(b.b_size - 2) / 2];
}

int max_value_purn(board b, int d, int player, int pos, int alpha, int beta)
{
	int v = -30000;
	bool turn = b.board_update(player, pos);
	if (trans == true)
	{
		int t = hash_extract(b, depth);
		if (t != 0) return t;
	}
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
	}
	if (turn)
	{
		int v = 30000;
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			if (trans == true) hash_insert(b, depth, v);
			return v;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value(player, i) == 0) {}
			else
			{
				v = min(v, max_value_purn(b, d, player, i, alpha, beta));
				if (alpha >= v)
				{
					return v;
				}
				beta = min(beta, v);
			}
		}
		if (trans == true) hash_insert(b, depth, v);
		return v;
	}
	else if (d != depth)
	{
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			hash_insert(b, depth, v);
			return v;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value((player + 1) % 2, i) == 0) {}
			else
			{
				v = max(v, min_value_purn(b, d + 1, (player + 1) % 2, i, alpha, beta));
				if (v >= beta)
				{
					hash_insert(b, depth, v);
					return v;
				}
				alpha = max(alpha, v);
			}
		}
	}
	else
	{
		v = evaluation(b);
	}
	if (trans == true) hash_insert(b, depth, v);
	return v;
}
int min_value_purn(board b, int d, int player, int pos, int alpha, int beta)
{
	int v = 30000;
	bool turn = b.board_update(player, pos);
	if (trans == true)
	{
		int t = hash_extract(b, depth);
		if (t != 0) return t;
	}
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
	}
	if (turn)
	{
		int v = -30000;
		int temp, m = -1;
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			if (trans == true) hash_insert(b, depth, v);
			return v;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value(player, i) == 0) {}
			else
			{
				temp = min_value_purn(b, d, player, i, alpha, beta);
				if (temp > v)
				{
					m = i;
					v = temp;
				}
				if (v >= beta)
				{
					if (trans == true) hash_insert(b, depth, v);
					return v;
				}
				alpha = max(alpha, v);
			}
		}
		if (trans == true) hash_insert(b, depth, v);
		return v;
	}
	else if (d != depth)
	{
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			if (trans == true) hash_insert(b, depth, v);
			return v;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value((player + 1) % 2, i) == 0) {}
			else
			{
				v = min(v, max_value_purn(b, d + 1, (player + 1) % 2, i, alpha, beta));
				if (alpha >= v)
				{
					if (trans == true) hash_insert(b, depth, v);
					return v;
				}
				beta = min(beta, v);
			}
		}
	}
	else
	{
		v = evaluation(b);
	}
	if (trans == true) hash_insert(b, depth, v);
	return v;
}
firstguess mini_max_purn(board b, int player, int alpha, int beta)
{
	int d = 0;
	firstguess v{ 0,0 };
	int temp, max_child = 1;
	if (b.terminal_check())
	{
		b.end_game(player);
		v.value = evaluation(b);
		return v;
	}
	for (int i = 2; i <= b.b_size / 2; i++)
	{
		if (b.board_value(player, i) == 0) {}
		else
		{
			temp = min_value_purn(b, d + 1, player, i, alpha, beta);
			if (temp > alpha)
			{
				alpha = temp;
				max_child = i;
			}
		}

	}
	v.value = alpha;
	v.pos = max_child;
	return v;
}
void depther(int b_size)
{
	if (time_r<5)
	{
		trans = false;
		strategy = 0;
		depth = 2;
	}
	else if (time_r<20)
	{
		init_zobrist(b_size);
		strategy = 0;
		depth = 4;
	}
	else if (time_r<50)
	{
		init_zobrist(b_size);
		strategy = 1;
		cutoff = 5;
	}
	else if (time_r<100 & time_r>50)
	{
		init_zobrist(b_size);
		strategy = 1;
		cutoff = 5;
	}
	else
	{
		init_zobrist(b_size);
		strategy = 2;
		if (b_size <= 3) depth = 11;
		else if (b_size <= 5) depth = 8;
		else depth = 7;
	}
}


firstguess itr_deepening(board b, int player)
{
	firstguess fg = { 0,0 };
	firstguess bg = fg;
	int b_count = 1;
	if (strategy == 0)
		fg = mini_max_purn(b, player, -30000, +30000);
	else if (strategy == 1)
	{
		for (int d = 1; d <= 6; d++)
		{
			depth = d;
			bg = mini_max_purn(b, player, -30000, +30000);
			if (bg.pos == fg.pos) b_count++;
			else b_count = 1;
			if (bg.value != fg.value) fg = bg;
			if (b_count == cutoff) break;
		}
	}
	else
	{
		fg = mini_max_purn(b, player, -30000, +30000);
	}
	return fg;
}

int main(int argc, char* argv[])
{
	int algo;
	array<int, 100> p1, p2;
	string cur_line;
	ifstream i_file;
	i_file.open("sample.txt");
	if (i_file.is_open())
	{
		getline(i_file, cur_line);
		algo = atoi(cur_line.c_str());
		getline(i_file, cur_line);
		start_player = atoi(cur_line.c_str());
		start_player = start_player % 2;
		getline(i_file, cur_line);
		time_r = atof(cur_line.c_str());
		getline(i_file, cur_line);
		istringstream s(cur_line);
		int p2_size = 0;
		string x;
		while (!s.eof())
		{
			getline(s, x, ' ');
			p2[p2_size++] = atoi(x.c_str());
		}
		getline(i_file, cur_line);
		istringstream ss(cur_line);
		int p1_size = 0;
		while (!ss.eof())
		{
			getline(ss, x, ' ');
			p1[p1_size++] = atoi(x.c_str());
		}
		getline(i_file, cur_line);
		int man_2 = atoi(cur_line.c_str());
		firstguess m;
		getline(i_file, cur_line);
		int man_1 = atoi(cur_line.c_str());
		depther(p1_size);
		board b(p1, p2, p1_size, p2_size, man_1, man_2);
		m = itr_deepening(b, start_player);
		if (start_player == 1) o_file1 << "B" << m.pos << endl;
		else o_file1 << "A" << m.pos << endl;
		bool twist = b.board_update(start_player, m.pos);
		while (twist)
		{
			if ((b.terminal_check()))
			{
				b.end_game(start_player);
				break;
			}
			m = itr_deepening(b, start_player);
			if (start_player == 1) o_file1 << "B" << m.pos << endl;
			else o_file1 << "A" << m.pos << endl;
			twist = b.board_update(start_player, m.pos);
		}
	}
}