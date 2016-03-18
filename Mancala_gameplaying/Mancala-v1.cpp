#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
ofstream o_file("traverse_log.txt");
int depth, start_player;
using namespace std;
struct ele
{
	int cur;
	int next;
	int level;
	vector<int> parents;
};
vector<ele> turns;
class board
{
public:
	array<int, 22> b_node;
	int b_size;
	board(array<int, 10> player1, array<int, 10> player2, int p1_size, int p2_size, int player1_m, int player2_m)
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
		terminal = true;
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
		b_node[abs_pos] = 0;
		for (i = abs_pos + 1;; i = (++i % (b_size)))
		{
			if (player == 0 && i == (b_size - 2) / 2) {}
			else if (player == 1 && i == b_size - 1) {}
			else
			{
				b_node[i] += 1;
				stones--;
				if (stones == 0) break;

			}
			if (stones == 0) break;
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
int max_value(board, int, int, int);
int min_value(board, int, int, int, bool, int level, vector<int>);
int max_value_purn(board b, int d, int player, int pos, int alpha, int beta);
int min_value_purn(board b, int d, int player, int pos, int alpha, int beta, bool state, int level, vector<int>);

void print_minimax(int player, int pos, int depth, int value)
{
	string node;
	if (pos == 0) node = "root";
	else if (player == 1) node = "B" + to_string(pos);
	else node = "A" + to_string(pos);
	if (value == 9999)
		o_file << node << "," << depth << "," << "Infinity" << endl;
	else if (value == -9999) o_file << node << "," << depth << "," << "-Infinity" << endl;
	else o_file << node << "," << depth << "," << value << endl;
}

void print_minimax(int player, int pos, int depth, int value, int alpha_1, int beta_1)
{
	string node;
	string v, a, b;
	if (pos == 0) node = "root";
	else if (player == 1) node = "B" + to_string(pos);
	else node = "A" + to_string(pos);
	if (value == 9999) v = "Infinity";
	else if (value == -9999) v = "-Infinity";
	else v = to_string(value);
	if (alpha_1 == 9999) a = "Infinity";
	else if (alpha_1 == -9999) a = "-Infinity";
	else a = to_string(alpha_1);
	if (beta_1 == 9999) b = "Infinity";
	else if (beta_1 == -9999) b = "-Infinity";
	else b = to_string(beta_1);
	o_file << node << "," << depth << "," << v << "," << a << "," << b << endl;

}
int evaluation(board b)
{
	if (start_player == 1) return b.b_node[(b.b_size - 2) / 2] - b.b_node[(b.b_size - 1)];
	else return b.b_node[(b.b_size - 1)] - b.b_node[(b.b_size - 2) / 2];
}

int max_value(board b, int d, int player, int pos)
{
	int alpha = -9999;
	bool turn = b.board_update(player, pos);
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
		print_minimax(player, pos, d, evaluation(b));
	}
	if (turn)
	{
		int beta = 9999;
		print_minimax(player, pos, d, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			beta = evaluation(b);
			print_minimax(player, pos, d, beta);
			return beta;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value(player, i) == 0) {}
			else
			{
				beta = min(beta, max_value(b, d, player, i));
				print_minimax(player, pos, d, beta);
			}
		}
		return beta;
	}
	else if (d != depth)
	{
		print_minimax(player, pos, d, alpha);
		if (b.terminal_check())
		{
			b.end_game(player);
			alpha = evaluation(b);
			print_minimax(player, pos, d, alpha);
			return alpha;
		}
		vector<int> a(1);
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value((player + 1) % 2, i) == 0) {}
			else
			{
				alpha = max(alpha, min_value(b, d + 1, (player + 1) % 2, i, false, 0, a));
				print_minimax(player, pos, d, alpha);
			}
		}
	}
	else alpha = evaluation(b);
	return alpha;
}
int min_value(board b, int d, int player, int pos, bool state, int level, vector<int> par)
{
	int alpha = 9999;
	bool turn = b.board_update(player, pos);
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
		print_minimax(player, pos, d, evaluation(b));
	}
	if (turn)
	{
		int beta = -9999;
		int temp, m = -1;
		print_minimax(player, pos, d, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			beta = evaluation(b);
			print_minimax(player, pos, d, beta);
			return beta;
		}
		par.insert(par.end(), pos);
		for (int i = 2; i <= ((b.b_size) / 2); i++)
		{
			if (b.board_value(player, i) == 0) {}
			else
			{
				temp = min_value(b, d, player, i, state, (level + 1), par);
				if (temp > beta)
				{
					m = i;
					beta = temp;
				}
				print_minimax(player, pos, d, beta);
			}
		}
		if (state == true && m != -1)
		{
			struct ele node;
			node.cur = pos;
			node.level = level;
			node.next = m;
			node.parents = par;
			turns.insert(turns.end(), node);
		}

		return beta;
	}
	else if (d != depth)
	{
		state = false;
		level = 0;
		print_minimax(player, pos, d, alpha);
		if (b.terminal_check())
		{
			b.end_game(player);
			alpha = evaluation(b);
			print_minimax(player, pos, d, alpha);
			return alpha;
		}
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value((player + 1) % 2, i) == 0) {}
			else
			{
				alpha = min(alpha, max_value(b, d + 1, (player + 1) % 2, i));
				print_minimax(player, pos, d, alpha);
			}
		}
	}
	else alpha = evaluation(b);
	return alpha;
}
int mini_max(board b, int player)
{
	int d = 0;
	int alpha = -9999;
	int temp, max_child = 1;
	vector<int> a(0);
	print_minimax(player, 0, 0, alpha);
	if (b.terminal_check())
	{
		b.end_game(player);
		alpha = evaluation(b);
		print_minimax(player, 0, d, alpha);
		return alpha;
	}
	for (int i = 2; i <= b.b_size / 2; i++)
	{
		if (b.board_value(player, i) == 0) {}
		else
		{
			temp = min_value(b, d + 1, player, i, true, 1, a);
			if (temp > alpha)
			{
				alpha = temp;
				max_child = i;
			}
			print_minimax(player, 0, d, alpha);
		}
	}
	struct ele node;
	node.cur = 1;
	node.level = 0;
	node.next = max_child;
	node.parents = a;
	turns.insert(turns.end(), node);
	return max_child;
}

int max_value_purn(board b, int d, int player, int pos, int alpha, int beta)
{
	int v = -9999;
	bool turn = b.board_update(player, pos);
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
		print_minimax(player, pos, d, evaluation(b), alpha, beta);
	}
	if (turn)
	{
		int v = 9999;
		print_minimax(player, pos, d, v, alpha, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			print_minimax(player, pos, d, v, alpha, beta);
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
					print_minimax(player, pos, d, v, alpha, beta);
					return v;
				}
				beta = min(beta, v);
				print_minimax(player, pos, d, v, alpha, beta);
			}
		}
		return v;
	}
	else if (d != depth)
	{
		print_minimax(player, pos, d, v, alpha, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			print_minimax(player, pos, d, v, alpha, beta);
			return v;
		}
		vector<int> a(1);
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value((player + 1) % 2, i) == 0) {}
			else
			{
				v = max(v, min_value_purn(b, d + 1, (player + 1) % 2, i, alpha, beta, false, 0, a));
				if (v >= beta)
				{
					print_minimax(player, pos, d, v, alpha, beta);
					return v;
				}
				alpha = max(alpha, v);
				print_minimax(player, pos, d, v, alpha, beta);
			}
		}
	}
	else
	{
		v = evaluation(b);
	}
	return v;
}
int min_value_purn(board b, int d, int player, int pos, int alpha, int beta, bool state, int level, vector<int> par)
{
	int v = 9999;
	bool turn = b.board_update(player, pos);
	if (!turn && (d == depth))
	{
		if (b.terminal_check()) b.end_game(player);
		print_minimax(player, pos, d, evaluation(b), alpha, beta);
	}
	if (turn)
	{
		int v = -9999;
		int temp, m = -1;
		print_minimax(player, pos, d, v, alpha, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			print_minimax(player, pos, d, v, alpha, beta);
			return v;
		}
		par.insert(par.end(), pos);
		for (int i = 2; i <= b.b_size / 2; i++)
		{
			if (b.board_value(player, i) == 0) {}
			else
			{
				temp = min_value_purn(b, d, player, i, alpha, beta, state, (level + 1), par);
				if (temp > v)
				{
					m = i;
					v = temp;
				}
				if (v >= beta)
				{
					print_minimax(player, pos, d, v, alpha, beta);
					if (state == true)
					{
						struct ele node;
						node.cur = pos;
						node.level = level;
						node.next = m;
						node.parents = par;
						turns.insert(turns.end(), node);
					}
					return v;
				}
				alpha = max(alpha, v);
				print_minimax(player, pos, d, v, alpha, beta);
			}
		}
		if (state == true)
		{
			struct ele node;
			node.cur = pos;
			node.level = level;
			node.next = m;
			node.parents = par;
			turns.insert(turns.end(), node);
		}
		return v;
	}
	else if (d != depth)
	{
		state = false;
		print_minimax(player, pos, d, v, alpha, beta);
		if (b.terminal_check())
		{
			b.end_game(player);
			v = evaluation(b);
			print_minimax(player, pos, d, v, alpha, beta);
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
					print_minimax(player, pos, d, v, alpha, beta);
					return v;
				}
				beta = min(beta, v);
				print_minimax(player, pos, d, v, alpha, beta);

			}
		}
	}
	else
	{
		v = evaluation(b);
	}
	return v;
}
int mini_max_purn(board b, int player)
{
	int d = 0;
	int alpha = -9999;
	int beta = 9999;
	int v;
	int temp, max_child = 1;
	vector<int> par;
	par.insert(par.end(), 0);
	print_minimax(player, 0, 0, alpha, alpha, beta);
	if (b.terminal_check())
	{
		b.end_game(player);
		v = evaluation(b);
		print_minimax(player, 0, d, v, alpha, beta);
		return v;
	}
	for (int i = 2; i <= b.b_size / 2; i++)
	{
		if (b.board_value(player, i) == 0) {}
		else
		{
			temp = min_value_purn(b, d + 1, player, i, alpha, beta, true, 1, par);
			if (temp > alpha)
			{
				alpha = temp;
				max_child = i;
			}
			print_minimax(player, 0, d, alpha, alpha, beta);
		}
	}
	struct ele node;
	node.cur = 1;
	node.level = 0;
	node.next = max_child;
	node.parents = par;
	turns.insert(turns.end(), node);
	return max_child;
}

bool parent_check(vector<ele>::iterator cur_node, vector<ele>::iterator it_node)
{
	vector<int> parent = cur_node->parents;
	vector<int> child = it_node->parents;
	for (int i = 0; i < parent.size(); i++) if (parent[i] != child[i]) return false;
	return true;
}

vector<ele>::iterator find_next(vector<ele>::iterator cur_node)
{
	for (vector<ele>::iterator i = turns.begin(); i <turns.end(); i++)
	{
		if (((i->cur == cur_node->next) && (i->level == cur_node->level + 1)) && parent_check(cur_node, i))
		{
			return i;
		}
	}
	return turns.end();
}

int main(int argc, char* argv[])
{
	int algo;
	array<int, 10> p1, p2;
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
		depth = atoi(cur_line.c_str());
		if (algo == 1) depth = 1;
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
		getline(i_file, cur_line);
		int man_1 = atoi(cur_line.c_str());
		board b(p1, p2, p1_size, p2_size, man_1, man_2);
		int move;
		if (algo == 2)
		{
			o_file << "Node" << "," << "Depth" << "," << "Value" << endl;
			move = mini_max(b, start_player);
		}

		else if (algo == 3)
		{
			o_file << "Node" << "," << "Depth" << "," << "Value" << "," << "Alpha" << "," << "Beta" << endl;
			move = mini_max_purn(b, start_player);
		}
		else
		{
			o_file << "Node" << "," << "Depth" << "," << "Value" << endl;
			move = mini_max(b, start_player);
		}
		std::vector<ele>::iterator cur_node = turns.end() - 1;
		cout << "turns size " << turns.size() << endl;
		while (5)
		{
			b.board_update(start_player, cur_node->next);
			cur_node = find_next(cur_node);
			if (cur_node == turns.end()) break;
		}
		if ((b.terminal_check()))
		{
			b.end_game(start_player);
		}
		b.board_print();
	}
}