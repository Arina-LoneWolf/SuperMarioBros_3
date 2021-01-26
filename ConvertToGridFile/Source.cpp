#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define map_width	210
#define map_height	27

#define cell_width	8
#define cell_height	6

#define mario_width		15
#define mario_height	27

#define goomba_width	16
#define goomba_height	15

#define para_goomba_width	20
#define para_goomba_height	24

#define koopa_width		15
#define koopa_height	26

#define piranha_width	16
#define piranha_height	24

#define trap_width		16
#define trap_height		32

#define green_trap_width		16
#define green_trap_height		24


#define brother_width	16
#define brother_height	24

#define brick_width		16
#define brick_height	16

#define pipe_width		32
#define pipe_height		48
#define short_pipe_height		32

#define lift_width		48
#define lift_height		16

#define SCENE_SECTION_UNKNOWN				-1

#define NO_CHOICE		100
#define PIPE_CHOICE		101
#define PIRANHA_CHOICE	103
#define SPECIAL			102

#define MAX_SCENE_LINE 1024

using namespace std;

static map<int, pair<int, int>> objSize{
	{1, make_pair(16, 16)},
	{2, make_pair(16, 15)},
	{3, make_pair(16, 15)},
	{7, make_pair(16, 24)},
	{11, make_pair(16, 16)},
	{16, make_pair(16, 16)},
	{19, make_pair(18, 28)},
	{20, make_pair(18, 28)},
	{21, make_pair(18, 28)},
	{27, make_pair(18, 28)},
	{25, make_pair(48, 16)},
	{23, make_pair(16, 24)},
	{22, make_pair(16, 16)}
};

vector<string> split(string line, string delimeter = "\t");
vector<string> split(string line, string delimeter);

void _Parsection_NO_CHOICE(string line, ofstream& ofs);
void _Parsection_PIPE_CHOICE(string line, ofstream& ofs);
void _Parsection_PIRANHA_CHOICE(string line, ofstream& ofs);
void _Parsection_SPECIAL(string line, ofstream& ofs);

wstring ToWSTR(string st);
void CalculateBBox(float x, float y, float w, float h, int& t, int& b, int& l, int& r)
{
	t = int(y / 180);
	b = ceil((y + h) / 180);
	l = int(x / 150);
	r = ceil((x + w) / 150);

}
//ground
void Calculate(float x, float y, float w, float h, int& t, int& b, int& l, int& r)
{
	t = int(y / 180);
	b = ceil((y + h * 16) / 180);
	l = int(x / 150);
	r = ceil((x + w * 16) / 150);
}

int main()
{
	ifstream f;
	f.open("in.txt");
	ofstream ofs;
	ofs.open("out.txt");
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line[0] == '*') ofs << endl;

		if (line == "[NO_CHOICE]") { section = NO_CHOICE; continue; }
		if (line == "[PIPE_CHOICE]") { section = PIPE_CHOICE; continue; }
		if (line == "[PIRANHA_CHOICE]") { section = PIRANHA_CHOICE; continue; }
		if (line == "[SPECIAL]") { section = SPECIAL; continue; }

		//
		// data section
		//
		switch (section)
		{
		case NO_CHOICE: _Parsection_NO_CHOICE(line, ofs); break;
		case PIPE_CHOICE: _Parsection_PIPE_CHOICE(line, ofs); break;
		case PIRANHA_CHOICE: _Parsection_PIRANHA_CHOICE(line, ofs); break;
		case SPECIAL: _Parsection_SPECIAL(line, ofs); break;
		}
	}

	f.close();
	ofs.close();
	system("pause");
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/

void _Parsection_NO_CHOICE(string line, ofstream& ofs)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = strtof(tokens[1].c_str(), NULL);
	float y = strtof(tokens[2].c_str(), NULL);
	int ani_set_id = atoi(tokens[3].c_str());
	int t, b, l, r;
	CalculateBBox(x, y, objSize.at(object_type).first, objSize.at(object_type).second, t, b, l, r);
	ofs << object_type << "\t" << x << "\t" << y << "\t" << ani_set_id << "\t" << t << "\t" << b << "\t" << l << "\t" << r << endl;
}

void _Parsection_PIPE_CHOICE(string line, ofstream& ofs)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = strtof(tokens[1].c_str(), NULL);
	float y = strtof(tokens[2].c_str(), NULL);
	int ani_set_id = atoi(tokens[3].c_str());
	int type = atoi(tokens[4].c_str());
	int t, b, l, r;
	if (type == 1 || type == 5)
		CalculateBBox(x, y, pipe_width, 32, t, b, l, r);
	else if (type == 2 || type == 6)
		CalculateBBox(x, y, pipe_width, 48, t, b, l, r);
	else if (type == 3)
		CalculateBBox(x, y, pipe_width, 64, t, b, l, r);
	else if (type == 4)
		CalculateBBox(x, y, pipe_width, 208, t, b, l, r);
	ofs << object_type << "\t" << x << "\t" << y << "\t" << ani_set_id << "\t" << t << "\t" << b << "\t" << l << "\t" << r << "\t" << type << endl;
}

void _Parsection_PIRANHA_CHOICE(string line, ofstream& ofs)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = strtof(tokens[1].c_str(), NULL);
	float y = strtof(tokens[2].c_str(), NULL);
	int ani_set_id = atoi(tokens[3].c_str());
	int type = atoi(tokens[4].c_str());
	int t, b, l, r;
	if (type == 1)
		CalculateBBox(x, y, 16, 32, t, b, l, r);
	else
		CalculateBBox(x, y, 16, 24, t, b, l, r);

	ofs << object_type << "\t" << x << "\t" << y << "\t" << ani_set_id << "\t" << t << "\t" << b << "\t" << l << "\t" << r << "\t" << type << endl;
}

void _Parsection_SPECIAL(string line, ofstream& ofs)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = strtof(tokens[1].c_str(), NULL);
	float y = strtof(tokens[2].c_str(), NULL);
	int ani_set_id = atoi(tokens[3].c_str());
	int w = atoi(tokens[4].c_str());
	int h = atoi(tokens[5].c_str());
	int t, b, l, r;
	Calculate(x, y, w, h, t, b, l, r);
	ofs << object_type << "\t" << x << "\t" << y << "\t" << ani_set_id << "\t" << t << "\t" << b << "\t" << l << "\t" << r << "\t" << w << "\t" << h << endl;
}


