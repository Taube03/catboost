#include "feature_penalties_options.h"

#include "json_helper.h"
#include "parse_per_feature_options.h"

using namespace NCB;
using namespace NJson;
using namespace NCatboostOptions;

namespace NCatboostOptions {
    void NCatboostOptions::TFeaturePenaltiesOptions::Load(const NJson::TJsonValue& options) {
        CheckedLoad(options, &PenaltiesForEachUse, &PenaltiesCoefficient);
    }

    void NCatboostOptions::TFeaturePenaltiesOptions::Save(NJson::TJsonValue* options) const {
        SaveFields(options, PenaltiesForEachUse, PenaltiesCoefficient);
    }

    static constexpr auto floatRegex = AsStringBuf("([0-9]+([.][0-9]*)?|[.][0-9]+)");

    static void LeaveOnlyNonTrivialOptions(TJsonValue* penaltiesJsonOptions) {
        TJsonValue nonTrivialOptions(EJsonValueType::JSON_MAP);
        auto& optionsRefMap = penaltiesJsonOptions->GetMapSafe();
        for (auto&& [feature, option] : optionsRefMap) {
            if (option.GetDoubleRobust() != 0) {
                nonTrivialOptions[feature] = std::move(option);
            }
        }
        *penaltiesJsonOptions = nonTrivialOptions;
    }

    static void ConvertFeaturePenaltiesToCanonicalFormat(NJson::TJsonValue* featurePenaltiesJsonOptions) {
        ConvertFeatureOptionsToCanonicalFormat<int>(floatRegex, featurePenaltiesJsonOptions);
        LeaveOnlyNonTrivialOptions(featurePenaltiesJsonOptions);
    }

    void ConvertAllFeaturePenaltiesToCanonicalFormat(NJson::TJsonValue* catBoostJsonOptions) {
        auto& treeOptions = (*catBoostJsonOptions)["tree_learner_options"];
        if (!treeOptions.Has("penalties")) {
            return;
        }

        TJsonValue& penaltiesRef = treeOptions["penalties"];
        if (penaltiesRef.Has("penalties_for_each_use")) {
            ConvertFeaturePenaltiesToCanonicalFormat(&penaltiesRef["penalties_for_each_use"]);
        }
    }
}
