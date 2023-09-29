#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "expected.hpp"
#include "token.hpp"

using tokens_result = tl::expected<std::vector<token_t>, std::string>;

tokens_result tokenize(std::string_view line);
