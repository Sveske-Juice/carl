#include "runtime/interpreter.h"
#include "lexer/token.h"
#include "parser/expression.h"
#include "parser/statement.h"
#include "runtime/carl_object.h"
#include "runtime/expressionrewriter.h"
#include "runtime/runtime_errors.h"
#include <cstring>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <vector>

Interpreter::Interpreter(std::vector<std::unique_ptr<Statement>> _statements)
    : statements(std::move(_statements)) {}

Interpreter::Interpreter(std::unique_ptr<Statement> _statement) {
    statements.push_back(std::move(_statement));
}

std::optional<Value> Interpreter::interpret() {
    for (int i = 0; i < statements.size(); i++) {
        statements[i]->accept(*this);
    }

    // Statements might not produce a value
    if (workingStack.size() == 0)
        return {};

    return workingStack.top();
}

void Interpreter::visitExpressionStatement(ExpressionStatement &statement) {
    statement.expression().accept(*this);
}

void Interpreter::visitDefineStatement(DefineStatement &statement) {
    // NOTE: this invalidates the statement since we move the pattern & replacement. Is this okay?
    std::unique_ptr<Rule> newDefinition = std::make_unique<Rule>(statement.borrowPattern(), statement.borrowReplacement());
    environment.addDefinition(statement.ruleName(), std::move(newDefinition));
}

void Interpreter::visitApplyStatement(ApplyStatement& statement) {
    auto definition = environment.getDefinition(statement.ruleName());
    if (definition == environment.end()) {
        throw NoRuleFound(Token(TokenType::APPLY, 0, 5, "apply"), "Definition '" + statement.ruleName() + "' is not defined");
    }
    ExpressionRewriter rewriter;
    auto result = rewriter.substitute(statement.borrowExpression(), definition->second->borrowPattern(), definition->second->borrowReplacement());
    std::cout << result->toString() << std::endl;
}

void Interpreter::visitLiteralExpression(LiteralExpression &expression) {
    switch (expression.type()) {
    case TokenType::STRING: {
        Value value;
        value.type = ValueType::VALUE_OBJ;

        // TODO: custom allocator
        ObjString *objStr = new ObjString();
        objStr->obj.objType = OBJ_STRING;

        objStr->length = expression.literal().size();
        objStr->chars = new char[objStr->length + 1];
        std::memcpy(objStr->chars, expression.literal().c_str(),
                    objStr->length);
        objStr->chars[objStr->length] = '\0';

        value.obj = reinterpret_cast<Obj *>(objStr);
        workingStack.push(value);
        break;
    }
    case TokenType::TRUE: {
        Value value;
        value.type = ValueType::VALUE_BOOL;
        value.number = 1.0; // 1 is true
        workingStack.push(value);
        break;
    }
    case TokenType::FALSE: {
        Value value;
        value.type = ValueType::VALUE_BOOL;
        value.number = 0.0; // 0 is false
        workingStack.push(value);
        break;
    }
    case TokenType::NUMBER: {
        Value value;
        value.type = ValueType::VALUE_NUMBER;
        value.number = std::stod(expression.literal()); // TODO: check for err
        workingStack.push(value);
        break;
    }

    default:
        throw std::exception(); // TODO: interpreter bug
    }
}

void Interpreter::visitUnaryExpression(UnaryExpression &expression) {
    // Eval operand
    expression.operand().accept(*this);
    Value operand = workingStack.top();
    workingStack.pop();
    const Token &op = expression.op();

    switch (op.type()) {
    case TokenType::MINUS:
        if (operand.type != ValueType::VALUE_NUMBER)
            throw TypeMismatch(op, "Type Mismatch, because can't negate a type "
                                   "that's not a number");

        Value negated;
        negated.type = ValueType::VALUE_NUMBER;
        negated.number = -operand.number;

        workingStack.push(negated);
        break;

    case TokenType::BANG: {
        if (operand.type != ValueType::VALUE_BOOL)
            throw TypeMismatch(
                op, "Type Mismatch, because can't NOT a non-boolean value");

        Value nutted; // hehe
        nutted.type = ValueType::VALUE_BOOL;
        nutted.number = operand.number == 1.0 ? 0.0 : 1.0;

        workingStack.push(nutted);
        break;
    }

    default:
        throw std::exception(); // TODO: interpreter bug
    }

    operand.dispose();
}

void Interpreter::visitBinaryExpression(BinaryExpression &expression) {
    // Eval left
    expression.left().accept(*this);
    Value l = workingStack.top();
    workingStack.pop();

    // Eval right
    expression.right().accept(*this);
    Value r = workingStack.top();
    workingStack.pop();

    const Token &op = expression.op();

    Value result;
    if (l.type == VALUE_NUMBER && r.type == VALUE_NUMBER) {
        result.type = ValueType::VALUE_NUMBER;
        switch (op.type()) {
        case PLUS:
            result.number = l.number + r.number;
            break;

        case MINUS:
            result.number = l.number - r.number;
            break;

        case STAR:
            result.number = l.number * r.number;
            break;

        case SLASH:
            result.number = l.number / r.number;
            break;

        default:
            throw std::exception(); // TODO: interpreter bug
        }
        // String concat
    } else if (IS_STRING(l) && IS_STRING(r) && op.type() == TokenType::PLUS) {
        ObjString *left = reinterpret_cast<ObjString *>(l.obj);
        ObjString *right = reinterpret_cast<ObjString *>(r.obj);

        size_t resultLength = left->length + right->length;
        char *resultStr = new char[resultLength + 1];
        std::memcpy(resultStr, left->chars, left->length);
        std::memcpy(resultStr + left->length, right->chars, right->length);
        resultStr[resultLength] = '\0';

        ObjString *resultObjString = new ObjString();
        resultObjString->length = resultLength;
        resultObjString->chars = resultStr;
        resultObjString->obj.objType = ObjectType::OBJ_STRING;
        result.type = ValueType::VALUE_OBJ;
        result.obj = reinterpret_cast<Obj *>(resultObjString);
    } else {
        throw TypeMismatch(op, "Type Mismatch, binary operation expects two "
                               "numbers or two strings");
    }

    l.dispose();
    r.dispose();
    workingStack.push(result);
}
