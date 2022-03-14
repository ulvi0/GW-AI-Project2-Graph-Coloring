#include <bits/stdc++.h>
#define toDigit(c) (c-'0')
#define MAX_SIZE 1000000
using namespace std;

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

vector<vector<int>> adj_list(MAX_SIZE, vector<int>());
vector<int> nodes;
long long int number_of_colors = 0;
void read_input_file(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	set<int> nodes_set;
	
	string line;
	while (getline(cin, line)){
		if(line[0] == '#')
			continue;

		transform(line.begin(), line.end(), line.begin(), asciitolower);

		if(line.find("color") != string::npos){

			for(char c:line){
				if(isdigit(c)){
					number_of_colors = number_of_colors*10 + toDigit(c);
				}
			}
		}
		else{
			int u = 0, v = 0;
			bool delimiter = false;
			for(char c:line){
				if(c == ','){
					delimiter = true;
				}
				else if (isdigit(c) && !delimiter){
					u = u * 10 + toDigit(c);
				}
				else if(isdigit(c) && delimiter){
					v = v * 10 + toDigit(c);
				}
			}

			adj_list[u].push_back(v);
			adj_list[v].push_back(u);

			nodes_set.insert(v);
			nodes_set.insert(u);
		}
	}
	while(!nodes_set.empty()){
		nodes.push_back(*nodes_set.begin());
		nodes_set.erase(nodes_set.begin());
	}
}



bool arc_reduce(pair<int, int> arc, vector<vector<bool>> &domains,
				 vector<vector<int>> &adj_list, queue<pair<int, int>> &change_tracker,
				 set<pair<int, int>> &openset){
	bool change = false;
	int x, y;
	tie(x, y) = arc;
	for (int valx = 0; valx < domains[x].size(); valx++){
		if (!domains[x][valx])
			continue;

		bool is_there_value = false;
		for(int valy = 0; valy < domains[y].size(); valy++){
			if(domains[y][valy] && valy != valx){
				is_there_value = true;
			}
		}
		if(!is_there_value){
			int domain_size = 0;
			for(int i = 0; i < domains[x].size(); i++) domain_size+=domains[x][i];
			openset.erase({domain_size, x});
			openset.insert({domain_size - 1, x});

			domains[x][valx] = false;
			change_tracker.push({x, valx});
			
			change = true;
		}
	}
	return change;
}
bool AC3(vector<vector<bool>> &domains, vector<vector<int>> &adj_list,
		 set<pair<int, int>> &openset, vector<int> &nodes, queue<pair<int, int>> &change_tracker){
	queue<pair<int, int>> worklist;
	map<pair<int, int>, bool> on_agenda;
  	for (auto x: nodes) 
  	{	
 		for(auto y: adj_list[x]){
 			worklist.push({x, y});
 			on_agenda[{x, y}] = true;
 		}
  	}
  	do {
  		pair<int, int> arc = worklist.front();
  		int x, y;
  		tie(x, y) = arc;
  		worklist.pop();
  		if (arc_reduce(arc, domains, adj_list, change_tracker, openset)){
  			bool is_empty_domain = true;
  			for(int i = 0; i < domains[x].size(); i++){
  				if(domains[x][i]){
  					is_empty_domain = false;
  					break;
  				}
  			}
  			if(is_empty_domain){
  				return false;
  			}
  			else{
  				for(auto y: adj_list[x]){
  					if(!on_agenda[{y, x}]){
  						worklist.push({y, x});
  						on_agenda[{y, x}] = true;
  					}
  					if(!on_agenda[{x, y}]){
  						worklist.push({x, y});
  						on_agenda[{x, y}] = true;
  					}
  				}
  			}
  		}
  	}while(!worklist.empty());
	return true;
}

int cnt = 0;

bool backtrack(set<pair<int, int>> &openset, vector<vector<bool>> &domains,
 vector<vector<int>> &adj_list, vector<int> &colors, vector<int> &nodes){
 	//cout << cnt ++ << endl;
	int v, _;
	if(openset.empty()){
		return true;
	}
	tie(_, v) = *openset.begin();
	openset.erase(openset.begin());

	for(int col = 0; col < domains[v].size(); col++){
		if(domains[v][col]){
			bool can_use_color = true;
			for(auto adj : adj_list[v]){
				if(col == colors[adj]){
					can_use_color = false;
					break;
				}
			}
			if(can_use_color){
				queue<pair<int, int>> change_tracker;

				for(int i = 0; i< domains[v].size(); i++){
					if(i != col){
						domains[v][i] = false;
						change_tracker.push({v, i});
					}
				}
				if(!AC3(domains, adj_list, openset, nodes, change_tracker)){
					while(!change_tracker.empty()){
						int x, y;
						tie(x, y) = change_tracker.front();
						change_tracker.pop();
						domains[x][y] = true;

						int domain_size = 0;
						for(int i = 0; i < domains[x].size(); i++) domain_size+=domains[x][i];
						openset.erase({domain_size, x});
						openset.insert({domain_size + 1, x});
					}
					
					continue;
				}
				colors[v] = col;
				if(backtrack(openset, domains, adj_list, colors, nodes))
					return true;

				while(!change_tracker.empty()){
						int x, y;
						tie(x, y) = change_tracker.front();
						change_tracker.pop();
						domains[x][y] = true;

						int domain_size = 0;
						for(int i = 0; i < domains[x].size(); i++) domain_size+=domains[x][i];
						openset.erase({domain_size, x});
						openset.insert({domain_size + 1, x});
				}
			}	
		}
	}
	colors[v] = -1;
	openset.insert({_, v});
	return false;
}
vector<int> colors(MAX_SIZE);

bool backtrack_start(int number_of_colors, vector<vector<int>> &adj_list, vector<int> &nodes){
	set<pair<int, int>> openset;
	vector<vector<bool>> domains(MAX_SIZE, vector<bool>(number_of_colors, true));
  	for (auto x: nodes) 
  	{
    	colors[x] = -1;
    	openset.insert({number_of_colors, x});
  	}
	return backtrack(openset, domains, adj_list, colors, nodes);
}




int main() {
	freopen("Input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
    read_input_file();
    /*for(int i = 0; i < nodes.size(); i++){
    	for(int j = 0; j < adj_list[i].size();j++){
    		cout << i << " " << adj_list[i][j] << endl;
    	}
    }*/
    if(backtrack_start(number_of_colors, adj_list, nodes)){
  		for (auto x : nodes) 
  		{
    		cout << x << " " << colors[x] << endl;
  		}
    }
    else{
    	cout << "-1" << endl;
    }
}
