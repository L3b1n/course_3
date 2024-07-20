#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <chrono>
#include <random>
#include <bitset>
#include <cmath>
#include <tuple>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>

using namespace std;
typedef long long ll;

struct Node
{
    long long num = 0ll;
    long long min = 0ll;
};

class Segment_tree
{
public:
    Segment_tree(int _size) : size(4 * _size + 5), n(_size) { tree.resize(size);} 

    void Add(int left, int right, long long data) { Add(left, right, data, 0, 0, n - 1);}

    long long FindMin(int left, int right) { return FindMin(left, right, 0, 0, n - 1);}

    void Print() { Print_const();}
private:
    int n;
    int size;
    std::vector<Node> tree;

    void Print_const()
    {
        for(auto x : tree){ std::cout << x.min << " ";} std::cout << "\n";
    }

    void Add(int left, int right, long long data, int v, int temp_left, int temp_right)
    {
        if(left > right){ return;}
        if(left == temp_left && temp_right == right){ tree[v].num += data; tree[v].min += data; return;}
        int medium = (temp_left + temp_right) >> 1;
        Add(left, std::min(right, medium), data, 2 * v + 1, temp_left, medium);
        Add(std::max(medium + 1, left), right, data, 2 * (v + 1), medium + 1, temp_right);
        tree[v].min = tree[v].num + std::min(tree[2 * v + 1].min, tree[2 * (v + 1)].min);
    }
    
    long long FindMin(int left, int right, int v, int temp_left, int temp_right)
    {
        if(left > right){ return LLONG_MAX;}
        if(left == temp_left && temp_right == right){ return tree[v].min;}
        int medium = (temp_left + temp_right) >> 1;
        return tree[v].num + 
               std::min(FindMin(left, std::min(medium, right), 2 * v + 1, temp_left, medium),
                        FindMin(std::max(medium + 1, left), right, 2 * (v + 1), medium + 1, temp_right));
    }
};


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	map<ll, ll> months = { {1, 0}, {2, 31}, {3, 59}, {4, 90}, {5, 120}, {6, 151}, {7, 181}, {8, 212}, {9, 243}, {10, 273}, {11, 304}, {12, 334} };
	map<ll, ll> mapa;
	ll debt = 0;
	ll n;
	cin >> n;
	vector<tuple<ll, ll, ll>> data(n), start(n);
	for (ll i = 0; i < n; ++i) {
		string money, day, time;
		cin >> money >> day >> time;
		ll days = months[stoll(day.substr(3, 2))] + stoll(day.substr(0, 2));
		ll minutes = stoll(time.substr(0, 2)) * 60 + stoll(time.substr(3, 2));
		minutes += days * 1440;
		if (money[0] == '+') {
			money.erase(0, 1);
		}
		ll val = stoll(money);
		data[i] = make_tuple(minutes, val, i);
	}
	start = data;
	sort(data.begin(), data.end());
    Segment_tree tree(n);
	for (int i = 0; i < n; i++) {
		int pos = lower_bound(data.begin(), data.end(), start[i]) - data.begin();
        std::cout << pos << '\n';
        tree.Add(pos, n - 1, get<1>(start[i]));
		cout << min(0ll, tree.FindMin(0, n - 1)) << endl;
	}
}