#include "json_helpers.h"

namespace NCB {
    void FromJson(const NJson::TJsonValue& value, TString* result) {
        *result = value.GetString();
    }
}