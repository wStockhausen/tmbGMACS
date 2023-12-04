#--script to test Selectivity functions
#----see inst/include/tmb/SelectivityFunctions.hpp
#
require(ggplot2);
#
#--Set working directory to "testing" folder
setwd("./testing");
#--compile and test SelectivityFunctions.hpp as a module "selfcns_module"
require(Rcpp)
require(RcppEigen)
require(TMB)
sourceCpp(file="SelectivityFunctions.cpp",
          embeddedR=FALSE,rebuild=TRUE,
          showOutput=TRUE,verbose=TRUE,
          dryRun=FALSE)

#--test window
#----size bin centers
zBs = seq(25,180,5)+2.5;
#----constants: window extent
consts = c(35,140);
#----calc window
res = window(zBs,consts[1],consts[2]);
tbl0a = tibble::tibble(type="window",z=zBs,s=res);
ggplot(tbl0a,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() +
  geom_hline(yintercept=1,linetype=2) + ylim(0,1.1) +
  labs(x="size",y="selectivity") + wtsPlots::getStdTheme();

#--test constantSel
#----size bin centers
zBs = seq(25,180,5)+2.5; #--define size bins
#----constants: window extent
consts = c(35,140);
#----parameters: selectivity
p   = 0.5;
#----calc the selectivity function
res = constantSel(zBs,p,consts);
tbl0b = tibble::tibble(type="constantSel",z=zBs,s=res);
#ggplot(tbl0b,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() + wtsPlots::getStdTheme();

#--test ascending logistic type 1
#----size bin centers
zBs = seq(25,180,5)+2.5;
#----constants: window extent, reference size
consts = c(35,140,130);
#----parameters: z50, slope
p = c(100,0.1);
#----calc the selectivity function
res = ascLogisticSel1(zBs,p,consts);
tbl1 = tibble::tibble(type="ascLogisticSel1",z=zBs,s=res);

#--test ascending logistic type 2
#----size bin centers
zBs = seq(25,180,5)+2.5;
#----constants: window extent, reference size
consts = c(35,140,130);
#----parameters: z50, z95
p = c(100,135);
#----calc the selectivity function
res = ascLogisticSel2(zBs,p,consts);
tbl2 = tibble::tibble(type="ascLogisticSel2",z=zBs,s=res);

#--test ascending normal type 1
#----size bin centers
zBs = seq(25,180,5)+2.5;
#----constants: window extent
consts = c(35,140);
p = c(100,20);
#----calc the selectivity function
res = ascNormalSel1(zBs,p,consts);
tbl3 = tibble::tibble(type="ascNormalSel1",z=zBs,s=res);

#--test double normal type 1
#----size bin centers
zBs = seq(25,180,5)+2.5; #--define size bins
#----constants: window extent
consts = c(35,140);
#----parameters: the ascending
p = c(80,20,40,10);
#----calc the selectivity function
res = dblNormalSel1(zBs,p,consts);
tbl4 = tibble::tibble(type="dblNormalSel1",z=zBs,s=res);

#--test spline type 1
#----size bin centers
zBs = seq(25,180,5)+2.5;
#----constants: window extent
consts = c(20,200);
#----parameters: {knot,value} pairs (values should be estimated on logistic scale?)
p    = c(40,0.1, 80,0.5, 100,0.8, 120,0.99);
#----calc the selectivity function
res  = splineSel1(zBs,p,consts);
tbl5 = tibble::tibble(type="splineSel1",z=zBs,s=res);
ggplot(tbl5,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() +
  geom_hline(yintercept=1,linetype=2) + ylim(0,1.1) +
  labs(x="size",y="selectivity") + wtsPlots::getStdTheme();

tbl = dplyr::bind_rows(tbl0a,tbl0b,tbl1,tbl2,tbl3,tbl4,tbl5);
ggplot(tbl,aes(x=z,y=s,colour=type)) + geom_line() + geom_point() +
  geom_hline(yintercept=1,linetype=2) + ylim(0,1.1) +
  labs(x="size",y="selectivity") + wtsPlots::getStdTheme();
