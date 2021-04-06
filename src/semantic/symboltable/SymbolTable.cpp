#include "SymbolTable.h"
#include "FunctionNameCollector.h"
#include "LocalNameCollector.h"
#include "FieldNameCollector.h"

#include <algorithm>
#include <sstream>
#include <iostream>

std::unique_ptr<SymbolTable> SymbolTable::build(ASTProgram* p) {
  auto fMap = FunctionNameCollector::build(p);
  auto lMap = LocalNameCollector::build(p, fMap);
  auto fSet = FieldNameCollector::build(p);
  return std::make_unique<SymbolTable>(fMap, lMap, fSet);
}

std::unique_ptr<SymbolTable> SymbolTable::build(ASTProgram* const p,
                                                const std::string& function_name) {
  // Perhaps a better option would be to create an overload on the collector?
  auto f_map = FunctionNameCollector::build(p);
  std::map<std::string, ASTDeclNode*> new_f_map;
  if (f_map.count(function_name) > 0)
    new_f_map.emplace(function_name, f_map.at(function_name));

  // What effect does carrying all of this over have?
  // There are names in the symbol table that are not in the function of interest...
  auto l_map = LocalNameCollector::build(p, new_f_map);
  auto f_set = FieldNameCollector::build(p);

  return std::make_unique<SymbolTable>(new_f_map, l_map, f_set);
}

ASTDeclNode* SymbolTable::getFunction(std::string s) {
  auto func = functionNames.find(s);
  if(func == functionNames.end()) {
    return nullptr;
  }
  return func->second;
}

std::vector<ASTDeclNode*> SymbolTable::getFunctions() {
  std::vector<ASTDeclNode*> funDecls;
  for (auto &pair : functionNames) {
    funDecls.push_back(pair.second);
  }
  return funDecls;
}

ASTDeclNode* SymbolTable::getLocal(std::string s, ASTDeclNode* f) {
  auto lMap = localNames.find(f)->second;
  auto local = lMap.find(s);
  if(local == lMap.end()) {
    return nullptr;
  }
  return local->second;
}

std::vector<ASTDeclNode*> SymbolTable::getLocals(ASTDeclNode* f) {
  auto lMap = localNames.find(f)->second;
  std::vector<ASTDeclNode*> localDecls;
  for (auto &pair : lMap) {
    localDecls.push_back(pair.second);
  }
  return localDecls;
}


std::vector<std::string> SymbolTable::getFields() {
  return fieldNames;
}

void SymbolTable::print(std::ostream &s) {
  s << "Functions : {";
  auto skip = true;
  for (auto e : functionNames) {
    if (skip) {
      skip = false;
      s << e.first;
      continue;
    }
    s << ", " + e.first;
  }
  s << "}\n";

  s << "Fields : {";
  skip = true;
  for (auto n : fieldNames) {
    if (skip) {
      skip = false;
      s << n;
      continue;
    }
    s << ", " + n;
  }
  s << "}\n";

  for (auto f : localNames) {
    s << "Locals for function " + f.first->getName() + " : {";
    skip = true;
    for (auto l : f.second) {
      if (skip) {
        skip = false;
        s << l.first;
        continue;
      }
      s << ", " + l.first;
    }
    s << "}\n";
  }
}
