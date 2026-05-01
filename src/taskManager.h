#pragma once
#include <string>
#include <vector>

// enum class: scoped, type-safe. You cant accidentally compare priority to integer

enum class Priority {LOW, MEDIUM, HIGH};

// inline -> makes safer to define a function in a header, and avoids linker errors in multiple files
inline std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
    }

    return "LOW";
}

inline Priority priorityFromString(const std::string& s) {
    if (s == "HIGH") return Priority::HIGH;
    if (s == "MEDIUM") return Priority::MEDIUM;
    return Priority::LOW;
}

struct Task {
    int id { 0 };
    std::string title;
    Priority priority { Priority::MEDIUM };
    std::string deadline;
    std::vector<std::string> tags;
    bool done { false }; 
};
