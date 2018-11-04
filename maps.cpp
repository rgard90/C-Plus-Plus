#include <set>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>
using namespace std;
int main(int argc, char *argv[])
{
 
    vector<string> tokens;
    string next_line;  // Each data line
    ifstream in(argv[1]);
    while (getline(in, next_line)) {
        istringstream iss(next_line);
        string token;
        while (iss >> token) {
            string nopunct = "";
            for(auto &c : token) { // Remove Punctuation      
                if (isalpha(c)) {
                    nopunct +=c;       
                }
            }
	        tokens.push_back(nopunct);
	    }
    }
    cout << "Number of words "<<tokens.size()<<endl;
  
    // maps
    map <deque <string>, vector <string>> wordmap;
    deque <string> last;
    last.push_back("");
    last.push_back("");
    for(auto s:tokens) {
        wordmap[last].push_back(s);
        last.push_back(s);
    }

    
    //Now generate text
    deque <string> current;
    current.push_back("");
    current.push_back("");
    
    
    for(int i = 0; i < 100; i++) {
        int ind = rand() % wordmap[current].size();
        cout << wordmap[current][ind]<<" ";
        current.push_back(wordmap[current][ind]);
        current.pop_back();
    }
    cout << endl;
    
}
