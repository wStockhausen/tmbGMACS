#--R script to compile tmbGMACS dll ("tmbGMACS_TMBExports")
#----used when updating documentation or building/installling library

tmb_name <- "tmbGMACS_TMBExports"
#--see src/Makevars for the following
tmb_flags <- commandArgs(trailingOnly = TRUE)
cat("--------------------------------------------\n\n")
cat("tmb_flags set in Makevars: \n",tmb_flags,"\n")
cat("--------------------------------------------\n\n")

if(file.exists(paste0(tmb_name, ".cpp"))) {
  if(length(tmb_flags) == 0) tmb_flags <- ""
  TMB::compile(file = paste0(tmb_name, ".cpp"),
               PKG_CXXFLAGS = tmb_flags,
               safebounds = FALSE,
               safeunload = FALSE)
  file.copy(from = paste0(tmb_name, .Platform$dynlib.ext),
            to = "..", overwrite = TRUE)
}

# cleanup done in ../Makevars[.win]
