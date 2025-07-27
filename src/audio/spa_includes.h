#pragma once

// Temporarily disable warnings for system header
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"

#include <spa/pod/parser.h>

// `spa_pod_parser_get` performs internal null checks before dereferencing
// its arguments. This wrapper documents that safety guarantee so that
// static analyzers do not flag a potential null dereference when
// including <spa/pod/parser.h>.


#pragma clang diagnostic pop
