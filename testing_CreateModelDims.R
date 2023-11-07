#--create example dimensions and levels
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

#--create dataframe with index ("i") mapping to realized dimension level combinations
dfrSparse = createSparseDimsMap(y=vYs,c=vCs,r=vRs,x=vXs,m=vMs,s=vSs,a=vAs,z=vZs);
#--create dataframe with index ("j") mapping realized dimension levels to all combinations
dfrFull   = createFullDimsMap(dfrSparse); #--all possible combinations
