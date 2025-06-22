#ifndef SEP_GLM_WRAPPER_H
#define SEP_GLM_WRAPPER_H

#ifdef __CUDACC__
#    pragma push_macro("__THROW")
#    pragma push_macro("__NTH")
#    undef __THROW
#    define __THROW
#    undef __NTH
#    define __NTH(fct) fct
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __CUDACC__
#    pragma pop_macro("__NTH")
#    pragma pop_macro("__THROW")
#endif

#endif  // SEP_GLM_WRAPPER_H
