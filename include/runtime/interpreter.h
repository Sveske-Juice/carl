#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser/iexpression_visitor.h"
#include "runtime/carl_object.h"
#include <memory>
#include <stack>

#define IS_STRING(value)                                                       \
    (value.type == ValueType::VALUE_OBJ &&                                     \
     value.obj->objType == ObjectType::OBJ_STRING)

class Interpreter : public IExpressionVisitor {
private:
    const std::unique_ptr<Expression> rootExpression;
    std::stack<Value> workingStack;

public:
    Interpreter(std::unique_ptr<Expression> _rootExpression);
    Value interpret();
    virtual void visitLiteralExpression(LiteralExpression &expression) override;
    virtual void visitBinaryExpression(BinaryExpression &expression) override;
    virtual void visitUnaryExpression(UnaryExpression &expression) override;
};

#endif
