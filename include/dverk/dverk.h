#ifndef dverk_dverk_h
#define dverk_dverk_h

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN typedef int (*dverk_f)(int *neq, double *t, double *y, double *ydot,
                              void *dat);

/**
 * @brief
 *
 * @param n number of equations
 * @param fcn name of subroutine for evaluating functions - the  subroutine
                itself must also be provided by the user - it should be of
                the following form
                    subroutine fcn(n, x, y, yprime)
                    integer n
                    double precision x, y(n), yprime(n)
 * @param x independent variable - initial value supplied by user
 * @param y dependent variable - initial values of components y(1), y(2),
                ..., y(n) supplied by user
 * @param xend value of x to which integration is to be carried out - it may
                be less than the initial value of x
 * @param tol tolerance - the subroutine attempts to control a norm of  the
                local  error  in  such  a  way  that  the  global error is
                proportional to tol. in some problems there will be enough
                damping  of  errors, as well as some cancellation, so that
                the global error will be less than tol. alternatively, the
                control   can   be  viewed  as  attempting  to  provide  a
                calculated value of y at xend which is the exact  solution
                to  the  problem y' = f(x,y) + e(x) where the norm of e(x)
                is proportional to tol.  (the norm  is  a  max  norm  with
                weights  that  depend on the error control strategy chosen
                by the user.  the default weight for the k-th component is
                1/max(1,abs(y(k))),  which therefore provides a mixture of
                absolute and relative error control.)
 * @param ind indicator - on initial entry ind must be set equal to  either
                1  or  2. if the user does not wish to use any options, he
                should set ind to 1 - all that remains for the user to  do
                then  is  to  declare c and w, and to specify nw. the user
                may also  select  various  options  on  initial  entry  by
                setting ind = 2 and initializing the first 9 components of
                c as described in the next section.  he may also  re-enter
                the  subroutine  with ind = 3 as mentioned again below. in
                any event, the subroutine returns with ind equal to
                3 after a normal return
                4, 5, or 6 after an interrupt (see options c(8), c(9))
                -1, -2, or -3 after an error condition (see below)
 * @param c communications vector - the dimension must be greater than or
            equal to 24, unless option c(1) = 4 or 5 is used, in which
            case the dimension must be greater than or equal to n+30
 * @param nw first dimension of workspace w -  must  be  greater  than  or
                equal to n
 * @param w workspace matrix - first dimension must be nw and second must
                be greater than or equal to 9
 * @return EXTERN

    subroutine dverk (n, fcn, x, y, xend, tol, ind, c, nw, w)
    integer n, ind, nw, k
    double precision x, y(n), xend, tol, c(1), w(nw,9), temp
***********************************************************************
                                                           *
     purpose - this is a runge-kutta  subroutine  based  on  verner's *
 fifth and sixth order pair of formulas for finding approximations to *
 the solution of  a  system  of  first  order  ordinary  differential *
 equations  with  initial  conditions. it attempts to keep the global *
 error proportional to  a  tolerance  specified  by  the  user.  (the *
 proportionality  depends  on the kind of error control that is used, *
 as well as the differential equation and the range of integration.)  *
                                                           *
     various options are available to the user,  including  different *
 kinds  of  error control, restrictions on step sizes, and interrupts *
 which permit the user to examine the state of the  calculation  (and *
 perhaps make modifications) during intermediate stages.              *
                                                           *
     the program is efficient for non-stiff systems.  however, a good *
 variable-order-adams  method  will probably be more efficient if the *
 function evaluations are very costly.  such a method would  also  be *
 more suitable if one wanted to obtain a large number of intermediate *
 solution values by interpolation, as might be the case  for  example *
 with graphical output.                                               *
                                                           *
                         hull-enright-jackson   1/10/76    *
                                                           *
***********************************************************************
     the subroutine  will  normally  return  with  ind  =  3,  having *
 replaced the initial values of x and y with, respectively, the value *
 of xend and an approximation to y at xend.  the  subroutine  can  be *
 called  repeatedly  with new values of xend without having to change *
 any other argument.  however, changes in tol, or any of the  options *
 described below, may also be made on such a re-entry if desired.     *
                                                                      *
     three error returns are also possible, in which  case  x  and  y *
 will be the most recently accepted values -                          *
     with ind = -3 the subroutine was unable  to  satisfy  the  error *
        requirement  with a particular step-size that is less than or *
        equal to hmin, which may mean that tol is too small           *
     with ind = -2 the value of hmin  is  greater  than  hmax,  which *
        probably  means  that the requested tol (which is used in the *
        calculation of hmin) is too small                             *
     with ind = -1 the allowed maximum number of fcn evaluations  has *
        been  exceeded,  but  this  can only occur if option c(7), as *
        described in the next section, has been used                  *
                                                                      *
     there are several circumstances that will cause the calculations *
 to  be  terminated,  along with output of information that will help *
 the user determine the cause of  the  trouble.  these  circumstances *
 involve  entry with illegal or inconsistent values of the arguments, *
 such as attempting a normal  re-entry  without  first  changing  the *
 value of xend, or attempting to re-enter with ind less than zero.    *
                                                                      *
***********************************************************************
                                                                      *
     options - if the subroutine is entered with ind = 1, the first 9 *
 components of the communications vector are initialized to zero, and *
 the subroutine uses only default values  for  each  option.  if  the *
 subroutine  is  entered  with ind = 2, the user must specify each of *
 these 9 components - normally he would first set them all  to  zero, *
 and  then  make  non-zero  those  that  correspond to the particular *
 options he wishes to select. in any event, options may be changed on *
 re-entry  to  the  subroutine  -  but if the user changes any of the *
 options, or tol, in the course of a calculation he should be careful *
 about  how  such changes affect the subroutine - it may be better to *
 restart with ind = 1 or 2. (components 10 to 24 of c are used by the *
 program  -  the information is available to the user, but should not *
 normally be changed by him.)                                         *
                                                                      *
  c(1)  error control indicator - the norm of the local error is  the *
           max  norm  of  the  weighted  error  estimate  vector, the *
           weights being determined according to the value of c(1) -  *
              if c(1)=1 the weights are 1 (absolute error control)    *
              if c(1)=2 the weights are 1/abs(y(k))  (relative  error *
                 control)                                             *
              if c(1)=3 the  weights  are  1/max(abs(c(2)),abs(y(k))) *
                 (relative  error  control,  unless abs(y(k)) is less *
                 than the floor value, abs(c(2)) )                    *
              if c(1)=4 the weights are 1/max(abs(c(k+30)),abs(y(k))) *
                 (here individual floor values are used)              *
              if c(1)=5 the weights are 1/abs(c(k+30))                *
              for all other values of c(1), including  c(1) = 0,  the *
                 default  values  of  the  weights  are  taken  to be *
                 1/max(1,abs(y(k))), as mentioned earlier             *
           (in the two cases c(1) = 4 or 5 the user must declare  the *
           dimension of c to be at least n+30 and must initialize the *
           components c(31), c(32), ..., c(n+30).)                    *
                                                                      *
  c(2)  floor value - used when the indicator c(1) has the value 3    *
                                                                      *
  c(3)  hmin specification - if not zero, the subroutine chooses hmin *
           to be abs(c(3)) - otherwise it uses the default value      *
              10*max(dwarf,rreb*max(weighted norm y/tol,abs(x))),     *
           where dwarf is a very small positive  machine  number  and *
           rreb is the relative roundoff error bound                  *
                                                                      *
  c(4)  hstart specification - if not zero, the subroutine  will  use *
           an  initial  hmag equal to abs(c(4)), except of course for *
           the restrictions imposed by hmin and hmax  -  otherwise it *
           uses the default value of hmax*(tol)**(1/6)                *
                                                                      *
  c(5)  scale specification - this is intended to be a measure of the *
           scale of the problem - larger values of scale tend to make *
           the method more reliable, first  by  possibly  restricting *
           hmax  (as  described  below) and second, by tightening the *
           acceptance requirement - if c(5) is zero, a default  value *
           of  1  is  used.  for  linear  homogeneous  problems  with *
           constant coefficients, an appropriate value for scale is a *
           norm  of  the  associated  matrix.  for other problems, an *
           approximation to  an  average  value  of  a  norm  of  the *
           jacobian along the trajectory may be appropriate           *
                                                                      *
  c(6)  hmax specification - four cases are possible                  *
           if c(6).ne.0 and c(5).ne.0, hmax is taken to be            *
              min(abs(c(6)),2/abs(c(5)))                              *
           if c(6).ne.0 and c(5).eq.0, hmax is taken to be  abs(c(6)) *
           if c(6).eq.0 and c(5).ne.0, hmax is taken to be            *
              2/abs(c(5))                                             *
           if c(6).eq.0 and c(5).eq.0, hmax is given a default  value *
              of 2                                                    *
                                                                      *
  c(7)  maximum number of function evaluations  -  if  not  zero,  an *
           error  return with ind = -1 will be caused when the number *
           of function evaluations exceeds abs(c(7))                  *
                                                                      *
  c(8)  interrupt number  1  -  if  not  zero,  the  subroutine  will *
           interrupt   the  calculations  after  it  has  chosen  its *
           preliminary value of hmag, and just before choosing htrial *
           and  xtrial  in  preparation for taking a step (htrial may *
           differ from hmag in sign, and may  require  adjustment  if *
           xend  is  near) - the subroutine returns with ind = 4, and *
           will resume calculation at the point  of  interruption  if *
           re-entered with ind = 4                                    *
                                                                      *
  c(9)  interrupt number  2  -  if  not  zero,  the  subroutine  will *
           interrupt   the  calculations  immediately  after  it  has *
           decided whether or not to accept the result  of  the  most *
           recent  trial step, with ind = 5 if it plans to accept, or *
           ind = 6 if it plans to reject -  y(*)  is  the  previously *
           accepted  result, while w(*,9) is the newly computed trial *
           value, and w(*,2) is the unweighted error estimate vector. *
           the  subroutine  will  resume calculations at the point of *
           interruption on re-entry with ind = 5 or 6. (the user  may *
           change ind in this case if he wishes, for example to force *
           acceptance of a step that would otherwise be rejected,  or *
           vice versa. he can also restart with ind = 1 or 2.)        *
                                                                      *
***********************************************************************
                                                                      *
  summary of the components of the communications vector              *
                                                                      *
     prescribed at the option       determined by the program         *
           of the user                                                *
                                                                      *
                                    c(10) rreb(rel roundoff err bnd)  *
     c(1) error control indicator   c(11) dwarf (very small mach no)  *
     c(2) floor value               c(12) weighted norm y             *
     c(3) hmin specification        c(13) hmin                        *
     c(4) hstart specification      c(14) hmag                        *
     c(5) scale specification       c(15) scale                       *
     c(6) hmax specification        c(16) hmax                        *
     c(7) max no of fcn evals       c(17) xtrial                      *
     c(8) interrupt no 1            c(18) htrial                      *
     c(9) interrupt no 2            c(19) est                         *
                                    c(20) previous xend               *
                                    c(21) flag for xend               *
                                    c(22) no of successful steps      *
                                    c(23) no of successive failures   *
                                    c(24) no of fcn evals             *
                                                                      *
  if c(1) = 4 or 5, c(31), c(32), ... c(n+30) are floor values        *
                                                                      *
***********************************************************************
 */
EXTERN void dverk(int *n, dverk_f fcn, double *x, double *y, double *xend,
                  double *tol, int *ind, double *c, int *nw, double *w,
                  void *dat);

#endif /* dverk_dverk_h */