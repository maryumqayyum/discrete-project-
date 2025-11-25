#include "ConsistencyChecker.h"
#include <iostream>
#include <algorithm>

using namespace std;

ConsistencyChecker::ConsistencyChecker() {}

// Student course management with prerequisite checking
bool ConsistencyChecker::enrollStudentInCourse(const string& student, const string& course,
    const string& timeSlot) {
    cout << "\n>> Processing Registration Request:\n";
    cout << "---------------------------------------\n";
    cout << "Student: " << student << "\n";
    cout << "Course: " << course << "\n";
    cout << "Time: " << timeSlot << "\n";
    cout << "---------------------------------------\n";

    // Check if student has completed prerequisites
    if (!hasCompletedPrerequisites(student, course)) {
        cout << "[DENIED] Missing required prerequisites!\n\n";

        if (prerequisites.count(course)) {
            cout << "Required prerequisites for " << course << ":\n";
            for (const string& prereq : prerequisites[course]) {
                bool completed = false;
                if (studentCompletedCourses.count(student)) {
                    for (const string& comp : studentCompletedCourses[student]) {
                        if (comp == prereq) {
                            completed = true;
                            break;
                        }
                    }
                }
                cout << "  -> " << prereq << (completed ? " [COMPLETED]" : " [MISSING]") << "\n";
            }
        }
        return false;
    }

    // Check for time conflicts
    for (const Enrollment& e : enrollments) {
        if (e.studentId == student && e.timeSlot == timeSlot) {
            cout << "[DENIED] Schedule conflict detected with " << e.courseId << "!\n";
            return false;
        }
    }

    // Enroll student
    enrollments.push_back({ student, course, timeSlot });
    studentCourses[student].push_back(course);

    cout << "[APPROVED] Registration successful!\n";
    cout << "---------------------------------------\n";

    return true;
}

void ConsistencyChecker::markCourseCompleted(const string& student, const string& course) {
    studentCompletedCourses[student].push_back(course);
    cout << "[OK] Recorded completion: " << course << " for " << student << "\n";
}

void ConsistencyChecker::displayStudentCourses(const string& student) const {
    cout << "\n>> Academic Record for: " << student << "\n";
    cout << "---------------------------------------\n";

    if (studentCompletedCourses.count(student)) {
        cout << "Completed Courses:\n";
        for (const string& course : studentCompletedCourses.at(student)) {
            cout << "  [DONE] " << course << "\n";
        }
    }

    if (studentCourses.count(student)) {
        cout << "\nCurrent Enrollment:\n";
        for (const string& course : studentCourses.at(student)) {
            cout << "  [ACTIVE] " << course;

            // Find time slot
            for (const Enrollment& e : enrollments) {
                if (e.studentId == student && e.courseId == course) {
                    cout << " @ " << e.timeSlot;
                    break;
                }
            }
            cout << "\n";
        }
    }

    if (!studentCourses.count(student) && !studentCompletedCourses.count(student)) {
        cout << "No academic records found.\n";
    }
    cout << "---------------------------------------\n";
}

bool ConsistencyChecker::hasCompletedPrerequisites(const string& student, const string& course) const {
    if (!prerequisites.count(course)) {
        return true; // No prerequisites required
    }

    // Check each prerequisite
    for (const string& prereq : prerequisites.at(course)) {
        bool found = false;

        if (studentCompletedCourses.count(student)) {
            for (const string& completed : studentCompletedCourses.at(student)) {
                if (completed == prereq) {
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            return false; // Missing prerequisite
        }
    }

    return true; // All prerequisites completed
}

void ConsistencyChecker::addEnrollment(const string& student, const string& course,
    const string& timeSlot) {
    enrollments.push_back({ student, course, timeSlot });
    studentCourses[student].push_back(course);
}

void ConsistencyChecker::addAssignment(const string& faculty, const string& course,
    const string& room) {
    assignments.push_back({ faculty, course, room });
}

void ConsistencyChecker::addPrerequisite(const string& course, const string& prereq) {
    prerequisites[course].push_back(prereq);
}

void ConsistencyChecker::addCourseCredit(const string& course, int credits) {
    courseCredits[course] = credits;
}

bool ConsistencyChecker::hasTimeConflict(const string& student) const {
    vector<string> timeSlots;

    for (const Enrollment& e : enrollments) {
        if (e.studentId == student) {
            for (const string& slot : timeSlots) {
                if (slot == e.timeSlot) return true;
            }
            timeSlots.push_back(e.timeSlot);
        }
    }
    return false;
}

bool ConsistencyChecker::hasRoomConflict() const {
    for (size_t i = 0; i < assignments.size(); i++) {
        for (size_t j = i + 1; j < assignments.size(); j++) {
            if (assignments[i].roomId == assignments[j].roomId) {
                // Same room - check if times conflict
                for (const Enrollment& e1 : enrollments) {
                    if (e1.courseId == assignments[i].courseId) {
                        for (const Enrollment& e2 : enrollments) {
                            if (e2.courseId == assignments[j].courseId) {
                                if (e1.timeSlot == e2.timeSlot) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ConsistencyChecker::hasFacultyConflict() const {
    for (size_t i = 0; i < assignments.size(); i++) {
        for (size_t j = i + 1; j < assignments.size(); j++) {
            if (assignments[i].facultyId == assignments[j].facultyId) {
                // Same faculty - check if times conflict
                for (const Enrollment& e1 : enrollments) {
                    if (e1.courseId == assignments[i].courseId) {
                        for (const Enrollment& e2 : enrollments) {
                            if (e2.courseId == assignments[j].courseId) {
                                if (e1.timeSlot == e2.timeSlot) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ConsistencyChecker::checkPrerequisites() {
    cout << "\n=== Checking Prerequisites ===\n";
    bool valid = true;

    for (const auto& pair : studentCourses) {
        const string& student = pair.first;
        const vector<string>& courses = pair.second;

        for (const string& course : courses) {
            if (prerequisites.count(course)) {
                for (const string& prereq : prerequisites[course]) {
                    bool found = false;
                    for (const string& taken : courses) {
                        if (taken == prereq) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "✗ Student " << student << " missing prerequisite "
                            << prereq << " for " << course << "\n";
                        valid = false;
                    }
                }
            }
        }
    }

    if (valid) cout << "✓ All prerequisites satisfied\n";
    return valid;
}

bool ConsistencyChecker::checkTimeConflicts() {
    cout << "\n=== Checking Time Conflicts ===\n";
    bool valid = true;

    for (const auto& pair : studentCourses) {
        if (hasTimeConflict(pair.first)) {
            cout << "✗ Student " << pair.first << " has time conflicts\n";
            valid = false;
        }
    }

    if (valid) cout << "✓ No time conflicts found\n";
    return valid;
}

bool ConsistencyChecker::checkRoomConflicts() {
    cout << "\n=== Checking Room Conflicts ===\n";

    if (hasRoomConflict()) {
        cout << "✗ Room conflicts detected\n";
        return false;
    }

    cout << "✓ No room conflicts\n";
    return true;
}

bool ConsistencyChecker::checkFacultyConflicts() {
    cout << "\n=== Checking Faculty Conflicts ===\n";

    if (hasFacultyConflict()) {
        cout << "✗ Faculty conflicts detected\n";
        return false;
    }

    cout << "✓ No faculty conflicts\n";
    return true;
}

bool ConsistencyChecker::checkCreditOverload(int maxCredits) {
    cout << "\n=== Checking Credit Overload ===\n";
    bool valid = true;

    for (const auto& pair : studentCourses) {
        int totalCredits = 0;
        for (const string& course : pair.second) {
            if (courseCredits.count(course)) {
                totalCredits += courseCredits[course];
            }
        }

        if (totalCredits > maxCredits) {
            cout << "✗ Student " << pair.first << " has " << totalCredits
                << " credits (max: " << maxCredits << ")\n";
            valid = false;
        }
    }

    if (valid) cout << "yes No credit overloads\n";
    return valid;
}

bool ConsistencyChecker::checkAll() {
    cout << "\n|-----------------------------------|\n";
    cout << "|   CONSISTENCY CHECK REPORT        |\n";
    cout << "|------------------------------------|\n";

    bool p = checkPrerequisites();
    bool t = checkTimeConflicts();
    bool r = checkRoomConflicts();
    bool f = checkFacultyConflicts();
    bool c = checkCreditOverload(18);

    bool allValid = p && t && r && f && c;

    cout << "\n" << string(40, '=') << "\n";
    if (allValid) {
        cout << "yes ALL CHECKS PASSED - System is consistent!\n";
    }
    else {
        cout << "no SOME CHECKS FAILED - Please review conflicts\n";
    }
    cout << string(40, '=') << "\n";

    return allValid;
}

void ConsistencyChecker::displayEnrollments() const {
    cout << "\n=== Current Enrollments ===\n";
    for (const Enrollment& e : enrollments) {
        cout << e.studentId << " -> " << e.courseId << " @ " << e.timeSlot << "\n";
    }
}

void ConsistencyChecker::displayAssignments() const {
    cout << "\n=== Faculty Assignments ===\n";
    for (const Assignment& a : assignments) {
        cout << a.facultyId << " teaches " << a.courseId << " in " << a.roomId << "\n";
    }
}

void ConsistencyChecker::displayReport() const {
    cout << "\n=== System Overview ===\n";
    cout << "Total Enrollments: " << enrollments.size() << "\n";
    cout << "Total Assignments: " << assignments.size() << "\n";
    cout << "Total Students: " << studentCourses.size() << "\n";
}