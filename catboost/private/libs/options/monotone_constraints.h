#pragma once

#include "load_options.h"

#include <library/json/writer/json_value.h>

#include <util/generic/fwd.h>
#include <util/generic/map.h>

NJson::TJsonValue* GetTreeLearnerOptions(NJson::TJsonValue* catBoostJsonOptions);
void ConvertMonotoneConstraintsToCanonicalFormat(const bool isPlain, NJson::TJsonValue* catBoostJsonOptions);
