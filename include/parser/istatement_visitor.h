#ifndef ISTATEMENT_VISITOR_H
#define ISTATEMENT_VISITOR_H

class ExpressionStatement;
class DefineStatement;

class IStatementVisitor {
    public:
        virtual void visitExpressionStatement(ExpressionStatement& statement) = 0;
        virtual void visitDefineStatement(DefineStatement& statement) = 0;
};

#endif
