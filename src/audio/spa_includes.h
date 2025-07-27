#pragma once

// Temporarily disable warnings for system header
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"

#include <spa/pod/parser.h>

// `spa_pod_parser_get` performs internal null checks before dereferencing
// its arguments. Warnings are suppressed because the analyzer cannot
// detect these checks across the header boundary.

#pragma clang diagnostic pop