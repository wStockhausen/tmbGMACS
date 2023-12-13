#--script to test Natural Mortality functions
#----see inst/include/tmb/NaturalMortalityFunctions.hpp
#
#--Set working directory to "testing" folder
setwd("./testing");

#--require ggplot2 for graphs
require(ggplot2);

#--compile and test NaturalMortalityFunctions.hpp as a module "m_module"
require(Rcpp)
#require(RcppEigen)
require(TMB)
cpp = sourceCpp(file="NaturalMortalityFunctions.cpp",
                embeddedR=FALSE,rebuild=TRUE,
                showOutput=TRUE,verbose=TRUE,
                dryRun=FALSE,cacheDir=".")

#--define size bins
zBs = seq(25,150,5);
p   = c(1,75);

#--test constantM
res = constantM(zBs,p);
tbl1 = tibble::tibble(type="constantM",z=zBs,m=res);

#--test lorenzenM
res = lorenzenM(zBs,p);
tbl2 = tibble::tibble(type="lorenzenM",z=zBs,m=res);

tbl = dplyr::bind_rows(tbl1,tbl2);
ggplot(tbl,aes(x=z,y=m,colour=type)) + geom_line() + geom_point() +
  geom_hline(yintercept=1,linetype=2) +
  labs(x="size",y="M") + wtsPlots::getStdTheme();
