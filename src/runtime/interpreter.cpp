#include "runtime/interpreter.h"
#include "lexer/token.h"
#include "parser/expression.h"
#include "runtime/carl_object.h"
#include "runtime/runtime_errors.h"
#include <cstring>
#include <fmt/core.h>
#include <iostream>

Interpreter::Interpreter(std::unique_ptr<Expression> _rootExpression)
    : rootExpression(std::move(_rootExpression)) {}

Value Interpreter::interpret() {
    rootExpression->accept(*this);
    while (workingStack.size() != 0) {
        const Value &res = workingStack.top();
        if (res.type == ValueType::VALUE_OBJ) {
            std::cout << "Object result: {" << res.type << ", "
                      << reinterpret_cast<ObjString *>(res.obj)->chars << "}"
                      << std::endl;
        } else {
            std::cout << "Primitive result: {" << res.type << ", " << res.number
                      << "}" << std::endl;
        }
        if (workingStack.size() == 1)
            break;
        workingStack.top().dispose();
        workingStack.pop();
    }
    return workingStack.top();
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
    const Value &operand = workingStack.top();
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
        ObjString* left = reinterpret_cast<ObjString *>(l.obj);
        ObjString* right = reinterpret_cast<ObjString *>(r.obj);

        size_t resultLength = left->length + right->length;
        char * resultStr = new char[resultLength + 1];
        std::memcpy(resultStr, left->chars, left->length);
        std::memcpy(resultStr + left->length, right->chars, right->length);
        resultStr[resultLength] = '\0';

        ObjString* resultObjString = new ObjString();
        resultObjString->length = resultLength;
        resultObjString->chars = resultStr;
        resultObjString->obj.objType = ObjectType::OBJ_STRING;
        result.type = ValueType::VALUE_OBJ;
        result.obj = reinterpret_cast<Obj *>(resultObjString);
    } else {
        throw TypeMismatch(
            op, "Type Mismatch, binary operation expects two numbers or two strings");
    }
    workingStack.push(result);
}
