#include "TypeConstraintCollectVisitor.h"
#include "ConstraintCollector.h"

TypeConstraintCollectVisitor::TypeConstraintCollectVisitor(
  SymbolTable *pTable,
  const std::map<std::string, std::shared_ptr<TipFunction>>& polys)
  : TypeConstraintVisitor(pTable,
                          polys,
                          std::move(buildConstraintHandler()))
{
}

std::unique_ptr<ConstraintHandler> TypeConstraintCollectVisitor::buildConstraintHandler() {
    return std::make_unique<ConstraintCollector>();
}

std::vector<TypeConstraint> &TypeConstraintCollectVisitor::getCollectedConstraints() {
    auto handler = dynamic_cast<ConstraintCollector *>(constraintHandler.get());
    if(!handler) {
        assert(0);
    }
    return handler->getCollectedConstraints();
}
