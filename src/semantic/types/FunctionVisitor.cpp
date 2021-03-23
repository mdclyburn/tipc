#include "FunctionVisitor.h"

#include <iostream>

#include "TypeConstraintCollectVisitor.h"
#include "TypeInference.h"
#include "Unifier.h"

FunctionVisitor::FunctionVisitor(SymbolTable* const syms) :
  ASTVisitor(),
  _symbol_table(syms)
{
}

bool FunctionVisitor::visit(ASTFunction* element)
{
  std::cout << "FunctionVisitor: visiting "
            << element->getName() << std::endl;

  const std::string function_name = element->getName();
  // Skip any function named main.
  if (function_name.compare("main") == 0) {
    std::cout << "Skipping a main function." << std::endl;
    return false;
  } else {
    std::cout << "Performing per-function typing for '"
              << function_name << "'." << std::endl;
  }

  // Perform type inference on this function.
  TypeConstraintCollectVisitor typing_visitor(_symbol_table);
  element->accept(&typing_visitor);
  std::unique_ptr<Unifier> unifier = std::make_unique<Unifier>(
    typing_visitor.getCollectedConstraints());

  std::cout << "Solving constraints for "
            << function_name << std::endl;
  unifier->solve();

  auto fn_inference = std::make_unique<TypeInference>(
    _symbol_table, std::move(unifier));
  std::cout << "INFERENCE FOR "
            << function_name
            << ":" << std::endl;
  fn_inference->print(std::cout);

  return false;
}

void FunctionVisitor::endVisit(ASTFunction* element)
{
  std::cout << "FunctionVisitor: leaving "
            << element->getName() << std::endl;

  return;
}
