#include "CourseGraph.h"
#include <iostream>
#include <algorithm>

using namespace std;

CourseGraph::CourseGraph() {
    n = 0;
}

void CourseGraph::addCourse(const string& name) {
    if (name2idx.count(name)) return;
    name2idx[name] = n++;
    idx2name.push_back(name);
    adj.push_back(vector<int>());
    indeg.push_back(0);
}

void CourseGraph::addPrereq(const string& pre, const string& course) {
    addCourse(pre);
    addCourse(course);
    int u = name2idx[pre];
    int v = name2idx[course];
    adj[u].push_back(v);
    indeg[v]++;
}

void CourseGraph::addEdge(int u, int v) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        adj[u].push_back(v);
        indeg[v]++;
    }
}

bool CourseGraph::dfsCycle(int u, vector<int>& state) const {
    state[u] = 1;
    for (int v : adj[u]) {
        if (state[v] == 1) return true;
        if (state[v] == 0 && dfsCycle(v, state)) return true;
    }
    state[u] = 2;
    return false;
}

bool CourseGraph::hasCycle() const {
    vector<int> state(n, 0);
    for (int i = 0; i < n; i++) {
        if (state[i] == 0 && dfsCycle(i, state))
            return true;
    }
    return false;
}

void CourseGraph::generateRecursive(vector<int>& current, vector<int>& current_indeg,
    vector<vector<int>>& results, unsigned long long cap) const
{
    if (results.size() >= cap) return;
    if ((int)current.size() == n) {
        results.push_back(current);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (current_indeg[i] == 0) {
            current_indeg[i] = -1;
            vector<int> affected;
            for (int v : adj[i]) {
                current_indeg[v]--;
                affected.push_back(v);
            }
            current.push_back(i);
            generateRecursive(current, current_indeg, results, cap);
            current.pop_back();
            current_indeg[i] = 0;
            for (int v : affected) current_indeg[v]++;
        }
    }
}

string CourseGraph::encodeState(const vector<int>& indeg) const {
    string s = "";
    for (int x : indeg) {
        if (x == -1) s += 'x';
        else s += char('0' + x);
    }
    return s;
}

unsigned long long CourseGraph::countRecursive(vector<int>& indeg_state) {
    string key = encodeState(indeg_state);
    if (memo.count(key)) return memo[key];

    bool all_taken = true;
    for (int x : indeg_state) if (x != -1) all_taken = false;
    if (all_taken) return memo[key] = 1;

    unsigned long long ways = 0;
    for (int i = 0; i < n; i++) {
        if (indeg_state[i] == 0) {
            indeg_state[i] = -1;
            vector<int> affected;
            for (int v : adj[i]) {
                indeg_state[v]--;
                affected.push_back(v);
            }
            ways += countRecursive(indeg_state);
            indeg_state[i] = 0;
            for (int v : affected) indeg_state[v]++;
        }
    }
    memo[key] = ways;
    return ways;
}

unsigned long long CourseGraph::countSequences() {
    memo.clear();
    vector<int> indeg_copy = indeg;
    return countRecursive(indeg_copy);
}

void CourseGraph::enumerateSequences(vector<vector<int>>& results, unsigned long long cap) {
    results.clear();
    vector<int> indeg_copy = indeg;
    vector<int> curr;
    generateRecursive(curr, indeg_copy, results, cap);
}

vector<string> CourseGraph::toNames(const vector<int>& seq) const {
    vector<string> out;
    for (int x : seq) out.push_back(idx2name[x]);
    return out;
}

void CourseGraph::displayCourses() const {
    cout << "\n>> Registered Courses in System:\n";
    cout << "---------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "  Course #" << i + 1 << ": " << idx2name[i] << "\n";
    }
    cout << "---------------------------------------\n";
    cout << "Total: " << n << " course(s)\n";
}

void CourseGraph::displayPrerequisites() const {
    cout << "\n>> Course Dependency Structure:\n";
    cout << "---------------------------------------\n";
    bool hasDeps = false;
    for (int u = 0; u < n; u++) {
        if (!adj[u].empty()) {
            hasDeps = true;
            cout << "  " << idx2name[u] << " requires completion of: ";
            for (size_t i = 0; i < adj[u].size(); i++) {
                cout << idx2name[adj[u][i]];
                if (i + 1 < adj[u].size()) cout << " & ";
            }
            cout << "\n";
        }
    }
    if (!hasDeps) {
        cout << "  (No dependencies defined)\n";
    }
    cout << "---------------------------------------\n";
}