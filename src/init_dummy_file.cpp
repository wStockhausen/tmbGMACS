// Dummy file required so that useDynLib(tmbGMACS, .registration=TRUE) doesn't fail on
// 'src' folder without a cpp file.

#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Visibility.h>

void attribute_visible R_init_tmbGMACS(DllInfo *dll) {
  R_registerRoutines(dll, NULL, NULL, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
