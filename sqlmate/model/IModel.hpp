#include <iostream>
#include <vector>

#if !defined(IMODEL)
#define IMODEL

namespace sqlmate {

class IModel
{
public:
    virtual ~IModel() = default;

    virtual std::string getTableName() const = 0;
};

}


#endif // IMODEL


