#include "runtime/interpreter.h"
#include "lexer/token.h"
#include "parser/expression.h"
#include "runtime/carl_object.h"
#include <cstring>
#include <iostream>

Interpreter::Interpreter(std::unique_ptr<Expression> _rootExpression) : rootExpression(std::move(_rootExpression)) {}

Value Interpreter::interpret() {
    rootExpression->accept(*this);
    while (workingStack.size() != 0) {
        const Value& res = workingStack.top();
        if (res.type == ValueType::VALUE_OBJ){
            std::cout << "Object result: {" << res.type  << ", " << reinterpret_cast<ObjString*>(res.obj)->chars << "}" << std::endl;
        }
        else {
            std::cout << "Primitive result: {" << res.type << ", " << res.number << "}" << std::endl;
        }
        if (workingStack.size() == 1)
            break;
        workingStack.top().dispose();
        workingStack.pop();
    }
    return workingStack.top();
}

void Interpreter::visitLiteralExpression(LiteralExpression& expression) {
    switch (expression.type()) {
        case TokenType::STRING:
        {
            Value value;
            value.type = ValueType::VALUE_OBJ;

            // TODO: custom allocator
            ObjString* objStr = new ObjString();
            objStr->obj.objType = OBJ_STRING;

            objStr->length = expression.literal().size();
            objStr->chars = new char[objStr->length + 1];
            std::memcpy(objStr->chars, expression.literal().c_str(), objStr->length);
            objStr->chars[objStr->length] = '\0';

            value.obj = reinterpret_cast<Obj *>(objStr);
            workingStack.push(value);
            break;
        }
        case TokenType::TRUE:
        {
            Value value;
            value.type = ValueType::VALUE_BOOL;
            value.number = 1.0; // 1 is true
            workingStack.push(value);
            break;
        }
        case TokenType::FALSE:
        {
            Value value;
            value.type = ValueType::VALUE_BOOL;
            value.number = 0.0; // 0 is false
            workingStack.push(value);
            break;
        }
        case TokenType::NUMBER:
        {
            Value value;
            value.type = ValueType::VALUE_NUMBER;
            value.number = std::stod(expression.literal()); // TODO: check for err
            workingStack.push(value);
            break;
        }

        default:
            throw std::exception(); // TODO: runtime err
    }
}

void Interpreter::visitUnaryExpression(UnaryExpression& expression) {
    // TODO: 
}

void Interpreter::visitBinaryExpression(BinaryExpression& expression) {
    // TODO: 
}
