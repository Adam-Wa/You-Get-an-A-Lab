#include "hello.hpp"
#include <sstream>
#include <iomanip>

// ── Private helpers ────────────────────────────────────────────────────────

int Gradebook::findStudent(const std::string & fullName) const {
    for (int i = 0; i < (int)m_students.size(); ++i)
        if (m_students[i].fullName() == fullName) return i;
    return -1;
}

int Gradebook::findAssignment(const std::string & name) const {
    for (int i = 0; i < (int)m_assignments.size(); ++i)
        if (m_assignments[i].name == name) return i;
    return -1;
}

// Grade is stored flat: row = student, col = assignment.
int Gradebook::gradeAt(int si, int ai) const {
    return m_grades[si * (int)m_assignments.size() + ai];
}

void Gradebook::setGrade(int si, int ai, int grade) {
    m_grades[si * (int)m_assignments.size() + ai] = grade;
}

// ── Public interface ───────────────────────────────────────────────────────

void Gradebook::addStudent(const std::string & first,
                           const std::string & last,
                           const std::string & id) {
    Student s;
    s.firstName = first;
    s.lastName  = last;
    s.studentId = id;
    m_students.push_back(s);

    // Add one -1 entry per existing assignment for this new student.
    for (int a = 0; a < (int)m_assignments.size(); ++a)
        m_grades.push_back(-1);
}

void Gradebook::addAssignment(const std::string & name, int totalPoints) {
    // Insert a -1 for every existing student at the new assignment column.
    // We rebuild the flat table with the extra column inserted.
    int ns = (int)m_students.size();
    int na = (int)m_assignments.size();  // old column count

    std::vector<int> newGrades;
    newGrades.reserve(ns * (na + 1));
    for (int s = 0; s < ns; ++s) {
        for (int a = 0; a < na; ++a)
            newGrades.push_back(m_grades[s * na + a]);
        newGrades.push_back(-1);         // new column for this student
    }
    m_grades = newGrades;

    Assignment asgn;
    asgn.name        = name;
    asgn.totalPoints = totalPoints;
    m_assignments.push_back(asgn);
}

bool Gradebook::enterGrade(const std::string & fullName,
                           const std::string & assignmentName,
                           int grade) {
    int si = findStudent(fullName);
    int ai = findAssignment(assignmentName);
    if (si == -1 || ai == -1) return false;
    if (grade < 0 || grade > m_assignments[ai].totalPoints) return false;
    setGrade(si, ai, grade);
    return true;
}

// ── report() ──────────────────────────────────────────────────────────────

std::string Gradebook::report() const {
    std::ostringstream out;
    int ns = (int)m_students.size();
    int na = (int)m_assignments.size();

    // Column widths
    const int W_NAME  = 20;
    const int W_ID    = 12;
    const int W_GRADE = 12;
    const int W_AVG   = 10;
    int lineLen = W_NAME + W_ID + na * W_GRADE + W_AVG;

    // Header
    out << std::left
        << std::setw(W_NAME) << "Name"
        << std::setw(W_ID)   << "ID";
    for (int a = 0; a < na; ++a)
        out << std::setw(W_GRADE) << m_assignments[a].name;
    out << std::setw(W_AVG) << "Average" << "\n";
    out << std::string(lineLen, '-') << "\n";

    // Sort students alphabetically by last name (bubble sort on index array)
    std::vector<int> order(ns);
    for (int i = 0; i < ns; ++i) order[i] = i;
    for (int i = 0; i < ns - 1; ++i)
        for (int j = 0; j < ns - 1 - i; ++j)
            if (m_students[order[j]].lastName > m_students[order[j+1]].lastName) {
                int tmp = order[j]; order[j] = order[j+1]; order[j+1] = tmp;
            }

    // Student rows
    for (int i = 0; i < ns; ++i) {
        int si = order[i];
        out << std::left
            << std::setw(W_NAME) << m_students[si].fullName()
            << std::setw(W_ID)   << m_students[si].studentId;

        int earned = 0, possible = 0;
        for (int a = 0; a < na; ++a) {
            int g = gradeAt(si, a);
            if (g == -1) {
                out << std::setw(W_GRADE) << "N/A";
            } else {
                std::ostringstream cell;
                cell << g << "/" << m_assignments[a].totalPoints;
                out << std::setw(W_GRADE) << cell.str();
                earned   += g;
                possible += m_assignments[a].totalPoints;
            }
        }

        // Average column
        if (possible == 0) {
            out << std::setw(W_AVG) << "N/A";
        } else {
            int pct10 = (earned * 1000) / possible;
            std::ostringstream avg;
            avg << pct10 / 10 << "." << pct10 % 10 << "%";
            out << std::setw(W_AVG) << avg.str();
        }
        out << "\n";
    }

    // Footer: max points per assignment
    out << std::string(lineLen, '-') << "\n";
    out << std::left
        << std::setw(W_NAME) << "Max Points"
        << std::setw(W_ID)   << "";
    for (int a = 0; a < na; ++a)
        out << std::setw(W_GRADE) << m_assignments[a].totalPoints;
    out << "\n";

    return out.str();
}

// ── assignmentReport() ────────────────────────────────────────────────────

std::string Gradebook::assignmentReport(const std::string & assignmentName) const {
    int ai = findAssignment(assignmentName);
    if (ai == -1) return "";

    std::ostringstream out;
    int ns = (int)m_students.size();
    const int W_NAME  = 20;
    const int W_ID    = 12;
    const int W_SCORE = 14;
    int lineLen = W_NAME + W_ID + W_SCORE;

    out << "Assignment: " << m_assignments[ai].name
        << "  (out of " << m_assignments[ai].totalPoints << " pts)\n"
        << std::string(lineLen, '=') << "\n"
        << std::left
        << std::setw(W_NAME)  << "Name"
        << std::setw(W_ID)    << "ID"
        << std::setw(W_SCORE) << "Score" << "\n"
        << std::string(lineLen, '-') << "\n";

    int totalEarned = 0, gradedCount = 0;
    for (int si = 0; si < ns; ++si) {
        int g = gradeAt(si, ai);
        out << std::left
            << std::setw(W_NAME)  << m_students[si].fullName()
            << std::setw(W_ID)    << m_students[si].studentId;
        if (g == -1) {
            out << std::setw(W_SCORE) << "---";
        } else {
            std::ostringstream cell;
            cell << g << "/" << m_assignments[ai].totalPoints;
            out << std::setw(W_SCORE) << cell.str();
            totalEarned += g;
            ++gradedCount;
        }
        out << "\n";
    }

    // Class average
    out << std::string(lineLen, '-') << "\n";
    if (gradedCount == 0) {
        out << "Class Average: N/A\n";
    } else {
        int possible = gradedCount * m_assignments[ai].totalPoints;
        int pct10    = (totalEarned * 1000) / possible;
        out << "Class Average: " << totalEarned << "/" << possible
            << "  (" << pct10 / 10 << "." << pct10 % 10 << "%)"
            << "  [" << gradedCount << " of " << ns << " graded]\n";
    }

    return out.str();
}