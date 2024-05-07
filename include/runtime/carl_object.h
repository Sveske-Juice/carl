#ifndef CARL_OBJECT_H
#define CARL_OBJECT_H

#include <cstdint>
#include <exception>

enum ValueType {
    VALUE_BOOL,
    VALUE_NULL,
    VALUE_NUMBER,
    VALUE_OBJ,
};

enum ObjectType {
    OBJ_STRING,
};

struct Obj {
    ObjectType objType;
};

struct ObjString {
    Obj obj;
    uint32_t length;
    char* chars;

    ~ObjString() {
        delete chars;
        chars = nullptr;
    }
};

struct Value {
    ValueType type;
    union {
        double number;
        Obj* obj;
    };

    void dispose() {
        if (type != ValueType::VALUE_OBJ)
            return;

        // Since we type pun we need to manually call "derived" objects
        // destructor instead of just 'delete obj'
        switch (obj->objType) {
            case OBJ_STRING:
                reinterpret_cast<ObjString* >(obj)->~ObjString();
                break;

            default:
                throw std::exception();
        }
        obj = nullptr;
    }
};


#endif
