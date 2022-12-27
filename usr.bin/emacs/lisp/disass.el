;;; Disassembler for compiled Emacs Lisp code
;; Copyright (C) 1986 Free Software Foundation
;;; By Doug Cutting (doug@csli.stanford.edu)

;; This file is part of GNU Emacs.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY.  No author or distributor
;; accepts responsibility to anyone for the consequences of using it
;; or for whether it serves any particular purpose or works at all,
;; unless he says so in writing.  Refer to the GNU Emacs General Public
;; License for full details.

;; Everyone is granted permission to copy, modify and redistribute
;; GNU Emacs, but only under the conditions described in the
;; GNU Emacs General Public License.   A copy of this license is
;; supposed to have been given to you along with GNU Emacs so you
;; can know your rights and responsibilities.  It should be in a
;; file named COPYING.  Among other things, the copyright notice
;; and this notice must be preserved on all copies.


(require 'byte-compile "bytecomp")

(defvar disassemble-column-1-indent 4 "*")

(defvar disassemble-column-2-indent 9 "*")

(defvar disassemble-recursive-indent 3 "*")

;(defun d (x)
;  (interactive "xDiss ")
;  (with-output-to-temp-buffer "*Disassemble*"
;    (disassemble-internal (list 'lambda '() x ''return-value)
;			  standard-output 0 t)))

(defun disassemble (object &optional stream indent interactive-p)
  "Print disassembled code for OBJECT on (optional) STREAM.
OBJECT can be a function name, lambda expression or any function object
returned by SYMBOL-FUNCTION.  If OBJECT is not already compiled, we will
compile it (but not redefine it)."
  (interactive (list (intern (completing-read "Disassemble function: "
					      obarray 'fboundp t))
		     nil 0 t))
  (or indent (setq indent 0))		;Default indent to zero
  (if interactive-p
      (with-output-to-temp-buffer "*Disassemble*"
	(disassemble-internal object standard-output indent t))
    (disassemble-internal object (or stream standard-output) indent nil))
  nil)

(defun disassemble-internal (obj stream indent interactive-p)
  (let ((macro 'nil)
	(name 'nil)
	(doc 'nil)
	args)
    (while (symbolp obj)
      (setq name obj
	    obj (symbol-function obj)))
    (if (subrp obj)
	(error "Can't disassemble #<subr %s>" name))
    (if (eq (car obj) 'macro)		;handle macros
	(setq macro t
	      obj (cdr obj)))
    (if (not (eq (car obj) 'lambda))
	(error "not a function"))
    (if (assq 'byte-code obj)
	nil
      (if interactive-p (message (if name
				     "Compiling %s's definition..."
				     "Compiling definition...")
				 name))
      (setq obj (byte-compile-lambda obj))
      (if interactive-p (message "Done compiling.  Disassembling...")))
    (setq obj (cdr obj))		;throw lambda away
    (setq args (car obj))		;save arg list
    (setq obj (cdr obj))
    (write-spaces indent stream)
    (princ (format "byte code%s%s%s:\n"
		   (if (or macro name) " for" "")
		   (if macro " macro" "")
		   (if name (format " %s" name) ""))
	   stream)
    (let ((doc (and (stringp (car obj)) (car obj))))
      (if doc
	  (progn (setq obj (cdr obj))
		 (write-spaces indent stream)
		 (princ " doc: " stream)
		 (princ doc stream)
		 (terpri stream))))
    (write-spaces indent stream)
    (princ " args: " stream)
    (prin1 args stream)
    (terpri stream)
    (let ((interactive (car (cdr (assq 'interactive obj)))))
      (if interactive
	  (progn (write-spaces indent stream)
		 (princ " interactive: " stream)
		 (if (eq (car-safe interactive) 'byte-code)
		     (disassemble-1 interactive stream
		       (+ indent disassemble-recursive-indent))
		   (prin1 interactive stream)
		   (terpri stream)))))
    (setq obj (assq 'byte-code obj))	;obj is now call to byte-code
    (disassemble-1 obj stream indent))
  (if interactive-p
      (message "")))

(defun disassemble-1 (obj &optional stream indent)
  "Prints the byte-code call OBJ to (optional) STREAM.
OBJ should be a call to BYTE-CODE generated by the byte compiler."
  (or indent (setq indent 0))		;default indent to 0
  (or stream (setq stream standard-output))
  (let ((bytes (car (cdr obj)))		;the byte code
	(ptr -1)			;where we are in it
	(constants (car (cdr (cdr obj)))) ;constant vector
	;(next-indent indent)
	offset tmp length)
    (setq length (length bytes))
    (terpri stream)
    (while (< (setq ptr (1+ ptr)) length)
      ;(setq indent next-indent)
      (write-spaces indent stream)	;indent to recursive indent
      (princ (setq tmp (prin1-to-string ptr)) stream) ;print line #
      (write-char ?\  stream)
      (write-spaces (- disassemble-column-1-indent (length tmp) 1)
		    stream)
      (setq op (aref bytes ptr))	;fetch opcode
      ;; Note: as offsets are either encoded in opcodes or stored as
      ;; bytes in the code, this function (disassemble-offset)
      ;; can set OP and/or PTR.
      (setq offset (disassemble-offset));fetch offset
      (setq tmp (aref byte-code-vector op))
      (if (consp tmp)
	  (setq ;next-indent (if (numberp (cdr tmp))
		;		(+ indent (cdr tmp))
		;	      (+ indent (funcall (cdr tmp) offset)))
		tmp (car tmp)))
      (setq tmp (symbol-name tmp))
      (princ tmp stream)		;print op-name for opcode
      (if (null offset)
	  nil
	(write-char ?\  stream)
	(write-spaces (- disassemble-column-2-indent (length tmp) 1)
		      stream)		;indent to col 2
	(princ				;print offset
	 (cond ((or (eq op byte-varref)
		    (eq op byte-varset)
		    (eq op byte-varbind))
		;; it's a varname (atom)
		(aref constants offset)) ;fetch it from constants
	       ((or (eq op byte-goto)
		    (eq op byte-goto-if-nil)
		    (eq op byte-goto-if-not-nil)
		    (eq op byte-goto-if-nil-else-pop)
		    (eq op byte-goto-if-not-nil-else-pop)
		    (eq op byte-call)
		    (eq op byte-unbind))
		;; it's a number
		offset)			;return it
	       ((or (eq op byte-constant)
		    (eq op byte-constant2))
		;; it's a constant
		(setq tmp (aref constants offset))
		;; but is constant byte code?
		(cond ((and (eq (car-safe tmp) 'lambda)
			    (assq 'byte-code tmp))
		       (princ "<compiled lambda>" stream)
		       (terpri stream)
		       (disassemble	;recurse on compiled lambda
			 tmp
			 stream
			 (+ indent disassemble-recursive-indent))
		       "")
		      ((eq (car-safe tmp) 'byte-code)
		       (princ "<byte code>" stream)
		       (terpri stream)
		       (disassemble-1	;recurse on byte-code object
			 tmp
			 stream
			 (+ indent disassemble-recursive-indent))
		       "")
		      ((eq (car-safe (car-safe tmp)) 'byte-code)
		       (princ "(<byte code>...)" stream)
		       (terpri stream)
		       (mapcar		;recurse on list of byte-code objects
			 (function (lambda (obj)
				     (disassemble-1
				       obj
				       stream
				       (+ indent disassemble-recursive-indent))))
			 tmp)
		       "")
		      ((and (eq tmp 'byte-code) 
			    (eq (aref bytes (+ ptr 4)) (+ byte-call 3)))
		       ;; this won't catch cases where args are pushed w/
		       ;; constant2.
		       (setq ptr (+ ptr 4))
		       "<compiled call to byte-code.  compiled code compiled?>")
		      (t
		       ;; really just a constant
		       (let ((print-escape-newlines t))
			 (prin1-to-string tmp)))))
	       (t "<error in disassembler>"))
	 stream))
      (terpri stream)))
  nil)


(defun disassemble-offset ()
  "Don't call this!"
  ;; fetch and return the offset for the current opcode.
  ;; return NIL if this opcode has no offset
  ;; OP, PTR and BYTES are used and set dynamically
  (let (tem)
    (cond ((< op byte-nth)
	   (setq tem (logand op 7))
	   (setq op (logand op 248))
	   (cond ((eq tem 6)
		  (setq ptr (1+ ptr))	;offset in next byte
		  (aref bytes ptr))
		 ((eq tem 7)
		  (setq ptr (1+ ptr))	;offset in next 2 bytes
		  (+ (aref bytes ptr)
		     (progn (setq ptr (1+ ptr))
			    (lsh (aref bytes ptr) 8))))
		 (t tem)))	;offset was in opcode
	  ((>= op byte-constant)
	   (setq tem (- op byte-constant)) ;offset in opcode
	   (setq op byte-constant)
	   tem)
	  ((or (= op byte-constant2)
	       (and (>= op byte-goto)
		    (<= op byte-goto-if-not-nil-else-pop)))
	   (setq ptr (1+ ptr))		;offset in next 2 bytes
	   (+ (aref bytes ptr)
	      (progn (setq ptr (1+ ptr))
		     (lsh (aref bytes ptr) 8))))
	  (t nil))))			;no offset


(defun write-spaces (n &optional stream)
  "Print N spaces to (optional) STREAM."
  (or stream (setq stream standard-output))
  (if (< n 0) (setq n 0))
  (if (eq stream (current-buffer))
      (insert-char ?\  n)
    (while (> n 0)
      (write-char ?\  stream)
      (setq n (1- n)))))

(defconst byte-code-vector
 '[<not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (varref . 1)
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (varset . -1)
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (varbind . 0);Pops a value, "pushes" a binding
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (call . -); #'-, not -1!
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (unbind . -);"pops" bindings
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (nth . -1)
   symbolp
   consp
   stringp
   listp
   (eq . -1)
   (memq . -1)
   not
   car
   cdr
   (cons . -1)
   list1
   (list2 . -1)
   (list3 . -2)
   (list4 . -3)
   length
   (aref . -1)
   (aset . -2)
   symbol-value
   symbol-function
   (set . -1)
   (fset . -1)
   (get . -1)
   (substring . -2)
   (concat2 . -1)
   (concat3 . -2)
   (concat4 . -3)
   sub1
   add1
   (eqlsign . -1) ;=
   (gtr . -1)     ;>
   (lss . -1)     ;<
   (leq . -1)     ;<=
   (geq . -1)     ;>=
   (diff . -1)    ;-
   negate         ;unary -
   (plus . -1)    ;+
   (max . -1)
   (min . -1)
   <not-an-opcode>
   (point . 1)
   (mark\(obsolete\) . 1)
   goto-char 
   insert
   (point-max . 1)
   (point-min . 1)
   char-after
   (following-char . 1)
   (preceding-char . 1)
   (current-column . 1)
   (indent-to . 1)
   (scan-buffer\(obsolete\) . -2)
   (eolp . 1)
   (eobp . 1)
   (bolp . 1)
   (bobp . 1)
   (current-buffer . 1)
   set-buffer
   (read-char . 1)
   set-mark\(obsolete\)
   interactive-p
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (constant2 . 1)
   goto;>>>
   goto-if-nil;>>
   goto-if-not-nil;>>
   (goto-if-nil-else-pop . -1)
   (goto-if-not-nil-else-pop . -1)
   return
   (discard . -1)
   (dup . 1)
   (save-excursion . 1);Pushes a binding
   (save-window-excursion . 1);Pushes a binding
   (save-restriction . 1);Pushes a binding
   (catch . -1);Takes one argument, returns a value
   (unwind-protect . 1);Takes one argument, pushes a binding, returns a value
   (condition-case . -2);Takes three arguments, returns a value
   (temp-output-buffer-setup . -1)
   temp-output-buffer-show
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   <not-an-opcode>
   (constant . 1)
   ])

