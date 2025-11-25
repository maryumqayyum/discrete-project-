#ifndef COURSEGRAPH_H
#define COURSEGRAPH_H

#include "Graph.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

class CourseGraph : public Graph {
private:
    std::vector<std::string> idx2name;
    std::unordered_map<std::string, int> name2idx;
    std::vector<int> indeg;
    std::map<std::string, unsigned long long> memo;

    bool dfsCycle(int u, std::vector<int>& state) const;
    void generateRecursive(std::vector<int>& current, std::vector<int>& current_indeg,
        std::vector<std::vector<int>>& results, unsigned long long cap) const;
    std::string encodeState(const std::vector<int>& indeg) const;
    unsigned long long countRecursive(std::vector<int>& indeg_state);

public:
    CourseGraph();
    void addCourse(const std::string& name);
    void addPrereq(const std::string& pre, const std::string& course);
    void addEdge(int u, int v) override;
    bool hasCycle() const override;
    unsigned long long countSequences();
    void enumerateSequences(std::vector<std::vector<int>>& results, unsigned long long cap = 10000);
    std::vector<std::string> toNames(const std::vector<int>& seq) const;
    void displayCourses() const;
    void displayPrerequisites() const;
    int getCourseCount() const { return n; }
};

#endif