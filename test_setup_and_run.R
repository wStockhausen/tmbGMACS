#--setup and run simple model
require(tmbGMACS);

#--NOTES:
##--a dimensions map is a dataframe/tibble in which
##----the first column ("i") is an index mapping to included dimension level combinations
##----the names of remaining columns are dimension names
##----each row represents a unique combination of the dimension levels

#--define model population dimensions
vYs = 2001:2005;                   attr(vYs,"dmnms")<-"y"; #--model years
vCs = c("fall","spring");          attr(vCs,"dmnms")<-"c"; #--seasons
vXs = c("male","female");          attr(vXs,"dmnms")<-"x"; #--sexes
vMs = c("immature","mature");      attr(vMs,"dmnms")<-"m"; #--maturity states
vSs = list(IMMATURE=c("new_shell"),                        #--shell condition
           MATURE  =c("new_shell",
                      "old_shell")); attr(vSs,"dmnms")<-c("m","s");
vZCs = seq(25.0,105,5);              attr(vZCs,"dmnms")<-"zc"; #--size bin cutpts
vZBs = calcMidPoints(vZCs);          attr(vZBs,"dmnms")<-"zb"; #--size bin midpoints
##--create model population dimensions map:
dfrMod = createSparseDimsMap(y=vYs,c=vCs,x=vXs,m=vMs,s=vSs,zb=vZBs);

#--define fleets
vFs  = c("TCF","NMFS");            attr(vFs,"dmnms")<-"f";  #--fleets

#--set up population dynamics for model run
lst_ctl = list(); lst_pars = list();

lst_ctl[["hasTerminalMolt"]] = TRUE;#--flag indicating terminal molt
lst_ctl[["fisheries"]] = c("TCF");  #--vector of fishing fleet names
lst_ctl[["surveys"]]   = c("NMFS"); #--vector of survey names

