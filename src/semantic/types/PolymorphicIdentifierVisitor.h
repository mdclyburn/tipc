#pragma once

#include <memory>
#include <optional>
#include <unordered_set>

#include "ASTFunction.h"
#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "SymbolTable.h"

/** Visitor that identifies polymorphic functions in TIP programs.
 *
 * This visitor makes type inferences for each function encountered in the program (except for 'main').
 * When a function's arguments or return value are free variables, the function is considered polymorphic.
 * Use PolymorhpicIdentifierVisitor.h::isPolymorphic() to query for polymorphic functions.
 */
class PolymorphicIdentifierVisitor : public ASTVisitor {
public:
  /** Constructor.
   *
   * \param syms Symbol table for the program under analysis.
   */
  explicit PolymorphicIdentifierVisitor(SymbolTable* const syms);

  bool visit(ASTFunction* element) override;
  bool visit(ASTFunAppExpr* element) override;

  void endVisit(ASTFunction* element) override;

  /** Returns a reference to the collection of polymorphic functions.
   */
  const std::unordered_set<std::string>& polymorphicFunctions() const;

private:
  SymbolTable* const _symbol_table;
  // Function currently inspected during AST traversal.
  std::optional<ASTFunction*> _current_fn;
  bool _fn_calls_fns;
  // Names of functions identified as polymorphic.
  std::unordered_set<std::string> _polymorphic_fns;
};
