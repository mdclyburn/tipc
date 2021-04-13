#include "TipFunction.h"
#include "TipTypeVisitor.h"
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
  std::cout << "Instantiating function as instance "
            << TipFunction::instance
            << std::endl;

  for (auto arg : this->getParams()) {
    std::cout << "Param: ";
    if (auto ref = std::dynamic_pointer_cast<TipRef>(arg)) {
      std::cout << "ref " << ref << " -> " << ref->getAddressOfField() << std::endl;
    } else {
      std::cout << arg << std::endl;
    }
  }
  std::cout << "Return: " << this->getReturnValue() << std::endl;

  const std::string instance_suffix = "-" + std::to_string(TipFunction::instance);

  std::vector<std::shared_ptr<TipType>> args;
  // Map from old alphas to new alphas.
  std::map<std::shared_ptr<TipAlpha>, std::shared_ptr<TipAlpha>> replacement_alphas;
  for (auto arg : this->getParams()) {
    // Capture any new alphas.
    std::vector<std::shared_ptr<TipAlpha>> arg_alphas;
    arg->populateAlphas(arg_alphas);
    for (auto arg_alpha : arg_alphas) {
      if (replacement_alphas.find(arg_alpha) == replacement_alphas.end()) {
        const std::string inst_name = arg_alpha->getName() + instance_suffix;
        auto repl_alpha = std::shared_ptr<TipAlpha>(
          new TipAlpha(arg_alpha->getNode(), inst_name));
        std::cout << "Replacement alpha: "
                  << arg_alpha << " -> " << repl_alpha
                  << " :: "
                  << *arg_alpha << " -> " << *repl_alpha
                  << std::endl;
        replacement_alphas[arg_alpha] = repl_alpha;
      }
    }

    // Directly replace alpha with its replacement.
    // Let other types recursively replace their alphas.
    if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(arg)) {
      args.push_back(replacement_alphas[alpha]);
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
    for (auto kv : replacement_alphas) {
      if (*alpha == *kv.first) {
        std::cout << "Found match for return: "
                  << alpha << " (" << *alpha << ") "
                  << " and " << kv.first << " (" << *kv.first << ")" << std::endl;
        match = kv.second;
        break;
      }
    }
    assert(match != nullptr);

    std::cout << "Return of function is direct alpha." << std::endl;
    std::cout << "  replace: " << alpha << " -> " << match
              << " :: "
              << *alpha << " -> " << *match
              << std::endl;
    // std::cout << "Return refers to same: "
    //           << (*alpha == *(std::static_pointer_cast<TipRef>(this->getParams().back())->getAddressOfField())) << std::endl;

    ret = match;
  } else {
    std::cout << "Return of function is not direct alpha. "
              << "Instantiating and using replaceAlpha." << std::endl;
    ret = std::shared_ptr<TipType>(this->getReturnValue()->instantiate());
    for (auto kv : replacement_alphas) {
      ret->replaceAlpha(kv.first, kv.second);
    }
  }

  TipFunction* const f = new TipFunction(args, ret);
  f->__is_instantiated = true;

  TipFunction::instance++;

  std::cout << "Instantiation: " << *f << std::endl;

  return f;
}
