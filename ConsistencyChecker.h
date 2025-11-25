#ifndef CONSISTENCYCHECKER_H
#define CONSISTENCYCHECKER_H

#include <string>
#include <vector>
#include <map>

struct Enrollment {
    std::string studentId;
    std::string courseId;
    std::string timeSlot;
};

struct Assignment {
    std::string facultyId;
    std::string courseId;
    std::string roomId;
};

class ConsistencyChecker {
private:
    std::vector<Enrollment> enrollments;
    std::vector<Assignment> assignments;
    std::map<std::string, std::vector<std::string>> prerequisites;
    std::map<std::string, std::vector<std::string>> studentCourses;
    std::map<std::string, std::vector<std::string>> studentCompletedCourses;
    std::map<std::string, int> courseCredits;

    bool hasTimeConflict(const std::string& student) const;
    bool hasRoomConflict() const;
    bool hasFacultyConflict() const;
    bool hasCompletedPrerequisites(const std::string& student, const std::string& course) const;

public:
    ConsistencyChecker();

    // Student course management
    bool enrollStudentInCourse(const std::string& student, const std::string& course,
        const std::string& timeSlot);
    void markCourseCompleted(const std::string& student, const std::string& course);
    void displayStudentCourses(const std::string& student) const;

    void addEnrollment(const std::string& student, const std::string& course,
        const std::string& timeSlot);
    void addAssignment(const std::string& faculty, const std::string& course,
        const std::string& room);
    void addPrerequisite(const std::string& course, const std::string& prereq);
    void addCourseCredit(const std::string& course, int credits);

    bool checkAll();
    bool checkPrerequisites();
    bool checkTimeConflicts();
    bool checkRoomConflicts();
    bool checkFacultyConflicts();
    bool checkCreditOverload(int maxCredits);

    void displayReport() const;
    void displayEnrollments() const;
    void displayAssignments() const;
};

#endif