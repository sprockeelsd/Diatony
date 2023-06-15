(cl:defpackage "gilf"
  (:nicknames "GILF")
   (:use common-lisp :cl-user :cl :cffi))

(in-package :gilf)

(print "Loading gecode-wrapper...")

(defparameter DFS 0)
(defparameter BAB 1)
; corresponds to enum values in gecode_problem.h, but can be used graphically in om
(defun bab ()
    BAB
)
(defun dfs ()
    DFS
)

;;;;;;;;;;;;;;;;;;;;;
;; Problem methods ;;
;;;;;;;;;;;;;;;;;;;;;

(cffi::defcfun ("create_new_problem" new-problem) :pointer
    "Creates a new instance of the problem. Returns a void* cast of a Problem*."
    (size :int) ; an integer representing the size
    (lower-bound-domain :int) ; an integer representing the lower bound of the domain
    (upper-bound-domain :int) ; an integer representing the upper bound of the domain
    ; TODO add here any additional arguments that your Problem constructor takes
)

(cffi::defcfun ("get_size" get-size) :int
    "Returns the size of the space."
    (sp :pointer) ; a void* cast of a Problem*
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Search engine methods ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

(cffi::defcfun ("create_solver" create-solver) :pointer
    "Creates a DFS<Problem> object. Returns a void* cast of a DFS<Problem> object."
    (sp :pointer) ; a void* cast of a Problem*
    (solver-type :int); an integer representing the type of the solver (see above)
)

(cffi::defcfun ("return_next_solution_space" return-next-solution-space) :pointer
    "Returns a pointer to the next solution of the problem. Returns a void* cast of a Problem*."
    (solver :pointer) ; a void* cast of a Base<Problem>* pointer
)

;;;;;;;;;;;;;;;;;;;;;;;
;; Solution handling ;;
;;;;;;;;;;;;;;;;;;;;;;;

(cffi::defcfun ("return_solution" return-solution) :pointer
    "Returns a int* that are the values of a solution."
        (sp :pointer) ; a void* cast of a Problem object that is a solution of the problem.
)

(defun solution-to-int-array (sp)
    "Returns the values the variables have taken in the solution as a list of integers. Casts a int* into a list of numbers."
    "sp is a void* cast of a Problem* that is a solution to the problem. Calling this funciton on a non-solution 
        will result in an error."
        (if (cffi::null-pointer-p sp) ; TODO check
            (error "No (more) solutions.")
        )
    (let* (
            (size (get-size sp))
            (ptr (return-solution sp))
        )
        (loop for i from 0 below size
            collect (cffi::mem-aref ptr :int i)
        )
    )
)