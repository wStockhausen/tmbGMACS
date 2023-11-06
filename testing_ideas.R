#'
#' @title Get 1-d index corresponding to array index
#'
getIndex8<-function(y,n,r,x,m,s,a,z){
    i = z+getIndex2(y,n,r,x,m,s,a)
}

require(tibble)
vYs = 2001:2005;                  attr(vYs,"dmnms")<-"y";
vCs = c("fall","spring");         attr(vCs,"dmnms")<-"c";
vRs = c("All EBS");               attr(vRs,"dmnms")<-"r";
vXs = c("MALE","FEMALE");         attr(vXs,"dmnms")<-"x";
vMs = list(  MALE=c("IMMATURE","MATURE"),
           FEMALE=c("IMMATURE","MATURE"));
attr(vMs,"dmnms")<-c("x","m");
vSs = list(MALE=  list(IMMATURE=c("NEW SHELL"),
                        MATURE=c("NEW SHELL","OLD SHELL")),
          FEMALE=list(IMMATURE=c("NEW SHELL"),
                        MATURE=c("NEW SHELL","OLD SHELL")));
attr(vSs,"dmnms")<-c("x","m","s");
vAs = list(MALE=  list(IMMATURE=list(`NEW SHELL`="NO AGEs"),
                         MATURE=list(`NEW SHELL`=c("PTM 0"),
                                     `OLD SHELL`=c("PTM 1","PTM 2"))),
          FEMALE=list(IMMATURE=list(`NEW SHELL`="NO AGEs"),
                        MATURE=list(`NEW SHELL`=c("PTM 0"),
                                    `OLD SHELL`=c("PTM 1","PTM 2"))));
attr(vAs,"dmnms")<-c("x","m","s","a");
vZs = list( MALE=  list(IMMATURE=list(`NEW SHELL`=list(`NO AGEs`=seq(40,60,5))),
                          MATURE=list(`NEW SHELL`=list(`PTM 0`=seq(80,120,10)),
                                      `OLD SHELL`=list(`PTM 1`=seq(80,120,10),
                                                       `PTM 2`=seq(80,120,10)))),
          FEMALE=list(IMMATURE=list(`NEW SHELL`=list(`NO AGEs`=seq(25,50,5))),
                        MATURE=list(`NEW SHELL`=list(`PTM 0`=seq(50,100,10)),
                                    `OLD SHELL`=list(`PTM 1`=seq(50,100,10),
                                                     `PTM 2`=seq(50,100,10)))));
attr(vZs,"dmnms")<-c("x","m","s","a","z");

#--test list traversal
traverseList<-function(lst0,name,level=0){
    col = paste0("v",level);
    cat(paste0(col,":"),name,"\n");
    if (inherits(lst0,"list")) {
        dfr = NULL;
        for (name0 in names(lst0)){
            res   = traverseList(lst0[[name0]],name0,level=level+1);
            level = res$level;#--reset level
            dfr1  = res$dfr;
            dfr1[[col]] = name;
            dfr   = dplyr::bind_rows(dfr,dfr1); #--copy
        }
    } else {
        cat("\t",lst0,"\n");
        dfr = tibble::tibble(val=lst0,name=name);
        names(dfr)[2] = col;
        if (level>0) return(list(level=level-1,dfr=dfr));
        #--level==0 drops out to last bit
    }
    if (level==0) {
        dfr = dfr |> dplyr::select(rev(seq(1,ncol(dfr)-1,1)));
        if (is.null(attr(lst0,"dmnms"))){
            names(dfr) =  name;
        } else {
            names(dfr) = attr(lst0,"dmnms");
        }
        return(dfr);
    }
    return(list(level=level-1,dfr=dfr));
}
dfry = traverseList(vYs,"y");
dfrp = traverseList(vZs,"z");

#' @param ... <[`dynamic-dots`][rlang::dyn-dots]> name/value dimension pairs
createIndexMap<-function(...){
    dfr = NULL;
    dots = rlang::list2(...);
    for (nm in names(dots)){
        dfrp = traverseList(dots[[nm]],nm);
        if (is.null(dfr)) {
            dfr = dfrp;
        } else {
            dfr = dfr |> dplyr::cross_join(dfrp,suffix=c("LFT","RGT"));
            nmsL = stringr::str_sub(stringr::str_subset(names(dfr),"LFT$"),end=-4);
            if (length(nmsL)>0){
                #--duplicate column names (ignoring trailing LFT/RGT) -> over-expanded rows
                #----keep only rows where ?LFT==?RGT
                for (nm in nmsL){
                  dfr = dfr |> dplyr::filter(.data[[paste0(nm,"LFT")]]==.data[[paste0(nm,"RGT")]]);
                }
                # #--remove "LFT" from column names, drop ?RGT columns
                nmsL = stringr::str_sub(stringr::str_subset(names(dfr),"LFT$"),end=-4);
                nmsA = stringr::str_subset(names(dfr),"RGT$",negate=TRUE);
                dfr  = dfr |> dplyr::select(dplyr::all_of(nmsA));
                nmsA = ifelse(nmsA %in% paste0(nmsL,"LFT"),
                              stringr::str_sub(nmsA,end=-4),nmsA);
                names(dfr) = nmsA;
            }
            #--convert all dimensions to factors
            for (nm in names(dfr)) dfr[[nm]] = factor(dfr[[nm]]);
        }
    }
    dfr = dfr |> dplyr::mutate(i=dplyr::row_number(),.before=1);
    return(dfr);
}
dfrAll = createIndexMap(y=vYs);
dfrAll = createIndexMap(y=vYs,c=vCs);
dfrAll = createIndexMap(y=vYs,z=vZs);
dfrAll = createIndexMap(a=vAs);
dfrAll = createIndexMap(z=vZs);
dfrAll = createIndexMap(a=vAs,z=vZs);
dfrAll = createIndexMap(y=vYs,c=vCs,r=vRs,x=vXs,m=vMs,s=vSs,a=vAs,z=vZs);

dnms = names(dfrAll)[2:length(names(dfrAll))];
dmsl = list();
dmsn = vector("numeric",length(dnms));
names(dmsn) = dnms;
for (dnm in dnms){
    dmsl[[dnm]] = levels(dfrAll[[dnm]]);
    dmsn[dnm]   = length(dmsl[[dnm]]);
}
attr(dmsl,"dmnms") <-dnms;
attr(dmsn,"dmnms") <-dnms;

calcIndexFull<-function(v,n,rev=FALSE){
    idx = 0; mi = 1;
    if (!rev){
        for (i in seq(1,length(v))){
            idx = idx + (v[i]-1)*mi;
            mi  = mi*n[i]
        }
    } else {
        for (i in rev(seq(1,length(v)))){
            idx = idx + (v[i]-1)*mi;
            mi  = mi*n[i]
        }
    }
    return(unname(idx)+1);
}
rev=TRUE;
calcIndexFull(c(1,1,1),c(5,5,5),rev=rev);
calcIndexFull(c(5,1,1),c(5,5,5),rev=rev);
calcIndexFull(c(1,5,1),c(5,5,5),rev=rev);
calcIndexFull(c(1,1,5),c(5,5,5),rev=rev);
calcIndexFull(c(5,5,1),c(5,5,5),rev=rev);
calcIndexFull(c(1,5,5),c(5,5,5),rev=rev);
calcIndexFull(c(5,5,5),c(5,5,5),rev=rev);


createIndexMapFull<-function(lst){
    dfr = NULL;
    for (nm in names(lst)) {
        #--nm = names(lst)[3];
        dfrp = traverseList(lst[[nm]],nm);
        if (is.null(dfr)){
            dfr = dfrp;
        } else {
            dfr  = dfr |> dplyr::cross_join(dfrp);
        }
    }
    dfr = dfr |> dplyr::mutate(j=dplyr::row_number(),.before=1)
    return(dfr);
}
dfrDMS = createIndexMapFull(lst=dmsl);

dfrA2D = dfrAll |> dplyr::full_join(dfrDMS,by=names(dms)) |>
           dplyr::mutate(i=ifelse(is.na(i),-1,i));
dfrD2A = dfrDMS |> dplyr::full_join(dfrAll,by=names(dms)) |>
           dplyr::mutate(i=ifelse(is.na(i),-1,i));


################################################################################
#--create tibble from lists
i=0;
lst = list(); lstv = list();
for (iy in 1:length(vYs)){
    iyv = vYs[iy];
    for (ic in 1:length(vCs)){
        icv = vCs[ic];
        for (ir in 1:length(vRs)){
            irv = vRs[ir];
            for (ix in 1:length(vXs)){
                #--ix=1;
                ixv = vXs[[ix]];
                for (im in 1:length(vMs[[ix]])){
                    #--im=1;
                    imv = vMs[[ix]][[im]];
                    for (is in 1:length(vSs[[ix]][[im]])){
                        #--is=1;
                        isv = vSs[[ix]][[im]][[is]];
                        for (ia in 1:length(vAs[[ix]][[im]][[is]])){
                            #--ia=1;
                            iav = vAs[[ix]][[im]][[is]][[ia]];
                            for (iz in 1:length(vZs[[ix]][[im]][[is]][[ia]])){
                                #--iz=1;
                                i = i+1;
                                izv = vZs[[ix]][[im]][[is]][[ia]][[iz]];
                                dfrYCRXMSAZ = tibble::tibble(i=i,
                                                             z=iz,#--cycles fastest
                                                             a=ia,
                                                             s=is,
                                                             m=im,
                                                             x=ix,
                                                             r=ir,
                                                             c=ic,
                                                             y=iy);#--cycles slowest
                                lst[[i]] = dfrYCRXMSAZ;
                                dfrYCRXMSAZv = tibble::tibble(i=i,
                                                             z=izv,#--cycles fastest
                                                             a=iav,
                                                             s=isv,
                                                             m=imv,
                                                             x=ixv,
                                                             r=irv,
                                                             c=icv,
                                                             y=iyv);#--cycles slowest
                                lstv[[i]] = dfrYCRXMSAZv;
                            }#-z
                        }#--a
                    }#--s
                }#--m
            }#--x
        }#--r
    }#--c
}#--y
dfrIndices = dplyr::bind_rows(lst);
dfrValues  = dplyr::bind_rows(lstv);

