#include "json_helpers.h"

namespace NCB {
    void FromJson(const TJsonValue& value, TString* result) {
        *result = value.GetString();
    }
}