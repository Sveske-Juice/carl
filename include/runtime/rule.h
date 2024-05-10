#ifndef RULE_H
#define RULE_H

#include "parser/expression.h"

#include <memory>

class Rule {
private:
    std::unique_ptr<Expression> pattern_;
    std::unique_ptr<Expression> replacement_;

public:
    Rule(std::unique_ptr<Expression> pattern,
         std::unique_ptr<Expression> replacement)
        : pattern_{std::move(pattern)}, replacement_{std::move(replacement)} {}
    Rule(Expression &pattern, Expression &replacement)
        : pattern_{std::unique_ptr<Expression>(&pattern)},
          replacement_{std::unique_ptr<Expression>(&replacement)} {}

    std::unique_ptr<Expression> borrowPattern() { return std::move(pattern_); }
    std::unique_ptr<Expression> borrowReplacement() { return std::move(replacement_); }
};

#endif
