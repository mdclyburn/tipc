#include "CalleeIdentifier.h"
#include "SemanticError.h"
#include <sstream>

std::vector<std::string> CalleeIdentifier::build(ASTFunction* func) {
  CalleeIdentifier visitor;
  func->accept(&visitor);
  return visitor.callees;
}

/*
 * By returning false, this implements a "shallow" visit of the program by skipping
 * all of the function bodies.
 */
bool CalleeIdentifier::visit(ASTFunAppExpr* call) {
  std::ostringstream stream;
  stream << *(call->getFunction());
  // Everything's going in there. Eventually quadratic in callsites rather than sites*callees
  callees.push_back(stream.str());
  return false;
}