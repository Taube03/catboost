#pragma once

#include "exception.h"

#include <library/json/writer/json_value.h>

#include <util/string/cast.h>

using NJson::TJsonValue;
using NJson::EJsonValueType;

namespace NCB {
    //TODO(taube): remove doppelgangers of this code
    template <typename T>
    void FromJson(const TJsonValue& value, T* result) {
        switch (value.GetType()) {
            case EJsonValueType::JSON_INTEGER:
                *result = T(value.GetInteger());
                break;
            case EJsonValueType::JSON_DOUBLE:
                *result = T(value.GetDouble());
                break;
            case EJsonValueType::JSON_UINTEGER:
                *result = T(value.GetUInteger());
                break;
            case EJsonValueType::JSON_STRING:
                *result = FromString<T>(value.GetString());
                break;
            default:
                CB_ENSURE("Incorrect format");
        }
    }

    template <>
    void FromJson(const TJsonValue& value, TString* result);

    template <typename T>
    T FromJson(const TJsonValue& value) {
        T result;
        FromJson(value, &result);
        return result;
    }
}