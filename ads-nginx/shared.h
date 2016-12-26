#include <vector>
#include <algorithm>
#include "string"
using namespace std;

struct Ad {
	vector<int> Geos;
	vector<int> Behaves;
	vector<int> Demos;
	vector<int> Placements;
	string Uri;
	int Units;
};

extern vector<Ad> Ads;