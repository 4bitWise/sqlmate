#include "./AModel.hpp"

namespace sqlmate {
    AModel::AModel() {

    }

    AModel::~AModel() {
        
    }

    std::string AModel::getTableName() const
    {
        return abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
    }
}
