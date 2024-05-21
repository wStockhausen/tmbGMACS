#--Set working directory to "testing" folder
dirPrj = rstudioapi::getActiveProject()
setwd(file.path(dirPrj,"testing"));

library(tmbGMACS);
library(TMB);

#--load the Rcpp module
gmacs <- Rcpp::Module("tmbgmacs", PACKAGE = "tmbGMACS")

#--create TMB model
obj <- MakeADFun(data = list(model="GMACS"),
                 parameters=list(p=10),
                 DLL = "tmbGMACS_TMBExports",
                 silent = FALSE);
obj$fn()
