#include <iostream>
#include <string>
#include <vector>
#include "taskManager.h"
#include <filesystem>

static void printUsage() {
    std::cout <<
        "usage:\n"
        "  taskr add \"title\" [--priority LOW|MEDIUM|HIGH] [--due YYYY-MM-DD] [--tag t1 --tag t2]\n"
        "  taskr list\n"
        "  taskr done <id>\n"
        "  taskr delete <id>\n"
        "  taskr filter <tag>\n";
}

// Safe integer parsing — returns false and prints an error if input is not numeric.
static bool parseInt(const std::string& s, int& out) {
    try {
        size_t pos;
        out = std::stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        std::cerr << "error: \"" << s << "\" is not a valid id.\n";
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printUsage(); return 1; }\
    std::filesystem::create_directories("data");
    TaskManager mgr("data/tasks.txt");
    
    mgr.load();

    std::string cmd { argv[1] };

    if (cmd == "add") {
        if (argc < 3) { std::cerr << "error: missing task title\n"; return 1; }

        std::string title = argv[2];
        Priority priority = Priority::MEDIUM;
        std::string deadline;
        std::vector<std::string> tags;
        
        // Parse optional flags
        for (int i = 3; i < argc; ++i) {
            std::string flag { argv[i] };

            if (flag == "--priority" && i + 1 < argc) {
                priority = priorityFromString(argv[++i]);
            } else if (flag == "--due" && i + 1 < argc) {
                deadline = argv[++i];
            } else if (flag == "--tag" && i + 1 < argc) {
                tags.push_back(argv[++i]);
            } else {
                std::cerr << "warning: unknown flag \"" << flag << "\"\n";
            }
        }

        mgr.add(title, priority, deadline, tags);
    } else if (cmd == "list" ) {
        mgr.list();
    } else if (cmd == "done") {
        if (argc < 3) { std::cerr << "error: missing task id\n"; return 1; }

        int id;

        if (!parseInt(argv[2], id)) return 1;
        if (!mgr.markDone(id)) std::cerr << "error: task #" << id << " not found\n";
    } else if (cmd == "delete") {
        if (argc < 3) { std::cerr << "error: missing task id\n"; return 1; }

        int id;
        if (!parseInt(argv[2], id)) return 1;

        if (!mgr.remove(id)) {
            std::cerr << "error: task #" << id << " not found\n";
        } else {
            std::cout << "deleted task #" << id << "\n";
        }  
    } else if (cmd == "filter") {
        if (argc < 3) { std::cerr << "error: missing tag\n"; return 1; }
        mgr.filter(argv[2]);
    } else if (cmd == "clear") {
        mgr.clearAll();
    } else {
        std::cerr << "unknown command: " << cmd << "\n";
        printUsage();
        return 1;
    }

    return 0;

}