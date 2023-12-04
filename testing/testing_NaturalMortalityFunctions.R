#--script to test Natural Mortality functions
#----see inst/include/tmb/NaturalMortalityFunctions.hpp
#
#--Set working directory to "testing" folder
setwd("./testing");
#--compile and test Dimensions.hpp as a module "dims_module"
require(Rcpp)
require(RcppEigen)
require(TMB)
sourceCpp(file="NaturalMortalityFunctions.cpp",embeddedR=FALSE,rebuild=TRUE,
          showOutput=TRUE,verbose=TRUE,dryRun=FALSE)

#--define size bins
zBs = seq(25,150,5);
p   = c(1,75);

#--test constantM
constantM(zBs,p);

#--test lorenzenM
lorenzenM(zBs,p);

