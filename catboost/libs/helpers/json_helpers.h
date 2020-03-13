#pragma once

#include "exception.h"

#include <library/json/writer/json_value.h>

#include <util/string/cast.h>

namespace NCB {
    //TODO(taube): remove doppelgangers of this code
    template <typename T>
    void FromJson(const NJson::TJsonValue& value, T* result) {
        switch (value.GetType()) {
            case NJson::EJsonValueType::JSON_INTEGER:
                *result = T(value.GetInteger());
                break;
            case NJson::EJsonValueType::JSON_DOUBLE:
                *result = T(value.GetDouble());
                break;
            case NJson::EJsonValueType::JSON_UINTEGER:
                *result = T(value.GetUInteger());
                break;
            case NJson::EJsonValueType::JSON_STRING:
                *result = FromString<T>(value.GetString());
                break;
            default:
                CB_ENSURE("Incorrect format");
        }
    }

    template <>
    void FromJson(const NJson::TJsonValue& value, TString* result);

    template <typename T>
    T FromJson(const NJson::TJsonValue& value) {
        T result;
        FromJson(value, &result);
        return result;
    }
}