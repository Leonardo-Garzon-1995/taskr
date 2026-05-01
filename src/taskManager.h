#pragma once 
#include <string>
#include <vector>
#include "task.h"

class TaskManager {
    public:
        explicit TaskManager(const std::string& filepath);

        void load();
        void save();

        void add(
            const std::string& title,
            Priority priority,
            const std::string& deadline,
            const std::vector<std::string>& tags
        );

        void list() const;
        void filter(const std::string& tag) const;
        bool markDone(int id);
        bool remove(int id);
    
    private:
        std::string filepath_;
        std::vector<Task> tasks_;
        int nextId_ { 1 };

        void printTask(const Task& t) const;

        static std::string joinTags(const std::vector<std::string>& tags);
        static std::vector<std::string> splitTags(const std::string& s);
};