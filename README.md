# README

<script src="README_files/libs/kePrint-0.0.1/kePrint.js"></script>
<link href="README_files/libs/lightable-0.0.1/lightable.css" rel="stylesheet" />


- [tmbGMACS](#tmbgmacs)
  - [Installation](#installation)
  - [Introduction](#introduction)
  - [Population model description](#population-model-description)
  - [Model dimensions](#model-dimensions)
  - [Aggregating over model
    dimensions](#aggregating-over-model-dimensions)

<!-- README.md is generated from README.qmd. Please edit README.qmd, then render README.md using `quarto render README.qmd`. -->
<!-- use `quarto render README.qmd` to build README.md prior to committing to keep README.md up-to-date-->
<!-- don't forget to commit and push the resulting figure files, so they display on GitHub and CRAN.-->

# tmbGMACS

<!-- badges: start -->
<!-- badges: end -->

tmbGMACS is an [R](https://www.r-project.org/) package that, when
completed, will provide a TMB-based implementation of the Generalized
Model for Assessing Crustacean Stocks
([GMACS](https://github.com/GMACS-project/GMACS_Assessment_code))
developed using AD MODEL BUILDER ([ADMB](http://www.admb-project.org)).
TMB ([Template Model Builder](https://kaskr.github.io/adcomp)) is an R
package similar to ADMB that provides the ability to develop and run
C++-based statistical models using automatic differentiation in the R
framework.

## Installation

You can install the development version of tmbGMACS from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("wStockhausen/tmbGMACS")
```

## Introduction

The purpose of tmbGMACS is to implement a population model for a fished
crustacean stock and fit it to fishery and survey data to estimate
various population processes (e.g., recruitment, growth, and mortality)
and population size in order to determine sustainable fishing limits and
targets. The ADMB version of GMACS is currently used to provide fishery
stock assessment information to the North Pacific Fishery Management
Council ([NPFMC](https://www.npfmc.org)) for a number of crab stocks in
the Bering Sea and Aleutian Islands (see, for example, the stock
assessment reports for [Bristol Bay red king
crab](https://meetings.npfmc.org/CommentReview/DownloadFile?p=b98b90b2-88ab-43c2-9487-c12cdb4e0a25.pdf&fileName=BBRKC%20SAFE%202022%20Final.pdf)
and [snow
crab](https://meetings.npfmc.org/CommentReview/DownloadFile?p=fca55335-ad34-4896-9b1e-4c09aa8342ce.pdf&fileName=EBS%20Snow%20SAFE%20FINAL.pdf)).

Here, the TMB model is referred to as “gmacs” while the R package is
referred to “tmbGMACS”. The R package provides functions to set up data
inputs to gmacs and associated likelihood functions, select functional
forms for various population processes, define estimable parameters and
associated priors or penalties, run the model, create plots and tables
from a fitted model, and compare multiple models.

## Population model description

The gmacs population model integrates the abundance of individuals in
various discrete population categories over time based on assumptions
regarding population processes consisting of recruitment, growth,
maturation, natural mortality, and fishing mortality. Typical population
categories might be sex and size, but it is possible to define an
arbitrary number of categories (e.g., region, sex, maturity state, shell
condition, and size). Although crustaceans cannot typically be aged, age
could be included as a population category if population processes such
as maturation were thought to have a strong age dependence.

Population abundance in gmacs is represented as vector, $\mathbf{n}$,
with elements $n_i$, where $i$ is a 1-dimensional index $i$ combining
time (year and season) and levels across the population categories. The
temporal nature of the calculations is more easily described, though, by
differentiating between time and the population categories, so
$\mathbf{n_t}$ is a vector representing the abundance in each category
at time $t$ (i.e., a vector with elements $\mathbf{n_t}_c$):

<span id="eq-1">$$\frac{d\mathbf{n_t}}{dt} = \left[ \mathbf{T_t}-(\mathbf{M_t}+\mathbf{F_t}) \right]*\mathbf{n_t} \qquad(1)$$</span>

where $\mathbf{T_t}$ is a $c$ x $c$ matrix representing transition rates
between population categories (e.g., growth or movement) at time $t$,
$\mathbf{M_t}$ is a similarly-sized diagonal matrix representing natural
mortality at time t, and $\mathbf{M_t}$ is a similarly-sized diagonal
matrix representing fishing mortality at time $t$. For gmacs, rates
within a (seasonal) timestep are assumed to be constant. Thus, @eqn-1
could be integrated to obtain

<span id="eq-2">$$\mathbf{n_{t+1}} = e^{\mathbf{A_t} \delta t}*\mathbf{n_t} \qquad(2)$$</span>

where $\delta t$ is the time step from $t$ to $t+1$, and
$\mathbf{A_t}=\left[ \mathbf{T_t}-(\mathbf{M_t}+\mathbf{F_t}) \right]$.
In practice, when $\mathbf{T_t}$ represents growth processes, these are
regarded as discrete-time processes with a season length of 0, so that

<span id="eq-3">$$ \mathbf{n_{t+1}} = \mathbf{T_t}*\mathbf{n_t}  \qquad(3)$$</span>

for seasons in which growth occurs and

<span id="eq-4">$$ \mathbf{n_{t+1}} = e^{-\left[ \mathbf{M_t}+\mathbf{F_t} \right]*\delta t}*\mathbf{n_t} \qquad(4)$$</span>

for seasons with non-0 lengths in which mortality occurred.

Recruitment, the addition of new individuals to the model population
categories, is also considered a discrete-time process associated with a
season length of 0, so

<span id="eq-5">$$ \mathbf{n_{t+1}} = \mathbf{R_t}+\mathbf{n_t} \qquad(5)$$</span>

when recruitment occurs, where $\mathbf{R_t}$ is a vector of length $c$.

Let $<\mathbf{R_s}>$ represent the equilibrium (i.e., longterm mean)
recruitment in (within-year) season $s$, and similarly for
$<\mathbf{T_s}>$ and $\mathbf{Z_s}=\mathbf{M_s}+\mathbf{F_s}$. The
equilibrium population size $<\mathbf{n_s}>$ in season $s$ is then
determined by the solution to

<span id="eq-6">$$<\mathbf{n_s}> =  \qquad(6)$$</span>

## Model dimensions

Model dimensions consist of year (**y**), season (**s**), and a set of
categories ({**c**}) into which the population is divided (e.g., sex and
size). Levels within a given population category can be nested within
another category (e.g., different size classes might be defined for
males and females if they exhibited substantially different growth
patterns). For a given year, population abundance is integrated across
seasons within the year and different processes can occur in different
seasons.

Dimensions are defined using the function . As an example
([Table 1](#tbl-Ex1)), the following creates a set of dimensions
encompassing year (**y**), season(**s**), sex (**x**), and size (**z**),
with different size classes for males and females. Note that the
“dimension name(s)” are added to the vector or list defining the levels
as an attribute “dmnms”.

``` r
library(tmbGMACS);
#> Create the individual dimensions as vectors or lists (for nested dimensions)
#> --assign the attribute "dmnms", the dimension name(s) (plural if nested)
#> ----model years
vYs = 2001:2005;                  attr(vYs,"dmnms")<-"y";
#> ----model seasons
vSs = c("fall","spring");         attr(vSs,"dmnms")<-"s";
#> ----model sexes
vXs = c("MALE","FEMALE");         attr(vXs,"dmnms")<-"x";
#> ----model sizes, nested with sexes
vZs = list(  MALE=seq(40,60,5),
           FEMALE=seq(25,50,5));  attr(vZs,"dmnms")<-c("x","z");
#>--create the dimensions "map"
dms = tmbGMACS::createSparseDimsMap(y=vYs,s=vSs,x=vXs,z=vZs);
```

<div id="tbl-Ex1">

|   i | y    | s      | x      | z   |
|----:|:-----|:-------|:-------|:----|
|   1 | 2001 | fall   | MALE   | 40  |
|   2 | 2001 | fall   | MALE   | 45  |
|   3 | 2001 | fall   | MALE   | 50  |
|   4 | 2001 | fall   | MALE   | 55  |
|   5 | 2001 | fall   | MALE   | 60  |
|   6 | 2001 | fall   | FEMALE | 25  |
|   7 | 2001 | fall   | FEMALE | 30  |
|   8 | 2001 | fall   | FEMALE | 35  |
|   9 | 2001 | fall   | FEMALE | 40  |
|  10 | 2001 | fall   | FEMALE | 45  |
|  11 | 2001 | fall   | FEMALE | 50  |
|  12 | 2001 | spring | MALE   | 40  |
|  13 | 2001 | spring | MALE   | 45  |
|  14 | 2001 | spring | MALE   | 50  |
|  15 | 2001 | spring | MALE   | 55  |
|  16 | 2001 | spring | MALE   | 60  |
|  17 | 2001 | spring | FEMALE | 25  |
|  18 | 2001 | spring | FEMALE | 30  |
|  19 | 2001 | spring | FEMALE | 35  |
|  20 | 2001 | spring | FEMALE | 40  |
|  21 | 2001 | spring | FEMALE | 45  |
|  22 | 2001 | spring | FEMALE | 50  |
|  23 | 2002 | fall   | MALE   | 40  |
|  24 | 2002 | fall   | MALE   | 45  |
|  25 | 2002 | fall   | MALE   | 50  |
|  26 | 2002 | fall   | MALE   | 55  |
|  27 | 2002 | fall   | MALE   | 60  |
|  28 | 2002 | fall   | FEMALE | 25  |
|  29 | 2002 | fall   | FEMALE | 30  |
|  30 | 2002 | fall   | FEMALE | 35  |
|  31 | 2002 | fall   | FEMALE | 40  |
|  32 | 2002 | fall   | FEMALE | 45  |
|  33 | 2002 | fall   | FEMALE | 50  |
|  34 | 2002 | spring | MALE   | 40  |
|  35 | 2002 | spring | MALE   | 45  |
|  36 | 2002 | spring | MALE   | 50  |
|  37 | 2002 | spring | MALE   | 55  |
|  38 | 2002 | spring | MALE   | 60  |
|  39 | 2002 | spring | FEMALE | 25  |
|  40 | 2002 | spring | FEMALE | 30  |
|  41 | 2002 | spring | FEMALE | 35  |
|  42 | 2002 | spring | FEMALE | 40  |
|  43 | 2002 | spring | FEMALE | 45  |
|  44 | 2002 | spring | FEMALE | 50  |
|  45 | 2003 | fall   | MALE   | 40  |
|  46 | 2003 | fall   | MALE   | 45  |
|  47 | 2003 | fall   | MALE   | 50  |
|  48 | 2003 | fall   | MALE   | 55  |
|  49 | 2003 | fall   | MALE   | 60  |
|  50 | 2003 | fall   | FEMALE | 25  |
|  51 | 2003 | fall   | FEMALE | 30  |
|  52 | 2003 | fall   | FEMALE | 35  |
|  53 | 2003 | fall   | FEMALE | 40  |
|  54 | 2003 | fall   | FEMALE | 45  |
|  55 | 2003 | fall   | FEMALE | 50  |
|  56 | 2003 | spring | MALE   | 40  |
|  57 | 2003 | spring | MALE   | 45  |
|  58 | 2003 | spring | MALE   | 50  |
|  59 | 2003 | spring | MALE   | 55  |
|  60 | 2003 | spring | MALE   | 60  |
|  61 | 2003 | spring | FEMALE | 25  |
|  62 | 2003 | spring | FEMALE | 30  |
|  63 | 2003 | spring | FEMALE | 35  |
|  64 | 2003 | spring | FEMALE | 40  |
|  65 | 2003 | spring | FEMALE | 45  |
|  66 | 2003 | spring | FEMALE | 50  |
|  67 | 2004 | fall   | MALE   | 40  |
|  68 | 2004 | fall   | MALE   | 45  |
|  69 | 2004 | fall   | MALE   | 50  |
|  70 | 2004 | fall   | MALE   | 55  |
|  71 | 2004 | fall   | MALE   | 60  |
|  72 | 2004 | fall   | FEMALE | 25  |
|  73 | 2004 | fall   | FEMALE | 30  |
|  74 | 2004 | fall   | FEMALE | 35  |
|  75 | 2004 | fall   | FEMALE | 40  |
|  76 | 2004 | fall   | FEMALE | 45  |
|  77 | 2004 | fall   | FEMALE | 50  |
|  78 | 2004 | spring | MALE   | 40  |
|  79 | 2004 | spring | MALE   | 45  |
|  80 | 2004 | spring | MALE   | 50  |
|  81 | 2004 | spring | MALE   | 55  |
|  82 | 2004 | spring | MALE   | 60  |
|  83 | 2004 | spring | FEMALE | 25  |
|  84 | 2004 | spring | FEMALE | 30  |
|  85 | 2004 | spring | FEMALE | 35  |
|  86 | 2004 | spring | FEMALE | 40  |
|  87 | 2004 | spring | FEMALE | 45  |
|  88 | 2004 | spring | FEMALE | 50  |
|  89 | 2005 | fall   | MALE   | 40  |
|  90 | 2005 | fall   | MALE   | 45  |
|  91 | 2005 | fall   | MALE   | 50  |
|  92 | 2005 | fall   | MALE   | 55  |
|  93 | 2005 | fall   | MALE   | 60  |
|  94 | 2005 | fall   | FEMALE | 25  |
|  95 | 2005 | fall   | FEMALE | 30  |
|  96 | 2005 | fall   | FEMALE | 35  |
|  97 | 2005 | fall   | FEMALE | 40  |
|  98 | 2005 | fall   | FEMALE | 45  |
|  99 | 2005 | fall   | FEMALE | 50  |
| 100 | 2005 | spring | MALE   | 40  |
| 101 | 2005 | spring | MALE   | 45  |
| 102 | 2005 | spring | MALE   | 50  |
| 103 | 2005 | spring | MALE   | 55  |
| 104 | 2005 | spring | MALE   | 60  |
| 105 | 2005 | spring | FEMALE | 25  |
| 106 | 2005 | spring | FEMALE | 30  |
| 107 | 2005 | spring | FEMALE | 35  |
| 108 | 2005 | spring | FEMALE | 40  |
| 109 | 2005 | spring | FEMALE | 45  |
| 110 | 2005 | spring | FEMALE | 50  |

Table 1: Example dimensions map.

</div>

A dimensions map has non-standard attributes: “dmnms”, “dmlvs”, and
“dmlns”. \* “dmnms” is a character vector with the dimension names. \*
“dmlvs” is a character vector with the unique levels for each dimension.
\* “dmlns” is a named integer vector with the length (number of unique
levels) of each dimension

The first column in the model dimensions map is simply the row number
associated with the combination of unique dimension levels the row
reflects. It also provides the index into the population abundance
vector for that combination of dimensions (and thus allows the
extraction of the associated dimension levels given the index value).

Most model quantities are represented as vectors, and thus have
associated dimensions maps in order to be able to identify the dimension
levels associated with each index value.

## Aggregating over model dimensions

Data fit in gmacs typically represent quantities aggregated over some
combination of population categories, so it is necessary to determine
the mappping from model indices to data indices. The function
[createAggregatorMap](man/createAggregatorMap.Rd) provides the required
mapping of 1-d indices from one dimensions map to another, as
illustrated in the following example ([Table 2](#tbl-Ex2)) using the
dimensions map from the previous example:

``` r
#> Example of how to create an aggregator map using "dms" from above
#> to aggregate by year (y) and sex (x) over season (s) and size (z). 
#>    Create dimensions map with only y and x
  aggDims = tmbGMACS::keepDims(dms,keep=c("y","x"));#--keeps only y and x dimensions, recreates 1-d vector index
#>    Create aggregator map from y,x,s,z to y,x
  aggMap  = tmbGMACS::createAggregatorMap(dms,aggDims,keepOrigDims=TRUE);
```

*j* is the vector index for aggDims, *i* is the vector index for dms.

<div id="tbl-Ex2">

|   j |   i | y    | s      | x      | z   |
|----:|----:|:-----|:-------|:-------|:----|
|   1 |   1 | 2001 | fall   | MALE   | 40  |
|   1 |   2 | 2001 | fall   | MALE   | 45  |
|   1 |   3 | 2001 | fall   | MALE   | 50  |
|   1 |   4 | 2001 | fall   | MALE   | 55  |
|   1 |   5 | 2001 | fall   | MALE   | 60  |
|   2 |   6 | 2001 | fall   | FEMALE | 25  |
|   2 |   7 | 2001 | fall   | FEMALE | 30  |
|   2 |   8 | 2001 | fall   | FEMALE | 35  |
|   2 |   9 | 2001 | fall   | FEMALE | 40  |
|   2 |  10 | 2001 | fall   | FEMALE | 45  |
|   2 |  11 | 2001 | fall   | FEMALE | 50  |
|   1 |  12 | 2001 | spring | MALE   | 40  |
|   1 |  13 | 2001 | spring | MALE   | 45  |
|   1 |  14 | 2001 | spring | MALE   | 50  |
|   1 |  15 | 2001 | spring | MALE   | 55  |
|   1 |  16 | 2001 | spring | MALE   | 60  |
|   2 |  17 | 2001 | spring | FEMALE | 25  |
|   2 |  18 | 2001 | spring | FEMALE | 30  |
|   2 |  19 | 2001 | spring | FEMALE | 35  |
|   2 |  20 | 2001 | spring | FEMALE | 40  |
|   2 |  21 | 2001 | spring | FEMALE | 45  |
|   2 |  22 | 2001 | spring | FEMALE | 50  |
|   3 |  23 | 2002 | fall   | MALE   | 40  |
|   3 |  24 | 2002 | fall   | MALE   | 45  |
|   3 |  25 | 2002 | fall   | MALE   | 50  |
|   3 |  26 | 2002 | fall   | MALE   | 55  |
|   3 |  27 | 2002 | fall   | MALE   | 60  |
|   4 |  28 | 2002 | fall   | FEMALE | 25  |
|   4 |  29 | 2002 | fall   | FEMALE | 30  |
|   4 |  30 | 2002 | fall   | FEMALE | 35  |
|   4 |  31 | 2002 | fall   | FEMALE | 40  |
|   4 |  32 | 2002 | fall   | FEMALE | 45  |
|   4 |  33 | 2002 | fall   | FEMALE | 50  |
|   3 |  34 | 2002 | spring | MALE   | 40  |
|   3 |  35 | 2002 | spring | MALE   | 45  |
|   3 |  36 | 2002 | spring | MALE   | 50  |
|   3 |  37 | 2002 | spring | MALE   | 55  |
|   3 |  38 | 2002 | spring | MALE   | 60  |
|   4 |  39 | 2002 | spring | FEMALE | 25  |
|   4 |  40 | 2002 | spring | FEMALE | 30  |
|   4 |  41 | 2002 | spring | FEMALE | 35  |
|   4 |  42 | 2002 | spring | FEMALE | 40  |
|   4 |  43 | 2002 | spring | FEMALE | 45  |
|   4 |  44 | 2002 | spring | FEMALE | 50  |
|   5 |  45 | 2003 | fall   | MALE   | 40  |
|   5 |  46 | 2003 | fall   | MALE   | 45  |
|   5 |  47 | 2003 | fall   | MALE   | 50  |
|   5 |  48 | 2003 | fall   | MALE   | 55  |
|   5 |  49 | 2003 | fall   | MALE   | 60  |
|   6 |  50 | 2003 | fall   | FEMALE | 25  |
|   6 |  51 | 2003 | fall   | FEMALE | 30  |
|   6 |  52 | 2003 | fall   | FEMALE | 35  |
|   6 |  53 | 2003 | fall   | FEMALE | 40  |
|   6 |  54 | 2003 | fall   | FEMALE | 45  |
|   6 |  55 | 2003 | fall   | FEMALE | 50  |
|   5 |  56 | 2003 | spring | MALE   | 40  |
|   5 |  57 | 2003 | spring | MALE   | 45  |
|   5 |  58 | 2003 | spring | MALE   | 50  |
|   5 |  59 | 2003 | spring | MALE   | 55  |
|   5 |  60 | 2003 | spring | MALE   | 60  |
|   6 |  61 | 2003 | spring | FEMALE | 25  |
|   6 |  62 | 2003 | spring | FEMALE | 30  |
|   6 |  63 | 2003 | spring | FEMALE | 35  |
|   6 |  64 | 2003 | spring | FEMALE | 40  |
|   6 |  65 | 2003 | spring | FEMALE | 45  |
|   6 |  66 | 2003 | spring | FEMALE | 50  |
|   7 |  67 | 2004 | fall   | MALE   | 40  |
|   7 |  68 | 2004 | fall   | MALE   | 45  |
|   7 |  69 | 2004 | fall   | MALE   | 50  |
|   7 |  70 | 2004 | fall   | MALE   | 55  |
|   7 |  71 | 2004 | fall   | MALE   | 60  |
|   8 |  72 | 2004 | fall   | FEMALE | 25  |
|   8 |  73 | 2004 | fall   | FEMALE | 30  |
|   8 |  74 | 2004 | fall   | FEMALE | 35  |
|   8 |  75 | 2004 | fall   | FEMALE | 40  |
|   8 |  76 | 2004 | fall   | FEMALE | 45  |
|   8 |  77 | 2004 | fall   | FEMALE | 50  |
|   7 |  78 | 2004 | spring | MALE   | 40  |
|   7 |  79 | 2004 | spring | MALE   | 45  |
|   7 |  80 | 2004 | spring | MALE   | 50  |
|   7 |  81 | 2004 | spring | MALE   | 55  |
|   7 |  82 | 2004 | spring | MALE   | 60  |
|   8 |  83 | 2004 | spring | FEMALE | 25  |
|   8 |  84 | 2004 | spring | FEMALE | 30  |
|   8 |  85 | 2004 | spring | FEMALE | 35  |
|   8 |  86 | 2004 | spring | FEMALE | 40  |
|   8 |  87 | 2004 | spring | FEMALE | 45  |
|   8 |  88 | 2004 | spring | FEMALE | 50  |
|   9 |  89 | 2005 | fall   | MALE   | 40  |
|   9 |  90 | 2005 | fall   | MALE   | 45  |
|   9 |  91 | 2005 | fall   | MALE   | 50  |
|   9 |  92 | 2005 | fall   | MALE   | 55  |
|   9 |  93 | 2005 | fall   | MALE   | 60  |
|  10 |  94 | 2005 | fall   | FEMALE | 25  |
|  10 |  95 | 2005 | fall   | FEMALE | 30  |
|  10 |  96 | 2005 | fall   | FEMALE | 35  |
|  10 |  97 | 2005 | fall   | FEMALE | 40  |
|  10 |  98 | 2005 | fall   | FEMALE | 45  |
|  10 |  99 | 2005 | fall   | FEMALE | 50  |
|   9 | 100 | 2005 | spring | MALE   | 40  |
|   9 | 101 | 2005 | spring | MALE   | 45  |
|   9 | 102 | 2005 | spring | MALE   | 50  |
|   9 | 103 | 2005 | spring | MALE   | 55  |
|   9 | 104 | 2005 | spring | MALE   | 60  |
|  10 | 105 | 2005 | spring | FEMALE | 25  |
|  10 | 106 | 2005 | spring | FEMALE | 30  |
|  10 | 107 | 2005 | spring | FEMALE | 35  |
|  10 | 108 | 2005 | spring | FEMALE | 40  |
|  10 | 109 | 2005 | spring | FEMALE | 45  |
|  10 | 110 | 2005 | spring | FEMALE | 50  |

Table 2: Example aggregator map (from indices i to indices j).

</div>
