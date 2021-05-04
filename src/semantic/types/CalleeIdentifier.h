#pragma once

#include "ASTVisitor.h"

/*! \class CalleeIdentifier
 *  \brief Collects the names of functions declared in the program.
 *
 * The function name builder visits a restricted set of nodes, just functions,
 * It records the name in the function map and checks that a name is declared at most once.
 * Errors are reported by throwing SemanticError exceptions.
 * \sa SemanticError
 */
class CalleeIdentifier : public ASTVisitor {
public:
  CalleeIdentifier() = default;
  // this vector is public so that the static method can access it
  std::vector<std::string> callees;
  static std::vector<std::string> build(ASTFunction* func);
  virtual bool visit(ASTFunAppExpr* call) override;
};