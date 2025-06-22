#pragma once
#pragma push_macro("_SPA_DEFINE_AUTOPTR_CLEANUP")
#pragma push_macro("_spa_autofree_cleanup_func")
#pragma push_macro("_spa_autoclose_cleanup_func")
#define _SPA_DEFINE_AUTOPTR_CLEANUP SPA_DEFINE_AUTOPTR_CLEANUP
#define _spa_autofree_cleanup_func spa_autofree_cleanup_func
#define _spa_autoclose_cleanup_func spa_autoclose_cleanup_func
#include <spa/utils/cleanup.h>
#pragma pop_macro("_spa_autoclose_cleanup_func")
#pragma pop_macro("_spa_autofree_cleanup_func")
#pragma pop_macro("_SPA_DEFINE_AUTOPTR_CLEANUP")
