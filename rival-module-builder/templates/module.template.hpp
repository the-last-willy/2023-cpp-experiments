#pragma once

#include "modules/Export.h"

#include "k2/core/model/Parameter.h"
#include "k2/core/model/slots/InputSlot.h"
#include "k2/core/model/slots/OutputSlot.h"
#include "k3/experimental/core/py2k3/InvokableToAbstractModule.h"

RIVAL_WARNING_PUSH
// RIVAL-145: Needs a way to specify the right EXPORT_MACRO when defining slots and parameters structures.
RIVAL_DISABLE_MSVC_WARNING(4251)

/**
 * @brief This module is a reader giving a python image view from a file path
 */
class EXPERIMENTAL_MODULES_EXPORT {{ name }} : public k3::py2k3::InvokableToAbstractModule
{
public:
    DECLARE_K2_RTTI({{ name }}, InvokableToAbstractModule)

    {{ name }}(k3::model::ModuleContainer* iface, const k2::model::Name& name);

    K3_MODEL_MODULE_BEGIN_INPUT_SLOT(inputs)
##  for input in inputs
        k2::model::InputSlot<{{ input.type }}> {{ input.name }} = {this, "{{ input.name }}"};
##  endfor
    K3_MODEL_MODULE_END_INPUT_SLOT(inputs)

    K3_MODEL_MODULE_BEGIN_PARAMETER(params)
##  for param in params
##      if param.validationPolicyType == ""
        k2::model::Parameter<{{ param.type }}> {{ param.name }} = {this, "{{ param.name }}", {}, {{ param.defaultValue }}};
##      else
        k2::model::Parameter<{{ param.type }}, {{ param.validationPolicyType }}> {{ param.name }} = {this, "{{ param.name }}"};
##      endif
##  endfor
    K3_MODEL_MODULE_END_PARAMETER(params)

    K3_MODEL_MODULE_BEGIN_OUTPUT_SLOT(outputs)
##  for output in outputs
        k2::model::OutputSlot<{{ output.type }}> {{ output.name }} = {this, "{{ output.name }}"};
##  endfor
    K3_MODEL_MODULE_END_OUTPUT_SLOT(outputs)
};

DEFINE_K2_RTTI(({{ name }}));

RIVAL_WARNING_POP // disable: 4251
