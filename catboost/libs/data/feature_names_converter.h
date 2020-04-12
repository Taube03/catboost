#pragma once

#include "meta_info.h"

#include <catboost/private/libs/options/feature_penalties_options.h>
#include <catboost/private/libs/options/load_options.h>
#include <catboost/private/libs/options/monotone_constraints.h>

#include <library/json/json_reader.h>

TMap<TString, ui32> MakeIndicesFromNames(const NCatboostOptions::TPoolLoadParams& poolLoadParams);
TMap<TString, ui32> MakeIndicesFromNames(const NCB::TDataMetaInfo& metaInfo);

void ConvertPerFeatureOptionsFromStringToIndices(const TMap<TString, ui32>& indicesFromNames, NJson::TJsonValue* options);

template <typename TSource>
void ConvertAllFeaturePenaltiesFromStringToIndices(
    const TSource& matchingSource,
    const bool isPlain,
    NJson::TJsonValue* catBoostJsonOptions
) {
    NJson::TJsonValue* optionsStorage = (isPlain ? catBoostJsonOptions : NCatboostOptions::GetPenaltiesOptions(catBoostJsonOptions));
    if (!optionsStorage) {
        return;
    }
    const auto namesToIndicesMap = MakeIndicesFromNames(matchingSource);

    auto& penaltiesRef = *optionsStorage;
    if (penaltiesRef.Has("feature_weights")) {
        ConvertPerFeatureOptionsFromStringToIndices(namesToIndicesMap, &penaltiesRef["feature_weights"]);
    }
    if (penaltiesRef.Has("first_feature_use_penalties")) {
        ConvertPerFeatureOptionsFromStringToIndices(namesToIndicesMap, &penaltiesRef["first_feature_use_penalties"]);
    }
}

template <typename TSource>
void ConvertMonotoneConstraintsFromStringToIndices(const TSource& matchingSource, bool isPlain, NJson::TJsonValue* catBoostJsonOptions) {
    NJson::TJsonValue* optionsStorage = (isPlain ? catBoostJsonOptions : GetTreeLearnerOptions(catBoostJsonOptions));
    if (!optionsStorage) {
        return;
    }
    auto& treeOptions = *optionsStorage;
    if (!treeOptions.Has("monotone_constraints")) {
        return;
    }

    const auto namesToIndicesMap = MakeIndicesFromNames(matchingSource);
    ConvertPerFeatureOptionsFromStringToIndices(namesToIndicesMap, &treeOptions["monotone_constraints"]);
}

template <typename TSource>
void ConvertParamsToCanonicalFormat(const TSource& stringsToIndicesMatchingSource, bool isPlain, NJson::TJsonValue* catBoostJsonOptions) {
    ConvertMonotoneConstraintsToCanonicalFormat(isPlain, catBoostJsonOptions);
    ConvertMonotoneConstraintsFromStringToIndices(stringsToIndicesMatchingSource, isPlain, catBoostJsonOptions);
    NCatboostOptions::ConvertAllFeaturePenaltiesToCanonicalFormat(isPlain, catBoostJsonOptions);
    ConvertAllFeaturePenaltiesFromStringToIndices(stringsToIndicesMatchingSource, isPlain, catBoostJsonOptions);
}

void ConvertIgnoredFeaturesFromStringToIndices(const NCatboostOptions::TPoolLoadParams& poolLoadParams, NJson::TJsonValue* catBoostJsonOptions);
void ConvertIgnoredFeaturesFromStringToIndices(const NCB::TDataMetaInfo& metaInfo, NJson::TJsonValue* catBoostJsonOptions);
void ConvertFeaturesToEvaluateFromStringToIndices(const NCatboostOptions::TPoolLoadParams& poolLoadParams, NJson::TJsonValue* catBoostJsonOptions);
