#include "TipFunction.h"
#include "TipTypeVisitor.h"
#include <algorithm>
#include <map>
#include <sstream>

uint32_t TipFunction::instance = 1;

TipFunction::TipFunction(std::vector<std::shared_ptr<TipType>> params,
                         std::shared_ptr<TipType> ret):
  TipCons(std::move(combine(params, ret)))
{
}

std::vector<std::shared_ptr<TipType>> TipFunction::combine(
        std::vector<std::shared_ptr<TipType>> params,
        std::shared_ptr<TipType> ret) {
    params.push_back(std::move(ret));
    return params;
}

std::vector<std::shared_ptr<TipType>> TipFunction::getParams() const {
    std::vector<std::shared_ptr<TipType>> params(arguments.begin(), arguments.end()-1);
    return params;
}

std::shared_ptr<TipType> TipFunction::getReturnValue() const {
    return arguments.back();
}

std::ostream &TipFunction::print(std::ostream &out) const {
    out << "(";
    int end_of_args = arguments.size() - 1;
    for(int i = 0; i < end_of_args; i++) {
        out << *arguments.at(i) << (i == end_of_args - 1 ? "" : ",");
    }
    out << ") -> " << *arguments.back();
    return out;
}

bool TipFunction::operator==(const TipType &other) const {
    auto otherTipFunction = dynamic_cast<const TipFunction *>(&other);
    if(!otherTipFunction) {
        return false;
    }

    if(arguments.size() != otherTipFunction->arguments.size()) {
        return false;
    }

    for(int i = 0; i < arguments.size(); i++) {
        if(*(arguments.at(i)) != *(otherTipFunction->arguments.at(i))) {
            return false;
        }
    }

    return *arguments.back() == *(otherTipFunction->arguments.back());
}

bool TipFunction::operator!=(const TipType &other) const {
    return !(*this == other);
}

void TipFunction::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto a : arguments) {
       a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

bool TipFunction::isInstantiated() const {
  return this->__is_instantiated;
}

TipType* TipFunction::instantiate() const {
  const std::string instance_suffix = "-" + std::to_string(TipFunction::instance);

  // Arguments type variables for the instantiation.
  std::vector<std::shared_ptr<TipType>> args;
  // Map from old alphas to new alphas specific to this instantiation.
  std::map<std::shared_ptr<TipAlpha>, std::shared_ptr<TipAlpha>> replacement_alphas;

  // Recreate parameters for the function.
  for (auto arg : this->getParams()) {
    // Find all alphas that will need to be handled specially.
    std::vector<std::shared_ptr<TipAlpha>> arg_alphas;
    if (auto alpha_arg = std::dynamic_pointer_cast<TipAlpha>(arg)) {
      arg_alphas.push_back(alpha_arg);
    } else {
      arg->populateAlphas(arg_alphas);
    }

    // Instantiate alphas and add them to replacement_alphas so they can
    // be re-used in parameters that have the same alpha.
    for (auto arg_alpha : arg_alphas) {
      // TODO: need a better way of accessing and finding alpha variables.
      // Pointers do not match, but using TipAlpha::operator== does.
      auto find_res = std::find_if(
        replacement_alphas.begin(),
        replacement_alphas.end(),
        [arg_alpha](const auto a) { return *a.first == *arg_alpha; });
      if (find_res == replacement_alphas.end()) {
        const std::string inst_name = arg_alpha->getName() + instance_suffix;
        auto repl_alpha = std::shared_ptr<TipAlpha>(
          new TipAlpha(arg_alpha->getNode(), inst_name));
        replacement_alphas[arg_alpha] = repl_alpha;
      }
    }

    // Directly replace alpha with its replacement.
    // Let other types recursively replace their alphas.
    if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(arg)) {
      args.push_back((*std::find_if(
                       replacement_alphas.begin(),
                       replacement_alphas.end(),
                       [alpha] (const auto a) { return *a.first == *alpha; })).second);
    } else {
      auto arg_instance = std::shared_ptr<TipType>(arg->instantiate());
      args.push_back(arg_instance);
      for (auto kv : replacement_alphas) {
        arg_instance->replaceAlpha(kv.first, kv.second);
      }
    }
  }

  // Replace the return type.
  std::shared_ptr<TipType> ret;
  if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(this->getReturnValue())) {
    std::shared_ptr<TipAlpha> match;
    // TODO: could use a better strategy for getting the match.
    // This would be fixed by a better way to index into the map.
    for (auto kv : replacement_alphas) {
      if (*alpha == *kv.first) {
        match = kv.second;
        break;
      }
    }

    assert(match != nullptr); // Gross...
    ret = match;
  } else {
    ret = std::shared_ptr<TipType>(this->getReturnValue()->instantiate());
    for (auto kv : replacement_alphas) {
      ret->replaceAlpha(kv.first, kv.second);
    }
  }

  TipFunction* const f = new TipFunction(args, ret);
  f->__is_instantiated = true; // <- Is this used in some way?

  TipFunction::instance++;

  return f;
}
