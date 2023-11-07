#'
#' @title Convert dimension values to factor levels in a dimensions map
#' @description Function to convert dimension values to factor levels in a dimensions map.
#' @param dfr - dimensions map dataframe/tibble (e.g. output from [createSparseDimsMap()])
#'
#' @return - dimensions map with dimension values converted to factors
#'
#' @details Dimension columns that are already factors are not converted.
#'
#' @export
#'
createDimsFactors<-function(dfr){
    dmnms = attr(dfr,"dmnms");
    dmlvs = attr(dfr,"dmlvs");
    for (dmnm in dmnms){
        if (is.factor(dfr[[dmnm]]))
            dfr[[dmnm]] = factor(dfr[[dmnm]],levels=dmlvs[[dmnm]]);
    }
    return(dfr);
}

# dfrp = createDimFactors(dfrSparse); str(dfrp);
# dfrp = createDimFactors(dfrp);      str(dfrp);
