#'
#' @title Create model
#' @description Function to create a gmacs model
#'
#' @param inputs - list of inputs to the model
#' @param mode - run mode ("run","debug")
#'
#' @details Creates a gmacs model with provided inputs.
#' #Example
#' inputs = list();
#' inputs$data = list(dims=1:3);
#' inputs$pars = list(pars=4:7);
#' model = tmbGMACS::createModel(inputs,mode="debug");
#'
#' @importFrom TMB MakeADFun
#' @export
#'
createModel<-function(inputs,mode="debug"){
    #--create data list
    lst_data = list(model="gmacs",
                    mode=mode);
    for (id in names(inputs$data)){
        lst_data[[id]] = inputs$data[[id]];
    }

    #--create parameters list
    lst_params = inputs$pars;

    #--create model
    model <- TMB::MakeADFun(data=lst_data,
                            parameters=lst_params,
                            DLL="tmbGMACS_TMBExports");
    return(model);
}
