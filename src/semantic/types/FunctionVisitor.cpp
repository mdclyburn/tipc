#include <iostream>

#include "FunctionVisitor.h"

FunctionVisitor::FunctionVisitor() :
    ASTVisitor()
{
}

bool FunctionVisitor::visit(ASTFunction* element)
{
  std::cout << "FunctionVisitor: visiting "
            << element->getName() << std::endl;

  return false;
}

void FunctionVisitor::endVisit(ASTFunction* element)
{
  std::cout << "FunctionVisitor: leaving "
            << element->getName() << std::endl;

  return;
}
