#pragma once

#include "enums.h"
#include "option.h"
#include "unimplemented_aware_option.h"

#include <util/generic/map.h>

namespace NJson {
    class TJsonValue;
}

namespace NCatboostOptions {
    constexpr float DEFAULT_FEATURE_PENALTY = 1;

    using TPerFeaturePenalty = TMap<ui32, float>;

    struct TFeaturePenaltiesOptions {
    public:
        TFeaturePenaltiesOptions()
            : PenaltiesForEachUse("penalties_for_each_use", {}, ETaskType::CPU)
            , PenaltiesCoefficient("penalties_coefficient", 1, ETaskType::CPU)
        {
        }

        bool operator==(const TFeaturePenaltiesOptions& rhs) const {
            return std::tie(PenaltiesForEachUse, PenaltiesCoefficient) ==
                std::tie(rhs.PenaltiesForEachUse, PenaltiesCoefficient);
        }

        bool operator!=(const TFeaturePenaltiesOptions& rhs) const {
            return !(rhs == *this);
        }

        void Save(NJson::TJsonValue* options) const;
        void Load(const NJson::TJsonValue& options);

        TCpuOnlyOption<TPerFeaturePenalty> PenaltiesForEachUse;
        TCpuOnlyOption<float> PenaltiesCoefficient;
        //TODO(taube): add more penalties
    };

    void ConvertAllFeaturePenaltiesToCanonicalFormat(NJson::TJsonValue* catBoostJsonOptions);
}