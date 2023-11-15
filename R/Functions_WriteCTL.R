#--write functions for CTL file
writeCTL<-function(lst,conn,justText=TRUE){
  if (!justText)
    if(!isOpen(conn)) conn = file(conn,open="w");
  str = c("#--tmbGMACS CTL file",
          "#--model population dynamics",
          "################################################################################"
          );
  str = c(str,writeCTL_InitialAbundance(lst[["InitAbd"]],conn,justText=TRUE));

  str = c(str,"END CTL FILE");
  if (!justText) cat(str,sep="\n",file=conn);
  return(paste(str,collapse="\n"));
}

writeCTL_InitialAbundance<-function(lst,conn,justText=TRUE){
  if (!justText)
    if(!isOpen(conn)) conn = file(conn,open="w");

  str = c("INITIAL ABUNDANCE--##########################################################",
          "##--options",
          "###--1. no initial population (build up over time from recruitment)",
          "###--2. based on unfished equilibrium and recruitment (R0)",
          "###--3. based on equilibrium with initial year fishing mortality and recruitment (Rini)",
          "###--4. free parameters (relative to reference class(es))",
          "##--");
  str = c(str,paste(lst$option,"    #--initial abundance option"));
  if (lst$option==1){
    str = c(str,"###--Option 1: no parameters");
  } else
  if (lst$option==2){
    str = c(str,"###--Option 2: parameters (R0)",
                "###-------------------------------------------------------------------------------------",
                readr::format_delim(lst[["R0_tbl"]],delim=" ",quote="none"),
                "END TABLE",
                "###-------------------------------------------------------------------------------------"
          );
  } else
  if (lst$option==3){
    str = c(str,"###--Option 3: parameters (Rini)",
                "###-------------------------------------------------------------------------------------",
                readr::format_delim(lst[["Rini_tbl"]],delim=" ",quote="none"),
                "END TABLE",
                "###-------------------------------------------------------------------------------------"
          );
  } else
  if (lst$option==4){
    str = c(str,"###--Option 4: free parameters",
                "###--reference classes",
                "###-------------------------------------------------------------------------------------",
                readr::format_delim(lst[["ref_pars_tbl"]],delim=" ",quote="none"),
                "END TABLE",
                "###-------------------------------------------------------------------------------------",
                "###--nonparametric initial abundance offset options",
                "####--1. initial values as abundance in class",
                "####--2. initial values as ln-scale offsets from specified class",
                "####--3. initial values as ln-scale deviations",
                "###--",
                paste(lst$opt4_vals,"     #--initial value scaling option"),
                "###--initial abundance offsets",
                "##------------------------------------------------------------------------------",
                readr::format_delim(lst[["off_pars_tbl"]],delim=" ",quote="none"),
                "# END TABLE",
                "##------------------------------------------------------------------------------"
          );
  }
  str = c(str,"END INITIAL ABUNDANCE SECTION","\n");

  if (!justText) cat(str,sep="\n",file=conn);
  return(paste(str,collapse="\n"));
}

cat(writeCTL(tst,"",justText=TRUE),sep="\n");
