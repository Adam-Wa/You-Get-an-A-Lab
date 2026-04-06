#include <string>
#include <vector>

// ── Simple data holders ────────────────────────────────────────────────────

struct Student {
    std::string firstName;
    std::string lastName;
    std::string studentId;
    std::string fullName() const { return firstName + " " + lastName; }
};

struct Assignment {
    std::string name;
    int totalPoints;
};

// ── Gradebook ──────────────────────────────────────────────────────────────
// Uses a flat vector of ints (one per student×assignment pair) for grades.
// No dynamic memory means no Big Three needed.

class Gradebook {
public:
    // Add a student (first name, last name, student ID).
    void addStudent(const std::string & first,
                    const std::string & last,
                    const std::string & id);

    // Add an assignment (name, total points possible).
    void addAssignment(const std::string & name, int totalPoints);

    // Record a grade. Grade must be 0..totalPoints.
    // Returns false if student/assignment not found or grade out of range.
    bool enterGrade(const std::string & fullName,
                    const std::string & assignmentName,
                    int grade);

    // Full grade report with a per-student average column.
    std::string report() const;

    // Per-assignment breakdown with class average.
    // Returns empty string if assignment not found.
    std::string assignmentReport(const std::string & assignmentName) const;

private:
    std::vector<Student>    m_students;
    std::vector<Assignment> m_assignments;

    // Flat grade table: grade at [studentIndex * numAssignments + assignmentIndex]
    // -1 means not yet graded.
    std::vector<int> m_grades;

    int findStudent(const std::string & fullName)        const;
    int findAssignment(const std::string & name)         const;
    int gradeAt(int si, int ai)                          const;
    void setGrade(int si, int ai, int grade);
};
