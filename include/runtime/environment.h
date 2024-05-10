#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "runtime/rule.h"

#include <string>
#include <unordered_map>
#include <memory>

class Environment {
private:
    std::unordered_map<std::string, std::unique_ptr<Rule>> definitions;
public:
    void addDefinition(std::string name, std::unique_ptr<Rule> definition) {
        // TODO: handle rule already exists
        definitions[name] = std::move(definition);
    }
};

#endif
