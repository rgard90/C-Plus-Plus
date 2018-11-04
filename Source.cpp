#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<deque>
#include<vector>
#include<set>
#include<unordered_map>
#include<map>
#include<random>
#include <ctime>

using namespace std;

string getrequest() {
	string request;
	cout << "\n Please make a request: ((f)ile, (t)ext, (g)enerate, (c)lear or (s)top) ";
	getline(cin, request);
	return request;
}

void getfromfile(string &training) {
	string name, line;
	ifstream fi;

	cout << "What file should I read? ";
	getline(cin, name);
	if ('"' == name[0]) {
		name = name.substr(1, name.size() - 2);
	}
	fi.open(name);

	if (fi.is_open()) {
		while (getline(fi, line)) {
			training += line + " ";
		}
	}
	else {
		cout << "I could not open the file: " << name << endl;
	}
}

void getfromcin(string &training) {
	string line;
	bool cont = true;

	while (cont) {
		getline(cin, line);
		if ("please stop" == line) {
			cont = false;
		}
		else {
			training += line + " ";
		}
	}
}

string cleantext(string &text) {
	string clean;
	clean.reserve(text.size());

	for (char c : text) {
		if (c > -1 && c < 255) {
			if (isalnum(c) || isspace(c)) {
				clean += c;
			}
			else if (ispunct(c)) {
				clean += string(" ") + c + " ";
			}
		}
	}
	return clean;
}


string gentext(string &training, int genlength) {
	string token;
	clock_t start;
	double elapsed;

	cout << "cleaning...";
	start = clock();
	string clean = cleantext(training);
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	istringstream iss(clean);

	cout << "make vector...";
	start = clock();
	// this next line blows my mind!! you can initialize a vector directly from a stream! May not be fast.
	//vector<string> tokens { istream_iterator<string>{iss}, istream_iterator<string>{} };
	// This works too:
	//vector<string> tokens = { istream_iterator<string>(iss), istream_iterator<string>() };
	vector<string> tokens;
	tokens.reserve(100000);
	while (iss >> token) {
		tokens.push_back(token);
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	unordered_map<string, vector<string>> model;

	deque<string> initialcontext = { "" };
	deque<string> context = initialcontext;

	cout << "make model...";
	start = clock();
	for (auto s : tokens) {
		ostringstream contextstream;
		for (auto x : context) {
			contextstream << x << " ";
		}
		model[contextstream.str()].push_back(s);
		context.push_back(s);
		context.pop_front();
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	cout << "make text...";
	start = clock();
	context = initialcontext;
	ostringstream oss;
	string next;

	for (auto i = 0; i < genlength; i++) {
		ostringstream contextstream;
		for (auto x : context) {
			contextstream << x << " ";
		}
		next = model[contextstream.str()][rand() % model[contextstream.str()].size()];
		context.push_back(next);
		context.pop_front();
		oss << next << " ";
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";


	return oss.str();
}

string gentext_original(string &training, int genlength) {
	string token;
	clock_t start;
	double elapsed;

	cout << "cleaning...";
	start = clock();
	string clean = cleantext(training);
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	istringstream iss(clean);

	cout << "make vector...";
	start = clock();
	// this next line blows my mind!! you can initialize a vector directly from a stream! May not be fast.
	//vector<string> tokens { istream_iterator<string>{iss}, istream_iterator<string>{} };
	// This works too:
	//vector<string> tokens = { istream_iterator<string>(iss), istream_iterator<string>() };
	vector<string> tokens;
	tokens.reserve(100000);
	while (iss >> token) {
		tokens.push_back(token);
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	map<deque<string>, vector<string>> model;

	deque<string> initialcontext = { "" };
	deque<string> context = initialcontext;

	cout << "make model...";
	start = clock();
	for (auto s : tokens) {
		model[context].push_back(s);
		context.push_back(s);
		context.pop_front();
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";

	context = initialcontext;
	ostringstream oss;
	string next;

	cout << "make text...";
	start = clock();
	context = initialcontext;
	for (int i = 0; i < genlength; i++) {
		next = model[context][rand() % model[context].size()];
		context.push_back(next);
		context.pop_front();
		oss << next << " ";
	}
	elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "(" << elapsed << " sec.)\n";


	return oss.str();
}

int main() {
	string request;
	string training;

	training.reserve(1000000);

	do {
		request = getrequest();


		if ("f" == request || "file" == request) {
			cout << "\nReading text from a file...\n";
			getfromfile(training);
		}
		else if ("t" == request || "text" == request) {
			cout << "\nReading text, I will stop on a line that says exactly: 'please stop'\n";
			getfromcin(training);
		}
		else if ("g" == request || "generate" == request) {
			int genlength = 0;
			cout << "\ngenerating text...\n";
			cout << "How many words of text should I generate? ";
			cin >> genlength;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Start of generated text\n";
			cout << gentext_original(training, genlength);
			cout << "\nEnd of generated text\n";

		}
		else if ("g2" == request || "generate2" == request) {
			int genlength = 0;
			cout << "\ngenerating text...\n";
			cout << "How many words of text should I generate? ";
			cin >> genlength;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Start of generated text\n";
			cout << gentext(training, genlength);
			cout << "\nEnd of generated text\n";

		}
		else if ("s" == request || "stop" == request) {
			cout << "\nstopping...\n";

		}
		else if ("c" == request || "clear" == request) {
			cout << "\nclearing...\n";
			training = "";

		}
		else
			cout << "Unknown request: " << request << endl;

	} while ("s" != request && "stop" != request);

	//system("pause");
	return 0;
}

