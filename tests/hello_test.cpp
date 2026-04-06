#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_constructor.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../src/hello.hpp"

TEST_CASE("Added students appear in report", "[addStudent]") {
    Gradebook gb;
    gb.addStudent("Tom", "Torres", "T001");
    gb.addStudent("Nina", "Navarro", "N002");
    std::string rep = gb.report();
    REQUIRE(rep.find("Torres")  != std::string::npos);
    REQUIRE(rep.find("Navarro") != std::string::npos);
}

TEST_CASE("Added assignments appear in report", "[addAssignment]") {
    Gradebook gb;
    gb.addAssignment("Lab 1", 30);
    gb.addAssignment("Exam 1", 100);
    std::string rep = gb.report();
    REQUIRE(rep.find("Lab 1")  != std::string::npos);
    REQUIRE(rep.find("Exam 1") != std::string::npos);
}

// ── enterGrade – valid ────────────────────────────────────────────────────

TEST_CASE("enterGrade accepts valid grades", "[enterGrade]") {
    Gradebook gb;
    gb.addStudent("Sam", "Smith", "S100");
    gb.addAssignment("Quiz", 50);

    SECTION("zero is valid")            { REQUIRE(gb.enterGrade("Sam Smith", "Quiz",  0) == true); }
    SECTION("full score is valid")      { REQUIRE(gb.enterGrade("Sam Smith", "Quiz", 50) == true); }
    SECTION("mid-range score is valid") { REQUIRE(gb.enterGrade("Sam Smith", "Quiz", 33) == true); }
}

// ── enterGrade – invalid ──────────────────────────────────────────────────

TEST_CASE("enterGrade rejects invalid inputs", "[enterGrade]") {
    Gradebook gb;
    gb.addStudent("Sam", "Smith", "S100");
    gb.addAssignment("Quiz", 50);

    SECTION("over max")              { REQUIRE(gb.enterGrade("Sam Smith", "Quiz",    51) == false); }
    SECTION("negative")             { REQUIRE(gb.enterGrade("Sam Smith", "Quiz",    -1) == false); }
    SECTION("unknown student")      { REQUIRE(gb.enterGrade("No Body",   "Quiz",    40) == false); }
    SECTION("unknown assignment")   { REQUIRE(gb.enterGrade("Sam Smith", "Unknown", 40) == false); }
}

// ── report() ──────────────────────────────────────────────────────────────

TEST_CASE("report shows score and N/A correctly", "[report]") {
    Gradebook gb;
    gb.addStudent("Luis", "Lopez", "L200");
    gb.addStudent("Anya", "Adams", "A300");
    gb.addAssignment("Project", 100);
    gb.enterGrade("Luis Lopez", "Project", 78);

    std::string rep = gb.report();
    REQUIRE(rep.find("78")  != std::string::npos);
    REQUIRE(rep.find("N/A") != std::string::npos);
}

TEST_CASE("report sorts students by last name", "[report]") {
    Gradebook gb;
    gb.addStudent("Zoe",  "Zebra", "Z001");
    gb.addStudent("Mike", "Mango", "M001");
    gb.addStudent("Anna", "Apple", "A001");

    std::string rep = gb.report();
    REQUIRE(rep.find("Apple") < rep.find("Mango"));
    REQUIRE(rep.find("Mango") < rep.find("Zebra"));
}

TEST_CASE("report grade can be overwritten", "[report]") {
    Gradebook gb;
    gb.addStudent("Jo", "Jones", "J001");
    gb.addAssignment("Quiz", 20);
    gb.enterGrade("Jo Jones", "Quiz", 10);
    gb.enterGrade("Jo Jones", "Quiz", 18);
    REQUIRE(gb.report().find("18") != std::string::npos);
}

TEST_CASE("empty gradebook does not crash", "[report]") {
    Gradebook gb;
    REQUIRE_NOTHROW(gb.report());
}

// ── Average column (Extra Credit #1) ──────────────────────────────────────

TEST_CASE("report includes Average header", "[average]") {
    Gradebook gb;
    gb.addStudent("Sam", "Smith", "S001");
    REQUIRE(gb.report().find("Average") != std::string::npos);
}

TEST_CASE("average is correct across multiple graded assignments", "[average]") {
    Gradebook gb;
    gb.addStudent("Sam", "Smith", "S001");
    gb.addAssignment("Quiz", 100);
    gb.addAssignment("Lab",   50);
    gb.enterGrade("Sam Smith", "Quiz", 80);
    gb.enterGrade("Sam Smith", "Lab",  40);
    // 80+40 = 120 / 150 = 80.0%
    REQUIRE(gb.report().find("80.") != std::string::npos);
}

TEST_CASE("average skips ungraded assignments", "[average]") {
    Gradebook gb;
    gb.addStudent("Kim", "King", "K001");
    gb.addAssignment("Quiz", 100);
    gb.addAssignment("Lab",   50);
    gb.enterGrade("Kim King", "Quiz", 90);
    // Lab not graded — average should be 90/100 = 90%
    REQUIRE(gb.report().find("90.") != std::string::npos);
}

TEST_CASE("average shows N/A when nothing is graded", "[average]") {
    Gradebook gb;
    gb.addStudent("Jo", "Jones", "J001");
    gb.addAssignment("Quiz", 50);
    REQUIRE(gb.report().find("N/A") != std::string::npos);
}

// ── assignmentReport() (Extra Credit #2) ──────────────────────────────────

TEST_CASE("assignmentReport returns empty for unknown assignment", "[assignmentReport]") {
    Gradebook gb;
    REQUIRE(gb.assignmentReport("Ghost") == "");
}

TEST_CASE("assignmentReport shows all student scores", "[assignmentReport]") {
    Gradebook gb;
    gb.addStudent("Anna", "Adams", "A001");
    gb.addStudent("Bob",  "Brown", "B002");
    gb.addAssignment("Lab", 50);
    gb.enterGrade("Anna Adams", "Lab", 45);
    gb.enterGrade("Bob Brown",  "Lab", 30);

    std::string rep = gb.assignmentReport("Lab");
    REQUIRE(rep.find("45") != std::string::npos);
    REQUIRE(rep.find("30") != std::string::npos);
}

TEST_CASE("assignmentReport shows placeholder for ungraded students", "[assignmentReport]") {
    Gradebook gb;
    gb.addStudent("Anna", "Adams", "A001");
    gb.addStudent("Bob",  "Brown", "B002");
    gb.addAssignment("Lab", 50);
    gb.enterGrade("Anna Adams", "Lab", 45);

    REQUIRE(gb.assignmentReport("Lab").find("---") != std::string::npos);
}

TEST_CASE("assignmentReport class average excludes ungraded students", "[assignmentReport]") {
    Gradebook gb;
    gb.addStudent("Anna", "Adams", "A001");
    gb.addStudent("Bob",  "Brown", "B002");
    gb.addAssignment("Quiz", 100);
    gb.enterGrade("Anna Adams", "Quiz", 80);

    std::string rep = gb.assignmentReport("Quiz");
    REQUIRE(rep.find("1 of 2") != std::string::npos);
    REQUIRE(rep.find("80.")    != std::string::npos);
}

TEST_CASE("assignmentReport shows N/A when no grades entered", "[assignmentReport]") {
    Gradebook gb;
    gb.addStudent("Sam", "Smith", "S001");
    gb.addAssignment("Quiz", 50);
    REQUIRE(gb.assignmentReport("Quiz").find("N/A") != std::string::npos);
}
