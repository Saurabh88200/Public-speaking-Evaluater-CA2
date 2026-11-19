// Coding Contest Problem Set Manager - CAP455 CA3 (Reuse & Delete)
// Builds on CA2 persistence without changing the fixed-size binary record.
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_PROBLEMS = 50;
const int TITLE_SIZE = 51;
const int TOPIC_SIZE = 31;
const int DIFFICULTY_SIZE = 7;
const char DATA_FILE[] = "data/12611227_CAP455_CA3_ProblemSetManager.dat";
const char TEMP_FILE[] = "data/12611227_CAP455_CA3_ProblemSetManager.tmp";
const char BACKUP_FILE[] = "data/12611227_CAP455_CA3_ProblemSetManager.bak";

// CA2 CARRIED FORWARD:
// The fixed-size storage layout is unchanged, so the supplied records remain usable.
struct ProblemRecord {
    int id;
    char title[TITLE_SIZE];
    char topic[TOPIC_SIZE];
    char difficulty[DIFFICULTY_SIZE];
    int points;
    int timeLimit;
};

bool readInt(istream &input, const string &prompt, int low, int high, int &value) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(input, line)) return false;

        istringstream parser(line);
        int candidate;
        char extra;
        if (parser >> candidate && !(parser >> extra)
            && candidate >= low && candidate <= high) {
            value = candidate;
            return true;
        }
        cout << "Invalid. Enter an integer from " << low << " to " << high << ".\n";
    }
}

bool readText(istream &input, const string &prompt, int maxLength, string &value) {
    while (true) {
        cout << prompt;
        if (!getline(input, value)) return false;

        if (value.find_first_not_of(" \t\r\n") == string::npos) {
            cout << "Text cannot be blank.\n";
        } else if (static_cast<int>(value.size()) > maxLength) {
            cout << "Text must contain at most " << maxLength << " characters.\n";
        } else {
            return true;
        }
    }
}

bool readDifficulty(istream &input, string &difficulty) {
    while (true) {
        if (!readText(input, "Difficulty (Easy/Medium/Hard): ",
                      DIFFICULTY_SIZE - 1, difficulty)) {
            return false;
        }
        if (difficulty == "Easy" || difficulty == "Medium"
            || difficulty == "Hard") {
            return true;
        }
        cout << "Difficulty must be Easy, Medium or Hard.\n";
    }
}

void copyText(char destination[], int capacity, const string &source) {
    strncpy(destination, source.c_str(), capacity - 1);
    destination[capacity - 1] = '\0';
}

void terminateRecordText(ProblemRecord &record) {
    record.title[TITLE_SIZE - 1] = '\0';
    record.topic[TOPIC_SIZE - 1] = '\0';
    record.difficulty[DIFFICULTY_SIZE - 1] = '\0';
}

class Problem {
private:
    int id;
    string title;
    string topic;
    string difficulty;
    int points;
    int timeLimit;

public:
    static int liveObjects;

    Problem()
        : id(0), title(""), topic(""), difficulty("Easy"),
          points(10), timeLimit(1) {
        ++liveObjects;
    }

    Problem(int id, const string &title, const string &topic,
            const string &difficulty, int points, int timeLimit)
        : id(id), title(title), topic(topic), difficulty(difficulty),
          points(points), timeLimit(timeLimit) {
        ++liveObjects;
    }

    Problem(const Problem &other)
        : id(other.id), title(other.title), topic(other.topic),
          difficulty(other.difficulty), points(other.points),
          timeLimit(other.timeLimit) {
        ++liveObjects;
    }

    virtual ~Problem() {
        --liveObjects;
    }

    void setData(int newId, const string &newTitle, const string &newTopic,
                 const string &newDifficulty, int newPoints, int newTimeLimit) {
        this->id = newId;
        this->title = newTitle;
        this->topic = newTopic;
        this->difficulty = newDifficulty;
        this->points = newPoints;
        this->timeLimit = newTimeLimit;
    }

    // CA3 NEW: the CA2 entity becomes an abstract polymorphic base class.
    virtual const char *typeLabel() const = 0;
    virtual string studyNote() const = 0;

    // CA3 type conversion: a Problem can be explicitly converted to its key.
    explicit operator int() const {
        return id;
    }

    ProblemRecord toRecord() const {
        ProblemRecord record = {};
        record.id = static_cast<int>(*this);
        copyText(record.title, TITLE_SIZE, title);
        copyText(record.topic, TOPIC_SIZE, topic);
        copyText(record.difficulty, DIFFICULTY_SIZE, difficulty);
        record.points = points;
        record.timeLimit = timeLimit;
        return record;
    }

    friend ostream &operator<<(ostream &output, const Problem &problem);
    friend istream &operator>>(istream &input, Problem &problem);
    friend bool operator==(const Problem &left, const Problem &right);
    friend bool operator<(const Problem &left, const Problem &right);
    friend bool operator!=(const Problem &left, const Problem &right);
};

int Problem::liveObjects = 0;

// CA3 NEW: two derived classes reuse the common Problem state and behavior.
class PracticeProblem : public Problem {
public:
    PracticeProblem() : Problem() {}

    PracticeProblem(int id, const string &title, const string &topic,
                    const string &difficulty, int points, int timeLimit)
        : Problem(id, title, topic, difficulty, points, timeLimit) {}

    explicit PracticeProblem(int id)
        : Problem(id, "", "", "Easy", 10, 1) {}

    const char *typeLabel() const {
        return "Practice";
    }

    string studyNote() const {
        return "Solve for concept practice and record the approach.";
    }
};

class ChallengeProblem : public Problem {
public:
    ChallengeProblem() : Problem() {}

    ChallengeProblem(int id, const string &title, const string &topic,
                     const string &difficulty, int points, int timeLimit)
        : Problem(id, title, topic, difficulty, points, timeLimit) {}

    const char *typeLabel() const {
        return "Challenge";
    }

    string studyNote() const {
        return "Attempt under the saved time limit before reviewing the solution.";
    }
};

// CA3 NEW: required stream and comparison operator overloads.
ostream &operator<<(ostream &output, const Problem &problem) {
    output << problem.id << '\t' << problem.title << '\t' << problem.topic << '\t'
           << problem.difficulty << '\t' << problem.points << '\t'
           << problem.timeLimit << "s\t" << problem.typeLabel();
    return output;
}

istream &operator>>(istream &input, Problem &problem) {
    int id, points, timeLimit;
    string title, topic, difficulty;

    if (!readInt(input, "Problem ID (1-9999): ", 1, 9999, id)
        || !readText(input, "Title (max 50 characters): ", TITLE_SIZE - 1, title)
        || !readText(input, "Topic (max 30 characters): ", TOPIC_SIZE - 1, topic)
        || !readDifficulty(input, difficulty)
        || !readInt(input, "Points (10-1000): ", 10, 1000, points)
        || !readInt(input, "Time limit in seconds (1-10): ", 1, 10, timeLimit)) {
        input.setstate(ios::failbit);
        return input;
    }

    problem.setData(id, title, topic, difficulty, points, timeLimit);
    return input;
}

bool operator==(const Problem &left, const Problem &right) {
    return left.id == right.id;
}

bool operator<(const Problem &left, const Problem &right) {
    if (left.id != right.id) return left.id < right.id;
    return left.title < right.title;
}

bool operator!=(const Problem &left, const Problem &right) {
    return !(left == right);
}

// CA3 NEW: factory creates the correct derived object with new.
Problem *createProblem(ProblemRecord record) {
    terminateRecordText(record);
    if (string(record.difficulty) == "Hard") {
        return new ChallengeProblem(record.id, record.title, record.topic,
                                    record.difficulty, record.points,
                                    record.timeLimit);
    }
    return new PracticeProblem(record.id, record.title, record.topic,
                               record.difficulty, record.points,
                               record.timeLimit);
}

// CA2 CARRIED FORWARD: binary file creation, opening and fixed-record access.
bool openDataFile(fstream &file) {
    file.open(DATA_FILE, ios::in | ios::out | ios::binary);
    if (file.is_open()) return true;

    ofstream createFile(DATA_FILE, ios::binary);
    if (!createFile.is_open()) return false;
    createFile.close();

    file.clear();
    file.open(DATA_FILE, ios::in | ios::out | ios::binary);
    return file.is_open();
}

long recordCount(fstream &file) {
    file.clear();
    file.seekg(0, ios::end);
    streamoff bytes = file.tellg();
    const streamoff recordSize = static_cast<streamoff>(sizeof(ProblemRecord));
    if (bytes < 0 || bytes % recordSize != 0) return -1;
    return static_cast<long>(bytes / recordSize);
}

bool readRecordAt(fstream &file, long index, ProblemRecord &record) {
    const streamoff position = static_cast<streamoff>(index)
                             * static_cast<streamoff>(sizeof(ProblemRecord));
    file.clear();
    file.seekg(position, ios::beg);
    return static_cast<bool>(
        file.read(reinterpret_cast<char *>(&record), sizeof(ProblemRecord))
    );
}

// CA2 search is retained; CA3 now compares polymorphic objects with operator==.
// Returns the record index, -1 when absent and -2 on a file error.
long findRecordById(int id, ProblemRecord &record) {
    fstream file;
    if (!openDataFile(file)) return -2;

    long total = recordCount(file);
    if (total < 0) return -2;

    PracticeProblem key(id);
    for (long index = 0; index < total; ++index) {
        if (!readRecordAt(file, index, record)) return -2;
        Problem *candidate = createProblem(record);
        bool match = (*candidate == key);
        delete candidate;
        if (match) return index;
    }
    return -1;
}

bool appendRecord(const ProblemRecord &record) {
    fstream file;
    if (!openDataFile(file)) return false;
    file.clear();
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char *>(&record), sizeof(ProblemRecord));
    file.flush();
    return static_cast<bool>(file);
}

bool overwriteRecord(long index, const ProblemRecord &record) {
    fstream file;
    if (!openDataFile(file)) return false;
    const streamoff position = static_cast<streamoff>(index)
                             * static_cast<streamoff>(sizeof(ProblemRecord));
    file.clear();
    file.seekp(position, ios::beg);
    file.write(reinterpret_cast<const char *>(&record), sizeof(ProblemRecord));
    file.flush();
    return static_cast<bool>(file);
}

long storedProblemCount() {
    fstream file;
    if (!openDataFile(file)) return -1;
    return recordCount(file);
}

int readConfirmation(const string &prompt) {
    string answer;
    while (true) {
        cout << prompt;
        if (!getline(cin, answer)) return -1;
        if (answer == "Y" || answer == "y") return 1;
        if (answer == "N" || answer == "n") return 0;
        cout << "Enter Y or N.\n";
    }
}

void displayHeader() {
    cout << "ID\tTitle\tTopic\tLevel\tPoints\tTime\tType\n";
}

bool addProblem() {
    long total = storedProblemCount();
    if (total < 0) {
        cout << "Could not read the data file.\n";
        return true;
    }
    if (total >= MAX_PROBLEMS) {
        cout << "Problem set is full.\n";
        return true;
    }

    PracticeProblem draft;
    if (!(cin >> draft)) return false;

    Problem *problem = createProblem(draft.toRecord());
    ProblemRecord existing = {};
    long existingIndex = findRecordById(static_cast<int>(*problem), existing);
    if (existingIndex == -2) {
        cout << "Could not read the data file.\n";
    } else if (existingIndex >= 0) {
        cout << "Error: ID " << static_cast<int>(*problem) << " already exists.\n";
    } else if (appendRecord(problem->toRecord())) {
        cout << "Problem saved as " << problem->typeLabel() << ".\n";
    } else {
        cout << "Could not save the problem.\n";
    }
    delete problem;
    return true;
}

void releaseProblems(Problem **problems, long count) {
    for (long index = 0; index < count; ++index) delete problems[index];
    delete[] problems;
}

// CA2 View is retained; CA3 adds a dynamic base-pointer array, polymorphism,
// operator< sorting and explicit delete/delete[] cleanup.
void viewAllProblems() {
    fstream file;
    if (!openDataFile(file)) {
        cout << "Could not read the data file.\n";
        return;
    }

    long total = recordCount(file);
    if (total < 0 || total > MAX_PROBLEMS) {
        cout << "Data file has an invalid record count.\n";
        return;
    }
    if (total == 0) {
        cout << "No problems saved.\n";
        return;
    }

    Problem **problems = new Problem *[total];
    long loaded = 0;
    for (; loaded < total; ++loaded) {
        ProblemRecord record = {};
        if (!readRecordAt(file, loaded, record)) {
            cout << "Could not read record " << loaded + 1 << ".\n";
            releaseProblems(problems, loaded);
            return;
        }
        problems[loaded] = createProblem(record);
    }

    // Selection sort deliberately uses the overloaded < operator.
    for (long start = 0; start < loaded - 1; ++start) {
        long smallest = start;
        for (long index = start + 1; index < loaded; ++index) {
            if (*problems[index] < *problems[smallest]) smallest = index;
        }
        if (smallest != start) {
            Problem *temporary = problems[start];
            problems[start] = problems[smallest];
            problems[smallest] = temporary;
        }
    }

    displayHeader();
    for (long index = 0; index < loaded; ++index) {
        cout << *problems[index] << '\n';
    }
    releaseProblems(problems, loaded);
}

bool searchProblem() {
    int id;
    if (!readInt(cin, "Problem ID to search: ", 1, 9999, id)) return false;

    ProblemRecord record = {};
    long index = findRecordById(id, record);
    if (index == -2) {
        cout << "Could not read the data file.\n";
    } else if (index == -1) {
        cout << "Problem not found.\n";
    } else {
        Problem *problem = createProblem(record);
        displayHeader();
        cout << *problem << '\n';
        cout << "Study note: " << problem->studyNote() << '\n';
        delete problem;
    }
    return true;
}

// CA2 seekp update is retained; CA3 adds record display, confirmation and
// operator!= protection so the unique ID cannot be changed.
bool updateProblem() {
    int id;
    if (!readInt(cin, "Problem ID to update: ", 1, 9999, id)) return false;

    ProblemRecord record = {};
    long index = findRecordById(id, record);
    if (index == -2) {
        cout << "Could not read the data file.\n";
        return true;
    }
    if (index == -1) {
        cout << "Problem not found.\n";
        return true;
    }

    Problem *current = createProblem(record);
    cout << "Current record:\n";
    displayHeader();
    cout << *current << '\n';

    int confirmation = readConfirmation("Update this problem? (Y/N): ");
    if (confirmation < 0) {
        delete current;
        return false;
    }
    if (confirmation == 0) {
        cout << "Update cancelled.\n";
        delete current;
        return true;
    }

    cout << "Enter replacement values. The problem ID must remain " << id << ".\n";
    PracticeProblem draft;
    if (!(cin >> draft)) {
        delete current;
        return false;
    }
    if (draft != *current) {
        cout << "Update cancelled: the problem ID cannot be changed.\n";
        delete current;
        return true;
    }

    Problem *replacement = createProblem(draft.toRecord());
    if (overwriteRecord(index, replacement->toRecord())) {
        cout << "Problem updated.\n";
    } else {
        cout << "Could not update the problem.\n";
    }
    delete replacement;
    delete current;
    return true;
}

// CA3 NEW: confirmed Delete rewrites fixed records through a temporary file.
bool deleteRecordAt(long removeIndex) {
    ifstream input(DATA_FILE, ios::binary);
    ofstream output(TEMP_FILE, ios::binary | ios::trunc);
    if (!input.is_open() || !output.is_open()) return false;

    ProblemRecord record = {};
    long index = 0;
    while (input.read(reinterpret_cast<char *>(&record), sizeof(ProblemRecord))) {
        if (index != removeIndex) {
            output.write(reinterpret_cast<const char *>(&record), sizeof(ProblemRecord));
            if (!output) return false;
        }
        ++index;
    }
    if (!input.eof()) return false;
    input.close();
    output.close();
    if (!output) return false;

    remove(BACKUP_FILE);
    if (rename(DATA_FILE, BACKUP_FILE) != 0) {
        remove(TEMP_FILE);
        return false;
    }
    if (rename(TEMP_FILE, DATA_FILE) != 0) {
        rename(BACKUP_FILE, DATA_FILE);
        remove(TEMP_FILE);
        return false;
    }
    remove(BACKUP_FILE);
    return true;
}

bool deleteProblem() {
    int id;
    if (!readInt(cin, "Problem ID to delete: ", 1, 9999, id)) return false;

    ProblemRecord record = {};
    long index = findRecordById(id, record);
    if (index == -2) {
        cout << "Could not read the data file.\n";
        return true;
    }
    if (index == -1) {
        cout << "Problem not found.\n";
        return true;
    }

    Problem *problem = createProblem(record);
    cout << "Record selected for deletion:\n";
    displayHeader();
    cout << *problem << '\n';

    int confirmation = readConfirmation("Delete this problem permanently? (Y/N): ");
    if (confirmation < 0) {
        delete problem;
        return false;
    }
    if (confirmation == 0) {
        cout << "Delete cancelled.\n";
    } else if (deleteRecordAt(index)) {
        cout << "Problem deleted.\n";
    } else {
        cout << "Could not delete the problem; the original file was preserved.\n";
    }
    delete problem;
    return true;
}

int main() {
    fstream initialFile;
    if (!openDataFile(initialFile)) {
        cout << "Error: cannot open " << DATA_FILE << ".\n";
        return 1;
    }
    initialFile.close();

    int choice;
    bool inputAvailable = true;
    while (inputAvailable) {
        cout << "\n=== Coding Contest Problem Set Manager - CA3 ===\n"
             << "1. Add Problem\n"
             << "2. View All Problems\n"
             << "3. Search by ID\n"
             << "4. Update Problem\n"
             << "5. Delete Problem\n"
             << "0. Exit\n";

        if (!readInt(cin, "Choice: ", 0, 5, choice) || choice == 0) break;

        switch (choice) {
            case 1: inputAvailable = addProblem(); break;
            case 2: viewAllProblems(); break;
            case 3: inputAvailable = searchProblem(); break;
            case 4: inputAvailable = updateProblem(); break;
            case 5: inputAvailable = deleteProblem(); break;
        }
    }

    cout << "Goodbye! Live objects: " << Problem::liveObjects << '\n';
    return 0;
}
