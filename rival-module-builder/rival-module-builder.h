#pragma once

#ifndef OUTPUTS_FOLDERPATH
#define OUTPUTS_FOLDERPATH
#error ""
#endif
#ifndef TEMPLATES_FOLDERPATH
#define TEMPLATES_FOLDERPATH
#error ""
#endif

#include <inja/inja.hpp>

#include <string>

class IModuleBuilder {
public:
    virtual void setName(const std::string &name) = 0;

    virtual void addInput(const std::string &type, const std::string &name) = 0;

    struct Param {
        std::string type;
        std::string validationPolicyType;
        std::string name;
        std::string validationPolicyDefaultValue;
        std::string defaultValue;
    };

    virtual void addParam(const Param &p) = 0;

    struct Output {
        std::string type;
        std::string name;
    };

    virtual void addOutput(const Output &) = 0;
};

class ModuleCppHeaderBuilder final : public IModuleBuilder {
public:
    void build() {
        inja::Environment env;

        inja::json data;
        data["name"] = moduleName;
        data["inputs"] = inja::json::array();
        data["params"] = inja::json::array();
        data["outputs"] = inja::json::array();

        for (auto &in: inputs) {
            data["inputs"].push_back(
                    {
                            {"type", std::get<0>(in)},
                            {"name", std::get<1>(in)}
                    }
            );
        }
        for (auto &pa: params) {
            auto &param = data["params"].emplace_back(inja::json::object());
            param["type"] = pa.type;
            param["validationPolicyType"] = pa.validationPolicyType;
            param["name"] = pa.name;
            param["defaultValue"] = pa.defaultValue;
        }
        for (auto &out: outputs) {
            auto &param = data["outputs"].emplace_back(inja::json::object());
            param["type"] = out.type;
            param["name"] = out.name;
        }

        auto templatePath = TEMPLATES_FOLDERPATH "module.template.hpp";
        auto outputPath = OUTPUTS_FOLDERPATH + headerFileName + ".hpp";
        env.write(templatePath, data, outputPath);
    }

    void setName(const std::string &name) override {
        this->moduleName = name;
    }

    void addInput(const std::string &type, const std::string &name) override {
        inputs.emplace_back(type, name);
    }

    void addParam(const Param &p) override {
        params.push_back(p);
    }

    void addOutput(const Output &out) override {
        outputs.push_back(out);
    }

    std::string headerFileName = "module";

private:
    std::string moduleName;
    std::vector<std::tuple<std::string, std::string>> inputs;
    std::vector<Param> params;
    std::vector<Output> outputs;
};
