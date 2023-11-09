
<!-- use `devtools::build_readme()` to build README.md prior to keep README.md up-to-date-->
<!-- don't forget to commit and push the resulting figure files, so they display on GitHub and CRAN.-->
<!-- README.md is generated from README.Rmd. Please edit this file, thhen generate that file. -->

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

$$\frac{d\mathbf{n_t}}{dt} = \left[ \mathbf{T_t}-(\mathbf{M_t}+\mathbf{F_t}) \right]*\mathbf{n_t}$$

where $\mathbf{T_t}$ is a $c$ x $c$ matrix representing transition rates
between population categories (e.g., growth or movement) at time $t$,
$\mathbf{M_t}$ is a similarly-sized diagonal matrix representing natural
mortality at time t, and $\mathbf{M_t}$ is a similarly-sized diagonal
matrix representing fishing mortality at time $t$. For gmacs, rates
within a (seasonal) timestep are assumed to be constant. Thus, @eqn-1
could be integrated to obtain

$$\mathbf{n_{t+1}} = e^{\mathbf{A_t} \delta t}*\mathbf{n_t}$$

where $\delta t$ is the time step from $t$ to $t+1$, and
$\mathbf{A_t}=\left[ \mathbf{T_t}-(\mathbf{M_t}+\mathbf{F_t}) \right]$.
In practice, when $\mathbf{T_t}$ represents growth processes, these are
regarded as discrete-time processes with a season length of 0, so that

$$ \mathbf{n_{t+1}} = \mathbf{T_t}*\mathbf{n_t} $$

for seasons in which growth occurs and

$$ \mathbf{n_{t+1}} = e^{-\left[ \mathbf{M_t}+\mathbf{F_t} \right]*\delta t}*\mathbf{n_t}$$

for seasons with non-0 lengths in which mortality occurred.

Recruitment, the addition of new individuals to the model population
categories, is also considered a discrete-time process associated with a
season length of 0, so

$$ \mathbf{n_{t+1}} = \mathbf{R_t}+\mathbf{n_t}$$ when recruitment
occurs, where $\mathbf{R_t}$ is a vector of length $c$.

Let $<\mathbf{R_s}>$ represent the equilibrium (i.e., longterm mean)
recruitment in (within-year) season $s$, and similarly for
$<\mathbf{T_s}>$ and $\mathbf{Z_s}=\mathbf{M_s}+\mathbf{F_s}$. The
equilibrium population size $<\mathbf{n_s}>$ in season $s$ is then
determined by the solution to

\$\$\<\> =

## Model dimensions

Model dimensions consist of year (**y**), season (**s**), and a set of
categories ({**c**}) into which the population is divided (e.g., sex and
size). Levels within a given population category can be nested within
another category (e.g., different size classes might be defined for
males and females if they exhibited substantially different growth
patterns). For a given year, population abundance is integrated across
seasons within the year and different processes can occur in different
seasons.

Dimensions are defined using the function . As an example, the following
creates a set of dimensions encompassing year (**y**), season(**s**),
sex (**x**), and size (**z**), with different size classes for males and
females. Note that the “dimension name(s)” are added to the vector or
list defining the levels as an attribute “dmnms”.

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

<table>
<thead>
<tr>
<th style="text-align:right;">
i
</th>
<th style="text-align:left;">
y
</th>
<th style="text-align:left;">
s
</th>
<th style="text-align:left;">
x
</th>
<th style="text-align:left;">
z
</th>
</tr>
</thead>
<tbody>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
11
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
12
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
13
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
14
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
15
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
16
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
17
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
18
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
19
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
20
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
21
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
22
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
23
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
24
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
25
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
26
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
27
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
28
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
29
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
30
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
31
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
32
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
33
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
34
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
35
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
36
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
37
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
38
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
39
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
40
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
41
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
42
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
43
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
44
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
45
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
46
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
47
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
48
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
49
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
50
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
51
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
52
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
53
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
54
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
55
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
56
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
57
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
58
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
59
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
60
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
61
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
62
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
63
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
64
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
65
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
66
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
67
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
68
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
69
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
70
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
71
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
72
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
73
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
74
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
75
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
76
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
77
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
78
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
79
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
80
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
81
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
82
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
83
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
84
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
85
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
86
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
87
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
88
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
89
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
90
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
91
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
92
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
93
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
94
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
95
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
96
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
97
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
98
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
99
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
100
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
101
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
102
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
103
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
104
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
105
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
106
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
107
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
108
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
109
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
110
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
</tbody>
</table>

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
illustrwated in the following example using the dimensions map from the
previous example:

``` r
#> Example of how to create an aggregator map using "dms" from above
#> to aggregate by year (y) and sex (x) over season (s) and size (z). 
#>    Create dimensions map with only y and x
  aggDims = tmbGMACS::keepDims(dms,keep=c("y","x"));#--keeps only y and x dimensions, recreates 1-d vector index
#>    Create aggregator map from y,x,s,z to y,x
  aggMap  = tmbGMACS::createAggregatorMap(dms,aggDims,keepOrigDims=TRUE);
```

*j* is the vector index for aggDims, *i* is the vector index for dms.
<table>
<thead>
<tr>
<th style="text-align:right;">
j
</th>
<th style="text-align:right;">
i
</th>
<th style="text-align:left;">
y
</th>
<th style="text-align:left;">
s
</th>
<th style="text-align:left;">
x
</th>
<th style="text-align:left;">
z
</th>
</tr>
</thead>
<tbody>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
1
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
2
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
3
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
4
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
5
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
6
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
7
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
8
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
9
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
10
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
11
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
12
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
13
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
14
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
15
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
1
</td>
<td style="text-align:right;">
16
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
17
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
18
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
19
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
20
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
21
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
2
</td>
<td style="text-align:right;">
22
</td>
<td style="text-align:left;">
2001
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
23
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
24
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
25
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
26
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
27
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
28
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
29
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
30
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
31
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
32
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
33
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
34
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
35
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
36
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
37
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
3
</td>
<td style="text-align:right;">
38
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
39
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
40
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
41
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
42
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
43
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
4
</td>
<td style="text-align:right;">
44
</td>
<td style="text-align:left;">
2002
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
45
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
46
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
47
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
48
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
49
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
50
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
51
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
52
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
53
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
54
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
55
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
56
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
57
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
58
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
59
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
5
</td>
<td style="text-align:right;">
60
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
61
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
62
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
63
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
64
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
65
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
6
</td>
<td style="text-align:right;">
66
</td>
<td style="text-align:left;">
2003
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
67
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
68
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
69
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
70
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
71
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
72
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
73
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
74
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
75
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
76
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
77
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
78
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
79
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
80
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
81
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
7
</td>
<td style="text-align:right;">
82
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
83
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
84
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
85
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
86
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
87
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
8
</td>
<td style="text-align:right;">
88
</td>
<td style="text-align:left;">
2004
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
89
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
90
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
91
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
92
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
93
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
94
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
95
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
96
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
97
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
98
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
99
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
fall
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
100
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
101
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
102
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
103
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
55
</td>
</tr>
<tr>
<td style="text-align:right;">
9
</td>
<td style="text-align:right;">
104
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
MALE
</td>
<td style="text-align:left;">
60
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
105
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
25
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
106
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
30
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
107
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
35
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
108
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
40
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
109
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
45
</td>
</tr>
<tr>
<td style="text-align:right;">
10
</td>
<td style="text-align:right;">
110
</td>
<td style="text-align:left;">
2005
</td>
<td style="text-align:left;">
spring
</td>
<td style="text-align:left;">
FEMALE
</td>
<td style="text-align:left;">
50
</td>
</tr>
</tbody>
</table>
