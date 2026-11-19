# Coding Contest Problem Set Manager - CAP455 CA3

Student: Saurabh Kumar  
Roll No.: 12611227  
Course: CAP455 - Object-Oriented Programming using C++  
Phase: CA3 - Reuse & Delete  
Repository:https://github.com/Saurabh88200/Public-speaking-Evaluater-CA2.git


This CA3 version builds on the CA2 binary-file project. It keeps the same
fixed-size record format, so the supplied CA2 data remains usable. At runtime,
Easy and Medium records become `PracticeProblem` objects; Hard records become
`ChallengeProblem` objects.

## Build and run

Run from this project folder:

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic src\12611227_CAP455_CA3_ProblemSetManager.cpp -o psm_ca3.exe
.\psm_ca3.exe
```

The executable expects this relative data file:

```text
data/12611227_CAP455_CA3_ProblemSetManager.dat
```

## Menu

```text
1. Add Problem
2. View All Problems
3. Search by ID
4. Update Problem
5. Delete Problem
0. Exit
```

Update and Delete show the selected record and require Y/N confirmation.
Delete rewrites the remaining fixed-size records and restores the original file
if replacement fails.

## CA3 requirement mapping

| Requirement | Implementation |
|---|---|
| Base + at least two derived classes | `Problem`, `PracticeProblem`, `ChallengeProblem` |
| `<<` and `>>` | Record display and validated input |
| `==` and `<` | ID matching and sorted display |
| One more operator | `!=` prevents changing the key during update |
| Type conversion | Explicit `Problem` to `int` conversion returns the ID |
| `new` / `delete` | Factory objects and dynamic array of base pointers |
| Virtual functions | Type label and study note |
| Virtual destructor | Cleanup through `Problem*` |
| Safe Update | Selected record, confirmation, fixed key and in-place write |
| Safe Delete | Selected record, confirmation and guarded file replacement |

## Folder layout

```text
README.md
src/12611227_CAP455_CA3_ProblemSetManager.cpp
data/12611227_CAP455_CA3_ProblemSetManager.dat
data/sample_input.txt
data/sample_output.txt
docs/12611227_CAP455_CA3_Edge_Case_Test_Table.md
docs/12611227_CAP455_CA3_Project_and_Progress_Report.pdf
tests/edge_cases_input.txt
tests/edge_cases_output.txt
tests/restart_input.txt
tests/restart_output.txt
submission/12611227_CAP455_CA3_ProblemSetManager.zip
```

## Validation

| Field | Rule |
|---|---|
| Problem ID | 1-9999 and unique |
| Title | Nonblank, maximum 50 characters |
| Topic | Nonblank, maximum 30 characters |
| Difficulty | Easy, Medium or Hard |
| Points | 10-1000 |
| Time limit | 1-10 seconds |

## Scope

This phase adds only the CA3 topics. Exceptions, templates, STL containers and
advanced export/backup features remain for CA4.

## AI assistance disclosure

OpenAI Codex was used for implementation review, testing and documentation.
The student should study the program and be ready to explain and modify every
line during the viva.
