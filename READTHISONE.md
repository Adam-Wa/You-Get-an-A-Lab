# CSC 122 Gradebook

## Files

| File | Purpose |
|---|---|
| `Gradebook.h` | `Student` struct, `Assignment` struct, `Gradebook` class declaration |
| `Gradebook.cpp` | All method implementations |
| `main.cpp` | Interactive menu — add students, assignments, grades, print reports |
| `test_gradebook.cpp` | Catch2 unit tests |

## Menu Options

```
1. Add student        – enter first name, last name, student ID
2. Add assignment     – enter name and total points
3. Enter grade        – enter student full name, assignment name, score
4. Print full report  – table of all grades + per-student average
5. Print assignment report – scores for one assignment + class average
6. Quit
```

## How Grades Work

- A grade of `N/A` means no score has been entered yet.
- `enterGrade` returns false (and prints an error in the menu) if:
    - the student or assignment name is not found
    - the score is below 0 or above the assignment's total points
- Calling `enterGrade` again on the same student/assignment overwrites the old grade.
- The **Average** column in the full report uses only graded assignments (N/A entries are skipped).
- The **assignment report** class average also skips ungraded students.