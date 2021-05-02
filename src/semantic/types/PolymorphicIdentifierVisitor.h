#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>

#include "ASTFunction.h"
#include "ASTFunAppExpr.h"
#include "ASTVisitor.h"
#include "SymbolTable.h"
#include "TipFunction.h"

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
  const std::set<std::string>& polymorphicFunctions() const;

  const std::map<std::string, std::shared_ptr<TipFunction>>&
  inferences();

  std::map<std::string, std::shared_ptr<TipFunction>>
  polymorphicInferences();

private:
  SymbolTable* const _symbol_table;
  // Function currently inspected during AST traversal.
  std::optional<ASTFunction*> _current_fn;
  bool _fn_calls_fns;
  // Names of functions identified as polymorphic.
  std::set<std::string> _polymorphic_fns;
  // Type inferences of polymorphic functions.
  std::map<std::string, std::shared_ptr<TipFunction>> _inferences;
};
