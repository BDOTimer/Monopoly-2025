#ifndef USER_MODEL_H
#define USER_MODEL_H
///----------------------------------------------------------------------------|
/// "user-model.h"
///----------------------------------------------------------------------------:
#include <string>
#include <vector>

namespace model
{

    ///---------------------------|
    /// vsl.                      |
    ///---------------------------:
    std::string  getLogo  () const;

    const Config getConfig() const;

    std::string  getCircle() const;

    std::string  getStep(unsigned idPlayer) const;

    ///---------------------------|
    /// controller.               |
    ///---------------------------:
    void doStep(std::string_view command, const std::vector<int>& args);

}

#endif // USER_MODEL_H
