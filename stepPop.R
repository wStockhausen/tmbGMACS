#--propagate population forward

#--initial population
N;

#--loop over years to advance population
for (iy in 1:(nYs-1)){
    #--iy=1;
    iyv = vYs[[iy]];
    #--loop over seasons
    for (ic in 1:(nCs-1)){
        icv = vCs[[ic]];
        for (is in 1:nSrvs) {
            srvN[[is]] = calcSurveys(N=N[[iy]][[ic]]);
        }
        N[[iy]][[ic+1]] = stepPopOneSeason(N=N[[iy]][[ic]],#--population at start of season
                                           M=M[[iy]][[ic]],#--natural mortality
                                           F=F[[iy]][[ic]],#--fishing mortality
                                           L=L[[iy]][[ic]],#--probability of molting
                                           G=G[[iy]][[ic]],#--growth transition
                                           T=T[[iy]][[ic]],#--probability of maturation
                                           V=V[[iy]][[ic]] #--probability of movement
                                          );
    }
    #--do final season and advance to beginning of next year
    ic = nCs;
    N[[iy+1]][[1]] = stepPopOneSeason(N=N[[iy]][[ic]],#--population at start of season
                                      M=M[[iy]][[ic]],#--natural mortality
                                      F=F[[iy]][[ic]],#--fishing mortality
                                      L=L[[iy]][[ic]],#--probability of molting
                                      G=G[[iy]][[ic]],#--growth transition
                                      T=T[[iy]][[ic]],#--probability of maturation
                                      V=V[[iy]][[ic]] #--probability of movement
                                      );
    #--add recruitment
    N[[iy+1]][[1]] = addRecruitment(N[[iy+1]][[1]],R[[iy]])
}
