#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser/iexpression_visitor.h"
#include "parser/istatement_visitor.h"
#include "parser/statement.h"
#include "runtime/carl_object.h"
#include "runtime/environment.h"
#include <memory>
#include <optional>
#include <stack>
#include <vector>

#define IS_STRING(value)                                                       \
    (value.type == ValueType::VALUE_OBJ &&                                     \
     value.obj->objType == ObjectType::OBJ_STRING)

class Interpreter : public IStatementVisitor, IExpressionVisitor {
private:
    Environment environment;
    std::vector<std::unique_ptr<Statement>> statements;
    std::stack<Value> workingStack;

public:
    std::string result; // TODO: move this to value returned by interpret();
    Interpreter();
    Interpreter(std::unique_ptr<Statement> _statement);
    Interpreter(std::vector<std::unique_ptr<Statement>> _statements);
    std::optional<Value> interpret();
    std::optional<Value> interpret(std::vector<std::unique_ptr<Statement>> statements_);
    virtual void visitExpressionStatement(ExpressionStatement& statement) override;
    virtual void visitDefineStatement(DefineStatement& statement) override;
    virtual void visitApplyStatement(ApplyStatement& statement) override;
    virtual void visitShowStatement(ShowStatement& statement) override;
    virtual void visitLiteralExpression(LiteralExpression &expression) override;
    virtual void visitBinaryExpression(BinaryExpression &expression) override;
    virtual void visitUnaryExpression(UnaryExpression &expression) override;
};

#endif
