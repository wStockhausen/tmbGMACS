/** \file Dimensions.hpp
 */
/**
* File: Dimensions.hpp
 *
 * Author: William Stockhausen
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Email: william.stockhausen@noaa.gov
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Generalized Model for Assessing Crustacean Stocks (GMACS) project.
 * GMACS is modeled after, and heavily indebted to, the NMFS FIMS project.
 * See LICENSE in the source folder for reuse information.
 *
*/

#ifndef GMACS_RCPP_DIMENSIONS_HPP
#define GMACS_RCPP_DIMENSIONS_HPP

#include <iostream>
#include <Rcpp.h>

namespace gmacs{
  class Dimensions{
    public:
      /** number of dimensions */
      int num_dims = 0;
      /** vector of dimension names */
      Rcpp::CharacterVector dims_names;
      /** vector with each element the number of unique levels in the corresponding dimension. */
      Rcpp::IntegerVector dims_nums;
      /** list with each element the vector of unique levels for the corresponding dimension. */
      Rcpp::List dims_lvls;
      /**
       * Matrix mapping from the "sparse" 1-d index value to the corresponding "dense" 1-d index value
       * and the associated level in each dimension. The matrix has 1+num_dims columns, with
       * the dense index value in the first column and the dimension levels in subsequent columns.
       *
       * int dense_idx = mapS2D[sparse_idx,0];
       * int dim1_idx  = mapS2D[sparse_idx,1]; //etc.
       */
      Rcpp::IntegerMatrix mapS2D;//"dense" indices by "sparse" row number
      /**
       * Vector mapping from the "dense" 1-d index value to the corresponding "sparse" 1-d index value
       * (or -1 if no corresponding spare value).
       *
       * int sparse_idx = mapD2S[dense_idx];
       */
      Rcpp::IntegerVector mapD2S;

      /**
       * Constructor.
       *
       * @param dims_names_ - character vector with dimension names
       * @param dims_lvls_ - list of character vectors with levels by dimension name
       * @param mapS2D_ - integer matrix (see details)
       * @param mapD2S_ - integer vector with elements corresponding to the sparse index value
       *
       * @details mapS2D_ should be an integer matrix with the row index corresponding to the
       * 1s-based sparse index value. The first column indicates the corresponding 1s-based
       * dense index value. Subsequent columns indicate the index of the corresponding
       * dimension level. Thus mapS2D_[i,1] is the dense index corresponding to i and
       * mapS2D_[i,j+1] is the index into the levels of dimension j corresponding to i.
       * mapS2D_ can be obtained using
       *  lst = tmbGMACS::createDimsMaps(...);
       *  matS2D_ = sapply(lst$dfrS2D,as.integer);
       *
       * mapD2S_ should be an integer vector where mapD2S_[i] gives the value of the
       * 1s-based sparse index corresponding to the dense index i (or -1 if there is
       * no corresponding sparse index). mapD2S_ can be obtained using
       *  lst = tmbGMACS::createDimsMaps(...);
       *  mapD2S_ = dfrD2S$idx;
       *
       * All indices in mapS2D and mapD2S are converted from 1s-based to 0s-based
       * for consistency with C++ indexing.
       */
      Dimensions(Rcpp::CharacterVector dims_names_,
                 Rcpp::List dims_lvls_,
                 Rcpp::IntegerMatrix mapS2D_,
                 Rcpp::IntegerVector mapD2S_):
        dims_names(dims_names_),dims_lvls(dims_lvls_),mapS2D(clone(mapS2D_)),mapD2S(clone(mapD2S_)){
        std::cout<<"in Dimensions::Dimensions()."<<std::endl;
        num_dims = dims_names.size();
        std::cout<<"number of dims is "<<num_dims<<std::endl;
        dims_nums = Rcpp::IntegerVector(num_dims);
        //get number of levels for each dimension
        for (int i=0;i<num_dims;i++){
          Rcpp::CharacterVector cv = dims_lvls[i];
          dims_nums[i] = cv.size();
          std::cout<<"number of levels for "<<dims_names[i]<<" is "<<dims_nums[i]<<std::endl;
        }
        //convert 1s-based (R) indices to 0s-based (C++) indices
        for (int i=0;i<mapS2D.nrow();i++){
          for (int j=0;j<mapS2D.ncol();j++) mapS2D(i,j) -= 1;
        }
        for (int i=0;i<mapD2S.size();i++) if (mapD2S(i)>0) mapD2S(i) -= 1;
      }

      /**
       * Returns the 0s-based sparse index corresponding to a vector indicating
       * the indices of the corresponding individual dimension levels (or -1 if
       * the levels do not correspond to a sparse index).
       */
      int getSparseIndex(Rcpp::IntegerVector lvls,bool zerosBased=true){
        //want to calculate the sparse index given the vector of individual dimension indices
        std::cout<<"in Dimensions::getSparseIndex(zerosBased="<<zerosBased<<")"<<std::endl;
        int idx = 0;
        //calculate dense index
        int idxd = getDenseIndex(lvls,zerosBased);
        //extract sparse index
        idx = mapD2S(idxd);
        return idx;
      }

      /**
       * Returns the 0s-based dense index corresponding to a vector indicating
       * the indices of the corresponding individual dimension levels.
       */
      int getDenseIndex(Rcpp::IntegerVector lvls,bool zerosBased=true){
        //want to calculate the dense index given the vector of individual dimension indices
        std::cout<<"in Dimensions::getDenseIndex(zerosBased="<<zerosBased<<")"<<std::endl;
        if (lvls.size()!=num_dims)
          Rcpp::stop("in Dimnsions::getDenseIndex: size of input vector must match number of dimensions.");
        int idx = 0;
        int mlt = 1;
        int adj = 1-(int) zerosBased;
        std::cout<<"adj = "<<std::endl;
        for (int i=(num_dims-1);i>-1;i--){
          idx += mlt*(lvls[i]-adj);//--adjust lvls for base
          mlt *= dims_nums[i];
          std::cout<<"\tdim["<<i<<"]: "<<i<<" "<<lvls[i]<<" "<<mlt<<" "<<idx<<std::endl;
        }
        return idx;
      }

      /**
       * Returns the dimensions levels corresponding to a sparse index value
       *
       *
       */
      Rcpp::CharacterVector getDimensionsLevelsFromSparseIndex(int idx, bool zerosBased=true){
        std::cout<<"in Dimensions::getDimensionsLevelsFromSparseIndex("<<idx<<" "<<zerosBased<<"): "<<std::endl;
        if (!zerosBased) idx -= 1;
        std::cout<<idx<<" "<<mapS2D.nrow()<<std::endl;
        Rcpp::IntegerVector ilvls = mapS2D.row(idx);//vector of 0's based dimension indices
        std::cout<<ilvls.size()<<std::endl;
        for (int i=0;i<=num_dims;i++) std::cout<<ilvls(i)<<" ";
        std::cout<<std::endl;
        Rcpp::CharacterVector clvls = Rcpp::CharacterVector(num_dims);
        for (int i=0;i<num_dims;i++){
          std::cout<<i<<" "<<ilvls(i+1)<<" ";
          Rcpp::CharacterVector cv = dims_lvls(i);//character values for levels of ith dimension
          clvls(i) = cv(ilvls(i+1));//first value in ilvls is the dense index, then come the dimension level indices
          std::cout<<clvls(i)<<std::endl;
        }
        return clvls;
      }

      /**
       * Returns the dimensions levels corresponding to a dense index value
       *
       *
       */
      Rcpp::CharacterVector getDimensionsLevelsFromDenseIndex(int idx, bool zerosBased=true){
        std::cout<<"in Dimensions::getDimensionsLevelsFromDenseIndex("<<idx<<" "<<zerosBased<<"): "<<std::endl;
        if (!zerosBased) idx -= 1;
        int rw = mapD2S(idx); //convert to sparse index, which is row into mapS2D
        std::cout<<rw<<" "<<mapS2D.nrow()<<std::endl;
        Rcpp::IntegerVector ilvls = mapS2D.row(rw);//vector of 0's based dimension indices
        std::cout<<"sanity check: idx = "<<idx<<". ilvls(0) = "<<ilvls(0)<<". Should be identical!"<<std::endl;
        std::cout<<ilvls.size()<<std::endl;
        for (int i=0;i<=num_dims;i++) std::cout<<ilvls(i)<<" ";
        std::cout<<std::endl;
        Rcpp::CharacterVector clvls = Rcpp::CharacterVector(num_dims);
        for (int i=0;i<num_dims;i++){
          std::cout<<i<<" "<<ilvls(i+1)<<" ";
          Rcpp::CharacterVector cv = dims_lvls(i);//character values for ith dimension
          clvls(i) = cv(ilvls(i+1));//first value in ilvls is the dense index, then come the dimension level indices
          std::cout<<clvls(i)<<std::endl;
        }
        return clvls;
      }

      /**
       * Returns the indices for the dimensions levels corresponding to a sparse index value
       *
       * Returned indices are 0s- or 1s-based depending on zerosBased.
       *
       */
      Rcpp::IntegerVector getDimensionsLevelIndicesFromSparseIndex(int idx, bool zerosBased=true){
        std::cout<<"in Dimensions::getDimensionsLevelIndicesFromSparseIndex("<<idx<<" "<<zerosBased<<"): "<<std::endl;
        if (!zerosBased) idx -= 1;
        std::cout<<idx<<" "<<mapS2D.nrow()<<std::endl;
        Rcpp::IntegerVector ilvls = mapS2D.row(idx);//vector of 0's based dimension indices
        if (!zerosBased)
          for (int i=0;i<=num_dims;i++)
            ilvls(i) += 1;
        return ilvls;
      }

      /**
       * Returns the indices for the dimensions levels corresponding to a dense index value
       *
       * Returned indices are 0s- or 1s-based depending on zerosBased.
       *
       */
      Rcpp::IntegerVector getDimensionsLevelIndicesFromDenseIndex(int idx, bool zerosBased=true){
        std::cout<<"in Dimensions::getDimensionsLevelIndicesFromDenseIndex("<<idx<<" "<<zerosBased<<"): "<<std::endl;
        if (!zerosBased) idx -= 1;
        int rw = mapD2S(idx); //convert to sparse index, which is row into mapS2D
        std::cout<<rw<<" "<<mapS2D.nrow()<<std::endl;
        Rcpp::IntegerVector ilvls = mapS2D.row(rw);//vector of 0's based dimension indices
        std::cout<<"sanity check: idx = "<<idx<<". ilvls(0) = "<<ilvls(0)<<". Should be identical!"<<std::endl;
        if (!zerosBased)
          for (int i=0;i<=num_dims;i++)
            ilvls(i) += 1;
        return ilvls;
      }
  };
}//--namespace gmacs

#define TESTING
#undef TESTING //comment out to include TESTING section
#ifdef TESTING
/*** R
#--could define R code to be run automatically after compilation by sourceCpp here.

#--see testing/testing.R for code that uses sourceCpp to compile the dims_module as
#--well as test it.
*/
#endif //--TESTING
#endif //--GMACS_RCPP_DIMENSIONS_HPP
