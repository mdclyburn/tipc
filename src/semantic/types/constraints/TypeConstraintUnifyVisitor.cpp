#include "TypeConstraintUnifyVisitor.h"
#include "ConstraintUnifier.h"

TypeConstraintUnifyVisitor::TypeConstraintUnifyVisitor(
  SymbolTable *pTable,
  const std::map<std::string, std::shared_ptr<TipFunction>>& polys) :
  TypeConstraintVisitor(pTable,
                        polys,
                        std::move(buildConstraintHandler()))
{
}

std::unique_ptr<ConstraintHandler> TypeConstraintUnifyVisitor::buildConstraintHandler() {
    return std::make_unique<ConstraintUnifier>();
}
