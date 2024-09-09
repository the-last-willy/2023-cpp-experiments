#include "rival-module-builder.h"

void describeMyModule(IModuleBuilder &builder) {
    builder.setName("MyModule");
    builder.addInput("kil::core::ImageData", "img");
    {
        IModuleBuilder::Param param;
        param.type = "k2::base::Int32";
        param.name = "myParam";
        param.defaultValue = "0";
        builder.addParam(param);
    }
    {
        IModuleBuilder::Param param;
        param.type = "k2::base::Int32";
        param.validationPolicyType = "k2::base::PositiveValidationPolicy";
        param.name = "myParam";
        param.defaultValue = "0";
        builder.addParam(param);
    }
    {
        IModuleBuilder::Output output;
        output.type="kil::core::ImageData";
        output.name="img";
        builder.addOutput(output);
    }
}

int main() {
    try {
        auto builder = ModuleCppHeaderBuilder();
        describeMyModule(builder);
        builder.build();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
