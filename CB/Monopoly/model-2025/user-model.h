#ifndef USER_MODEL_H
#define USER_MODEL_H
///----------------------------------------------------------------------------|
/// "user-model.h"
///----------------------------------------------------------------------------:
#include <string>
#include <vector>

#include "model/config-model.h"

namespace model
{
    ///---------------------------|
    /// vsl.                      |
    ///---------------------------:
    const Config getConfig();

    std::string  getLogo(unsigned id);

    ///---------------------------|
    /// controller.               |
    ///---------------------------:
    std::string doStep(std::string_view command, const std::vector<int>& args);

}

#endif // USER_MODEL_H
