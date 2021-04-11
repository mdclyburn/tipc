#include "TipFunction.h"
#include "TipTypeVisitor.h"
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

  const std::string instance_suffix = "-" + std::to_string(TipFunction::instance);

  std::vector<std::shared_ptr<TipType>> args;
  for (auto arg : this->getParams()) {
    if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(arg)) {
      const std::string inst_name = alpha->getName() + instance_suffix;
      std::shared_ptr<TipAlpha> inst_alpha(
        new TipAlpha(alpha->getNode(), inst_name));
      args.push_back(inst_alpha);
    } else {
      args.push_back(std::shared_ptr<TipType>(arg->instantiate()));
    }
  }

  std::shared_ptr<TipType> ret;
  if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(this->getReturnValue())) {
    ret = std::shared_ptr<TipAlpha>(
      new TipAlpha(alpha->getNode(), alpha->getName() + instance_suffix));
  } else {
    ret = std::shared_ptr<TipType>(this->getReturnValue()->instantiate());
  }

  TipFunction* const f = new TipFunction(args, ret);
  f->__is_instantiated = true;

  TipFunction::instance++;

  return f;
}
