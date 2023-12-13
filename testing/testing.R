#--Set working directory to "testing" folder
setwd("./testing");

library(tmbGMACS);
library(TMB);
obj <- MakeADFun(data = list(model="GMACS"),
                 parameters=list(p=10),
                 DLL = "tmbGMACS_TMBExports",
                 silent = FALSE);
obj$fn()
