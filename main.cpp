#include <iostream>
#include <string>
#include "src/hello.hpp"

// main.cpp – Interactive driver for the CSC 122 Gradebook

// Print the menu options
void printMenu() {
    std::cout << "\n--- Gradebook Menu ---\n"
              << "1. Add student\n"
              << "2. Add assignment\n"
              << "3. Enter grade\n"
              << "4. Print full report\n"
              << "5. Print assignment report\n"
              << "6. Quit\n"
              << "Choice: ";
}

int main() {
    Gradebook gradebook;
    int choice = 0;

    while (choice != 6) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();  // discard leftover newline

        if (choice == 1) {
            std::string first, last, id;
            std::cout << "First name: ";  std::getline(std::cin, first);
            std::cout << "Last name: ";   std::getline(std::cin, last);
            std::cout << "Student ID: ";  std::getline(std::cin, id);
            gradebook.addStudent(first, last, id);
            std::cout << "Student added.\n";

        } else if (choice == 2) {
            std::string name;
            int pts;
            std::cout << "Assignment name: ";   std::getline(std::cin, name);
            std::cout << "Total points: ";      std::cin >> pts;
            std::cin.ignore();
            gradebook.addAssignment(name, pts);
            std::cout << "Assignment added.\n";

        } else if (choice == 3) {
            std::string fullName, assignName;
            int grade;
            std::cout << "Student full name (First Last): ";
            std::getline(std::cin, fullName);
            std::cout << "Assignment name: ";
            std::getline(std::cin, assignName);
            std::cout << "Grade: ";
            std::cin >> grade;
            std::cin.ignore();
            if (gradebook.enterGrade(fullName, assignName, grade))
                std::cout << "Grade entered.\n";
            else
                std::cout << "Error: student/assignment not found or grade out of range.\n";

        } else if (choice == 4) {
            std::cout << "\n" << gradebook.report();

        } else if (choice == 5) {
            std::string assignName;
            std::cout << "Assignment name: ";
            std::getline(std::cin, assignName);
            std::string rep = gradebook.assignmentReport(assignName);
            if (rep.empty())
                std::cout << "Assignment not found.\n";
            else
                std::cout << "\n" << rep;

        } else if (choice != 6) {
            std::cout << "Invalid choice. Please enter 1-6.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}