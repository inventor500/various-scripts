#!/usr/bin/sbcl --script

(load "~/quicklisp/setup.lisp")

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload '(:dexador :cl-json) :silent t))

(defpackage :zip-from-city
  (:use cl))

(in-package :zip-from-city)

(defun send-request (city state)
  "Send a request with city and state to the USPS website"
  (dex:post "https://tools.usps.com/tools/app/ziplookup/zipByCityState"
            :content (list (cons "city" city)
                           (cons "state" state))
            :headers '(("DNT" . "1")
                       ("Origin" . "https://tools.usps.com")
                       ("Referer" . "https://tools.usps.com/zip-code-lookup.htm?bycitystate")
                       ("User-Agent" . "Mozilla/5.0 (Windows NT 10.0; rv:91.0) Gecko/20100101 Firefox/91.0")
                       ("X-Requested-With" . "XMLHttpRequest"))))

(defun main-search ()
  (if (= 2 (length uiop:*command-line-arguments*))
      (let* ((response (send-request (car uiop:*command-line-arguments*) (nth 1 uiop:*command-line-arguments*))) (response-parsed (cl-json:decode-json-from-string response)))
        (loop for entry in (cdr (nth 1 (cddr response-parsed)))
              do (format t "Zip: ~a~cType: ~a~%" (cdr (car entry)) #\Tab (let ((entry-type (cdr (nth 1 entry)))) (if entry-type entry-type "UNKNOWN")))))
      (format t "Invalid number of arguments: ~a!~%" (length uiop:*command-line-arguments*))))

(main-search)
