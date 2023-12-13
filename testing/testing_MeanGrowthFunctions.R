#--script to test mnGrw functions
#----see inst/include/tmb/MeanGrowthFunctions.hpp

#--Set working directory to "testing" folder
setwd("./testing");

#--require ggplot2
require(ggplot2);

#--compile and test meanGrowthFunctions.cpp as a module "mngrwfcns_module"
require(Rcpp)
#require(RcppEigen)
#require(TMB)
cpp = sourceCpp(file="meanGrowthFunctions.cpp",
                embeddedR=FALSE,rebuild=TRUE,
                showOutput=TRUE,verbose=TRUE,
                dryRun=FALSE,cacheDir=".")

#----size bins
zCs = seq(25,185,5);     #--cutpoints
zBs = seq(25,180,5)+2.5; #--size bins

#----parameters & constants
#------mean growth 2: mnZ = a2*exp([ln(b2/a2)/ln(zb/za)]*ln(z_/za))
za = 25; zb = 130;
a2 = 40; b2 = 155;
zGr2 = c(za,zb);#--constants:  pre-molt reference sizes za, zb
pGr2 = c(a2,b2);#--parameters: mean post-molt sizes at z=za, z=zbzb

#----mean growth 1: mnZ = a1*exp(b1*ln(z_))
b1   = log(b2/a2)/log(zb/za);
lna1 = log(a2) - log(b2/a2)/log(zb/za)*log(za);
a1   = exp(lna1);
zGr1 = c(0,0);#--constants:  ignored
pGr1 = c(a1,b1);#--parameters: mean postmolt size at z=1, ln-scale slope

#----mean growth 3: mnZ = a1*exp(b1*ln(z_/za))
b3 = log(b2/a2)/log(zb/za); #--same as b1
a3 = exp(lna1 + b3*log(za));
zGr3 = c(za,0);#--constants:  pre-molt reference size za (second constant ignored)
pGr3 = c(a3,b3);#--parameters: mean post-molt size at z=za, ln-scale slope

#--test mnGrw1
res = mnGrw1(zBs,pGr1,zGr1);
tbl1 = tibble::tibble(type="mean growth 1",z=zBs,s=res);
ggplot(tbl1,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() + wtsPlots::getStdTheme();

#--test mnGrw2
res = mnGrw2(zBs,pGr2,zGr2);
tbl2 = tibble::tibble(type="mean growth 2",z=zBs,s=res);
ggplot(tbl2,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() + wtsPlots::getStdTheme();

#--test mnGrw3
res = mnGrw3(zBs,pGr3,zGr3);
tbl3 = tibble::tibble(type="mean growth 3",z=zBs,s=res);
ggplot(tbl3,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() + wtsPlots::getStdTheme();

#--plot mean growth results
tbl = dplyr::bind_rows(tbl1,tbl2,tbl3);
ggplot(tbl,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() +
  geom_abline(slope=1,linetype=2) +
  labs(x="pre-molt size",y="mean post-molt size") + wtsPlots::getStdTheme();



