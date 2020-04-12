#include "monotone_constraints.h"
#include "parse_per_feature_options.h"

#include <util/generic/strbuf.h>
#include <util/string/cast.h>
#include <util/string/split.h>

#include <regex>

using namespace NCB;
using namespace NJson;
using namespace NCatboostOptions;

static constexpr auto constraintRegex = AsStringBuf("0|1|-1");

static void LeaveOnlyNonTrivialConstraints(TJsonValue* monotoneConstraintsJsonOptions) {
    TJsonValue nonTrivialConstraints(EJsonValueType::JSON_MAP);
    const auto& constraintsRefMap = monotoneConstraintsJsonOptions->GetMapSafe();
    for (const auto& [feature, constraint] : constraintsRefMap) {
        if (constraint.GetIntegerSafe() != 0) {
            nonTrivialConstraints[feature] = constraint;
        }
    }
    *monotoneConstraintsJsonOptions = nonTrivialConstraints;
}

NJson::TJsonValue* GetTreeLearnerOptions(NJson::TJsonValue* catBoostJsonOptions) {
    if (!catBoostJsonOptions->Has("tree_learner_options")) {
        return nullptr;
    }
    return  &(*catBoostJsonOptions)["tree_learner_options"];
}

void ConvertMonotoneConstraintsToCanonicalFormat(const bool isPlain, TJsonValue* catBoostJsonOptions) {
    NJson::TJsonValue* optionsStorage = (isPlain ? catBoostJsonOptions : GetTreeLearnerOptions(catBoostJsonOptions));
    if (!optionsStorage) {
        return;
    }
    auto& treeOptions = *optionsStorage;

    if (!treeOptions.Has("monotone_constraints")) {
        return;
    }
    TJsonValue& constraintsRef = treeOptions["monotone_constraints"];
    ConvertFeatureOptionsToCanonicalFormat<int>(AsStringBuf("monotone_constraints"),
                                                constraintRegex,
                                                &constraintsRef);
    LeaveOnlyNonTrivialConstraints(&constraintsRef);
}
