// Coding Contest Problem Set Manager - CAP455 CA2 (Persist & Search)
// Fixed-size binary records with persistent Add, View, Search and Update.
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
const char DATA_FILE[] = "data/12611227_CAP455_CA2_ProblemSetManager.dat";

// Plain fixed-size structure used only for binary file storage.
struct ProblemRecord {
    int id;
    char title[TITLE_SIZE];
    char topic[TOPIC_SIZE];
    char difficulty[DIFFICULTY_SIZE];
    int points;
    int timeLimit;
};

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

    // Default constructor
    Problem()
        : id(0), title(""), topic(""), difficulty(""), points(0), timeLimit(0) {
        ++liveObjects;
    }

    // Parameterised constructor
    Problem(int id, const string &title, const string &topic,
            const string &difficulty, int points, int timeLimit)
        : id(id), title(title), topic(topic), difficulty(difficulty),
          points(points), timeLimit(timeLimit) {
        ++liveObjects;
    }

    // Copy constructor
    Problem(const Problem &other)
        : id(other.id), title(other.title), topic(other.topic),
          difficulty(other.difficulty), points(other.points),
          timeLimit(other.timeLimit) {
        ++liveObjects;
    }

    // Destructor
    ~Problem() {
        --liveObjects;
    }

    void setData(int id, const string &title, const string &topic,
                 const string &difficulty, int points, int timeLimit) {
        this->id = id;
        this->title = title;
        this->topic = topic;
        this->difficulty = difficulty;
        this->points = points;
        this->timeLimit = timeLimit;
    }

    inline int getId() const { return id; }

    ProblemRecord toRecord() const {
        ProblemRecord record = {};
        record.id = id;
        copyText(record.title, TITLE_SIZE, title);
        copyText(record.topic, TOPIC_SIZE, topic);
        copyText(record.difficulty, DIFFICULTY_SIZE, difficulty);
        record.points = points;
        record.timeLimit = timeLimit;
        return record;
    }

    friend void display(const Problem &problem);
};

int Problem::liveObjects = 0;

void display(const Problem &problem) {
    cout << problem.id << '\t' << problem.title << '\t' << problem.topic << '\t'
         << problem.difficulty << '\t' << problem.points << '\t'
         << problem.timeLimit << "s\n";
}

Problem problemFromRecord(ProblemRecord record) {
    terminateRecordText(record);
    return Problem(record.id, record.title, record.topic, record.difficulty,
                   record.points, record.timeLimit);
}

bool readInt(const string &prompt, int low, int high, int &value) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) return false;

        istringstream input(line);
        int candidate;
        char extra;
        if (input >> candidate && !(input >> extra)
            && candidate >= low && candidate <= high) {
            value = candidate;
            return true;
        }
        cout << "Invalid. Enter an integer from " << low << " to " << high << ".\n";
    }
}

bool readText(const string &prompt, int maxLength, string &value) {
    while (true) {
        cout << prompt;
        if (!getline(cin, value)) return false;

        if (value.find_first_not_of(" \t\r\n") == string::npos) {
            cout << "Text cannot be blank.\n";
        } else if (static_cast<int>(value.size()) > maxLength) {
            cout << "Text must contain at most " << maxLength << " characters.\n";
        } else {
            return true;
        }
    }
}

bool readDifficulty(string &difficulty) {
    while (true) {
        if (!readText("Difficulty (Easy/Medium/Hard): ",
                      DIFFICULTY_SIZE - 1, difficulty)) {
            return false;
        }
        if (difficulty == "Easy" || difficulty == "Medium" || difficulty == "Hard") {
            return true;
        }
        cout << "Difficulty must be Easy, Medium or Hard.\n";
    }
}

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
    file.seekg(position, ios::beg); // CA2: random-access read with seekg
    return static_cast<bool>(
        file.read(reinterpret_cast<char *>(&record), sizeof(ProblemRecord))
    );
}

// Returns record index, -1 when absent, and -2 on a file error.
long findRecordById(int id, ProblemRecord &record) {
    fstream file;
    if (!openDataFile(file)) return -2;

    long total = recordCount(file);
    if (total < 0) return -2;

    for (long index = 0; index < total; ++index) {
        if (!readRecordAt(file, index, record)) return -2;
        if (record.id == id) return index;
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
    file.seekp(position, ios::beg); // CA2: in-place update with seekp
    file.write(reinterpret_cast<const char *>(&record), sizeof(ProblemRecord));
    file.flush();
    return static_cast<bool>(file);
}

long storedProblemCount() {
    fstream file;
    if (!openDataFile(file)) return -1;
    return recordCount(file);
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

    int id;
    if (!readInt("Problem ID (1-9999): ", 1, 9999, id)) return false;

    ProblemRecord existing = {};
    long existingIndex = findRecordById(id, existing);
    if (existingIndex == -2) {
        cout << "Could not read the data file.\n";
        return true;
    }
    if (existingIndex >= 0) {
        cout << "Error: ID " << id << " already exists.\n";
        return true;
    }

    string title, topic, difficulty;
    int points, timeLimit;
    if (!readText("Title (max 50 characters): ", TITLE_SIZE - 1, title)) return false;
    if (!readText("Topic (max 30 characters): ", TOPIC_SIZE - 1, topic)) return false;
    if (!readDifficulty(difficulty)) return false;
    if (!readInt("Points (10-1000): ", 10, 1000, points)) return false;
    if (!readInt("Time limit in seconds (1-10): ", 1, 10, timeLimit)) return false;

    Problem problem(id, title, topic, difficulty, points, timeLimit);
    if (appendRecord(problem.toRecord())) cout << "Problem saved.\n";
    else cout << "Could not save the problem.\n";
    return true;
}

void viewAllProblems() {
    fstream file;
    if (!openDataFile(file)) {
        cout << "Could not read the data file.\n";
        return;
    }

    long total = recordCount(file);
    if (total < 0) {
        cout << "Data file has an invalid size.\n";
        return;
    }
    if (total == 0) {
        cout << "No problems saved.\n";
        return;
    }

    Problem problems[MAX_PROBLEMS];
    int loaded = 0;
    long limit = total < MAX_PROBLEMS ? total : MAX_PROBLEMS;

    for (long index = 0; index < limit; ++index) {
        ProblemRecord record = {};
        if (!readRecordAt(file, index, record)) {
            cout << "Could not read record " << index + 1 << ".\n";
            return;
        }
        terminateRecordText(record);
        problems[loaded].setData(record.id, record.title, record.topic,
                                 record.difficulty, record.points, record.timeLimit);
        ++loaded;
    }

    cout << "ID\tTitle\tTopic\tLevel\tPoints\tTime\n";
    Problem *current = problems;
    for (int index = 0; index < loaded; ++index, ++current) {
        display(*current);
    }
}

bool searchProblem() {
    int id;
    if (!readInt("Problem ID to search: ", 1, 9999, id)) return false;

    ProblemRecord record = {};
    long index = findRecordById(id, record);
    if (index == -2) {
        cout << "Could not read the data file.\n";
    } else if (index == -1) {
        cout << "Problem not found.\n";
    } else {
        Problem problem = problemFromRecord(record);
        cout << "ID\tTitle\tTopic\tLevel\tPoints\tTime\n";
        display(problem);
    }
    return true;
}

bool updateProblem() {
    int id;
    if (!readInt("Problem ID to update: ", 1, 9999, id)) return false;

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

    Problem current = problemFromRecord(record);
    cout << "Current record:\n";
    display(current);
    cout << "Enter replacement values (the ID remains " << id << ").\n";

    string title, topic, difficulty;
    int points, timeLimit;
    if (!readText("New title (max 50 characters): ", TITLE_SIZE - 1, title)) return false;
    if (!readText("New topic (max 30 characters): ", TOPIC_SIZE - 1, topic)) return false;
    if (!readDifficulty(difficulty)) return false;
    if (!readInt("New points (10-1000): ", 10, 1000, points)) return false;
    if (!readInt("New time limit in seconds (1-10): ", 1, 10, timeLimit)) return false;

    Problem updated(current); // meaningful use of the copy constructor
    updated.setData(id, title, topic, difficulty, points, timeLimit);

    if (overwriteRecord(index, updated.toRecord())) cout << "Problem updated.\n";
    else cout << "Could not update the problem.\n";
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
        cout << "\n=== Coding Contest Problem Set Manager - CA2 ===\n"
             << "1. Add Problem\n"
             << "2. View All Problems\n"
             << "3. Search by ID\n"
             << "4. Update Problem\n"
             << "0. Exit\n";

        if (!readInt("Choice: ", 0, 4, choice) || choice == 0) break;

        switch (choice) {
            case 1: inputAvailable = addProblem(); break;
            case 2: viewAllProblems(); break;
            case 3: inputAvailable = searchProblem(); break;
            case 4: inputAvailable = updateProblem(); break;
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
