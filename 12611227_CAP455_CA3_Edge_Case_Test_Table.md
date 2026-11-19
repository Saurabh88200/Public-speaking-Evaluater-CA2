# CAP455 CA3 Edge-Case Test Table

Project: Coding Contest Problem Set Manager  
Student: Saurabh Kumar  
Roll No.: 12611227

| ID | Case | Expected result | Status |
|---|---|---|---|
| T01 | Menu choice 9 | Reject and ask again | Pass |
| T02 | ID 0, then ID 1 | Reject 0; accept boundary 1 | Pass |
| T03 | Blank title | Reject blank text | Pass |
| T04 | Difficulty `Expert`, then `Easy` | Reject invalid value; accept Easy | Pass |
| T05 | Points 9/10 and time 0/1 | Reject below limits; accept lower boundaries | Pass |
| T06 | Add duplicate ID 1 | Reject duplicate key | Pass |
| T07 | Update ID 1 but enter replacement ID 2 | Cancel update because key changed | Pass |
| T08 | Update ID 1 with points 1000 and time 10 | Accept upper boundaries | Pass |
| T09 | Delete ID 1 and answer N | Keep record | Pass |
| T10 | Delete ID 1 and answer Y | Delete selected record | Pass |
| T11 | Search deleted ID 1 | Report not found | Pass |
| T12 | Restart with supplied records | Records 101-103 remain readable | Pass |

Evidence files: `tests/edge_cases_output.txt` and `tests/restart_output.txt`.

Compilation and all three scripted runs completed with exit code 0. The final
line of each run reported `Live objects: 0`, confirming that dynamically
allocated objects were released.
