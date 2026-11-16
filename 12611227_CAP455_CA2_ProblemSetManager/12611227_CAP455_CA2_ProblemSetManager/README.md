# Coding Contest Problem Set Manager - CAP455 CA2

Student: Saurabh Kumar  
Roll No.: 12611227  
Course: CAP455 - Object-Oriented Programming using C++  
Phase: CA2 - Persist & Search

This is a separate CA2 version of the CA1 Problem Set Manager. It stores coding
contest problems in a fixed-size binary file, so records remain available after
the program closes and starts again.

## Folder layout

```text
README.md
src/12611227_CAP455_CA2_ProblemSetManager.cpp
data/12611227_CAP455_CA2_ProblemSetManager.dat
data/sample_input.txt
data/sample_output.txt
docs/12611227_CAP455_CA2_Test_Evidence.md
docs/12611227_CAP455_CA2_Project_and_Progress_Report.pdf
tests/validation_input.txt
tests/validation_output.txt
tests/restart_input.txt
tests/restart_output.txt
```

## Build and run

Run these commands from this project folder:

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic src\12611227_CAP455_CA2_ProblemSetManager.cpp -o psm_ca2.exe
.\psm_ca2.exe
```

The program uses this relative data path:

```text
data/12611227_CAP455_CA2_ProblemSetManager.dat
```

Run the executable from the project root so it can find the `data` folder.
The `.dat` file is binary and should not be edited in a text editor.

## Menu

```text
1. Add Problem
2. View All Problems
3. Search by ID
4. Update Problem
0. Exit
```

Allowed values:

| Field | Validation |
|---|---|
| Problem ID | 1-9999 and unique |
| Title | nonblank, at most 50 characters |
| Topic | nonblank, at most 30 characters |
| Difficulty | Easy, Medium or Hard |
| Points | 10-1000 |
| Time limit | 1-10 seconds |

## Features by phase

| Phase | Implemented features |
|---|---|
| CA1 | Entity class, static member, inline getter, friend display function, `this`, strings, object array, pointers, validated Add/View/Search and unique IDs |
| CA2 | Default, parameterised and copy constructors; destructor; `fstream` binary fixed-size records; persistent Add/View; `seekg` search; in-place `seekp` update |

The binary record uses fixed-capacity character arrays. The program converts
between that storage record and the `Problem` class, which continues to use
`std::string`.

## Test evidence

The `tests` folder contains a clean-file validation run and a separate restart
run. The restart output proves that saved and updated records survive after the
first process exits. Details are in
`docs/12611227_CAP455_CA2_Test_Evidence.md`.

To demonstrate persistence manually:

1. Run the program and add or update a record.
2. Choose Exit.
3. Start the program again.
4. View or search for the same record.

## Scope

CA2 adds only the required persistence, reading, search and basic update work.
Delete, inheritance, polymorphism, operator overloading, dynamic memory,
exceptions, templates, STL algorithms and advanced features belong to later
phases and are not included here.

## AI assistance disclosure

OpenAI Codex was used for implementation review, testing and documentation.
The student should study the program and be ready to explain and modify every
line during the viva.
