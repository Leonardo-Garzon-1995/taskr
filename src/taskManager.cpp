#include "taskManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

TaskManager::TaskManager(const std::string& filepath) 
    :filepath_(filepath)
{}

// PERSISTANCE
void TaskManager::load() {
    std::ifstream file(filepath_); // opens the file 
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Format: id|title|priority|deadline|tags|done
        std::istringstream ss(line);
        std::string part;
        Task task;

        std::getline(ss, part, '|'); task.id = std::stoi(part);
        std::getline(ss, part, '|'); task.title = part;
        std::getline(ss, part, '|'); task.priority = priorityFromString(part);
        std::getline(ss, part, '|'); task.deadline = part;
        std::getline(ss, part, '|'); task.tags = splitTags(part);
        std::getline(ss, part, '|'); task.done = (part == "1");

        tasks_.push_back(task);
        if (task.id >= nextId_) nextId_ = task.id +1;
    }
}

void TaskManager::save() const {
    std::ofstream file(filepath_, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "error: could not write to " << filepath_ << '\n';
        return;
    }

    for (const auto& t : tasks_) {
        file << t.id << "|"
            << t.title << "|"
            << priorityToString(t.priority) << "|"
            << t.deadline << "|"
            << joinTags(t.tags) << "|"
            << (t.done ? "1" : "0") << "\n";
    }
}

// OPERATIONS

void TaskManager::add(
    const std::string& title,
    Priority priority,
    const std::string& deadline,
    const std::vector<std::string>& tags) {
        Task t;
        t.id = nextId_++;
        t.title = title;
        t.priority = priority;
        t.deadline = deadline;
        t.tags = tags;
        t.done = false;

        tasks_.push_back(t);

        save();
        std::cout << "added task #" << t.id << ": " << t.title << "\n";
}

void TaskManager::list() const {
    if (tasks_.empty()) {
        std::cout << "no tasks yet. use: taskr add \"title\" [--priority HIGH] [--due YYYY-MM-DD] [--tag work]\n";
        return;
    }

    for (const auto& t : tasks_) {
        printTask(t);
    }
}

void TaskManager::filter(const std::string& tag) const {
    bool found { false };
    for (const auto& t : tasks_) {
        for (const auto& tg : t.tags) {
            if (tag == tg) { printTask(t); found = true; break;}
        }
    }

    if (!found) std::cout << "no tasks tagged \"" << tag << "\"\n";
}

bool TaskManager::markDone(int id) {
    for ( auto& t : tasks_) {
        if (t.id == id) {
            t.done = true;
            save();

            std::cout << "done: [#" << id << "] " << t.title << '\n';
            return true;
        }
    }

    return false;   
}   

bool TaskManager::remove(int id) {
    auto it = std::remove_if(tasks_.begin(), tasks_.end(),
        [id](const Task& t) {return t.id == id;});
    
    if (it == tasks_.end()) return false;
    tasks_.erase(it, tasks_.end());
    save();
    return true;
} 

// DISPLAY

void TaskManager::printTask(const Task& t) const {
    // Done status
    std::cout << (t.done ? "[✓]" : "[]");

    // Priority badge
    std::cout << " [" << priorityToString(t.priority) << "]";

    // ID and title
    std::cout << " #" << t.id << " " << t.title;

    // optional deadline
    if (!t.deadline.empty()) std::cout << " due: " << t.deadline;

    // optional tags
    if (!t.tags.empty()) {
        std::cout << " tags:";
        for (size_t i = 0; i < t.tags.size(); ++i) {
            if (i > 0) std::cout << ",";
            std::cout << t.tags[i]; 
        }
    }

    std::cout << '\n';
}

// SERALIZATION HELPERS

std::string TaskManager::joinTags(const std::vector<std::string>& tags) {
    std::string result;

    for (size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) result += ",";
        result += tags[i];
    }

    return result;
}

std::vector<std::string> TaskManager::splitTags(const std::string& s) {
    std::vector<std::string> tags;
    if (s.empty()) return tags;

    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) tags.push_back(token);
    }

    return tags;
}

