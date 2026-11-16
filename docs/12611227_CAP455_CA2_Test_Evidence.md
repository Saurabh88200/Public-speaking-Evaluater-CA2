# CAP455 CA2 Test Evidence - Problem Set Manager

Student: Saurabh Kumar  
Roll No.: 12611227

## Environment

- Compiler: g++ with C++11
- Build flags: `-std=c++11 -Wall -Wextra -pedantic`
- Storage: fixed-size binary `ProblemRecord` entries

## Test results

| ID | Type | Test | Expected result | Result |
|---|---|---|---|---|
| T01 | Boundary | View an empty data file | `No problems saved.` | Pass |
| T02 | Invalid | Search ID 999 in an empty file | `Problem not found.` | Pass |
| T03 | Invalid | Update ID 999 in an empty file | `Problem not found.` | Pass |
| T04 | Invalid | Menu choice 9 | Rejected; asks again for 0-4 | Pass |
| T05 | Boundary/invalid | IDs 0, 10000 and `abc`, then ID 1 | Invalid values rejected; ID 1 accepted | Pass |
| T06 | Invalid | Blank title and topic | Blank text rejected | Pass |
| T07 | Invalid | Difficulty `hard`, then `Easy` | Invalid spelling rejected; valid value accepted | Pass |
| T08 | Boundary | Points 9 then 10; time limit 0 then 1 | Out-of-range values rejected; lower bounds accepted | Pass |
| T09 | Duplicate | Add ID 1 again | Duplicate ID rejected | Pass |
| T10 | Boundary | Add ID 9999, 1000 points, 10 seconds | Upper bounds accepted | Pass |
| T11 | Normal | Update the last record | Same binary record is overwritten using `seekp` | Pass |
| T12 | Normal | Search ID 9999 after update | Updated values are read using `seekg` | Pass |
| T13 | Normal | Add, list and search sample records | All fixed-size records display correctly | Pass |
| T14 | Persistence | Exit and start a second process | Three records and the update remain available | Pass |

## Evidence files

- `tests/validation_input.txt` and `tests/validation_output.txt`: normal,
  invalid and boundary inputs from an initially empty data file.
- `data/sample_input.txt` and `data/sample_output.txt`: creates three sample
  records, reads them, searches, updates one record and reads it again.
- `tests/restart_input.txt` and `tests/restart_output.txt`: a separate program
  run that reads the already saved sample data without adding it again.
- `data/12611227_CAP455_CA2_ProblemSetManager.dat`: nonempty binary data file
  produced by the sample run.

The first process was closed before the restart run. Therefore T14 verifies
that records survive a program restart rather than remaining only in memory.
