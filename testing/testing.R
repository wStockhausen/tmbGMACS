#--Set working directory to "testing" folder
setwd("./testing");
#--source example of setting up dimensions in R
#----creates "lst", a list with elements dfrS2F and dfrF2S
source("../R/DimensionsFunctions.R")
source("../R/DimensionsUtilities.R")
source("../inst/examples/example-CreateDimsMaps.R");
dfrS2D = lst$dfrS2D; dfrD2S = lst$dfrD2S;
dims_names = attr(dfrS2D,"dmnms");
dims_lvls  = attr(dfrS2D,"dmlvs");
matS2D = sapply(dfrS2D,as.integer);
matD2S = sapply(dfrD2S,as.integer);
vecD2S = dfrD2S$sparse_idx;

#--compile and test Dimensions.hpp as a module "dims_module"
require(Rcpp)
sourceCpp(file="Dimensions.cpp",embeddedR=FALSE,rebuild=TRUE,
          showOutput=TRUE,verbose=TRUE,dryRun=FALSE)

#--create Dimensions object
dims_obj = new(Dimensions,
               dims_names_=dims_names,
               dims_lvls_=dims_lvls,
               matS2D_=matS2D,
               mapD2S=vecD2S);
testDims<-function(dims_obj,dims_ids,zerosBased=TRUE){
  idxd = dims_obj$getDenseIndex(dims_ids,zerosBased=zerosBased);
  res1 = dims_obj$getDimensionsLevelsFromDenseIndex(idxd+1,FALSE);
  cat(idxd+1,res1,"\n")
  idxs = dims_obj$getSparseIndex(dims_ids,zerosBased=zerosBased);
  res2 = dims_obj$getDimensionsLevelsFromSparseIndex(idxs+1,FALSE);
  cat(idxs+1,  res2,"\n")
  return(list(idxd=idxd,res1=res1,idxs=idxs,res2=res2));
}
rw = 7; rw_ids = matS2D[rw,];
cat(rw,rw_ids,"\n")
cat(rw_ids[1],matD2S[rw_ids[1],],"\n");
res = testDims(dims_obj,dims_ids=rw_ids[2:length(rw_ids)],zerosBased=FALSE);
cat(rw,rw_ids,"\n")
cat(res$idxs,res$idxd,res$res2,"\n");
