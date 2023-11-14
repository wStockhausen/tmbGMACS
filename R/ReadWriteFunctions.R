readCTL<-function(fn){
  if (!file.exists(fn)) stop("File ",fn," does not exist!");
  con = file(fn,open="r");
  lst = list();
  ln=readLines(con,n=1); nl=1;
  while(ln!="END CTL FILE"){
    if (stringr::str_starts(ln,"[:blank:]*#")){
      #--skip line
    } else if (stringr::str_starts(ln,"[:blank:]*INITIAL ABUNDANCE")){
        res = readCTL_InitialAbundance(con,nl);
        nl = res$nl;
        lst[["InitAbd"]] = res$lst;
    } else if (stringr::str_starts(ln,"[:blank:]*RECRUITMENT")){
        res = readCTL_Recruitment(con,nl);
        nl = res$nl;
        lst[["rec"]] = res$lst;
    } else if (stringr::str_starts(ln,"[:blank:]*NATURAL MORTALITY")){
        res = readCTL_NaturalMortality(con,nl);
        nl = res$nl;
        lst[["nm"]] = res$lst;
    # } else if (stringr::str_starts(ln,"[:blank:]*GROWTH")){
    #     res = readCTL_Growth(con,nl);
    #     nl = res$nl;
    #     lst[["grw"]] = res$lst;
    # } else if (stringr::str_starts(ln,"[:blank:]*MATURITY")){
    #     res = readCTL_Maturity(con,nl);
    #     nl = res$nl;
    #     lst[["mat"]] = res$lst;
    # } else if (stringr::str_starts(ln,"[:blank:]*SELECTIVITY")){
    #     res = readCTL_Selectivity(con,nl);
    #     nl = res$nl;
    #     lst[["sel"]] = res$lst;
    # } else if (stringr::str_starts(ln,"[:blank:]*FISHING MORTALITY")){
    #     res = readCTL_FishingMortality(con,nl);
    #     nl = res$nl;
    #     lst[["fsh"]] = res$lst;
    # } else if (stringr::str_starts(ln,"[:blank:]*SURVEY INDICES")){
    #     res = readCTL_SurveyIndices(con,nl);
    #     nl = res$nl;
    #     lst[["srv"]] = res$lst;
    }
    ln=readLines(con,n=1); nl = nl+1;
    cat("in readCTL",nl,"\n\t",ln,"\n");
  }
  cat("CTL file: reached END CTL FILE at line",nl,"\n");
  close(con);
  return(lst);
}

parseVal<-function(con,nl,ln=NULL){
  if (is.null(ln)) ln=readLines(con,n=1); nl=nl+1;#--read next line
  while(isOpen(con)){
    if (stringr::str_starts(ln,"[:blank:]*#")){
      #--skip comment line
    } else {
      #--split by whitespace and drop anything after comment character
      strp1 = scan(text=ln,what=character(),comment.char="#",quiet=TRUE,strip.white=TRUE);
      #--parse remaining string
      val = readr::parse_guess(strp1);
      return(list(nl=nl,val=val));
    }
    ln=readLines(con,n=1); nl=nl+1;
  }#--while
  str = paste0("Error reading file using parseVal! Last line read was ",ln,"\n",nnl,"\n");
  stop(str);
}

readParamsTable<-function(con,nl){
  type = "TABLE";
  cat("reading",type,"section starting on line",nl,"\n");
  ln=readLines(con,n=1); nl=nl+1;
  done = FALSE;
  str = "###--PARAMS TABLE--";
  while(isOpen(con)&(!stringr::str_starts(ln,paste0("[:blank:]*END ",type)))){
    cat("\t",nl,":\t",ln,"\n");
    if (stringr::str_starts(ln,"[:blank:]*#")){
      #--skip line
      cat("skipping line: ",ln,"\n")
    } else {
      str = paste0(str,"\n",ln);
      #cat("building up table string:\n",str,"\n")
    }
    ln=readLines(con,n=1); nl=nl+1;
  }
  cat("section ended on line",nl,"\n");
  cat("table:\n",str,"\n");
  tbl = readr::read_table(I(str),skip=1,comment="#");
  return(list(nl=nl,tbl=tbl));
}

readCTL_InitialAbundance<-function(con,nl){
  type = "INITIAL ABUNDANCE";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  #--read section
  res = parseVal(con,nl);#--need to pass ln
  option = res$val; nl = res$nl;
  cat("optInitAbund =",option,"\n");
  lst[["option"]] = option;
  if (option==2){
    #--read R0 parameter specification
    res = readParamsTable(con,nl);
    nl = res$nl;
    lst[["R0_tbl"]] = res$tbl;
    rm(res);
  } else if (option==3){
    #--read Rini parameter specification
    res = readParamsTable(con,nl);
    nl = res$nl;
    lst[["Rini_tbl"]] = res$tbl;
    rm(res);
  } else if (option==4){
    #--read reference class specification
    res = readParamsTable(con,nl);
    nl = res$nl;
    lst[["ref_pars_tbl"]] = res$tbl;
    rm(res);
    #--read free parameters specification
    res = parseVal(con,nl);#--don't pass ln, need to start with next line
    opt4_vals = res$val;  nl = res$nl;
    cat("opt4_vals =",opt4_vals,"\n");
    lst[["opt4_vals"]] = opt4_vals;
    res = readParamsTable(con,nl);
    nl = res$nl;
    lst[["off_pars_tbl"]] = res$tbl;
    rm(res);
  }
  ln=readLines(con,n=1); nl=nl+1;
  while(isOpen(con)&(!stringr::str_starts(ln,paste0("[:blank:]*END ",type)))){
    cat("\t",nl,":\t",ln,"\n");
    ln=readLines(con,n=1); nl=nl+1;
  }#--while
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_Recruitment<-function(con,nl){
  type = "RECRUITMENT";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  #--read recruitment function section
  cat("reading recruitment function section\n")
  res = readParamsTable(con,nl);
  nl = res$nl;
  lst[["rec_fcns"]] = res$tbl;
  rm(res);

  #--read size function parameters section
  cat("reading recruitment-at-size parameters section\n")
  res = readParamsTable(con,nl);
  nl = res$nl;
  lst[["rec_z_pars"]] = res$tbl;
  rm(res);
  #--read size function parameter devs section
  #cat("reading size function parameter devs section\n")
  #--read size function parameter dev covariates section
  #cat("reading size function parameter dev covariates section\n")

  #--annual rec params
  cat("reading recruitment parameters section\n")
  res = readParamsTable(con,nl);
  nl = res$nl;
  lst[["rec_pars"]] = res$tbl;
  rm(res);

  #--annual sex ratio
  cat("reading annual sex ratio section\n")
  res = readParamsTable(con,nl);
  nl = res$nl;
  lst[["rec_pars"]] = res$tbl;
  rm(res);
  #--read size function parameter devs section
  #--read size function parameter dev covariates section
  ln=readLines(con,n=1); nl=nl+1;
  while(isOpen(con)&(!stringr::str_starts(ln,paste0("[:blank:]*END ",type)))){
    cat("\t",nl,":\t",ln,"\n");
    ln=readLines(con,n=1); nl=nl+1;
    cat(stringr::str_starts(ln,paste0("[:blank:]*END ",type)),"\t","\n")
  }#--while
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_NaturalMortality<-function(con,nl){
  type = "NATURAL MORTALITY";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_Growth<-function(con,nl){
  type = "GROWTH";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_Maturity<-function(con,nl){
  type = "MATURITY";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_Selectivity<-function(con,nl){
  type = "SELECTIVITY";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_FishingMortality<-function(con,nl){
  type = "FISHING MORTALITY";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

readCTL_SurveyIndices<-function(con,nl){
  type = "SURVEY INDICES";
  cat("reading",type,"section starting on line",nl,"\n");
  lst = list();
  ln=readLines(con,n=1); nl=nl+1;
  #--read section
  cat(type,"section ended on line",nl,"\n");
  return(list(nl=nl,lst=lst));
}

#tst = readCTL("test_ctl.txt");

