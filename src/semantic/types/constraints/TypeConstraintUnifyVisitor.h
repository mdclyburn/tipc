#pragma once

#include "ConstraintHandler.h"
#include "SymbolTable.h"
#include "TypeConstraintVisitor.h"
#include <memory>

/*! \class TypeConstraintUnifyVisitor
 *
 *  \brief Visitor generates type constraints and unifies them on-the-fly.
 */
class TypeConstraintUnifyVisitor: public TypeConstraintVisitor {
public:
  explicit TypeConstraintUnifyVisitor(
    SymbolTable *pTable,
    const std::map<std::string, std::shared_ptr<TipFunction>>& polys);

private:
    static std::unique_ptr<ConstraintHandler> buildConstraintHandler();
};
