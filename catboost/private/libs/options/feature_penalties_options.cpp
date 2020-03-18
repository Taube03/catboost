#include "feature_penalties_options.h"

#include "json_helper.h"
#include "parse_per_feature_options.h"

using namespace NCB;
using namespace NJson;
using namespace NCatboostOptions;

namespace NCatboostOptions {
    void NCatboostOptions::TFeaturePenaltiesOptions::Load(const NJson::TJsonValue& options) {
        CheckedLoad(options, &FeatureWeights, &PenaltiesCoefficient);
    }

    void NCatboostOptions::TFeaturePenaltiesOptions::Save(NJson::TJsonValue* options) const {
        SaveFields(options, FeatureWeights, PenaltiesCoefficient);
    }

    static constexpr auto floatRegex = AsStringBuf("([0-9]+([.][0-9]*)?|[.][0-9]+)");

    static void LeaveOnlyNonTrivialOptions(const float defaultValue, TJsonValue* penaltiesJsonOptions) {
        TJsonValue nonTrivialOptions(EJsonValueType::JSON_MAP);
        const auto& optionsRefMap = penaltiesJsonOptions->GetMapSafe();
        for (const auto& [feature, option] : optionsRefMap) {
            if (option.GetDoubleRobust() != defaultValue) {
                nonTrivialOptions[feature] = option;
            }
        }
        *penaltiesJsonOptions = nonTrivialOptions;
    }

    static void ConvertFeaturePenaltiesToCanonicalFormat(const float defaultValue, NJson::TJsonValue* featurePenaltiesJsonOptions) {
        ConvertFeatureOptionsToCanonicalFormat<float>(floatRegex, featurePenaltiesJsonOptions);
        LeaveOnlyNonTrivialOptions(defaultValue, featurePenaltiesJsonOptions);
    }

    void ConvertAllFeaturePenaltiesToCanonicalFormat(NJson::TJsonValue* catBoostJsonOptions) {
        auto& treeOptions = (*catBoostJsonOptions)["tree_learner_options"];
        if (!treeOptions.Has("penalties")) {
            return;
        }

        TJsonValue& penaltiesRef = treeOptions["penalties"];
        if (penaltiesRef.Has("feature_weights")) {
            ConvertFeaturePenaltiesToCanonicalFormat(DEFAULT_FEATURE_WEIGHT, &penaltiesRef["feature_weights"]);
        }
    }
}
