#include "PolymorphicIdentifierVisitor.h"

#include <iostream>

#include "TipFunction.h"
#include "TypeConstraintCollectVisitor.h"
#include "TypeInference.h"
#include "Unifier.h"

PolymorphicIdentifierVisitor::PolymorphicIdentifierVisitor(SymbolTable* const syms) :
  ASTVisitor(),
  _symbol_table(syms)
{
  _fn_calls_fns = false;
}

bool PolymorphicIdentifierVisitor::visit(ASTFunction* element)
{
  const std::string function_name = element->getName();
  // Skip any function named main.
  if (function_name.compare("main") == 0) {
    return false;
  }

  // Perform type inference on this function.
  // Note: the symbol table provided to the visitor is one
  // for the entire program and is not specific to the function
  // currently under analysis.
  TypeConstraintCollectVisitor typing_visitor(_symbol_table);
  element->accept(&typing_visitor);

  // Produce a solution from the collected constraints.
  std::unique_ptr<Unifier> unifier = std::make_unique<Unifier>(
    typing_visitor.getCollectedConstraints());
  unifier->solve();

  auto fn_inference = std::make_unique<TypeInference>(
    _symbol_table, std::move(unifier));

  // Get a type inference for the given function. If it contains a
  // free variable, then it is a polymorphic function.
  ASTDeclNode* const decl = _symbol_table->getFunction(function_name);
  auto inferred_type = fn_inference->getInferredType(decl);
  if (inferred_type->containsFreeVariable()) {
    // We want to look for function calls in this function.
    _current_fn = element;
    _fn_calls_fns = false;
    return true;
  } else {
    // We do not care to traverse into monomorphic functions.
    _current_fn = std::nullopt;
    return false;
  }

  return true;
}

bool PolymorphicIdentifierVisitor::visit(ASTFunAppExpr* element)
{
  _fn_calls_fns = true;
  return false;
}

void PolymorphicIdentifierVisitor::endVisit(ASTFunction* element)
{
  const std::string function_name = element->getName();
  std::cout << "Finished polymorphic analysis for"
            << " '" << function_name << "': ";
  if (_current_fn.has_value() && !_fn_calls_fns) {
    std::cout << "non-recursive";
    _polymorphic_fns.emplace(function_name);
  } else if (_current_fn.has_value() && _fn_calls_fns) {
    std::cout << "possibly recursive";
  } else {
    std::cout << "monomorphic";
  }
  std::cout << std::endl;

  return;
}

const std::unordered_set<std::string>&
PolymorphicIdentifierVisitor::polymorphicFunctions() const {
  return _polymorphic_fns;
}
