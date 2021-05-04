#pragma once

#include "ASTProgram.h"
#include "ASTNode.h"
#include "SymbolTable.h"
#include "TypeInference.h"
#include <memory>

/*! \class SemanticAnalysis
 *  \brief Stores the results of semantic analysis passes.
 *
 * This class provides the analyze method to run a set of semantic analyses, including
 * l-value checking for assignment statements, proper use of symbols, and type checking.   
 * \sa SymbolTable
 * \sa TypeInference
 */
class SemanticAnalysis {
  std::unique_ptr<SymbolTable> symTable;
  std::unique_ptr<TypeInference> typeResults;

public:
  SemanticAnalysis(std::unique_ptr<SymbolTable> s, std::unique_ptr<TypeInference> t) 
          : symTable(std::move(s)), typeResults(std::move(t)) {}

  /*! \fn analyze
   *  \brief Perform semantic analysis on program AST.
   *
   * Run weeding, symbol, and type checking analysis.  Errors in any of these
   * result in a SemanticError.  If no errors then ownership of semantic analysis
   * results are transfered to caller.
   * \sa SemanticError
   * \param ast The program AST
   * \return The unique pointer to the semantic analysis structure.
   */
  static std::unique_ptr<SemanticAnalysis> analyze(ASTProgram* ast, bool dirrec); 

  /*! \fn getSymbolTable
   *  \brief Returns the symbol table computed for the program.
   * \sa SymbolTable
   */
  SymbolTable* getSymbolTable(); 

  /*! \fn getTypeResults
   *  \brief Returns the type inferrence results.
   * \sa TypeInference
   */
  TypeInference* getTypeResults();
};
