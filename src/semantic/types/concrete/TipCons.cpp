#include "TipCons.h"
#include "TipTypeVisitor.h"

int TipCons::arity() const {
    return arguments.size();
}

namespace {
/*! \brief Determine that subtypes of two objects are the same
 */
template<typename T>
bool sameType(const TipType *x, const TipType *y) {
  return dynamic_cast<T const *>(x) && dynamic_cast<T const *>(y);
}
} // end anyonymous namespace

/*! \brief Check for dynamic subtype and artity agreement
 * We explicitly test the types here which is not robust to
 * the addition of new subtypes of TipCons.  Extend this if you
 * add such a subtype.
 */
bool TipCons::doMatch(TipType const * t) const {
    // Check if they are both the same TipType subtype
    if (sameType<TipFunction>(t, this) ||
        sameType<TipInt>(t, this) ||
        sameType<TipRecord>(t, this) ||
        sameType<TipRef>(t, this)) {
      auto tipCons = dynamic_cast<TipCons const *>(t);
      return tipCons->arity() == arity();
    }
    return false;
}

TipCons::TipCons(std::vector<std::shared_ptr<TipType>> arguments) : arguments(std::move(arguments)) { }

void TipCons::setArguments(std::vector<std::shared_ptr<TipType>> &a) {
    arguments = a;
}

const std::vector<std::shared_ptr<TipType>> &TipCons::getArguments() const {
    return arguments;
}

bool TipCons::containsFreeVariable() const {
  for (auto it = arguments.begin(); it != arguments.end(); it++) {
    if ((*it)->containsFreeVariable()) {
      return true;
    }
  }

  return false;
}

void TipCons::populateAlphas(std::vector<std::shared_ptr<TipAlpha>>& alphas) {
  std::cout << "Cons populating alphas..." << std::endl;
  for (auto arg : this->arguments) {
    if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(arg)) {
      std::cout << "  found alpha " << *alpha << " (" << alpha << ")" << std::endl;
      alphas.push_back(alpha);
    } else {
      std::cout << "  recursing to populate alphas..." << std::endl;
      arg->populateAlphas(alphas);
    }
  }

  return;
}

void TipCons::replaceAlpha(const std::shared_ptr<TipAlpha>& original,
                  const std::shared_ptr<TipAlpha>& replacement) {
  std::cout << "TipCons: replaceAlpha "
            << original << " (" << *original << ") "
            << "with " << replacement << " (" << *replacement << ")"
            << std::endl;

  for (uint32_t i = 0; i < this->arguments.size(); i++) {
    if (auto alpha = std::dynamic_pointer_cast<TipAlpha>(arguments[i])) {
      if (alpha->getName().compare(original->getName()) == 0) {
        std::cout << "TipCons: replacing " << *alpha << " with " << *replacement << std::endl;
        arguments[i] = replacement;
      } else {
        std::cout << "TipCons: no match: "
                  << *alpha
                  << " =/= "
                  << *original
                  << std::endl;
      }
    } else {
      std::cout << "TipCons: recursing for replacement." << std::endl;
      arguments[i]->replaceAlpha(original, replacement);
    }
  }

  return;
}
